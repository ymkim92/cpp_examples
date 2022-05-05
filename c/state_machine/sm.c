#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// *************************************
#define LRF_BYTE_SYNC								0x59
#define LRF_BYTE_ACK								0x3c

#define LRF_BYTE_CMD_SMM							0xc3
#define LRF_BYTE_CMD_SMM_WITH_STATUS				0x12
#define LRF_BYTE_CMD_SMM_WITH_LOW_VISIBILITY		0xd3
#define LRF_BYTE_CMD_SMM_QUICK						0xdd
#define LRF_BYTE_CMD_CMM							0xda

#define LRF_BYTE_CMD_READ_STATUS					0xc7
#define LRF_BYTE_CMD_READ_RANGE_WINDOW				0x30
#define LRF_BYTE_CMD_READ_ID						0xc0
#define LRF_BYTE_CMD_READ_INFO						0xc2

#define LRF_BYTE_CFG_POINTER						0xc5
#define LRF_BYTE_CFG_MIN_RANGE						0x31
#define LRF_BYTE_CFG_MAX_RANGE						0x32
#define LRF_BYTE_CFG_BAUD_RATE						0xc8
#define LRF_BYTE_CFG_RESET_ERROR_CNT				0xcb
#define LRF_BYTE_QRY_STATUS							0xc7

#define LRF_CFG_ARG_POINTER_OFF						0
#define LRF_CFG_ARG_POINTER_ON						1

#define LRF_CFG_CMM_FREQ_0HZ						0
#define LRF_CFG_CMM_FREQ_1HZ						1
#define LRF_CFG_CMM_FREQ_4HZ						2
#define LRF_CFG_CMM_FREQ_10HZ						3
#define LRF_CFG_CMM_FREQ_20HZ						4
#define LRF_CFG_CMM_FREQ_100HZ						5
#define LRF_CFG_CMM_FREQ_200HZ						6

#define LRF_SIZE_ACK								4
#define LRF_SIZE_CFG_POINTER						3
#define LRF_SIZE_CFG_MIN_RANGE						4
#define LRF_SIZE_CFG_MAX_RANGE						4
#define LRF_SIZE_CMD_READ_INFO						2
#define LRF_SIZE_CMD_READ_ID						2
#define LRF_SIZE_CMD_SMM							2
#define LRF_SIZE_CMD_REPLY_INFO						40
#define LRF_SIZE_CMD_REPLY_ID						73
#define LRF_SIZE_CMD_CMM_CONFIG						3
#define LRF_SIZE_CMD_CMM_DATA						9
#define LRF_SIZE_QRY_STATUS							2
#define LRF_SIZE_REP_QRY_STATUS						6

#define printfLog           printf
// *************************************
#define PrintLogLrfStateMachine(a, b)               printfLog("%s (%u, %u)\r\n", __func__, a, b)

#define LRF_CHECKSUM_XOR_BYTE						0x50
#define LRF_MSG_HEADER_SIZE         2
#define LRF_MSG_CHECKSUM_SIZE       1

#define LRF_MSG_GREETING_END_CHAR   '\n'

/*
LRF (1): 00 
LRF (28): 0A 0D 4C 52 46 31 32 37 2D 4D 34 20 20 20 20 20 20 20 32 2E 34 2E 32 35 2E 32 0D 0A

LRF (73): 59 C0 4C 52 46 31 32 37 2D 4D 34 20 20 20 20 20 20 0D 0A 00 00 01 20 11 00 00 00 11 00 00 00 11 00 00 0D 0A 52 31 32 37 34 32 39 30 37 37 0D 0A 14 14 C1 B6 31 39 2D 31 32 2D 31 30 0D 0A 31 33 3A 30 30 3A 34 37 0D 0A EF 
LRF (4): 59 C5 3C 0A // laser point
LRF (4): 59 31 3C 96 // min range
LRF (4): 59 32 3C 97 // max range
*/




static uint8_t *lrfMessage;
static uint16_t lrfMessageSize;

uint8_t message0[] = { 0x59, 0xDA, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68};
uint16_t size0 = sizeof(message0);
uint8_t message1[] = {0x59, 0xDA, 0x05, 0x00};
uint8_t message2[] = {0x00, 0x00, 0x00, 0x00, 0x68};
uint16_t size1 = sizeof(message1);
uint16_t size2 = sizeof(message2);

uint8_t message3[] = { 0x59, 0xC5, 0x3C, 0x0A };
uint16_t size3 = sizeof(message3);

uint8_t message4[] = { 0x0a, 0x0d, 0x4c, 0x52, 0x46, 0x31, 0x32, 0x37, 0x2d, 0x4d, 0x34, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x32, 0x2e, 0x34, 0x2e, 0x32, 0x35, 0x2e, 0x32, 0x0d, 0x0a};
uint16_t size4 = sizeof(message4);

uint8_t ui8Checksum;

uint8_t ui8LrfData[6];
bool bLrfDataReady= false;
bool bLrfConfigOk= false;

struct lrfMsgState;
typedef void lrfMsgStateFn(struct lrfMsgState *);

struct lrfMsgState
{
    lrfMsgStateFn * m_next;
    uint16_t m_pos; 
    uint16_t m_posFromSync; 
};

lrfMsgStateFn lrfStateGreeting, lrfStateWait, lrfStateStarted, lrfStateData, lrfStateStatus, 
    lrfStateConfigLaserPoint;

bool CompareLrfMessageChecksum(uint8_t calChecksum, uint8_t msgChecksum);

static void PrintBuf(uint8_t *buf, uint16_t size)
{
    int i;
    for (i=0; i<size; i++)
    {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

static uint8_t GetLrfChecksum(uint8_t *buf, uint16_t size)
{
	uint8_t sum=0;
	while (size > 0)
	{
		sum += *buf;
		buf += 1;
		size -= 1;
	}

	return sum ^ LRF_CHECKSUM_XOR_BYTE;
}

void lrfStateGreeting(struct lrfMsgState * state)
{
    state->m_pos = 0;
    state->m_posFromSync = 0;
    PrintLogLrfStateMachine(state->m_pos, state->m_posFromSync);

    if ((lrfMessageSize > 2) && (LRF_MSG_GREETING_END_CHAR == lrfMessage[lrfMessageSize-1]))
    {
        printfLog("%s", lrfMessage);
        state->m_next = lrfStateWait;
    }
}

void lrfStateWait(struct lrfMsgState * state)
{
    state->m_posFromSync = 0;
    PrintLogLrfStateMachine(state->m_pos, state->m_posFromSync);

    ui8Checksum = lrfMessage[state->m_pos];
    if (LRF_BYTE_SYNC == lrfMessage[state->m_pos])
    {
        state->m_next = lrfStateStarted;
        state->m_pos += 1;
        state->m_posFromSync += 1;
    }
    else
    {
        printfLog("Wrong SYNC received at first index\r\n");
        state->m_next = lrfStateWait;
        state->m_pos = lrfMessageSize;
    }
}

void lrfStateStarted(struct lrfMsgState * state)
{
    bool bUnknowMsgType = false;
    PrintLogLrfStateMachine(state->m_pos, state->m_posFromSync);
    ui8Checksum += lrfMessage[state->m_pos];

    if (LRF_BYTE_CMD_CMM == lrfMessage[state->m_pos])
    {
        state->m_next = lrfStateData;
    }
    else if (LRF_BYTE_CMD_READ_STATUS == lrfMessage[state->m_pos])
    {
        state->m_next = lrfStateStatus;
    }
    else if (LRF_BYTE_CFG_POINTER == lrfMessage[state->m_pos])
    {
        state->m_next = lrfStateConfigLaserPoint;
    }
    else
    {
        bUnknowMsgType = true;
    }

    if (bUnknowMsgType)
    {
        state->m_next = lrfStateWait;
        state->m_pos = lrfMessageSize;
    }
    else
    {
        state->m_pos += 1;
        state->m_posFromSync += 1;
    }
}

void lrfStateData(struct lrfMsgState * state)
{
    PrintLogLrfStateMachine(state->m_pos, state->m_posFromSync);
    if ((LRF_SIZE_CMD_CMM_DATA - LRF_MSG_CHECKSUM_SIZE) > state->m_posFromSync)
    {
        ui8Checksum += lrfMessage[state->m_pos];
        ui8LrfData[state->m_posFromSync - LRF_MSG_HEADER_SIZE] = lrfMessage[state->m_pos];
    }
    else
    {
        bLrfDataReady = CompareLrfMessageChecksum(ui8Checksum, lrfMessage[state->m_pos]);
        state->m_next = lrfStateWait;
    }
    state->m_pos += 1;
    state->m_posFromSync += 1;
}

void lrfStateConfigLaserPoint(struct lrfMsgState * state)
{
    PrintLogLrfStateMachine(state->m_pos, state->m_posFromSync);
    if ((LRF_SIZE_ACK - LRF_MSG_CHECKSUM_SIZE) > state->m_posFromSync)
    {
        ui8Checksum += lrfMessage[state->m_pos];
        if (LRF_BYTE_ACK != lrfMessage[state->m_pos])
        {
            printfLog("ACK Error: ACK(0x%x) != 0x%x\r\n", LRF_BYTE_ACK, lrfMessage[state->m_pos]);
            // lrfStats.m_parserErrorAckType += 1
            state->m_next = lrfStateWait;
            state->m_pos += lrfMessageSize;
            return;
        }
    }
    else
    {
        bLrfConfigOk = CompareLrfMessageChecksum(ui8Checksum, lrfMessage[state->m_pos]);
        state->m_next = lrfStateWait;
    }
    state->m_pos += 1;
    state->m_posFromSync += 1;
}

void lrfStateStatus(struct lrfMsgState * state)
{
    PrintLogLrfStateMachine(state->m_pos, state->m_posFromSync);
}

int main(void)
{
    int i=0;
    struct lrfMsgState state = { lrfStateGreeting, 0, 0 };
    printf("bLrfDataReady: %d\n", bLrfDataReady);
    printf("bLrfConfigOk: %d\n", bLrfConfigOk);

    for (i=0; i<2; i++)
    {
        if (i==0)
        {
            lrfMessage = message4;
            lrfMessageSize = size4;
        }
        else
        {
            lrfMessage = message0;
            lrfMessageSize = size0;
        }
        state.m_pos = 0;
        while(state.m_next) 
        {
            state.m_next(&state);
            if (lrfMessageSize <= state.m_pos)
                break;
        }
    }
    printf("bLrfDataReady: %d\n", bLrfDataReady);
    PrintBuf(ui8LrfData, 6);
    printf("\nbLrfConfigOk: %d\n", bLrfConfigOk);
    return 0;
}

bool CompareLrfMessageChecksum(uint8_t calChecksum, uint8_t msgChecksum)
{
    bool ret = (calChecksum ^ LRF_CHECKSUM_XOR_BYTE) == msgChecksum;
    if (ret == false)
    {
        // lrfStats.m_parserErrorChecksum += 1;
    }
    return ret;
}