#include "switch_controller_xiao.h"

#pragma once

// LEDに使うピン
const uint16_t LED_RED_PIN = 1;
const uint16_t LED_GREEN_PIN = 3;
const uint16_t LED_BLUE_PIN = 2;
const uint16_t LED_WHITE_PIN = 10;

// 明るさ
const uint16_t LED_BRIGHTNESS_100 = 31;
const uint16_t LED_BRIGHTNESS_50 = 15;

// ボタンを押してから離すまでの時間
const uint16_t BUTTON_PUSHING_MSEC = 45;

void initAutoCommandUtil();
void pushButton(Button button, int delay_after_pushing_msec, int loop_num=1, int pushing_msec=BUTTON_PUSHING_MSEC, int finish_waiting_msec=BUTTON_PUSHING_MSEC);
void pushHatButton(Hat button, int delay_after_pushing_msec, int loop_num=1, int pushing_msec=BUTTON_PUSHING_MSEC, int finish_waiting_msec=BUTTON_PUSHING_MSEC);
void pushHatButtonContinuous(Hat button, int pushing_time_msec);
void tiltJoystick(int lx_per, int ly_per, int rx_per, int ry_per, int tilt_time_msec);
void redLED(int brightness);
void whiteLED(int brightness);
void blueLED(int brightness);
void greenLED(int brightness);
