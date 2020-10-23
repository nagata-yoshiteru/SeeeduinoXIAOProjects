/**
 * @file auto_command_util.cpp
 * @author interimadd
 * @brief よく使うSwitchへの入力をまとめたライブラリ
 * @version 0.1
 * @date 2020-01-14
 */
#include "auto_command_util_xiao.h"
#include "Adafruit_TinyUSB.h"
#include <Arduino.h>

// 長押し時のLED点滅時間間隔
const uint16_t LED_INTERVAL = 500;

/**
 * @brief 初期化(全LEDのセットアップ/点灯/初期 B 3連打)
 */
void initAutoCommandUtil(){
  // ピンを初期化
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_WHITE_PIN, OUTPUT);
  // 全点灯
  redLED(LED_BRIGHTNESS_100);
  whiteLED(LED_BRIGHTNESS_100);
  blueLED(LED_BRIGHTNESS_100);
  greenLED(LED_BRIGHTNESS_100);
  // 初期 B 3連打
  pushButton(Button::B, 200, 3);
  // 全点灯
  redLED(LED_BRIGHTNESS_100);
  whiteLED(LED_BRIGHTNESS_100);
  blueLED(LED_BRIGHTNESS_100);
  greenLED(LED_BRIGHTNESS_100);
  // 0.5秒待機
  delay(200);
  // 全消灯
  redLED(0);
  whiteLED(0);
  blueLED(0);
  greenLED(0);
}

/**
 * @brief Switchコントローラーのボタンを押すときのLED操作
 * 
 * @param button 押すボタン
 * @param control 押す(=1)か離す(=0)か
 */
void buttonLED(Button button, int control)
{
    switch (button)
    {
        case Button::A:
            redLED(control * LED_BRIGHTNESS_100);
            break;
        case Button::B:
            blueLED(control * LED_BRIGHTNESS_100);
            break;
        case Button::X:
            greenLED(control * LED_BRIGHTNESS_100);
            break;
        case Button::Y:
            blueLED(control * LED_BRIGHTNESS_100);
            greenLED(control * LED_BRIGHTNESS_100);
            break;
        case Button::L:
            blueLED(control * LED_BRIGHTNESS_100);
            redLED(control * LED_BRIGHTNESS_100);
            break;
        case Button::R:
            greenLED(control * LED_BRIGHTNESS_100);
            redLED(control * LED_BRIGHTNESS_100);
            break;
        case Button::ZL:
            blueLED(control * LED_BRIGHTNESS_100);
            redLED(control * LED_BRIGHTNESS_100);
            break;
        case Button::ZR:
            greenLED(control * LED_BRIGHTNESS_100);
            redLED(control * LED_BRIGHTNESS_100);
            break;
        default:
            whiteLED(control * LED_BRIGHTNESS_100);
            break;
    }
}

/**
 * @brief Switchコントローラーのハットボタンを押すときのLED操作
 * 
 * @param hat 押すボタン
 * @param control 押す(=1)か離す(=0)か
 */
void hatLED(Hat hat, int control)
{
    whiteLED(control * LED_BRIGHTNESS_50);
    blueLED((hat % 2) & control * LED_BRIGHTNESS_100);
    greenLED(((hat >> 1) % 2) & control * LED_BRIGHTNESS_100);
    redLED((((hat >> 2) % 2) && control) * LED_BRIGHTNESS_100);
}

/**
 * @brief Switchコントローラーのスティックを押すときのLED操作
 * 
 * @param hat 押すボタン
 * @param control 押す(=1)か離す(=0)か
 */
void stickLED(int l, int r, int control)
{
    whiteLED(control * LED_BRIGHTNESS_50);
    blueLED(l & control * LED_BRIGHTNESS_100);
    greenLED(r & control * LED_BRIGHTNESS_100);
}

/**
 * @brief Switchコントローラーのボタンを押す
 * 
 * @param button 押すボタン
 * @param delay_after_pushing_msec ボタンを押し終えた後の待ち時間 
 * @param loop_num ボタンを押す回数
 */
void pushButton(Button button, int delay_after_pushing_msec, int loop_num, int pushing_msec, int finish_waiting_msec)
{
    for(int i=0; i<loop_num; i++)
    {
        buttonLED(button, 1);
        SwitchController().pressButton(button);
        delay(pushing_msec);
        SwitchController().releaseButton(button);
        buttonLED(button, 0);
        delay(delay_after_pushing_msec);
    }
    delay(finish_waiting_msec);
}

/**
 * @brief Switchコントローラーの矢印ボタンを押す
 * 
 * @param button 押す矢印ボタン
 * @param delay_after_pushing_msec ボタンを押し終えた後の待ち時間 
 * @param loop_num ボタンを押す回数
 */
void pushHatButton(Hat button, int delay_after_pushing_msec, int loop_num, int pushing_msec, int finish_waiting_msec)
{
    for(int i=0;i<loop_num;i++)
    {
        hatLED(button, 1);
        SwitchController().pressHatButton(button);
        delay(pushing_msec);
        SwitchController().releaseHatButton();
        hatLED(button, 0);
        delay(delay_after_pushing_msec);
    }
    delay(finish_waiting_msec);
}

/**
 * @brief Switchコントローラーの矢印ボタンを指定時間の間押し続ける
 * 
 * @param button 押す矢印ボタン
 * @param pushing_time_msec ボタンを押す時間の長さ
 */
void pushHatButtonContinuous(Hat button, int pushing_time_msec)
{
    SwitchController().pressHatButton(button);
    int remaining_time_msec = pushing_time_msec;
    while(true)
    {
        hatLED(button, 1);
        if(remaining_time_msec > LED_INTERVAL){
            remaining_time_msec -= LED_INTERVAL;
            delay(LED_INTERVAL);
        }else{
            delay(remaining_time_msec);
            break;
        }
        whiteLED(LED_BRIGHTNESS_100);
        if(remaining_time_msec > LED_INTERVAL){
            remaining_time_msec -= LED_INTERVAL;
            delay(LED_INTERVAL);
        }else{
            delay(remaining_time_msec);
            break;
        }
    }
    SwitchController().releaseHatButton();
    hatLED(button, 0); 
    delay(BUTTON_PUSHING_MSEC);
}

/**
 * @brief Switchのジョイスティックの倒し量を設定する
 * 
 * @param lx_per LスティックのX方向倒し量[％] -100~100の範囲で設定
 * @param ly_per LスティックのY方向倒し量[％] -100~100の範囲で設定
 * @param rx_per RスティックのX方向倒し量[％] -100~100の範囲で設定
 * @param ry_per RスティックのY方向倒し量[％] -100~100の範囲で設定
 * @param tilt_time_msec スティックを倒し続ける時間
 */
void tiltJoystick(int lx_per, int ly_per, int rx_per, int ry_per, int tilt_time_msec)
{
    SwitchController().setStickTiltRatio(lx_per, ly_per, rx_per, ry_per);
    int remaining_time_msec = tilt_time_msec;
    while(true)
    {
        stickLED(lx_per || ly_per, rx_per || ry_per, 1);
        if(remaining_time_msec > LED_INTERVAL){
            remaining_time_msec -= LED_INTERVAL;
            delay(LED_INTERVAL);
        }else{
            delay(remaining_time_msec);
            break;
        }
        whiteLED(0);
        if(remaining_time_msec > LED_INTERVAL){
            remaining_time_msec -= LED_INTERVAL;
            delay(LED_INTERVAL);
        }else{
            delay(remaining_time_msec);
            break;
        }
    }
    SwitchController().setStickTiltRatio(0, 0, 0, 0);
    stickLED(0, 0, 0); 
    delay(BUTTON_PUSHING_MSEC);
}

/**
 * @brief 赤色LEDを操作 (0=OFF ～ LED_BRIGHTNESS_100=ON)
 */
void redLED(int brightness){
  pinMode(LED_RED_PIN, OUTPUT);
  analogWrite(LED_RED_PIN, brightness < LED_BRIGHTNESS_100 ? brightness : LED_BRIGHTNESS_100);
}

/**
 * @brief 白色LEDを操作 (0=OFF ～ LED_BRIGHTNESS_100=ON)
 */
void whiteLED(int brightness){
  pinMode(LED_WHITE_PIN, OUTPUT);
  analogWrite(LED_WHITE_PIN, brightness < LED_BRIGHTNESS_100 ? brightness : LED_BRIGHTNESS_100);
}

/**
 * @brief 青色LEDを操作 (0=OFF ～ LED_BRIGHTNESS_100=ON)
 */
void blueLED(int brightness){
  pinMode(LED_BLUE_PIN, OUTPUT);
  analogWrite(LED_BLUE_PIN, brightness < LED_BRIGHTNESS_100 ? brightness : LED_BRIGHTNESS_100);
}

/**
 * @brief 緑色LEDを操作 (0=OFF ～ LED_BRIGHTNESS_100=ON)
 */
void greenLED(int brightness){
  pinMode(LED_GREEN_PIN, OUTPUT);
  analogWrite(LED_GREEN_PIN, brightness < LED_BRIGHTNESS_100 ? brightness : LED_BRIGHTNESS_100);
}
