/*******************************************************************************
 LCD Graphics Source File

 Mohit M 
 30-MAR-2021
 *******************************************************************************/

#include "app_lcd.h"
#include "common.h"

/* Application data */
APP_LCD_DATA app_lcd;

void APP_LCD_Initialize(void) {
    /* Place the state machine in its initial state */
    app_lcd.state = APP_LCD_STATE_INIT;
}

void APP_LCD_Tasks(void) {

    /* Check the application's current state. */
    switch (app_lcd.state) {
            /* Application's initial state. */
        case APP_LCD_STATE_INIT:
            break;

            /* State machine idle state */
        case APP_LCD_STATE_IDLE:
            break;

        default:
            break;
    }
}
