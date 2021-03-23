#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PPS_PERIOD_MS               1000   // This needs to be adjusted if pps period setting in GPS configuration script is changed

typedef struct 
{
    uint64_t m_ppsTimeUs;
    uint64_t m_prevPpsTimeUs;
    uint64_t m_timeStampMs;
    uint32_t m_timeMs;    
} bltime_t;
/*
    timestamp                                                                m_time
BL: 20935000  ppsTimeMs 106732, m_ppsTimeMs 107732, m_prevPpsTimeMs 106732 1 20934800 800 107532 ms
BL: 20935200  ppsTimeMs 107732, m_ppsTimeMs 107732, m_prevPpsTimeMs 106732 0 20935000 0 107732 ms
BL: 20935400  ppsTimeMs 107732, m_ppsTimeMs 107732, m_prevPpsTimeMs 106732 0 20935200 200 107932 ms
BL: 20935600  ppsTimeMs 107732, m_ppsTimeMs 107732, m_prevPpsTimeMs 106732 0 20935400 400 108132 ms
BL: 20935800  ppsTimeMs 107732, m_ppsTimeMs 107732, m_prevPpsTimeMs 106732 0 20935600 600 108332 ms
BL: 20936000  ppsTimeMs 107732, m_ppsTimeMs 108732, m_prevPpsTimeMs 107732 1 20935800 800 108532 ms
BL: 20936200  ppsTimeMs 108732, m_ppsTimeMs 108732, m_prevPpsTimeMs 107732 0 20936000 0 108732 ms
BL: 20936400  ppsTimeMs 108732, m_ppsTimeMs 108732, m_prevPpsTimeMs 107732 0 20936200 200 108932 ms
BL: 20936600  ppsTimeMs 108732, m_ppsTimeMs 108732, m_prevPpsTimeMs 107732 0 20936400 400 109132 ms
BL: 20936800  ppsTimeMs 108732, m_ppsTimeMs 108732, m_prevPpsTimeMs 107732 0 20936600 600 109332 ms
BL: 20937000  ppsTimeMs 108732, m_ppsTimeMs 109732, m_prevPpsTimeMs 108732 1 20936800 800 109532 ms
BL: 20937200  ppsTimeMs 109732, m_ppsTimeMs 109732, m_prevPpsTimeMs 108732 0 20937000 0 109732 ms
BL: 20937400  ppsTimeMs 109732, m_ppsTimeMs 109732, m_prevPpsTimeMs 108732 0 20937200 200 109932 ms
BL: 20937600  ppsTimeMs 109732, m_ppsTimeMs 109732, m_prevPpsTimeMs 108732 0 20937400 400 110132 ms
BL: 20937800  ppsTimeMs 109732, m_ppsTimeMs 109732, m_prevPpsTimeMs 108732 0 20937600 600 110332 ms
BL: 20938000  ppsTimeMs 109732, m_ppsTimeMs 110732, m_prevPpsTimeMs 109732 1 20937800 800 110532 ms
BL: 20938200  ppsTimeMs 110732, m_ppsTimeMs 110732, m_prevPpsTimeMs 109732 0 20938000 0 110732 ms

*/
uint32_t Hwi_disable();
void Hwi_restore(uint32_t hwikey);

uint64_t TodToSystemTimeUs(uint32_t tod, bool useprevppstime /* = false*/, bool compensatertbeforepps /* = false*/);
void PrintPacketTime(bltime_t *bl);

uint64_t m_ppsTimeUs;        // Local time (Us) of last PPS event.
uint64_t m_prevPpsTimeUs;    // Local time (Us) of previous PPS event.

int main()
{
    bltime_t bl;

    int32_t bline_time;
    uint32_t bline_timeStamp, packet_timeStampUs;

    //              m_ppsTimeUs m_prevPpsTimeUs m_timeStampMs m_timeMs   
    bl = (bltime_t){107732000,  106732000,      20935000,     20934800};
    PrintPacketTime(&bl);    

    bl = (bltime_t){107732000, 106732000, 20935200, 20935000};
    PrintPacketTime(&bl);    

    printf("\n");
    bl = (bltime_t){107732000, 106732000, 86399600, 86399400};
    PrintPacketTime(&bl);    
    bl = (bltime_t){107732000, 106732000, 86399800, 86399600};
    PrintPacketTime(&bl);    
    bl = (bltime_t){108732000, 107732000, 0, 86399800};
    PrintPacketTime(&bl);    
    return 0;
}

void PrintPacketTime(bltime_t *bl)    
{
    uint64_t packet_timeStampUs;

    m_ppsTimeUs = bl->m_ppsTimeUs;
    m_prevPpsTimeUs = bl->m_prevPpsTimeUs;
    // bool usePrevPpsTs = (bl->m_timeMs < bl->m_timeStampMs) && ((bl->m_timeStampMs % 1000) == 0);
    bool usePrevPpsTs = (bl->m_timeStampMs % 1000) == 0;

    packet_timeStampUs = TodToSystemTimeUs(bl->m_timeMs, usePrevPpsTs, false); // convert to local time
    // bl->m_timeStampMs, bl->m_timeMs, usePrevPpsTs, packet_timeStampUs, (uint32_t)(packet_timeStampUs/1000));
    printf("%10lu, %10u, %d, %10lu, %10u\n", bl->m_timeStampMs, bl->m_timeMs, usePrevPpsTs, packet_timeStampUs, (uint32_t)(packet_timeStampUs/1000));
}

uint64_t TodToSystemTimeUs
(
        uint32_t tod,
        bool useprevppstime /* = false*/,
        bool compensatertbeforepps /* = false*/
)
{
    uint64_t result = 0;
    uint64_t ppsTimeUs = 0;

    uint32_t hwikey = Hwi_disable();
    if(useprevppstime == true)
    {
        ppsTimeUs = m_prevPpsTimeUs;
    }
    else
    {
        static uint64_t lastPpstime = 0;

        if(m_ppsTimeUs != lastPpstime && (tod % PPS_PERIOD_MS) != 0)
        {
            ppsTimeUs = m_prevPpsTimeUs;
        }
        else
        {
            ppsTimeUs = m_ppsTimeUs;
        }
        lastPpstime = ppsTimeUs;

    }
    Hwi_restore(hwikey);

    uint64_t ppsOffsetUs = ((tod % PPS_PERIOD_MS) * 1000); // Extract time after PPS and convert to us

    // Has the first PPS event happened?
    if(ppsTimeUs != 0)
    {
        result = ppsTimeUs + ppsOffsetUs;

        // Sometimes in HD2 AUX mode, gps sends messages at the start of 1000ms epoch (tod % 1000 == 0)
        // before PPS is generated. This happens because messages are already synced with PPS signal in
        // MAIN gps but AUX PPS is still not.
        // This code works around this issue.
        if(compensatertbeforepps == true)
        {
            static uint64_t lastUsedPpsTimestamp = 0;
            if((ppsOffsetUs == 0) && (lastUsedPpsTimestamp == ppsTimeUs))// Was PPS event supposed to happen but did not?
            {
                // compensate
                result += PPS_PERIOD_MS * 1000; // to uS
            }
            lastUsedPpsTimestamp = ppsTimeUs;
        }
    }

    return result;
}

uint32_t Hwi_disable()
{
    return 0;
}
void Hwi_restore(uint32_t hwikey)
{

}
