/*******************************************************************************
 File System Handler Header File

 Mohit M 
 26-MAR-2021
 *******************************************************************************/

#ifndef _APP_FS_H
#define _APP_FS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

/* Application state data type */
typedef enum {
    /* Application's state machine's initial state. */
    /* State machine waits for the sdcard to be mounted */
    APP_FS_STATE_MOUNT_WAIT = 0,

    /* State machine idle state */
    APP_FS_STATE_IDLE,

} APP_FS_STATES;

/* Application variable data type */
typedef struct {
    /* The application's current state */
    APP_FS_STATES state;

    /* Flag to indicate media mount status */
    volatile bool mounted;

} APP_FS_DATA;

void APP_FS_Initialize(void);
void APP_FS_Tasks(void);

#endif /* _APP_FS_H */
