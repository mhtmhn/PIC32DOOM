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
#include "gfx/driver/processor/2dgpu/libnano2D.h"

/* LCD Parameters */
#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define LCD_BITS_PERPIXEL 16
#define LCD_FRAME_BYTES ((LCD_WIDTH*LCD_HEIGHT*LCD_BITS_PERPIXEL)/8)

/* Framebuffer Parameters */
#define FB_WIDTH 320
#define FB_HEIGHT 200 
#define FB_BITS_PERPIXEL 16
#define FB_FRAME_BYTES ((FB_WIDTH*FB_HEIGHT*FB_BITS_PERPIXEL)/8)

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

    /* nano2D GPU variables */
    /* GLCD layer 0*/
    n2d_buffer_t glcdlayer0;

    /* GLCD layer 0 rectangle */
    n2d_rectangle_t rectglcd0;

    /* Framebuffer */
    n2d_buffer_t framebuffer;

    /* Framebuffer rectangle */
    n2d_rectangle_t rectfbuff;

    /* Framebuffer orientation */
    n2d_orientation_t orientation;

    /* Trigger update callback on VSync */
    bool update;

} APP_LCD_DATA;

void APP_LCD_VSync_Handler(void);
void APP_LCD_Initialize(void);
void APP_LCD_Tasks(void);

#endif /* _APP_LCD_H */
