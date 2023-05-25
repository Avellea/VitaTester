#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>
#include <psp2/motion.h>
#include <vita2d.h>

/* Font buffer */
extern unsigned int basicfont_size;
extern unsigned char basicfont[];

SceCtrlData     pad;
SceTouchData    touch;
SceMotionState  motion_state;

signed char lx;
signed char ly;
signed char rx;
signed char ry;
int fxTouch;
int fyTouch;
int bxTouch;
int byTouch;

#define lerp(value, from_max, to_max) ((((value*10) * (to_max*10))/(from_max*10))/10)
#define PI 3.14159265

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

    /* Setup image buffers */
    vita2d_texture *bg = vita2d_load_PNG_file("app0:/icons/background.png");
    vita2d_texture *cross = vita2d_load_PNG_file("app0:/icons/cross.png");
    vita2d_texture *circle = vita2d_load_PNG_file("app0:/icons/circle.png");
    vita2d_texture *square = vita2d_load_PNG_file("app0:/icons/square.png");
    vita2d_texture *triangle = vita2d_load_PNG_file("app0:/icons/triangle.png");
    vita2d_texture *select = vita2d_load_PNG_file("app0:/icons/select.png");
    vita2d_texture *start = vita2d_load_PNG_file("app0:/icons/start.png");
    vita2d_texture *ltrigger = vita2d_load_PNG_file("app0:/icons/ltrigger.png");
    vita2d_texture *rtrigger = vita2d_load_PNG_file("app0:/icons/rtrigger.png");
    vita2d_texture *analog = vita2d_load_PNG_file("app0:/icons/analog.png");
    vita2d_texture *dpad = vita2d_load_PNG_file("app0:/icons/dpad.png");
    vita2d_texture *frontTouch = vita2d_load_PNG_file("app0:/icons/finger_gray.png");
    vita2d_texture *backTouch = vita2d_load_PNG_file("app0:/icons/finger_blue.png");

    while (1) {
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

        vita2d_font_draw_textf(font, 10, 525, (pad.lx == 127 & pad.ly == 127) ? GREEN : WHITE, 19, "Left: (%3dX, %3dY)", pad.lx, pad.ly);

        vita2d_font_draw_textf(font, 724, 525, (pad.rx == 127 & pad.ry == 127) ? GREEN : WHITE, 19, "Right: (%3dX, %3dY)", pad.rx, pad.ry);

        /* Update joystick values */
        lx = (signed char)pad.lx - 128;
        ly = (signed char)pad.ly - 128;
        rx = (signed char)pad.rx - 128;
        ry = (signed char)pad.ry - 128;

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
        sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
        for (int i = 0; i < touch.reportNum; i++) {
            fxTouch = (lerp(touch.report[i].x, 1919, 960) - 50);
            fyTouch = (lerp(touch.report[i].y, 1087, 544) - 56.5);
            vita2d_draw_texture(frontTouch, fxTouch, fyTouch);
        }

        /* Draw rear touch on screen */
        sceTouchPeek(SCE_TOUCH_PORT_BACK, &touch, 1);
        for (int i = 0; i < touch.reportNum; i++) {
            bxTouch = (lerp(touch.report[i].x, 1919, 960) - 50);
            byTouch = (lerp(touch.report[i].y, 1285, 855) - 113);
            vita2d_draw_texture(backTouch, bxTouch, byTouch);
        }

        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    vita2d_fini();

    /* Cleanup */
    vita2d_free_font(font);
    vita2d_free_texture(bg);
    vita2d_free_texture(cross);
    vita2d_free_texture(circle);
    vita2d_free_texture(square);
    vita2d_free_texture(triangle);
    vita2d_free_texture(select);
    vita2d_free_texture(start);
    vita2d_free_texture(ltrigger);
    vita2d_free_texture(rtrigger);
    vita2d_free_texture(analog);
    vita2d_free_texture(dpad);
    vita2d_free_texture(frontTouch);
    vita2d_free_texture(backTouch);

	sceKernelExitProcess(0);

    return 0;
}
