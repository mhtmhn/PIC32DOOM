/*******************************************************************************
 DOOM Config

 Mohit M 
 4-APR-2021
 *******************************************************************************/

#ifndef _CONFIG_H
#define _CONFIG_H

/* Define to the full name of this package. */
#define PACKAGE_NAME "PIC32DOOM"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "PIC32DOOM 1.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "PIC32DOOM.tar"

/* Change this when you create your awesome forked version */
#define PROGRAM_PREFIX "PIC32DOOM"

/* Define to the directory where all game files are located */
#define FILES_DIR "0:/"

/* Assert block size is set to 1024 for umm_malloc 
 * Note: Find DDR Heap Manager config in common.c 
 */
#ifndef UMM_BLOCK_BODY_SIZE
#error UMM_BLOCK_BODY_SIZE not defined, define and set to 1024!
#else 
#if (UMM_BLOCK_BODY_SIZE < 1024)
#error UMM_BLOCK_BODY_SIZE defined but not set or less than 1024!
#endif
#endif

#endif
