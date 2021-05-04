// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

/*******************************************************************************
 Target Video Source File

 Mohit M
 8-APR-2021
 *******************************************************************************/

static const char
rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include "config.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_event.h"
#include "d_main.h"
#include "i_video.h"
#include "z_zone.h"

#include "tables.h"
#include "doomkeys.h"
#include "m_controls.h"

#include <stdint.h>
#include <stdbool.h>
#include <common.h>

#define GFX_RGB565(r, g, b) ((((r & 0xF8) >> 3) << 11) | (((g & 0xFC) >> 2) << 5) | ((b & 0xF8) >> 3))
#define GFX_RGB565_R(color) ((0xF800 & color) >> 11)
#define GFX_RGB565_G(color)	((0x07E0 & color) >> 5)
#define GFX_RGB565_B(color)	(0x001F & color)

// The screen buffers; these are modified to draw things to the screen
byte *I_VideoBuffer = NULL;
void *APP_LCD_FrameBuffer = NULL;

// If true, game is running as a screensaver
boolean screensaver_mode = false;

// Flag indicating whether the screen is currently visible:
// when the screen isnt visible, don't render the screen
boolean screenvisible;

// Mouse acceleration
//
// This emulates some of the behavior of DOS mouse drivers by increasing
// the speed when the mouse is moved fast.
//
// The mouse input values are input directly to the game, but when
// the values exceed the value of mouse_threshold, they are multiplied
// by mouse_acceleration to increase the speed.
float mouse_acceleration = 2.0;
int mouse_threshold = 10;

// Gamma correction level to use
int usegamma = 1;

// Mouse enable
int usemouse = 0;

// If true, keyboard mapping is ignored, like in Vanilla Doom.
// The sensible thing to do is to disable this if you have a non-US
// keyboard.
int vanilla_keyboard_mapping = true;

// Weapon switcher variable from g_game.c, 
// doesn't pick weapon if it's not available
// Set to +1 or -1, for next or previous respectively
// Here we only use +1
extern int next_weapon;

/* Input System Service */
/* Input listener instance */
SYS_INP_InputListener input_listener;

/* Touch point x */
uint32_t touch_x;

/* Touch point y */
uint32_t touch_y;

/* Touch state */
bool touch_pressed;

/* Opacity of touch controls layer */
uint32_t controls_opacity = 0;

typedef struct {
    byte r;
    byte g;
    byte b;
} colour_t;

// Palette converted to RGB565
static uint16_t rgb565_palette[256];

// Debug print
#define DEBUG 0

/* Static Functions */
static void touchDownHandler(const SYS_INP_TouchStateEvent * const evt) {
    if (evt->index == 0) {
        touch_pressed = true;
        touch_x = evt->x;
        touch_y = evt->y;
    }
}

static void touchUpHandler(const SYS_INP_TouchStateEvent * const evt) {
    if (evt->index == 0) {
        touch_pressed = false;
    }
}

void I_InitGraphics(void) {
    I_VideoBuffer = (byte*) Z_Malloc(SCREENWIDTH * SCREENHEIGHT, PU_STATIC, NULL);
    COMMON_APP_LCD_GetFBAddress(&APP_LCD_FrameBuffer);
    screenvisible = true;

    /* Initialize Input System Service */
    input_listener.handleTouchDown = &touchDownHandler;
    input_listener.handleTouchUp = &touchUpHandler;
    SYS_INP_AddListener(&input_listener);
    touch_pressed = false;

}

void I_ShutdownGraphics(void) {
    Z_Free(I_VideoBuffer);
}

void I_StartFrame(void) {

}

void I_GetEvent(void) {
    static bool first_run = true, unhide_controls = false, prev_state = false;
    static int key_mem = -1;

    event_t event;

    if (prev_state != touch_pressed) {
        prev_state = touch_pressed;
        event.type = prev_state ? ev_keydown : ev_keyup;
        /* Only on release, else will fire twice */
        event.data1 = prev_state ? -1 : key_mem;
        event.data2 = -1;
        event.data3 = -1;

        /* Make sure previous key is released irrespective of release position */
        D_PostEvent(&event);

        if ((touch_x >= 106 && touch_y >= 79)
                && (touch_x <= 158 && touch_y <= 116)) {
            event.data1 = KEY_STRAFE_R;
#if DEBUG
            printf("key strafe right\n");
#endif
        }

        if ((touch_x >= 0 && touch_y >= 78)
                && (touch_x <= 55 && touch_y <= 116)) {
            event.data1 = KEY_STRAFE_L;
#if DEBUG
            printf("key strafe left\n");
#endif
        }

        if ((touch_x >= 61 && touch_y >= 113)
                && (touch_x <= 100 && touch_y <= 150)) {
            event.data1 = KEY_UPARROW;
#if DEBUG
            printf("key uparr\n");
#endif
        }

        if ((touch_x >= 61 && touch_y >= 196)
                && (touch_x <= 100 && touch_y <= 240)) {
            event.data1 = KEY_DOWNARROW;
#if DEBUG
            printf("key downarr\n");
#endif
        }

        if ((touch_x >= 103 && touch_y >= 152)
                && (touch_x <= 149 && touch_y <= 197)) {
            event.data1 = KEY_RIGHTARROW;
#if DEBUG
            printf("key rightarr\n");
#endif
        }

        if ((touch_x >= 9 && touch_y >= 153)
                && (touch_x <= 55 && touch_y <= 197)) {
            event.data1 = KEY_LEFTARROW;
#if DEBUG
            printf("key leftarr\n");
#endif
        }

        if ((touch_x >= 362 && touch_y >= 134)
                && (touch_x <= 404 && touch_y <= 174)) {
            event.data1 = KEY_ENTER;
            D_PostEvent(&event);
            event.data1 = KEY_USE;
#if DEBUG
            printf("key use/enter\n");
#endif
        }

        if ((touch_x >= 406 && touch_y >= 178)
                && (touch_x <= 450 && touch_y <= 220)) {
            event.data1 = KEY_FIRE;
#if DEBUG
            printf("key fire\n");
#endif
        }

        if ((touch_x >= 0 && touch_y >= 0)
                && (touch_x <= 84 && touch_y <= 38)) {
            event.data1 = KEY_ESCAPE;
#if DEBUG
            printf("key escape\n");
#endif
        }

        if ((touch_x >= 160 && touch_y >= 6)
                && (touch_x <= 232 && touch_y <= 32)) {
            event.data1 = KEY_F2;
#if DEBUG
            printf("key save\n");
#endif
        }

        if ((touch_x >= 244 && touch_y >= 6)
                && (touch_x <= 326 && touch_y <= 32)) {
            event.data1 = KEY_F3;
#if DEBUG
            printf("key load\n");
#endif
        }

        if ((touch_x >= 396 && touch_y >= 0)
                && (touch_x <= 480 && touch_y <= 38)) {
            event.data1 = -1;
            next_weapon = 1;
#if DEBUG
            printf("key weapon switch\n");
#endif
        }

        if (first_run) {
            unhide_controls = true;
            first_run = false;
        }

        key_mem = event.data1;
        D_PostEvent(&event);
    }

    if (unhide_controls) {
        controls_opacity += 17;
        /* The secret menu uses a number within [0,15]*17, default is 8*17 */
        if (controls_opacity >= 136) {
            unhide_controls = false;
        }
        GFX_Set(GFXF_LAYER_ACTIVE, 1);
        GFX_Set(GFXF_LAYER_ALPHA_AMOUNT, controls_opacity);
    }
}

void I_StartTic(void) {

    I_GetEvent();
}

void I_UpdateNoBlit(void) {
}

void I_FinishUpdate(void) {
    int x, y;
    byte index;

    for (y = 0; y < SCREENHEIGHT; y++) {
        for (x = 0; x < SCREENWIDTH; x++) {

            index = I_VideoBuffer[y * SCREENWIDTH + x];
            ((uint16_t*) APP_LCD_FrameBuffer)[y * SCREENWIDTH + x] =
                    rgb565_palette[index];
        }
    }

    /* Trigger VSync to blit using GPU */
    COMMON_APP_LCD_VSyncInterruptEnable();
}

void I_ReadScreen(byte* scr) {

    memcpy(scr, I_VideoBuffer, SCREENWIDTH * SCREENHEIGHT);
}

void I_SetPalette(byte* palette) {
    int i;
    colour_t* c;

    for (i = 0; i < 256; i++) {

        c = (colour_t*) palette;

        rgb565_palette[i] = GFX_RGB565(
                gammatable[usegamma][c->r],
                gammatable[usegamma][c->g],
                gammatable[usegamma][c->b]);

        palette += 3;
    }
}

// Given an RGB value, find the closest matching palette index.

int I_GetPaletteIndex(int r, int g, int b) {
    int best, best_diff, diff;
    int i;
    colour_t color;

    best = 0;
    best_diff = INT_MAX;

    for (i = 0; i < 256; ++i) {
        color.r = GFX_RGB565_R(rgb565_palette[i]);
        color.g = GFX_RGB565_G(rgb565_palette[i]);
        color.b = GFX_RGB565_B(rgb565_palette[i]);

        diff = (r - color.r) * (r - color.r)
                + (g - color.g) * (g - color.g)
                + (b - color.b) * (b - color.b);

        if (diff < best_diff) {
            best = i;
            best_diff = diff;
        }

        if (diff == 0) {

            break;
        }
    }

    return best;
}

void I_BeginRead(void) {
}

void I_EndRead(void) {
}

void I_SetWindowTitle(char *title) {
}

void I_GraphicsCheckCommandLine(void) {
}

void I_SetGrabMouseCallback(grabmouse_callback_t func) {
}

void I_EnableLoadingDisk(void) {
}

void I_BindVideoVariables(void) {
}

void I_DisplayFPSDots(boolean dots_on) {
}

void I_CheckIsScreensaver(void) {
}
