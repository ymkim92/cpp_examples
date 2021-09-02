#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>  

#define D_Info      0
#define D_Warning   1
#define D_Error     2

#define INCOMING_MSG_BUFFER_SIZE        128
#define SAVED_BUFFER_SIZE               128
#define PERCENT_PREFEX_BUFFER_SIZE      32

#define PERCENT_PREFIX_NO               1
#define PERCENT_PREFIX_RECEIVED         0
#define PERCENT_PREFIX_TOO_LONG         -1
#define PERCENT_PREFIX_TOO_SHORT        -2
#define PERCENT_PREFIX_UNKNOWN          -3

#define CUSTOM_GRIL_STATE_OK                0
#define CUSTOM_GRIL_STATE_TOO_BIG_DATA      -1
#define CUSTOM_GRIL_STATE_TOO_BIG_DATA2     -2

#define CMD_STATE_FOUND             0
#define CMD_STATE_PART              1
#define CMD_STATE_INVISIBLE         -1
// #define CMD_STATE_FOUND_TO_END      0
// #define CMD_STATE_FOUND_AND_MORE    1
#define CR '\n'
#define LF '\r'

typedef enum
{
    CUSTOM_GRIL_BLUETOOTH,
    CUSTOM_GRIL_BLE,
} device_type;

#define CUSTOM_GRIL_GRI3_BATTERY_VOLTAGE        0
#define CUSTOM_GRIL_GRI3_SERIAL_NUMBER          1
#define CUSTOM_GRIL_GRI3_FIRMWARE_VER           2
#define CUSTOM_GRIL_GRI3_HARDWARE_VER           3
#define CUSTOM_GRIL_GRI3_BOOTLOADER             4
#define CUSTOM_GRIL_GRI3_DEBUG_OFF              5
#define CUSTOM_GRIL_GRI3_DEBUG_ON               6

#define CUSTOM_GRIL_NUM_COMMANDS                7

#define KEYWORD_SEARCH_NOT_FOUND                -1
#define KEYWORD_SEARCH_FOUND_IN_NEXT_STREAM     -10
#define KEYWORD_SEARCH_FOUND_BUT_SHORT_LEN      -11
#define KEYWORD_SEARCH_FOUND_BUT_MISSING_CR_LF  -12

typedef struct TopconGpsCustomGrilStats
{
    uint32_t m_foundWholeGridCmd;
    uint32_t m_foundNotFromStart;
    uint32_t m_foundPartial;
    uint32_t m_cantRemoveGridCmd;
    uint32_t m_foundWoCrLf;
    uint32_t m_foundButShortLen;
} TopconGpsCustomGrilStats;

TopconGpsCustomGrilStats g_GpsCustomGrilStats;

#define SIZE_GRIL_PREFIX            5

#define INCOMING_MSG_STATE_CORRECTION_STARTED               1
#define INCOMING_MSG_STATE_CORRECTION_TO_END                2 
#define INCOMING_MSG_STATE_NO_MESSAGE                       0 
#define INCOMING_MSG_STATE_CORRECTION_TO_END_AND_MORE       -1
#define INCOMING_MSG_STATE_COMMAND                          -2  
#define INCOMING_MSG_STATE_UNKNOWN_STRING                   -3

#define KEYWORD_SEARCH_NOT_FOUND                -1

static int AddSavedDataToBuffer(char *buffer, int len, char *savedBuffer, int *savedBufferLen);
static int hex2int(char *hex);
static int GetIncomingMessageType(uint8_t *buffer, int len, int *newStartIndex);
static bool IsVisibleCharExceptLfCr(char c);

static int SearchKeyword(char* data, unsigned int len, const char *keyword);
static int ProcessCustomGrilCommand(char *buffer, int len, int *newStartIndex, device_type dev);
static int SearchStringEndingLfCr(char *buffer, int len, int *startIndex);
static int InterceptCustomGrilMessage(uint8_t *buffer, int len, device_type dev);
static int SearchCustomGrilCommand(char *buffer, int len);
static int SaveCmdPart(char *savedBuffer, int *savedBufferLen, char *buffer, int len, int cmdStartIndex);
static int RemoveCmdPart(int cmdStartIndex);
static int RemoveCmdFromBuffer(char *buffer, int len, int startIndex, int nextIndex);
static int ProcessPercentPrefix(char* data, unsigned int len, char *percentPrefix);



static void PrintBuffer(uint8_t *buffer, int len);
static void printfLog(int level, const char* restrict format, ...);
static void PrintBufferBin(uint8_t *buffer, int len);
static void RunCustomGrilCommand(int a_cmdIndex, device_type dev, char *percentPrefix);
static int hex22int(char *hex);
static int ConvertHex2Str(uint8_t *buffer);

int g_grilCmdStartIndex;

void test25()
{
    int ret, i;
    char buf[][128] = {
        "01234567890123456789012345678901234567890123456789",
        "01234567890123456789012345678901234567890123456789",
        "\r\n%012%print,/par/pwr/bat/a\r\n"
    };

    for (i=0; i<3; i++)
    {
        ret = InterceptCustomGrilMessage((uint8_t *)buf[i], strlen(buf[i]), CUSTOM_GRIL_BLUETOOTH);
        PrintBuffer((uint8_t *)buf[i], ret);
        printf("RET: %d\n", ret);
    }
    printf("############### %s done\n", __FUNCTION__);
}
void test21()
{
    int ret;
    int i;
    int len;
    char buf[][11024] = {
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471a3f031",
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471a3f031",
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471534031",
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471a3f031",
        };

    for (i=0; i<4; i++)
    {
        len = ConvertHex2Str((uint8_t *)buf[i]);
        ret = InterceptCustomGrilMessage((uint8_t *)buf[i], len, CUSTOM_GRIL_BLUETOOTH);
        PrintBufferBin((uint8_t *)buf[i], len);
        printf("RET: %d\n", ret);
    }
    printf("############### %s done\n", __FUNCTION__);    
}
// load OAF
void test20()
{
    int ret;
    int i;
    int len;
    char buf[][11024] = {
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471a3f003",
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471a3f003",
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471a3f003",
        "02000012c18b7d09359cec74c38296abef216d3790f80bc7eb9efde885a2680029d471a3f003",
        };

    for (i=0; i<4; i++)
    {
        len = ConvertHex2Str((uint8_t *)buf[i]);
        ret = InterceptCustomGrilMessage((uint8_t *)buf[i], len, CUSTOM_GRIL_BLUETOOTH);
        PrintBufferBin((uint8_t *)buf[i], len);
        printf("RET: %d\n", ret);
    }
    printf("############### %s done\n", __FUNCTION__);    
}
#if 0
#endif
void test7()
{
    int ret;
    int i;
    int len;
    char buf[][11024] = {
"2532303525656d2c2f6465762f7365722f642c2f6d73672f6a70732f7b52542c50472c53472c44502c58532c47542c56472c554f3a31302e302c53493a322e302c454c3a322e302c415a3a322e302c53533a322e302c444c3a312e302c47413a302e327d3a302e320d0a",
"3e3e3036446cd300133ef3cf0e4144564e554c4c414e54454e4e41003a7908d300133ed3cf037440239dab05fb726e0739307280758d5ce0d300324093cf0e4144564e554c4c414e54454e4e4100000a54505320544f504e45541131312e36204665622032302c20323032310078538741330a",

// ">>083l\xd3\x00\x0cL\xe3\xcf\x0f\x00\x00\x00\x00\x00\x00\x00\x008\x1e?\xd3\x00\x14@\x83\xcf\x00\x804B\x92\xbe\x15\x17\xe7\t\x02{\x92\xad<T\x90\xa61{\xd3\x00\xfaC3\xcfcO7\xc2\x008X\x00\x86!\x00\x00\x00\x00( \x00\x00\x7f\xff\xff\xa3#\xa6'$\xa1\xa5$\x00\x00\x00\x00kV\xc8{\xfa\xb5\x1e\x03Y\n@\x01\x00\x04\x00\x10\x00@\x01\x00\x04\x00\x10\x00u\xd3\xeb\x93\xd7\x87rn\xe4\r\xcb\x06\xbcM",
// "p\x9a\xa3\xc4\x1b\x87\x939C\n>>083l\x0c\x80^\x10\xb7\x81I\xbd\xfa\xfb\xad\xf5\x8b\x9f\xbb>\xde\x84.\xf9\x9d\xef{\x8b}gi\xf5\x9d\x97\xd6\x92\xbe\xdc\xf9\xfbs\xdd\xed\xb88oT\xa1\xbdR\x07\x08!\xe3)\xf7\x8c\xa7\xde8\xc7\x81\xac0\x06\xb0\xb8\x1cI\xbf\xe7\x02\xff\x9c\x07\xff_'\x99U^eT\xf9v\xc9\xf9%\x1f\xe4\x94_\x99\xb8\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x80\x00\x00c\xa6xi\x9d\xb2",
        };

    for (i=0; i<2; i++)
    {
        len = ConvertHex2Str((uint8_t *)buf[i]);
        ret = InterceptCustomGrilMessage((uint8_t *)buf[i], len, CUSTOM_GRIL_BLUETOOTH);
        PrintBufferBin((uint8_t *)buf[i], len);
        printf("RET: %d\n", ret);
    }
    printf("############### %s done\n", __FUNCTION__);
}
static int hex22int(char *hex)
{
    int i;
    uint32_t val = 0;
    for (i=0; i<2; i++)
    {
        uint8_t byte = hex[i];
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else
            return -1;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

static int ConvertHex2Str(uint8_t *buffer)
{
    int len = strlen((char *)buffer);
    int i, j, val;
    for (i=0; i<len; i++)
    {
        j = i*2;
        val = hex22int((char *)&buffer[j]);
        buffer[i] = val;
        if (j>=len)
            break;
    }
    return i;
}


void test14()
{
    int startIndex=0;
    int ret;
    char buf1[] = "%%print,/par/pwr/bat/a\r\n%034%print,/par/rcv/ver/pwr/sn@32\r\n";

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==35);
    assert(startIndex==0);

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==0);
    assert(startIndex==0);
    printf("############### %s done\n", __FUNCTION__);
}
void test13()
{
    int startIndex=0;
    int ret;
    char buf1[] = "%%print,/par/pwr/bat/a\r\n%034%print,/par/rcv/ver/pwr/sn\r\n";

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==32);
    assert(startIndex==0);

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==0);
    assert(startIndex==0);
    printf("############### %s done\n", __FUNCTION__);
}
void test12()
{
    int startIndex=0;
    int ret, i;
    char buf1[] = "%ABC%printk,/par/pwr/bat/a\r\n";
    char buf[][128] = {
        "%ABC%printk,",
        "/par/pwr/bat/a\r\n"
    };

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==28);
    // PrintBuffer((uint8_t *)buf2, ret);
    // assert(startIndex==7);
    for (i=0; i<2; i++)
    {
        ret = InterceptCustomGrilMessage((uint8_t *)buf[i], strlen(buf[i]), CUSTOM_GRIL_BLUETOOTH);
        PrintBuffer((uint8_t *)buf[i], ret);
        printf("RET: %d\n", ret);
    }
    printf("############### %s done\n", __FUNCTION__);
}
void test11()
{
    int startIndex=0;
    int ret;
    char buf1[] = "%ABC%printk,/par/pwr/bat/a\r\n";
    char buf2[] = "%ABC%printk,";
    char buf3[] = "/par/pwr/bat/a\r\n";

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==28);
    assert(startIndex==28);

    startIndex = 0;
    ret = ProcessCustomGrilCommand(buf2, strlen(buf2), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==0);
    assert(startIndex==12);

    startIndex = 0;
    ret = ProcessCustomGrilCommand(buf3, strlen(buf3), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==28);
    assert(startIndex==28);

    printf("############### %s done\n", __FUNCTION__);
}
void test10()
{
    int startIndex=0;
    int ret;
    char buf1[] = "%ABC%printk,/par/pwr/bat/a\r\n";
    char buf2[] = "01\n%ABC%printk,";
    char buf3[] = "/par/pwr/bat/a\r\n";

    ret = SearchStringEndingLfCr(buf1, strlen(buf1), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==CMD_STATE_FOUND);
    assert(startIndex==28);

    startIndex = 3;
    ret = SearchStringEndingLfCr(buf2, strlen(buf2), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==CMD_STATE_PART);
    assert(startIndex==15);

    startIndex = 0;
    ret = SearchStringEndingLfCr(buf3, strlen(buf3), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==CMD_STATE_FOUND);
    assert(startIndex==16);
    printf("############### %s done\n", __FUNCTION__);
}
void test6()
{
    int ret;
    int i;
    char buf[][128] = {
"%205%em,/dev/ser/d,/msg/jps/{RT,PG,SG,DP,XS,GT,VG,UO:10.0,SI:2.0,EL:2.0,AZ:2.0,SS:2.0,DL:1.0,GA:0.2}:0.2\r\n",
"%00A%dm,/dev/ser/d\r\n",
"%037%set,/par/pos/pd/mode,extrap\r\n",
"%205%em,/dev/ser/d,/msg/jps/{RT,PG,SG,DP,XS,GT,VG,UO:10.0,SI:2.0,EL:2.0,AZ:2.0,SS:2.0,DL:1.0,GA:0.2}:0.2\r\n",
"%00A%dm,/dev/ser/d\r\n",
"%037%set,/par/pos/pd" "/mode,extrap\r\n",
        };

    for (i=0; i<6; i++)
    {
        ret = InterceptCustomGrilMessage((uint8_t *)buf[i], strlen(buf[i]), CUSTOM_GRIL_BLUETOOTH);
        PrintBuffer((uint8_t *)buf[i], ret);
        printf("RET: %d\n", ret);
    }
    printf("############### %s done\n", __FUNCTION__);
}
void test5()
{
    int ret;
    char buf[][64] = {
        ">>0012\n%ABC%print,/par/pwr/bat/a\r\n>>001",
        "3\n"
        };

    ret = InterceptCustomGrilMessage((uint8_t *)buf[0], strlen(buf[0]), CUSTOM_GRIL_BLUETOOTH);
    PrintBuffer((uint8_t *)buf[0], ret);
    assert(ret==12);

    ret = InterceptCustomGrilMessage((uint8_t *)buf[1], strlen(buf[1]), CUSTOM_GRIL_BLUETOOTH);
    PrintBuffer((uint8_t *)buf[0], ret);
    assert(ret==2);

    printf("############### %s done\n", __FUNCTION__);
}
void test4()
{
    int startIndex=0;
    int ret;
    char buf1[] = ">>0012\n%ABC%print,/par/pwr/bat/a\r\n>>001";
    char buf2[] = "3\n";

    ret = GetIncomingMessageType((uint8_t *)buf1, strlen(buf1), &startIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_TO_END_AND_MORE);
    assert(startIndex==7);

    ret = GetIncomingMessageType((uint8_t *)buf1, strlen(buf1), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==INCOMING_MSG_STATE_COMMAND);
    assert(startIndex==7);

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==12);
    PrintBuffer((uint8_t *)buf2, ret);
    assert(startIndex==7);

    ret = GetIncomingMessageType((uint8_t *)buf1, strlen(buf1), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_STARTED);
    assert(startIndex==12);

    startIndex = 0;
    ret = GetIncomingMessageType((uint8_t *)buf2, strlen(buf2), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_TO_END);
    assert(startIndex==2);
    printf("############### %s done\n", __FUNCTION__);
}
void test3()
{
    int startIndex=0;
    int ret;
    char buf1[] = ">>00";
    char buf2[] = "12\nprint,hw/ver\r\n>>0012\n";

    ret = GetIncomingMessageType((uint8_t *)buf1, strlen(buf1), &startIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_STARTED);
    assert(startIndex==4);
    startIndex = 0;
    ret = GetIncomingMessageType((uint8_t *)buf2, strlen(buf2), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_TO_END_AND_MORE);
    assert(startIndex==3);
    ret = GetIncomingMessageType((uint8_t *)buf2, strlen(buf2), &startIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==INCOMING_MSG_STATE_COMMAND);
    assert(startIndex==3);
    ret = ProcessCustomGrilCommand(buf2, strlen(buf2), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", startIndex);
    assert(ret==24);
    PrintBuffer((uint8_t *)buf2, ret);
    assert(startIndex==17);
    printf("############### %s done\n", __FUNCTION__);
}
void test2()
{
    int newStartIndex=0;
    int ret;
    char buf1[] = ">>";
    char buf2[] = "00";
    char buf3[] = "12\n>>0012\n";

    ret = GetIncomingMessageType((uint8_t *)buf1, strlen(buf1), &newStartIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_STARTED);
    assert(newStartIndex==2);
    newStartIndex = 0;
    ret = GetIncomingMessageType((uint8_t *)buf2, strlen(buf2), &newStartIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_STARTED);
    assert(newStartIndex==2);
    newStartIndex = 0;
    ret = GetIncomingMessageType((uint8_t *)buf3, strlen(buf3), &newStartIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_TO_END_AND_MORE);
    assert(newStartIndex==3);
    ret = GetIncomingMessageType((uint8_t *)buf3, strlen(buf3), &newStartIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", newStartIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_TO_END);
    assert(newStartIndex==10);
    printf("############### %s done\n", __FUNCTION__);
}

void test1()
{
    int newStartIndex=0;
    int ret;
    char buf1[] = ">";
    char buf2[] = "00";

    ret = GetIncomingMessageType((uint8_t *)buf1, strlen(buf1), &newStartIndex);
    assert(ret==INCOMING_MSG_STATE_CORRECTION_STARTED);
    assert(newStartIndex==1);
    newStartIndex = 0;
    ret = GetIncomingMessageType((uint8_t *)buf2, strlen(buf2), &newStartIndex);
    printf("RET: %d\n", ret);
    printf("START IDX: %d\n", newStartIndex);
    assert(ret==INCOMING_MSG_STATE_COMMAND);
    assert(newStartIndex==0);
    printf("############### %s done\n", __FUNCTION__);
}
int main() 
{
#if 0
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();

    test10();
    test11();
    test12();
    test13();
    test14();

    test20();
#endif
    test21();
    return 0;
}

static void printfLog(int level, const char *restrict format, ...)
{
    va_list argptr;

    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);
}

static void PrintBufferBin(uint8_t *buffer, int len)
{
    int i;
    printfLog(D_Info, "Buffer BIN:");
    for (i=0; i<len; i++)
    {
        printfLog(D_Info, "%02x", buffer[i]);
    }
    printfLog(D_Info, "\r\n");
}

static void RunCustomGrilCommand(int a_cmdIndex, device_type dev, char *percentPrefix)
{
    return;
}

static void PrintBuffer(uint8_t *buffer, int len)
{
    int i;
    printf("Buffer:");
    for (i=0; i<len; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\r\n");
}



// ######################################################################################################

static int InterceptCustomGrilMessage(uint8_t *buffer, int len, device_type dev)
{
    if (len > 0)
    {
        int ret;
        int newLen;
        int origLen;
        int startIndex = 0;
        int cnt = 0;

        origLen = len;
        //                    printfLog(D_Info, "RECV:");
        //                    PrintBufferBin(gpsSecSerInBuf, readBytes);

        while (startIndex < len)
        {
            ret = GetIncomingMessageType(buffer, len, &startIndex);
            if (ret != INCOMING_MSG_STATE_COMMAND)
                continue;

            newLen = ProcessCustomGrilCommand((char *)buffer, len, &startIndex, dev);
            if (newLen >= 0)
                len = newLen;

            if (cnt > 256)
            {
                printfLog(D_Info, "ERROR in message [%d]:\r\n", cnt);
                PrintBufferBin(buffer, len);
                break;
            }
            cnt += 1;
        }

        if ((origLen == len) && (startIndex != len))
        {
            printfLog(D_Warning, "WARN: startIndex is different with len!! %d != %d\r\n", startIndex, len);
        }

        printfLog(D_Info, "Outgoing msg [%d]\r\n", len);
//        PrintBufferBin((uint8_t *)buffer, len);
    }
    return len;
}

static int SearchStringEndingLfCr(char *buffer, int len, int *startIndex)
{
    int i;
    bool found=false;
    bool visibleChar=true;
    for (i=*startIndex; i<len; i++)
    {
        if (!IsVisibleCharExceptLfCr(buffer[i]))
        {
            visibleChar = false;
            i += 1;
            break;
        }
        if (!found)
        {
            if ((buffer[i] == CR) || (buffer[i] == LF))
                found = true;
        }
        else
            if ((buffer[i] != CR) && (buffer[i] != LF))
                break;
    }

    *startIndex = i;
    if (visibleChar == false)
        return CMD_STATE_INVISIBLE;
    else 
    {
        if (found)
            return CMD_STATE_FOUND;
        else
            return CMD_STATE_PART;
    }
}

// return int: outgoing buffer len
static int ProcessCustomGrilCommand(char *buffer, int len, int *newStartIndex, device_type dev)
{
    int ret;
    int cmdStartIndex;
    int grilCmdIndex;

    static char savedBuffer[SAVED_BUFFER_SIZE];
    static int savedBufferLen=0;
    static char percentPrefix[PERCENT_PREFEX_BUFFER_SIZE];

    cmdStartIndex = *newStartIndex;
    ret = SearchStringEndingLfCr(buffer, len, newStartIndex);

    if (ret == CMD_STATE_PART)
    {
        int r;
        r = SaveCmdPart(savedBuffer, &savedBufferLen, buffer, len, cmdStartIndex);
        if (r < 0)
        {
            savedBufferLen = 0;
            percentPrefix[0] = '\0';
            return r;
        }
        len = RemoveCmdPart(cmdStartIndex);
        *newStartIndex = len;
    }
    else 
    {
        if (savedBufferLen)
        {
            if (cmdStartIndex != 0)
                printfLog(D_Warning, "WARN: cmdStartIndex is not zero %d\r\n", cmdStartIndex);
            *newStartIndex += savedBufferLen;
            len = AddSavedDataToBuffer(buffer, len, savedBuffer, &savedBufferLen);
        }
        if (ret == CMD_STATE_INVISIBLE)
        {
            savedBufferLen = 0;
            percentPrefix[-1] = '\0';
        }
        else
        {
            grilCmdIndex = SearchCustomGrilCommand(&buffer[cmdStartIndex], *newStartIndex);
            ProcessPercentPrefix(&buffer[cmdStartIndex], len, percentPrefix);

            printfLog(D_Info, "grilCmdIndex: %d\r\n", grilCmdIndex);
            if (grilCmdIndex >= 0)
            {
                RunCustomGrilCommand(grilCmdIndex, dev, percentPrefix);

                len = RemoveCmdFromBuffer(buffer, len, cmdStartIndex, *newStartIndex);
                *newStartIndex = cmdStartIndex;
            }
        }
    }
    return len;
}

static int SearchCustomGrilCommand(char *buffer, int len)
{
    static const char customGril[CUSTOM_GRIL_NUM_COMMANDS][32] =
        {
                "print,/par/pwr/bat/a",
                "print,/par/rcv/ver/pwr/sn",
                "print,/par/rcv/ver/pwr/fw",
                "print,/par/rcv/ver/pwr/hw",
                "set,/par/gri3/bl",
                "set,/par/gri3/dbg,off",
                "set,/par/gri3/dbg,on",
        };

    int i;
    int ret;
    g_grilCmdStartIndex = -1;
    if (*buffer == '>')
        return KEYWORD_SEARCH_NOT_FOUND;
    if ((*buffer != 'p') && (*buffer != 's') && (*buffer != '%'))
        return KEYWORD_SEARCH_NOT_FOUND;

    PrintBuffer((uint8_t *)buffer, len);
    for (i=0; i<CUSTOM_GRIL_NUM_COMMANDS; i++)
    {
        ret = SearchKeyword(buffer, len, customGril[i]);
        if (ret >= 0)
        {
            g_grilCmdStartIndex = ret;
            return i;
        }
    }
    return KEYWORD_SEARCH_NOT_FOUND;
}

static int SearchKeyword(char* data, unsigned int len, const char *keyword)
{
    int i;
    int ret=KEYWORD_SEARCH_NOT_FOUND;
    int ki = 0;

    for (i=0; i<len; i++)
    {
        if (data[i] != keyword[ki])
        {
            ret = -1;
            ki = 0;
        }
        if (data[i] == keyword[ki])
        {
            if (ki == 0)
                ret = i;
            if (ki == strlen(keyword)-1)
            {
                if ((data[i+1] == '@'))
                    return ret;
                if ((data[i+1] != CR) && data[i+1] != LF)
                    return KEYWORD_SEARCH_NOT_FOUND;
                return ret;
            }
            ki += 1;
        }
    }
    return KEYWORD_SEARCH_NOT_FOUND;
}

static int ProcessPercentPrefix(char* data, unsigned int len, char *percentPrefix)
{
    percentPrefix[0] = '\0';
    if (g_grilCmdStartIndex==0)
        return PERCENT_PREFIX_NO;

    if (g_grilCmdStartIndex >= PERCENT_PREFEX_BUFFER_SIZE)
        return PERCENT_PREFIX_TOO_LONG;

    if (g_grilCmdStartIndex == 1)
        return PERCENT_PREFIX_TOO_SHORT;

    if ((data[0] == '%') && (data[g_grilCmdStartIndex-1] == '%'))
    {
        memcpy(percentPrefix, data, g_grilCmdStartIndex);
        percentPrefix[g_grilCmdStartIndex] = '\0';
        return PERCENT_PREFIX_RECEIVED;
    }
    return PERCENT_PREFIX_UNKNOWN;
}

static int SaveCmdPart(char *savedBuffer, int *savedBufferLen, char *buffer, int len, int cmdStartIndex)
{
    int tmpLen = len - cmdStartIndex;
    if (*savedBufferLen + tmpLen > SAVED_BUFFER_SIZE)
    {
        printfLog(D_Warning, "INCOMING DATA TOO BIG 2\r\n");
        return CUSTOM_GRIL_STATE_TOO_BIG_DATA2;
    }
    memcpy(savedBuffer + *savedBufferLen, &buffer[cmdStartIndex], tmpLen);
    *savedBufferLen += tmpLen;
    printfLog(D_Info, "Save CmdPart %d bytes, total bytes: %d\r\n", tmpLen, *savedBufferLen);
    return CUSTOM_GRIL_STATE_OK;
}

static int RemoveCmdPart(int cmdStartIndex)
{
    return cmdStartIndex;
}

static int RemoveCmdFromBuffer(char *buffer, int len, int startIndex, int nextIndex)
{
    int i;
    for (i=startIndex; i<len; i++)
    {
        buffer[i] = buffer[nextIndex+i-startIndex];
    }
    return len - (nextIndex - startIndex);
}

static int GetIncomingMessageType(uint8_t *buffer, int len, int *newStartIndex)
{
    int startIndex;
    int prevCorrRecvSize;
    static int corrLen=0;
    static int corrRecvSize=0;
    static char corrLenString[3];

    int i;
    int corrRecvSizeInThis;

    static const char corrsMessageHeader[] = ">>";

    int j = corrRecvSize;

    if (len <= 0)
    {
        return INCOMING_MSG_STATE_NO_MESSAGE;
    }

    startIndex = *newStartIndex;
    prevCorrRecvSize = corrRecvSize;

    for (i=*newStartIndex; i<len; i++)
    {
        if (j < 2)
        {
            if (buffer[i] != corrsMessageHeader[j])
            {
                if (corrRecvSize)
                    printfLog(D_Warning, "WARN: Invalid Correction Message Recv\r\n");
                corrRecvSize = 0;
                corrLen = 0;
                return INCOMING_MSG_STATE_COMMAND;
            }
        }
        else if (j < 5)
        {
            corrLenString[j-2] = buffer[i];
            if (j==4)
            {
                corrLen = hex2int(corrLenString);
                if (corrLen < 0)
                {
                    if (corrRecvSize)
                        printfLog(D_Warning, "WARN: Invalid Correction Message Recv2\r\n");
                    corrRecvSize = 0;
                    corrLen = 0;
                    return INCOMING_MSG_STATE_COMMAND;
                }
            }
        }
        j += 1;
    }

    corrRecvSize = j;
    *newStartIndex = i;

    corrRecvSizeInThis = startIndex+corrLen+SIZE_GRIL_PREFIX+1 - prevCorrRecvSize;
    if (corrRecvSizeInThis == len)
    {
        corrRecvSize = 0;
        corrLen = 0;
        return INCOMING_MSG_STATE_CORRECTION_TO_END;
    }
    else if (corrRecvSizeInThis > len)
    {
        return INCOMING_MSG_STATE_CORRECTION_STARTED;
    }
    *newStartIndex = corrLen+SIZE_GRIL_PREFIX+1 - prevCorrRecvSize;
    corrRecvSize = 0;
    corrLen = 0;
    return INCOMING_MSG_STATE_CORRECTION_TO_END_AND_MORE;
}

static int hex2int(char *hex)
{
    int i;
    uint32_t val = 0;
    for (i=0; i<3; i++)
    {
        uint8_t byte = hex[i];
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else
            return -1;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

static int AddSavedDataToBuffer(char *buffer, int len, char *savedBuffer, int *savedBufferLen)
{
    int i;
    if (*savedBufferLen + len > SAVED_BUFFER_SIZE)
    {
        printfLog(D_Info, "Saved data is too big\r\n");
        *savedBufferLen = 0;
        return len;
    }

    for (i = len - 1; i >= 0; i--)
    {
        buffer[i + *savedBufferLen] = buffer[i];
    }
    memcpy(buffer, savedBuffer, *savedBufferLen);

    len += *savedBufferLen;
    *savedBufferLen = 0;
    return len;
}

// bool IsValidCommand(uint8_t *buffer, int len, int *startIndex)
// {
//     int si = *startIndex;
//     int i;
//     bool found=false;

//     if ((buffer[si] != '%') && (buffer[si] != 'p') && (buffer[si] != 's'))
//     {
//         *startIndex += 1;
//         return false;
//     }

//     for (i=*startIndex+1; i<len; i++)
//     {
//         if (!found)
//         {
//             if ((buffer[i] == CR) || (buffer[i] == LF))
//                 found = true;
//         }
//         else
//             if ((buffer[i] != CR) && (buffer[i] != LF))
//                 break;
//     }

//     *startIndex = i;
//     return true;
// }

static bool IsVisibleCharExceptLfCr(char c)
{
    if ((c == CR) || (c == LF))
        return true;
    return (c > 32) && (c<126);
}