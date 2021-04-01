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
} COMMON_DATA;

void COMMON_Initialize(void);
void COMMON_Tasks(void);

#endif /* _COMMON_H */
