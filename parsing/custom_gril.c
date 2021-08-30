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
#define SAVED_BUFFER_SIZE               512
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

#define KEYWORD_SEARCH_NOT_FOUND                -1

static int AddSavedDataToBuffer(char *buffer, int len, char *savedBuffer, int *savedBufferLen);
static int hex2int(char *hex);
static int GetIncomingMessageType(uint8_t *buffer, int len, int *newStartIndex);

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

void test20()
{
    int startIndex=0;
    int ret;
    char buf1[] = ">>001a\n%ABC%print,/par/pwr/bat/a\r\n%print,/par/rcv/ver/pwr/sn\r\n>>002a";
    // char buf2[] = "b\n%%print,/par/pwr/bat/a\r\n>>002>>\n";

    ret = ProcessCustomGrilCommand(buf1, strlen(buf1), &startIndex, CUSTOM_GRIL_BLUETOOTH);
    printf("RET: %d\n", ret);
    printf("############### %s done\n", __FUNCTION__);
}
#if 0
b'#OFF#\r\n'
b'#OFF#\r\n'
b'%%dm\r\n'
b'%000%print,/cur/term\r\n'
b'%002%print,/par/rcv/model\r\n'
b'%000%print,/cur/term\r\n'
b'%001%print,/cur/term\r\n'
b'%015%set,/par/dev/ser/d/imode,cmd\r\n'
b'%1EA%set,/par/mc/dozer/restartConnection,on\r\n'
b'%055%print,/par/rcv/id\r\n'
b'%056%print,/par/rcv/ver/main\r\n'
b'%007%set,/par/pos/elm,13\r\n'
b'%054%print,/par/opts/GPS1:on\r\n'
b'%054%print,/par/opts/_GPS:on\r\n'
b'%054%print,/par/opts/GLO1:on\r\n'
b'%054%print,/par/opts/_GLO:on\r\n'
b'%054%print,/par/opts/CDIF:on\r\n'
b'%054%print,/par/opts/VDIF:on\r\n'
b'%054%print,/par/opts/PDIF:on\r\n'
b'%054%print,/par/opts/CMRO:on\r\n'
b'%054%print,/par/opts/CMRI:on\r\n'
b'%054%print,/par/opts/RTMI:on\r\n'
b'%054%print,/par/opts/RTMO:on\r\n'
b'%054%print,/par/opts/RTKR:on\r\n'
b'%054%print,/par/opts/RTKB:on\r\n'
b'%054%print,/par/opts/RS_A:on\r\n'
b'%054%print,/par/opts/RS_B:on\r\n'
b'%054%print,/par/opts/RS_C:on\r\n'
b'%054%print,/par/opts/RS_D:on\r\n'
b'%054%print,/par/opts/OPEN:on\r\n'
b'%054%print,/par/opts/WAAS:on\r\n'
b'%054%print,/par/opts/QZSS:on\r\n'
b'%054%print,/par/opts/_GAL:on\r\n'
b'%054%print,/par/opts/BEID:on\r\n'
b'%054%print,/par/opts/QZS1:on\r\n'
b'%054%print,/par/opts/GAL1:on\r\n'
b'%054%print,/par/opts/BDS1:on\r\n'
b'%054%print,/par/opts/DANT:on\r\n'
b'%054%print,/par/opts/TEAM:on\r\n'
b'%054%print,/par/opts/DYNA:on\r\n'
b'%0E9%set,/par/rover/base/pos/cur,got\r\n'
b'%0EE%set,/par/rover/base/ant/cur,got\r\n'
b'%0EF%print,/par/rover/base/ant/got/id\r\n'
b'%0F0%print,/par/rover/base/ant/got/m_offs\r\n'
b'%068%dm,/dev/ser/b,/msg/nmea/GGA\r\n'
b'%015%set,/par/dev/ser/a/imode,cmd\r\n'
b'%015%set,/par/dev/ser/b/imode,cmd\r\n'
b'%015%set,/par/dev/ser/c/imode,cmd\r\n'
b'%015%set,/par/dev/modem/a/imode,cmd\r\n'
b'%015%set,/par/dev/tcp/a/imode,cmd\r\n'
b'%015%set,/par/dev/tcp/b/imode,cmd\r\n'
b'%015%set,/par/dev/tcp/c/imode,cmd\r\n'
b'%015%set,/par/dev/tcp/d/imode,cmd\r\n'
b'%015%set,/par/dev/tcp/e/imode,cmd\r\n'
b'%015%set,/par/dev/usb/a/imode,cmd\r\n'
b'%015%set,/par/dev/mvp/a/imode,cmd\r\n'
b'%015%set,/par/dev/mvp/b/imode,cmd\r\n'
b'%015%set,/par/dev/mvp/c/imode,cmd\r\n'
b'%015%set,/par/dev/mvp/d/imode,cmd\r\n'
b'%015%set,/par/dev/ntrip/a/imode,cmd\r\n'
b'%04C%set,/par/pos/pd/reset,on\r\n'
b'%024%set,/par/cur/term/jps/1,{rtcm3,-1,y,/dev/mvp/a}\r\n'
b'%027%set,/par/cur/term/jps/2,{none,-1,n,""}\r\n'
b'%01E%set,/par/dev/mvp/a/imode,rtcm3\r\n'
b'%021%set,/par/dev/ser/d/jps/0,{nscmd,37,n,""}\r\n'
b'%020%set,/par/dev/ser/d/imode,jps\r\n'
b'%01C%set,/par/base/mode/,off\r\n'
b'%01E%set,/par/dev/mvp/a/imode,rtcm3\r\n'
b'%028%set,/par/pos/pd/port,/dev/mvp/a\r\n'
b'%029%set,/par/pos/pd/period,1.0\r\n'
b'%09B%set,hd/mode,off\r\n'
b'%02C%set,/par/pos/mode/cur,pd\r\n'
b'%047%set,/par/pos/pd/textr,10\r\n'
b'%0F4%set,/par/pos/pd/inuse,-1\r\n'
b'%064%set,/par/pos/pd/vrs,off\r\n'
b'%066%set,/par/pos/pd/fkp,off\r\n'
b'%0EB%set,rover/base/rcv/cur,got\r\n'
b'%06A%set,/par/rtcm/rover/ver,v2.3\r\n'
b'%06B%set,/par/rtcm/rover/reset,on\r\n'
b'%100%set,raw/clp/static,off\r\n'
b'%04E%set,raw/clp/loops,off\r\n'
b'%052%set,raw/corr/ca/code,mpnew\r\n'
b'%107%set,/par/lock/gps/l2c,{off,off}\r\n'
b'%104%set,lock/glo/fcn,on\r\n'
b'%108%set,/par/lock/glo/l2c,on\r\n'
b'%10B%set,/par/lock/glo/pcode,{off,off}\r\n'
b'%1ED%set,lock/beidou/sat,on\r\n'
b'%1EF%set,lock/beidou/b2,on\r\n'
b'%1F3%set,/par/pos/navsys/galileo,on\r\n'
b'%1F1%set,lock/galileo/sat,on\r\n'
b'%1F5%set,lock/qzss/sat,on\r\n'
b'%1F7%set,/par/lock/qzss/l2c,{on,on}\r\n'
b'%0EB%set,rover/base/rcv/cur,got\r\n'
b'%039%set,/par/pos/pd/dyn,1\r\n'
b'%03C%set,raw/msint,200\r\n'
b'%03B%set,/par/pos/msint,200\r\n'
b'%00A%dm,/dev/ser/d\r\n'
b'%037%set,/par/pos/pd/mode,extrap\r\n'
b'%205%em,/dev/ser/d,/msg/jps/{RT,PG,SG,DP,XS,GT,VG,UO:10.0,SI:2.0,EL:2.0,AZ:2.0,SS:2.0,DL:1.0,GA:0.2}:0.2\r\n'
b'%00A%dm,/dev/ser/d\r\n'
b'%037%set,/par/pos/pd/mode,extrap\r\n'
b'%205%em,/dev/ser/d,/msg/jps/{RT,PG,SG,DP,XS,GT,VG,UO:10.0,SI:2.0,EL:2.0,AZ:2.0,SS:2.0,DL:1.0,GA:0.2}:0.2\r\n'
b'%00A%dm,/dev/ser/d\r\n'
b'%037%set,/par/pos/pd/mode,extrap\r\n'
b'%205%em,/dev/ser/d,/msg/jps/{RT,PG,SG,DP,XS,GT,VG,UO:10.0,SI:2.0,EL:2.0,AZ:2.0,SS:2.0,DL:1.0,GA:0.2}:0.2\r\n'
b'%00A%dm,/dev/ser/d\r\n'
b'%037%set,/par/pos/pd/mode,extrap\r\n'
b'%205%em,/dev/ser/d,/msg/jps/{RT,PG,SG,DP,XS,GT,VG,UO:10.0,SI:2.0,EL:2.0,AZ:2.0,SS:2.0,DL:1.0,GA:0.2}:0.2\r\n'
b'%00A%dm,/dev/ser/d\r\n'
b'%037%set,/par/pos/pd/mode,extrap\r\n'
b'%205%em,/dev/ser/d,/msg/jps/{RT,PG,SG,DP,XS,GT,VG,UO:10.0,SI:2.0,EL:2.0,AZ:2.0,SS:2.0,DL:1.0,GA:0.2}:0.2\r\n'
b'>>06Dl\xd3\x00\x13>\xf3\xcf\x0eADVNULLANTENNA\x00:y\x08\xd3\x00\x13>\xd3\xcf\x03t@#\x9d\xab\x05\xfbrn\x0790r\x80u\x8d\\\xe0\xd3\x002@\x93\xcf\x0eADVNULLANTENNA\x00\x00\nTPS TOPNET\x1111.6 Feb 20, 2021\x00xS\x87A3\n'
b">>083l\xd3\x00\x0cL\xe3\xcf\x0f\x00\x00\x00\x00\x00\x00\x00\x008\x1e?\xd3\x00\x14@\x83\xcf\x00\x804B\x92\xbe\x15\x17\xe7\t\x02{\x92\xad<T\x90\xa61{\xd3\x00\xfaC3\xcfcO7\xc2\x008X\x00\x86!\x00\x00\x00\x00( \x00\x00\x7f\xff\xff\xa3#\xa6'$\xa1\xa5$\x00\x00\x00\x00kV\xc8{\xfa\xb5\x1e\x03Y\n@\x01\x00\x04\x00\x10\x00@\x01\x00\x04\x00\x10\x00u\xd3\xeb\x93\xd7\x87rn\xe4\r\xcb\x06\xbcM"
b"p\x9a\xa3\xc4\x1b\x87\x939C\n>>083l\x0c\x80^\x10\xb7\x81I\xbd\xfa\xfb\xad\xf5\x8b\x9f\xbb>\xde\x84.\xf9\x9d\xef{\x8b}gi\xf5\x9d\x97\xd6\x92\xbe\xdc\xf9\xfbs\xdd\xed\xb88oT\xa1\xbdR\x07\x08!\xe3)\xf7\x8c\xa7\xde8\xc7\x81\xac0\x06\xb0\xb8\x1cI\xbf\xe7\x02\xff\x9c\x07\xff_'\x99U^eT\xf9v\xc9\xf9%\x1f\xe4\x94_\x99\xb8\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x80\x00\x00c\xa6xi\x9d\xb2"
b'\xcbS\xef\xdf4\xd8\xd9e\xb4O\x8du@\x00\x80\x01\x0074\n>>083l\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00\\\xda\x9a0\xd3\x00\xddC\xd3\xcf\x93{\x8c\x82\x0080\x0e0\x00\x00\x00\x00\x000@\x00\x00\x7f\xff\xfd\x15\x15\x19\x05))4\xf1\x94\x12\x9c\xbfm\x00Y\xaf\n\xde\x03\x80\x02\x00\x08\x00 \x00\x80\x02\x00\x08'
b'\x00:\x16t\xa6\xe6\xaf\x8c\xeb\x1f\x16#\xf2\xe5E\xee\x0b.\x0f\xfa \xb4<\xdc\xf8A\xf7E9\n>>083l\xe3\xd0 \x86\x81N\x81\x94\x04\x00\t(\x06\xf7\xde\x1b?xm~ q\xe8\xfe?\xa3\xf9\x9e\xaa^\x06\xb9\x86\x1a\xe6(p\x02!J\xb8\x05*\xdc\x15\xf5\xd8\x8df\x825\x9b\x89$N\x05\xd8\x90\x17a\xa0m\xeb\x81\xbcn\x06\xf1\xa8!\xd7\x9f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfe\x00\x00\r4\xcbL\xf3\xd3<'
b'\xe9\xa3\x0c0\xc3\x0c1\x96J\xe0\x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x0091\n>>083l\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x00\xa4Sb\xd3\x00\xe3FS\xcfcN]\x00\x008z\xcc\x80\x00\x00\x00\x00\x00 \x02\x00\x00\x7f\xff\xfb\xd43\xfb\xd3\xfb\xd4"\xab\xe4\x00\x00\x00\x00\x00\x05\x03F\\TY\xe0`2\x0f\xa8b-@\x01\x00\x04\x00\x10\x00@\x01\x00\x04\x00'
b'\x10\x00@\x01\x00\x07g\xde\xe1\xe2\xa2ED\xf9\xde\xf4De\xd4\xcc`\x86!\x17\x87I\x8e\x98\x81\xfa\x04\xed\xe1\xa3\xcc\x90\xb7\xb1\xa68 \xb0\xfc}\x19O\xf494\n>>07Bl<`*\x06\x00\xa61~K\xdb\xf8\xe9\x98f|A\x9bH\x02\x1d\x80\x08chrwa\xc6\xf3\x80\x04\x99\xfft\xdf\xe0\xc0\xdfz\xdd\x81\xc3\xc8\x05\x8e\xdfr\xdd=\xb5\xd7\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x80\x00\x06\x19MM\x97\xdf\x95w'
b']\x95\x15\xcfu\xf7\xdbt\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00\x04$\xe0\x02E0\n'
b">>083l\xd3\x00\xfaC3\xcfcOGb\x008X\x00\x86!\x00\x00\x00\x00( \x00\x00\x7f\xff\xff\xa3#\xa6'$\xa1\xa5$\x00\x00\x00\x00kV\xb8y\xf9\xb5]\xfbW\n\xc0\x01\x00\x04\x00\x10\x00@\x01\x00\x04\x00\x10\x00\x1d\xee;\xccw\xfcv\x80\xeb\xc1\xdaF\x9e\r1\x9a\x1e\xe9\xc7\xd2\x9b\xa1\xe0Vp\xa2\xa1&\x08\x0e\x0f\xe4\x1d\xf7\xa0[@\x06\x87\x82N\xa4\x9dH\xe1\x07nJ\x1d\xb9\x18w\x00\xe0\xe4\xcf\x83\x93"
b'6\x0e5\x98mgaB0\n>>083l\xb5\x9d\x06\xe9M\xf5\xec_\xd7\xb1\xbfd\xed\x81\x87\xd0\x06\x1f@\x1a\x03\xc1)\xe9\x04\xa7\x9c\x13\x8d\x8f\x9a \x1eh\x80y\x83z\x13\xd5\x18OTAD\xb8\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x80\x00\x00c\xa67\xe7\x8d\xb2\xcbS\xef\xe1,\xd8\xd9]\x93\xcf\x85u@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00'
b'\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00j\xfa\x9c\x120E\n>>083l\xd3\x00\xddC\xd3\xcf\x93{\x9c"\x0080\x0e0\x00\x00\x00\x00\x000@\x00\x00\x7f\xff\xfd\x15\x15\x19\x05))4\xf1\x94\x12\x9c\xbfe\x02Yn\xea\xe2\x03\x80\x02\x00\x08\x00 \x00\x80\x02\x00\x08\x00\x05}\x8bu\x14J}\xd9\x00\xe9\xe7\xef\xcf\xff\xc5~\xdc\x18\xff2\xa4`\xb3z&\xfc-\xd8\xe6\xd8\x8d\xf6\x9a\xe5\xde\x1b\xbd\xf3p\x089P\x80'
b'\xe5C\x03\xd3\xc4%;X\x94\xee"n0\x80\x90b\x02A\x98\ro\xe1\xda\x1c\x87hnAF\n>>083l\x1e\xec\'\xce|\x1f9\xf2}5\xa6\x1f)H|\xa4\xa2\x02\xf6\xfd\x04-\xf4\x10\xbf\xd6S\xbf\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfe\x00\x00\r4\xcbL\xf3\xd3L\xe9\xa3\x0cp\xc3\x0c1\x96J\xe0\x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80'
b'\x01\x00\x02\x00\x00\xe9n\xbc\xd3\x00\xe3FS\xcfcNl\xa0\x008z\xcc\x80\x00\x00\x00\x00\x00 \x02\x00\x00\x7f\xff\xfb\xd4306\n>>083l\xfb\xd3\xfb\xd4"\xab\xe4\x00\x00\x00\x00\x00\x05\x03F\\TY\xe0`\xb2\x0f\xb0b-@\x01\x00\x04\x00\x10\x00@\x01\x00\x04\x00\x10\x00@\x01\x00\x07hn\xe3"\x8d\x05\x10\xfaz\xf5\x88g\xf0\xd0H\xa2\xc1Q\x9d\xe8{\xcf\xf8R\xf1|\x08\xa0\x1a\x18\x8e\xb1UX\xab\xf2\x13\xfd'
b'\x1a\xb7\xf4A\xe8(N \x9fR~q\x19\xf9~\x90h\x95\xe1\xa3\xad\x82\x90\xee\n1\x1f\xdc9_m\xfc\xfd\x99;\xf5\xc7P\x07\x95\xa0\x162\x01"`\x03B5\n>>04Fl\tG{\xa3\xfd\xd8\xf2\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x80\x00\x06\x19MM\x97\xdf\x8dw]\x95\x15\xcfm\xf7\xddt\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 '
b'\x00\x04[\xbe\x9460\n'
b">>083l\xd3\x00\xfaC3\xcfcOW\x02\x008X\x00\x86!\x00\x00\x00\x00( \x00\x00\x7f\xff\xff\xa3#\xa6'$\xa1\xa5$\x00\x00\x00\x00kv\xb0w\xf8\xb5\x9d\xfbU\x0b\xc0\x01\x00\x04\x00\x10\x00@\x01\x00\x04\x00\x10\x00\x05\xd8\x0b\x8e\x17\x83x\x16\xef\xad\xe2\xe6q\xcc\xdb\x19o\x0e\xb8\x1c\xc86hI\xc0\x8c@\xf0\xf2\rc\xd4\xc5\xe7\xa0o?\xee\x87M\x9d\xdb;6\x1b\x01g.\x05\x9c\x98\x16\x8f\x1e\xe8\xed{\xa3"
b'\xa9\xeew`j\x85a40\n>>083l\xaa\x15\x06\xbb0\x08q\x10!\xc4\x80\x8d9\x81T\xc0\x05R\xf8\x16\xd3>i>\xf9\xa4\xf9\xe7\x82\xf7\x9a\x03\x9eh\x0ey\x81\xaf\xeeJg\xb9)~\xec\r\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x80\x00\x00a\xa6w\xe9\x9d\xb2\xcb[\xef\xdf,\xd7\xdbe\xb3\xcf\x85u\xc0\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00'
b'\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00L\x00\x93\xdc57\n>>083l\xd3\x00\xddC\xd3\xcf\x93{\xab\xc2\x0080\x0e0\x00\x00\x00\x00\x000@\x00\x00\x7f\xff\xfd\x15\x15\x19\x05))4\xf1\x94\x12\x9c\xcfa\x04Yn\xba\xea\x02\x80\x02\x00\x08\x00 \x00\x80\x02\x00\x08\x000\xc0b\n\xc1bn\x04\xe1\x01\xa7\xcc\xb4\xd9\x8e2ma\xc4\xc4\x13\x83\xbb\xfa\xef\xfc\xaf\xda\x1d\x1cz\x82s\xff7\xecq\\\xd7G\x93\x95~'
b'NV\xf9x\x0e!:\x80\x84\xea\xa2."\xfaY\x85\xe9f\x1f\xaa\x01\xdef.y\x98\xb55D\n>>083l\xe7\xad(\x0e\xa1`:\x87\x017\xfd\xf8Ag\xe1\x04\xff\x94w\x88>` \xf9\x88\x89\xf6\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xfe\x00\x00\r4\xcfM3\xd3<\xe9\xa3\x0cq\xc7\x1cq\x96J\xe0\x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80'
b'\x01\x00\x02\x00\x00\xd0\xf6p\xd3\x00\xe3FS\xcfcN|@\x008z\xcc\x80\x00\x00\x00\x00\x00 \x02\x00\x00\x7f\xff\xfb\xd4368\n>>083l\xfb\xd3\xfb\xd4"\xab\xe4\x00\x00\x00\x00\x00\x05\x03F\\TY\xe0`\xb2\x0f\xb8b-@\x01\x00\x04\x00\x10\x00@\x01\x00\x04\x00\x10\x00@\x01\x00\x07dN\xda\xe2^\xc4\xb2\xfa\xd6\xf6Ni\x1c\xd2\xf8\xbda\x82ds\xc8\xe8nt\xdd\xb8.\xa4f@c`\xfd\x19(\xb3\x11\xfd'  
b'\x0c\x87\xf4\t@%\x9d\x80\x94\x8d\xfe\x86\xc3\xf9\xd5 i\xb5\xc1\xa8-\x82\xf4\xf0\x0b\xc1(E\x0b!\x11C{\x1e\x8b\xeb\xdc\xb0-y\xa0\xad\xc2\x00q\xde\x002B\n>>04FlG/\x83r\xfd\xf8.\x7f\xff\xff\xff\xff\xff\xff\xff\xff\xff\x80\x00\x05\xf9ME\x97\xdf\x95w]\x95\x16Ou\xf8]t\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 \x00@\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00 '
b'\x00\x04\x81[\xda07\n'
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


void test13()
{
    int startIndex=0;
    int ret, i;
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
    // PrintBuffer((uint8_t *)buf2, ret);
    // assert(startIndex==7);
    // for (i=0; i<2; i++)
    // {
    //     ret = InterceptCustomGrilMessage((uint8_t *)buf[i], strlen(buf[i]), CUSTOM_GRIL_BLUETOOTH);
    //     PrintBuffer((uint8_t *)buf[i], ret);
    //     printf("RET: %d\n", ret);
    // }
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
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();
    // test6();
    // test7();

    // test8();
    // test8();
    // test9();
    test10();
    test11();
    test12();
    test13();

    // test20();
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
        int startIndex = 0;
        int cnt = 0;

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

            if (cnt > 10)
            {
                printfLog(D_Info, "ERROR in message [%d]:\r\n", cnt);
                PrintBufferBin(buffer, len);
                break;
            }
            cnt += 1;
        }
        printfLog(D_Info, "Outgoing msg:");
        PrintBufferBin((uint8_t *)buffer, len);
    }
    return len;
}

static int SearchStringEndingLfCr(char *buffer, int len, int *startIndex)
{
    int i;
    bool found=false;
    for (i=*startIndex; i<len; i++)
    {
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
    if (found)
        return CMD_STATE_FOUND;
    else
        return CMD_STATE_PART;
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
            return r;
        len = RemoveCmdPart(cmdStartIndex);
    }
    else
    {
        if (savedBufferLen)
        {
            if (cmdStartIndex != 0)
                printfLog(D_Warning, "WARN: cmdStartInde is not zero %d\r\n", cmdStartIndex);            
            *newStartIndex += savedBufferLen;
            len = AddSavedDataToBuffer(buffer, len, savedBuffer, &savedBufferLen);
        }

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
    printfLog(D_Info, "Save CmdPart %d bytes, total bytes: %d\n", tmpLen, *savedBufferLen);
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