/*******************************************************************************
 Common Interface Source File

 Mohit M 
 1-APR-2021
 *******************************************************************************/

#include "common.h"

/* Application data */
COMMON_DATA common;

void COMMON_Initialize(void) {
    /* Place the state machine in its initial state */
    common.state = COMMON_STATE_INIT;
}

void COMMON_Tasks(void) {

    /* Check the application's current state. */
    switch (common.state) {
            /* Application's initial state. */
        case COMMON_STATE_INIT:
            break;

            /* State machine idle state */
        case COMMON_STATE_IDLE:
            break;

        default:
            break;
    }
}
