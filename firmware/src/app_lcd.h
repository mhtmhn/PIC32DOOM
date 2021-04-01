/*******************************************************************************
 LCD Graphics Header File

 Mohit M 
 30-MAR-2021
 *******************************************************************************/

#ifndef _APP_LCD_H
#define _APP_LCD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

/* Application state data type */
typedef enum {
    /* Application state machine's initial state */
    APP_LCD_STATE_INIT = 0,

    /* State machine idle state */
    APP_LCD_STATE_IDLE,

} APP_LCD_STATES;

/* Application variable data type */
typedef struct {
    /* The application's current state */
    APP_LCD_STATES state;

} APP_LCD_DATA;

void APP_LCD_Initialize(void);
void APP_LCD_Tasks(void);

#endif /* _APP_LCD_H */
