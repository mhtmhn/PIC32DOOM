/*******************************************************************************
 Common Interface Header File

 Mohit M 
 1-APR-2021
 *******************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

/* APP index */
typedef enum {
    /* File System Handler */
    COMMON_APP_FS = 0,

    /* LCD Graphics */
    COMMON_APP_LCD,

} COMMON_APPS;

/* Application state data type */
typedef enum {
    /* Application state machine's initial state */
    COMMON_STATE_INIT = 0,

    /* State machine idle state */
    COMMON_STATE_IDLE,

} COMMON_STATES;

/* Application variable data type */
typedef struct {
    /* The application's current state */
    COMMON_STATES state;

    /* Time in ms since boot */
    uint32_t ticks;

    /* APP_FS Idle flag*/
    bool app_fs_idle;

    /* APP_LCD Idle flag*/
    bool app_lcd_idle;

} COMMON_DATA;

void COMMON_Initialize(void);
void COMMON_Tasks(void);

/* COMMON Interface Utilities */

/* Set Idle flag */
void COMMON_SetAppIdle(COMMON_APPS app, bool is_idle);

/* Get app Idle flag */
bool COMMON_GetAppIdle(COMMON_APPS app);

/* Checks if all apps are Idle */
bool COMMON_AllAppsIdle(void);

/* Time in ms since boot */
inline uint32_t COMMON_GetTicks(void);

/* Delay/Sleep */
inline void COMMON_DelayMS(uint32_t);

/* DDR Heap Manager variables*/
void *UMM_MALLOC_CFG_HEAP_ADDR;
uint32_t UMM_MALLOC_CFG_HEAP_SIZE;

/* APP_LCD Interface */

/* Disable VSync */
inline void COMMON_APP_LCD_VSyncInterruptDisable(void);

/* Enable VSync, this also triggers a blit of the frame buffer */
inline void COMMON_APP_LCD_VSyncInterruptEnable(void);

/* Update on Vsync */
void COMMON_APP_LCD_UpdateOnVSync(void);

/* Get Framebuffer Address */
void COMMON_APP_LCD_GetFBAddress(void **);

/* Get unallocated 32-bit aligned DDR start address and size */
size_t COMMON_APP_LCD_GetFreeDDR(void **);

#endif /* _COMMON_H */
