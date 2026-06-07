#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/touch.h>
#include <psp2/motion.h>
#include <vita2d.h>

// horrible, please remember to NOT do this in your own code, kids!
#include "analog.png.h"
#include "background.png.h"
#include "circle.png.h"
#include "cross.png.h"
#include "dpad.png.h"
#include "finger_blue.png.h"
#include "finger_gray.png.h"
#include "ltrigger.png.h"
#include "rtrigger.png.h"
#include "select.png.h"
#include "square.png.h"
#include "start.png.h"
#include "triangle.png.h"

/* Font buffer */
extern unsigned int basicfont_size;
extern unsigned char basicfont[];

SceCtrlData     pad;
SceTouchData    touch;
SceMotionState  motion_state;

static inline float lerp(float value, float from_max, float to_max)
{
    return (value * to_max) / from_max;
}

#define PI 3.14159265f

#define BLACK   RGBA8(  0,   0,   0, 255)
#define WHITE   RGBA8(255, 255, 255, 255)
#define GREEN   RGBA8(  0, 255,   0, 255)
#define RED     RGBA8(255,   0,   0, 255)
#define BLUE    RGBA8(  0,   0, 255, 255)

int main()
{
    vita2d_init();
    vita2d_set_clear_color(BLACK);

    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, 1);
    
    sceMotionStartSampling();

    sceMotionSetAngleThreshold(45);

    vita2d_font *font = vita2d_load_font_mem(basicfont, basicfont_size);

    struct TextureEntry {
        const unsigned char *data;
        const char *name;
        vita2d_texture **target;
    };

    vita2d_texture *bg = NULL;
    vita2d_texture *cross = NULL;
    vita2d_texture *circle = NULL;
    vita2d_texture *square = NULL;
    vita2d_texture *triangle = NULL;
    vita2d_texture *select = NULL;
    vita2d_texture *start = NULL;
    vita2d_texture *ltrigger = NULL;
    vita2d_texture *rtrigger = NULL;
    vita2d_texture *analog = NULL;
    vita2d_texture *dpad = NULL;
    vita2d_texture *frontTouch = NULL;
    vita2d_texture *backTouch = NULL;

    struct TextureEntry textures[] = {
        { background_png, "background", &bg },
        { cross_png, "cross", &cross },
        { circle_png, "circle", &circle },
        { square_png, "square", &square },
        { triangle_png, "triangle", &triangle },
        { select_png, "select", &select },
        { start_png, "start", &start },
        { ltrigger_png, "ltrigger", &ltrigger },
        { rtrigger_png, "rtrigger", &rtrigger },
        { analog_png, "analog", &analog },
        { dpad_png, "dpad", &dpad },
        { finger_gray_png, "frontTouch", &frontTouch },
        { finger_blue_png, "backTouch", &backTouch },
    };

    if (!font) {
        sceClibPrintf("Failed to load font\n");
        goto cleanup0;
    }

    for (size_t i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i) {
        *textures[i].target = vita2d_load_PNG_buffer((void*)textures[i].data);
        if (!*textures[i].target) {
            sceClibPrintf("Failed to load %s texture\n", textures[i].name);
            goto cleanup1;
        }
    }

    bool running = true;
    while (running) {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        sceMotionGetState(&motion_state);

        vita2d_start_drawing();
        vita2d_clear_screen();

        /* Display background */
        vita2d_draw_texture(bg, 0, 54);

        /* Display infos */
        vita2d_font_draw_text(font, 724, 20, WHITE, 19, "VitaTester by SMOKE");

        vita2d_font_draw_text(font, 568, 40, WHITE, 19, "Enhanced by Pookette and Avellea");

        vita2d_font_draw_textf(font, 10, 20, WHITE, 19, "Accelerometer: (%5.2fX, %5.2fY, %5.2fZ)", motion_state.acceleration.x, motion_state.acceleration.y, motion_state.acceleration.z);

        vita2d_font_draw_textf(font, 10, 40, WHITE, 19, "Gyroscope: (%5.2fX, %5.2fY, %5.2fZ)", motion_state.angularVelocity.x, motion_state.angularVelocity.y, motion_state.angularVelocity.z);

        vita2d_font_draw_textf(font, 10, 525, (pad.lx == 127 && pad.ly == 127) ? GREEN : WHITE, 19, "Left: (%3dX, %3dY)", pad.lx, pad.ly);

        vita2d_font_draw_textf(font, 724, 525, (pad.rx == 127 && pad.ry == 127) ? GREEN : WHITE, 19, "Right: (%3dX, %3dY)", pad.rx, pad.ry);

        /* Update joystick values */
        signed char lx = (signed char)pad.lx - 128;
        signed char ly = (signed char)pad.ly - 128;
        signed char rx = (signed char)pad.rx - 128;
        signed char ry = (signed char)pad.ry - 128;

        /* Draw and move left analog stick on screen */
        vita2d_draw_texture(analog, (86 + lx / 8), (284 + ly / 8));

        /* Draw and move right analog on screen */
        vita2d_draw_texture(analog, (803 + rx / 8), (284 + ry / 8));

        /* Draw the up directional button if pressed */
        if (pad.buttons & SCE_CTRL_UP) {
            vita2d_draw_texture(dpad, 59, 134);
        }

        /* Draw the down directional button if pressed */
        if (pad.buttons & SCE_CTRL_DOWN) {
            vita2d_draw_texture_rotate(dpad, 94, 231, 3.14f);
        }

        /* Draw the left directional button if pressed */
        if (pad.buttons & SCE_CTRL_LEFT) {
            vita2d_draw_texture_rotate(dpad, 65, 203, -1.57f);
        }

        /* Draw the right directional button if pressed */
        if (pad.buttons & SCE_CTRL_RIGHT) {
            vita2d_draw_texture_rotate(dpad, 123, 203, 1.57f);
        }

        /* Draw cross button if pressed */
        if (pad.buttons & SCE_CTRL_CROSS) {
            vita2d_draw_texture(cross, 830, 202);
        }

        /* Draw circle button if pressed */
        if (pad.buttons & SCE_CTRL_CIRCLE) {
            vita2d_draw_texture(circle, 869, 165);
        }

        /* Draw square button if pressed */
        if (pad.buttons & SCE_CTRL_SQUARE) {
            vita2d_draw_texture(square, 790, 165);
        }

        /* Draw triangle button if pressed */
        if (pad.buttons & SCE_CTRL_TRIANGLE) {
            vita2d_draw_texture(triangle, 830, 127);
        }

        /* Draw select button if pressed */
        if (pad.buttons & SCE_CTRL_SELECT) {
            vita2d_draw_texture(select, 781, 375);
        }

        /* Draw start button if pressed */
        if (pad.buttons & SCE_CTRL_START) {
            vita2d_draw_texture(start, 841, 373);
        }

        /* Draw left trigger if pressed */
        if (pad.buttons & SCE_CTRL_LTRIGGER) {
            vita2d_draw_texture(ltrigger, 38, 40);
        }

        /* Draw right trigger if pressed */
        if (pad.buttons & SCE_CTRL_RTRIGGER) {
            vita2d_draw_texture(rtrigger, 720, 40);
        }

        /* Draw front touch on screen */
        if (sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1) > 0) {
            for (int i = 0; i < touch.reportNum; i++) {
                int fxTouch = (int)(lerp(touch.report[i].x, 1919.0f, 960.0f) - 50.0f);
                int fyTouch = (int)(lerp(touch.report[i].y, 1087.0f, 544.0f) - 56.5f);
                vita2d_draw_texture(frontTouch, fxTouch, fyTouch);
            }
        }

        /* Draw rear touch on screen */
        if (sceTouchPeek(SCE_TOUCH_PORT_BACK, &touch, 1) > 0) {
            for (int i = 0; i < touch.reportNum; i++) {
                int bxTouch = (int)(lerp(touch.report[i].x, 1919.0f, 960.0f) - 50.0f);
                int byTouch = (int)(lerp(touch.report[i].y, 1285.0f, 855.0f) - 113.0f);
                vita2d_draw_texture(backTouch, bxTouch, byTouch);
            }
        }

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

cleanup1:
    for (size_t i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i) {
        if (*textures[i].target) {
            vita2d_free_texture(*textures[i].target);
        }
    }

    if (font) {
        vita2d_free_font(font);
    }

cleanup0:
    vita2d_fini();
    sceKernelExitProcess(0);
    return 0;
}
