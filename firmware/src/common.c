/*******************************************************************************
 Common Interface Source File

 Mohit M 
 1-APR-2021
 *******************************************************************************/

#include "common.h"
#include "peripheral/evic/plib_evic.h"
#include "../../umm_malloc/umm_malloc.h"

/* Application data */
COMMON_DATA common;

/* COMMON Interface Utilities*/

/* DDR Heap Manager variables*/
void *UMM_MALLOC_CFG_HEAP_ADDR = NULL;
uint32_t UMM_MALLOC_CFG_HEAP_SIZE = 0;

/* TIME system service callback */
static void COMMOM_TickCounter(uintptr_t dummy) {
    common.ticks++;
}

/* Time in ms since boot */
uint32_t COMMON_GetTicks(void) {
    return common.ticks;
}

/* Delay/Sleep */
inline void COMMON_DelayMS(uint32_t delay_ms) {
    CORETIMER_DelayMs(delay_ms);
}

void COMMON_Initialize(void) {
    /* Place the state machine in its initial state */
    common.state = COMMON_STATE_INIT;
    common.ticks = 0;
}

void COMMON_Tasks(void) {

    /* Check the application's current state */
    switch (common.state) {
            /* Application's initial state */
        case COMMON_STATE_INIT:
            /* Setup 1 ms tick counter callback */
            SYS_TIME_CallbackRegisterMS(COMMOM_TickCounter, NULL, 
                    1, SYS_TIME_PERIODIC);
            /* Initialize DDR2 heap manager */
            UMM_MALLOC_CFG_HEAP_SIZE = 
                    COMMON_APP_LCD_GetFreeDDR(&UMM_MALLOC_CFG_HEAP_ADDR);
            printf("Allocating Heap at %p, %X bytes \r\n", 
                    UMM_MALLOC_CFG_HEAP_ADDR, 
                    UMM_MALLOC_CFG_HEAP_SIZE);
            umm_init();
            /* All done, Idle */
            common.state = COMMON_STATE_IDLE;
            break;

            /* State machine idle state */
        case COMMON_STATE_IDLE:
            break;

        default:
            break;
    }
}
