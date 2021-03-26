/*******************************************************************************
  PIC32DOOM Main File

  Mohit M 
  26-MAR-2021
 *******************************************************************************/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "definitions.h"
#include "doom_logo.h"

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);
    /* Terminal splash screen */
    doom_logo();
    printf("\r\nSystem Initialized\r\n");

    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();
    }

    return (EXIT_FAILURE);
}
