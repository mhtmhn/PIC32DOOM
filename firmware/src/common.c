/*******************************************************************************
 Common Interface Source File

 Mohit M 
 1-APR-2021
 *******************************************************************************/

#include "common.h"
#include "peripheral/evic/plib_evic.h"

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

/* APP_LCD Interface*/

/* Disable VSync*/
inline void COMMON_APP_LCD_VSyncInterruptDisable(void) {
    GLCDINTbits.VSYNCINT = 0;
}

/* Enable VSync, this also triggers a blit of the frame buffer*/
inline void COMMON_APP_LCD_VSyncInterruptEnable(void) {
    GLCDINTbits.VSYNCINT = 1;
}

/* Update on Vsync, called inside the VSync Handler */
__WEAK void COMMON_APP_LCD_UpdateOnVSync(void) {

}
