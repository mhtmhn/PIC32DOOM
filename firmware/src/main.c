/*******************************************************************************
  PIC32DOOM Main Source File

  Mohit M 
  26-MAR-2021
 *******************************************************************************/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "definitions.h"
#include "doom_logo.h"
#include "../../chocolate_doom/doom.h"

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);
    doom_logo();

    /* Start Message */
    printf("Please Insert SD Card\r\n");

    while (true) {
        /* Maintain state machines */
        SYS_Tasks();
        if (COMMON_AllAppsIdle()) {
            printf("Starting...\r\n");
            D_DoomMain();
        }
    }

    return (EXIT_FAILURE);
}
