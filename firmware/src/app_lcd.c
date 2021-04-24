/*******************************************************************************
 LCD Graphics Source File

 Mohit M 
 30-MAR-2021
 *******************************************************************************/

#include "app_lcd.h"
#include "common.h"
#include "gfx/driver/controller/glcd/plib_glcd.h"
#include "peripheral/evic/plib_evic.h"
#include <sys/kmem.h>

/* Application data */
APP_LCD_DATA app_lcd;

void APP_LCD_VSync_Handler(void) {
    /* Blit framebuffer using nano2D GPU */
    n2d_blit(&app_lcd.glcdlayer0, N2D_NULL,
            &app_lcd.framebuffer, N2D_NULL,
            N2D_BLEND_NONE);

    /* Update callback on VSync */
    if (app_lcd.update)
        COMMON_APP_LCD_UpdateOnVSync();

    /* Disable and clear GLCD VSYNC interrupt */
    COMMON_APP_LCD_VSyncInterruptDisable();
    EVIC_SourceStatusClear(INT_SOURCE_GLCD);
}

void APP_LCD_Initialize(void) {
    /* Place the state machine in its initial state */
    app_lcd.state = APP_LCD_STATE_INIT;

    /* GFX Init */
    if (GFX_Open(0, 0, 0, NULL) == NULL)
        __builtin_software_breakpoint();

    /* GFX Layer 0 Configuration */
    GFX_Set(GFXF_LAYER_ACTIVE, 0);
    GFX_Set(GFXF_LAYER_BUFFER_COUNT, 1);
    GFX_Set(GFXF_LAYER_POSITION, 0, 0);
    GFX_Set(GFXF_COLOR_MODE, GFX_COLOR_MODE_RGBA_8888);
    GFX_Set(GFXF_LAYER_ALPHA_AMOUNT, 255);
    GFX_Set(GFXF_LAYER_VISIBLE, GFX_TRUE);
    GFX_Set(GFXF_LAYER_ENABLED, GFX_TRUE);

    /* GFX Layer 1 Configuration */
    GFX_Set(GFXF_LAYER_ACTIVE, 1);
    GFX_Set(GFXF_LAYER_BUFFER_COUNT, 1);
    GFX_Set(GFXF_LAYER_POSITION, 0, 0);
    GFX_Set(GFXF_COLOR_MODE, GFX_COLOR_MODE_RGBA_8888);
    GFX_Set(GFXF_LAYER_ALPHA_AMOUNT, 255);
    GFX_Set(GFXF_LAYER_VISIBLE, GFX_TRUE);
    GFX_Set(GFXF_LAYER_ENABLED, GFX_TRUE);

    /* nano2D GPU Configuration */
    /* Framebuffer orientation */
    app_lcd.orientation = N2D_0;

    /* GLCD Layer 0 */
    GFX_Set(GFXF_LAYER_ACTIVE, 0);
    GFX_Get(GFXF_LAYER_BUFFER_ADDRESS, 0, &app_lcd.glcdlayer0.memory);
    app_lcd.glcdlayer0.gpu = KVA_TO_PA(app_lcd.glcdlayer0.memory);
    app_lcd.glcdlayer0.format = N2D_RGBA8888;
    app_lcd.glcdlayer0.width = LCD_WIDTH;
    app_lcd.glcdlayer0.height = LCD_HEIGHT;
    app_lcd.glcdlayer0.orientation = app_lcd.orientation;
    app_lcd.glcdlayer0.stride = app_lcd.glcdlayer0.width * LCD_BITS_PERPIXEL / 8;

    /* GLCD Layer 1 */
    GFX_Set(GFXF_LAYER_ACTIVE, 1);
    GFX_Get(GFXF_LAYER_BUFFER_ADDRESS, 0, &app_lcd.glcdlayer1.memory);
    app_lcd.glcdlayer1.gpu = KVA_TO_PA(app_lcd.glcdlayer1.memory);
    app_lcd.glcdlayer1.format = N2D_RGBA8888;
    app_lcd.glcdlayer1.width = LCD_WIDTH;
    app_lcd.glcdlayer1.height = LCD_HEIGHT;
    app_lcd.glcdlayer1.orientation = app_lcd.orientation;
    app_lcd.glcdlayer1.stride = app_lcd.glcdlayer1.width * LCD_BITS_PERPIXEL / 8;

    /* Framebuffer */
    app_lcd.framebuffer.memory = (void*) ((uint8_t*) app_lcd.glcdlayer1.memory +
            LCD_FRAME_BYTES);
    app_lcd.framebuffer.gpu = KVA_TO_PA(app_lcd.framebuffer.memory);
    app_lcd.framebuffer.format = N2D_RGB565;
    app_lcd.framebuffer.width = FB_WIDTH;
    app_lcd.framebuffer.height = FB_HEIGHT;
    app_lcd.framebuffer.orientation = app_lcd.orientation;
    app_lcd.framebuffer.stride = app_lcd.framebuffer.width * FB_BITS_PERPIXEL / 8;

    /* Update callback on VSync */
    app_lcd.update = true;
}

void APP_LCD_Tasks(void) {

    /* Check the application's current state. */
    switch (app_lcd.state) {
            /* Application's initial state. */
        case APP_LCD_STATE_INIT:
            /* Relay app state to common interface */
            COMMON_SetAppIdle(COMMON_APP_LCD, false);
            /* Init LCD memory */
            n2d_fill(&app_lcd.glcdlayer0, N2D_NULL, 0xFF000000, N2D_BLEND_NONE);
            n2d_fill(&app_lcd.glcdlayer1, N2D_NULL, 0, N2D_BLEND_NONE);
            /* All done, Idle */
            app_lcd.state = APP_LCD_STATE_IDLE;
            break;

            /* State machine idle state */
        case APP_LCD_STATE_IDLE:
            /* Relay app state to common interface */
            COMMON_SetAppIdle(COMMON_APP_LCD, true);
            break;

            /* The default state should never be executed */
        default:
            break;
    }
}

/* APP_LCD Interface */

/* Disable VSync */
inline void COMMON_APP_LCD_VSyncInterruptDisable(void) {
    GLCDINTbits.VSYNCINT = 0;
}

/* Enable VSync, this also triggers a blit of the frame buffer */
inline void COMMON_APP_LCD_VSyncInterruptEnable(void) {
    GLCDINTbits.VSYNCINT = 1;
}

/* Update on Vsync, called inside the VSync Handler */
void COMMON_APP_LCD_UpdateOnVSync(void) {
    /* Turn on LCD backlight */
    TM4301B_BACKLIGHT_Set();
    app_lcd.update = false;
}

/* Get Framebuffer Address */
void COMMON_APP_LCD_GetFBAddress(void **address) {
    *address = app_lcd.framebuffer.memory;
}

/* Get unallocated 32-bit aligned DDR start address and size */
size_t COMMON_APP_LCD_GetFreeDDR(void **address) {
    /* Align to avoid issues while casting and de-referencing */
    uint32_t addr = (uint32_t) ((uint8_t*) app_lcd.framebuffer.memory +
            FB_FRAME_BYTES);

    while (addr % 4 != 0) {
        addr++;
    }

    *address = (void*) addr;

    /* Hardcoded upper bound 0xA9E00000 as specified by libnano2D_hal.c 
     * was chosen for compatibility with other PIC32MZ Chips with only 32 MB 
     * of internal DDR. AFAIK DOOM doesn't need more than 16 MiB.
     */
    uint32_t size = 0xA9E00000 - addr;

    while (size % 4 != 0) {
        size--;
    }

    return size;
}