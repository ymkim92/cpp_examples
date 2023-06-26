# Message format

This is a typical message format
```c
typedef struct
{
    // header
    uint8_t m_sync1;
    uint8_t m_sync2;
    uint8_t m_type;
    uint8_t m_reserved;

    // crc
    uint16_t m_crc;

    // body
    uint16_t m_dataLen;
    uint8_t *m_data;
} Message_t;
```

# implement pack/unpack function

## top down approach (?)
The approach implements whole packet encode/decode functions.
Then it implements the pack/unpack functions only for data part.

```c
#define INIT_CRC 0
#define PACKET_HEADER_LEN 4

typedef struct
{
    uint8_t m_dataX;
    uint8_t m_dataY;
    uint8_t m_dataZ;
    uint8_t m_reserved2;
} Data1_t;


void EncodeMessagePacket(uint8_t* packet, Message_t* message)
{
    memcpy(packet, message, PACKET_HEADER_LEN);
    memcpy(&packet[PACKET_HEADER_LEN], message->m_data, message->m_dataLen);

    uint16_t index = PACKET_HEADER_LEN + message->m_dataLen;
    SetCrc(CalculateCrc(packet, index));
}

void DecodeMessagePacket(Message_t* message, uint8_t* packet, uint16_t packetLen)
{
    // compare the received crc with calculated crc
    // rxCrc = ..

    // header
    memcpy(message, packet, PACKET_HEADER_LEN);

    // data
    message->m_dataLen = packetLen - PACKET_HEADER_LEN - SPI_PACKET_CRC_LEN;
    memcpy(message->m_data, &packet[PACKET_HEADER_LEN], message->m_dataLen);

    // crc
    message->m_crc = rxCrc;
}

void PackData1(uint8_t* dst, Data1_t* data1)
{

}

void UnpackData1(Data1_t* data1, uint8_t* dst)
{

}
```

## bottom up approach (?)
You can define `SetHeader()` and `SetCrc()` functions.
Then use them in each pack/unpack functions.



```c

typedef struct
{
    // header
    uint8_t m_sync1;
    uint8_t m_sync2;
    uint8_t m_type;
    uint8_t m_reserved;

    // crc
    uint16_t m_crc;

    // body
    uint8_t m_dataX;
    uint8_t m_dataY;
    uint8_t m_dataZ;
    uint8_t m_reserved2;
} Data1_t;

size_t PackData1(uint8_t *outMsg, Data1_t *inMsg)
{
    SetHeader(outMsg, (uint8_t *)inMsg);
    // assign body part
    SetCrc16(outMsg, LEN_ERROR_DIAG_REPLY);

    return LEN_ERROR_DIAG_REPLY;
}
```
