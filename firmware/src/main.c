/*******************************************************************************
  PIC32DOOM Main Source File

  Mohit M 
  26-MAR-2021
 *******************************************************************************/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "definitions.h"

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);

    while (true) {
        /* Maintain state machines */
        SYS_Tasks();
    }

    return (EXIT_FAILURE);
}
