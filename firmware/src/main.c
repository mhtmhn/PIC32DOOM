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

    /* Run until all critical tasks are performed */
    while (true) {
        /* Maintain state machines */
        SYS_Tasks();
        /* All critical tasks done, run game */
        if (COMMON_AllAppsIdle()) {
            printf("Starting...\r\n");
            D_DoomMain();
            break;
        }
    }

    /* Run Forever */
    while (true) {
        /* Maintain state machines */
        SYS_Tasks();
        /* Perform game engine tasks */
        D_DoomTask();
    }

    return (EXIT_FAILURE);
}
