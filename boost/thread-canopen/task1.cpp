#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <stdint.h>
#include <stdbool.h>

#include <time.h>

#include "task1.h"

#define TRUE                            true
#define FALSE                           false

#define PUBLIC
#define MEM
#define MCO_DECL_INSTANCE_PTR_
#define tCopKernel      bool
#define tTime           uint64_t
#define HBC_PROCESS_TIME_CONTROL        TRUE
#define UNUSED(x)       ((void)(x))
#define MCO_CHECK_INSTANCE_STATE        (void)
#define ASSERTMSG(a, b)                 ((void)(a, b))
#define MCO_GLB_VAR (var)                (var)
#define kCopSuccessful                  TRUE
#define kNmtErrCtrlEvHbcConnectionLost  FALSE
#define MCO_INSTANCE_PARAM_IDX_()  
#define tFastByte       uint8_t


bool *m_fpHbcEventCallback = NULL;

int sum(int a)
{
    static int sum = 0;
    sum = sum + a;
    return sum;
}

tTime PUBLIC TgtGetTickCount (void)
{

	tTime  dwTimerTickTemp;
	struct timespec spec;

    //gettimeofday(&Timestamp, &TimeZone);
	clock_gettime(CLOCK_MONOTONIC,&spec);

    dwTimerTickTemp =  spec.tv_sec * 10000;
    dwTimerTickTemp += spec.tv_nsec / 100/1000 ;

    return ((tTime)dwTimerTickTemp);
}


tCopKernel PUBLIC HbcProcess (MCO_DECL_INSTANCE_PTR_
    tTime MEM* pNextCallTime_p)
{

tCopKernel      Ret             = kCopSuccessful;
tHbcInfo MEM*   pHbcInfo;
tTime           DeltaTime;
tFastByte       Count;

	UNUSED(pNextCallTime_p);
#if (HBC_PROCESS_TIME_CONTROL != FALSE)

    tTime       NextCallTime;

#endif // (HBC_PROCESS_TIME_CONTROL != FALSE)

    MCO_CHECK_INSTANCE_STATE ();

    #if (HBC_PROCESS_TIME_CONTROL != FALSE)
    {
        ASSERTMSG (pNextCallTime_p != NULL, "HbcProcess(): pointer pNextCallTime_p should never be NULL!\n");

        NextCallTime = *pNextCallTime_p;
    }
    #endif // (HBC_PROCESS_TIME_CONTROL != FALSE)

    // get number of heartbeat consumers and address to first entry
    pHbcInfo = MCO_GLB_VAR (m_HbcInitParam.m_paHbcProducerTable);
    Count    = (tFastByte) MCO_GLB_VAR (m_HbcInitParam.m_bHbcMaxProducerEntries);

    // walk through whole heartbeat consumer table

    tTime now = TgtGetTickCount ();

    while (Count > 0)
    {
        // printf("YK count: %d\n", Count);
        // check if consumer is active - next consumer if not
        // and check if first message should be received now - next consumer if so
        if ((pHbcInfo->m_CfgConsHbTime != 0) && (pHbcInfo->m_bFirstMsgFlag == FALSE))
        {
            // calculate past time
            DeltaTime =  now - pHbcInfo->m_LastRecvTime;

            // printf("YK delta: %d, cfgConsHbTime: %d\n", (int)DeltaTime, (int)pHbcInfo->m_CfgConsHbTime);

            // check if time runs over
            if (DeltaTime < pHbcInfo->m_CfgConsHbTime)
            {
            #if (HBC_PROCESS_TIME_CONTROL != FALSE)

                // calculate rest of time till next checking
                DeltaTime = pHbcInfo->m_CfgConsHbTime - DeltaTime;

                // check if this time is less than next calling time
                if (NextCallTime > DeltaTime)
                {
                    // get this time as next calling time
                    NextCallTime = DeltaTime;
                }

            #endif // (HBC_PROCESS_TIME_CONTROL != FALSE)
            }
            else
            {
                printf("YK delta: %d, cfgConsHbTime: %d, firstMsgFlag: %d, Count: %d\n", 
                (int)DeltaTime, (int)pHbcInfo->m_CfgConsHbTime, pHbcInfo->m_bFirstMsgFlag, Count);
                // If time runs over, then message was lost.
                // HbcCbReceive() sets m_LastRecvTime to current time. So it should
                // never occur running over this timeout. If so, message was lost.

                // next message is a new message
                // So it is possible to generate event "connected".
                pHbcInfo->m_bFirstMsgFlag = TRUE;

                // send heartbeat event "connection lost"
                if (MCO_GLB_VAR (m_fpHbcEventCallback) != NULL)
                {
                    MCO_GLB_VAR (m_fpHbcEventCallback) (MCO_INSTANCE_PARAM_IDX_()
                        pHbcInfo->m_bNodeId,
                        kNmtErrCtrlEvHbcConnectionLost);
                }

                #if (HBC_PROCESS_TIME_CONTROL != FALSE)
                // check if heartbeat time is less than next calling time
                if (NextCallTime > pHbcInfo->m_CfgConsHbTime)
                {
                    // get this time as next calling time
                    NextCallTime = pHbcInfo->m_CfgConsHbTime;
                }
                #endif // (HBC_PROCESS_TIME_CONTROL != FALSE)
            }
        }

        pHbcInfo++;
        Count--;
    }

    #if (HBC_PROCESS_TIME_CONTROL != FALSE)
    {
        *pNextCallTime_p = NextCallTime;
    }
    #endif // (HBC_PROCESS_TIME_CONTROL != FALSE)

    return Ret;

}

void workerTask()
{
    std::cout << "Task: running" << std::endl;

    sum(2);

    std::cout << "Task: finished" << std::endl;
}
