// /*********************************************************************
//  Adafruit invests time and resources providing this open source code,
//  please support Adafruit and open-source hardware by purchasing
//  products from Adafruit!
 
//  MIT license, check LICENSE for more information
//  Copyright (c) 2019 Ha Thach for Adafruit Industries
//  All text above, and the splash screen below must be included in
//  any redistribution
// *********************************************************************/
 
#include "Adafruit_TinyUSB.h"
#include "auto_command_util_xiao.h"
#include "switch_controller_xiao.h"

const int TIME_TO_HATCHING_SEC = 135;

// Buttonに使うピン
const uint16_t BTN_IN1_PIN = 8;
const uint16_t BTN_IN2_PIN = 9;

// Speakerに使うピン
const uint16_t SPK_PIN = 0;

// Resetに使うピン
const uint16_t RESET_PIN = 0;

// mode
const uint16_t MODE_BITS = 4;
int mode = 0;

// レイドバトルが終わるまでの時間
const int BATTLE_FINISH_SEC = 200;

// 日付変更用
int day_count = 1;

// スティック倒し量記憶用
int lx_per = 0, ly_per = 0, rx_per = 0, ry_per = 0;

String cmds[8] = {"\0"}; // 分割された文字列を格納する配列

// その他，キャッシュ用変数
int v0 = 0, v1 = 0, v2 = 0, v3 = 0;


int split(String data, char delimiter, String *dst){
    int index = 0;
    int arraySize = (sizeof(data)/sizeof((data)[0]));  
    int datalength = data.length();
    dst[0] = "\0";
    for (int i = 0; i < datalength; i++) {
        char tmp = data.charAt(i);
        if ( tmp == delimiter ) {
            index++;
            dst[index] = "\0";
            if ( index > (arraySize - 1)) return -1;
        }
        else dst[index] += tmp;
    }
    return (index + 1);
}

// 空飛ぶタクシーでハシノマはらっぱに移動
void moveToInitialPlayerPosition()
{
  pushButton(Button::X, 1000);
  pushHatButtonContinuous(Hat::LEFT_UP, 1000);
  pushButton(Button::A, 2400);
  pushButton(Button::A, 1300, 2);
  delay(2000);
}

// 初期位置から育て屋さんに移動し卵を受け取る
void getEggFromBreeder()
{
  // 初期位置(ハシノマはらっぱ)から育て屋さんのところまで移動
  pushButton(Button::PLUS, 1000);
  tiltJoystick(0, 0, 100, 0, 2000);
  tiltJoystick(30, -100, 0, 0, 1990);
  pushButton(Button::PLUS, 1000);
  // 育て屋さんから卵をもらう
  pushButton(Button::A, 1000, 4);
  pushButton(Button::B, 500, 10);
}

// 初期位置(ハシノマはらっぱ)からぐるぐる走り回る
void runAround(int run_time_sec)
{
  // 野生ポケモンとのエンカウントを避けるため初期位置から少し移動する
  tiltJoystick(100, 0, 0, 0, 600);
  // delayの秒数がintの最大値を越えないように30秒ごとに実行する
  for (int i = 0; i < run_time_sec / 30; i++)
  {
    tiltJoystick(100, 100, -100, -100, 30000);
  }
  tiltJoystick(100, 100, -100, -100, (run_time_sec % 30) * 1000);
}

// 卵が孵化するのを待つ
void waitEggHatching()
{
  pushButton(Button::B, 500, 40);
}

// 孵化した手持ちのポケモンをボックスに預ける
// box_line : 何列目にポケモンを預けるか
void sendPokemonToBox(int box_line)
{
  // ボックスを開く
  pushButton(Button::X, 1000);
  pushHatButtonContinuous(Hat::LEFT_UP, 1000);
  pushHatButton(Hat::RIGHT, 500);
  pushButton(Button::A, 2000);
  pushButton(Button::R, 2000);
  // 手持ちの孵化したポケモンを範囲選択
  pushHatButton(Hat::LEFT, 500);
  pushHatButton(Hat::DOWN, 500);
  pushButton(Button::Y, 500, 2);
  pushButton(Button::A, 500);
  pushHatButtonContinuous(Hat::DOWN, 2000);
  pushButton(Button::A, 500);
  // ボックスに移動させる
  pushHatButton(Hat::RIGHT, 500, box_line + 1);
  pushHatButton(Hat::UP, 500);
  pushButton(Button::A, 500);
  // ボックスを閉じる
  pushButton(Button::B, 1500, 3);
}

// ボックスを次のボックスに移動させる
void moveToNextBox()
{
  // ボックスを開く
  pushButton(Button::X, 1000);
  pushHatButtonContinuous(Hat::LEFT_UP, 1000);
  pushHatButton(Hat::RIGHT, 500);
  pushButton(Button::A, 2000);
  pushButton(Button::R, 2000);
  // ボックスを移動
  pushHatButton(Hat::UP, 500);
  pushHatButton(Hat::RIGHT, 500);
  // ボックスを閉じる
  pushButton(Button::B, 1500, 3);
}

// 手持ちが1体の状態から、卵受け取り→孵化を繰り返す
void reciveAndHatchEggs()
{
  for (int egg_num = 0; egg_num < 5; egg_num++)
  {
    moveToInitialPlayerPosition();
    getEggFromBreeder();
    moveToInitialPlayerPosition();
    runAround(TIME_TO_HATCHING_SEC);
    waitEggHatching();
  }
}

// 孵化シーケンスを実行
void execHatchingSequence()
{
  for (int box_line = 0; box_line < 6; box_line++)
  {
    reciveAndHatchEggs();
    sendPokemonToBox(box_line);
  }
  moveToNextBox();
}

// 巣穴の前からひとりレイドを始め、レイドポケモンを倒し、捕まえる
void startRaidBattle(){
    // ひとりではじめる
    pushButton(Button::A, 1500);
    pushHatButton(Hat::DOWN, 500);
    // レイドバトル中はA連打
    for(int i=0; i<BATTLE_FINISH_SEC; i++){
        pushButton(Button::A, 500, 2);
    }
}

// レイドバトル後もしばらく続くAボタン連打の後の画面から、
// 巣穴の前の最初のポジションに戻す
void resetStateRaidBattle(){
    tiltJoystick(0,0,0,0,100);
    pushButton(Button::B, 1000, 4);
    delay(1000);
    pushButton(Button::A, 1000, 2);
    pushButton(Button::B, 1000, 3);
}

// レイドバトルをし、ポケモンを捕まえ、ボックスに預けるを繰り返し、
// ボックスが一杯になったら次のボックスへ移動する
void execRaidBattleSequence(){
    for(int send_line=0; send_line<6; send_line++){
        // 手持ちが一杯になるまでレイドバトルを行う
        for(int i=0; i<5; i++){
            startRaidBattle();
            resetStateRaidBattle();
        }
        // 捕まえたポケモンを全て預ける
        sendPokemonToBox(send_line);
    }
    // ボックスが一杯になったところでボックスを移動する
    moveToNextBox();
}

void goToTimeSetting()
{
  // ホーム画面 > 設定
  pushButton(Button::HOME, 500);
  pushHatButton(Hat::DOWN, 30);
  pushHatButton(Hat::RIGHT, 30, 4);
  pushButton(Button::A, 300);
  // 設定 > 本体 > 日付と時刻
  pushHatButtonContinuous(Hat::DOWN, 2000);
  pushHatButton(Hat::RIGHT, 100);
  pushHatButton(Hat::DOWN, 30, 5);
  pushButton(Button::A, 100);
  // 日付と時刻 > 現在の日付と時刻
  pushHatButton(Hat::DOWN, 30, 3);
}

// IDくじ
void ID()
{
  //ロトミ起動 > IDくじ
  pushButton(Button::A, 300, 2);
  pushHatButton(Hat::DOWN, 150);
  pushButton(Button::A, 35, 40);
  pushButton(Button::B, 35, 125);
  // ホーム画面 > 現在の日付と時刻
  goToTimeSetting();
}


void day1day30()
{
  pushButton(Button::A, 68);
  pushHatButton(Hat::RIGHT, 25, 2, 40, 1);
  pushHatButton(Hat::UP, 25, 1, 40, 1);
  pushHatButton(Hat::RIGHT, 25, 3, 40, 1);
  pushButton(Button::A, 25);
}

void day1day30Multi()
{
  pushButton(Button::A, 67);
  pushHatButton(Hat::LEFT, 25, 3, 40, 1);
  pushHatButton(Hat::UP, 25, 1, 40, 1);
  pushHatButton(Hat::RIGHT, 25, 3, 40, 1);
  pushButton(Button::A, 25);
}

void day31day1()
{
  day1day30();
  delay(200);
  pushButton(Button::A, 67);
  pushHatButton(Hat::LEFT, 25, 3, 40, 1);
  pushHatButton(Hat::UP, 25, 1, 40, 1);
  pushHatButton(Hat::RIGHT, 25, 3, 40, 1);
  pushButton(Button::A, 25);
}

int changeDate()
{
  if (day_count == 30) {
    day31day1(); // 30 -> 31 -> 1
    day_count = 1;
    return 1;
  }
  else {
    day1day30();
    return ++day_count;
  }
}

void ID2()
{ // ホーム画面 > ゲーム画面
  pushButton(Button::HOME, 1000, 2);
}

// ワットを回収するシーケンス
void execWattGainSequence()
{
    // 募集開始
    pushButton(Button::A, 2800);
    // 日付変更
    goToTimeSetting();
    day1day30();
    ID2();
    // レイド募集中止
    pushButton(Button::B, 800);
    pushButton(Button::A, 3400);
    // ワット回収
    pushButton(Button::A, 600);
    pushButton(Button::B, 600);
    pushButton(Button::B, 600);
}

// IDくじ > マニア
void moveFromIDToMania()
{

}

// マニア > 掘り出し物市
void moveFromManiaToHoridashi()
{

}

// 掘り出し物市 > マニア
void moveFromHoridashiToMania()
{

}

// マニア > IDくじ
void moveFromManiaToID()
{

}

// 通知音
void notificationSound()
{
  tone(SPK_PIN, 1760, 150);
  delay(200);
  tone(SPK_PIN, 1760, 150);
  delay(200);
  tone(SPK_PIN, 1760, 150);
}

// 再起動
void restartApp()
{
  pushButton(Button::HOME, 700);
  pushButton(Button::X, 700);
  pushButton(Button::A, 100, 30);
  delay(13000);
  pushButton(Button::A, 100, 15);
  delay(7000);
}

// キャンプ
void inAndOutCamp()
{
  pushButton(Button::B, 100, 20);
  pushButton(Button::X, 700);
  pushHatButtonContinuous(Hat::LEFT_UP, 1000);
  pushHatButton(Hat::RIGHT, 500);
  pushHatButton(Hat::DOWN, 500);
  pushButton(Button::A, 2000);
  delay(10000);
  pushButton(Button::B, 2000);
  pushButton(Button::A, 2000);
}

// Fortnite クリエ放置
void fortniteCreativeLeave()
{
  pushButton(Button::X, 200);
  delay(5000);
  pushButton(Button::A, 200);
  for (int i = 0; i < 32; i++)
  {
    delay(30000);
  }
  pushButton(Button::PLUS, 200);
  for (int i = 0; i < 3; i++)
  {
    pushHatButton(Hat::RIGHT, 200);
    delay(300);
  }
  pushHatButton(Hat::UP, 200);
  delay(1000);
  pushButton(Button::A, 200);
  delay(1000);
  pushButton(Button::A, 200);
  delay(30000);
}

// get button input
int getButton()
{
  if (digitalRead(BTN_IN1_PIN) == LOW) return 0;
  if (digitalRead(BTN_IN2_PIN) == LOW) return 1;
  return -1;
}

// get num input
int getInput(int inputBits)
{
  int in = 0, b = 0, k = 1;
  if (cmds[0] == "MODE_INPUT") return cmds[2].toInt();
  pinMode(BTN_IN1_PIN, INPUT_PULLUP);
  pinMode(BTN_IN2_PIN, INPUT_PULLUP);
  for (int i = 0; i < inputBits; i++)
  {
    int j = -1;
    while ((j = getButton()) == -1){
      if (Serial1.available() > 0) {
        String serialStr = Serial1.readStringUntil('\n');
        split(serialStr, ',', cmds);
        if (cmds[0] == "MODE_INPUT") return cmds[1].toInt();
        //else return 14; // 外部コントロール
      }
      delay(8);
      if (b >= LED_BRIGHTNESS_100) k = -1;
      if (b <= 0) k = 1;
      b += k;
      whiteLED(b);
    }
    if (j == 1) blueLED(1); else greenLED(1);
    tone(SPK_PIN, 880, j == 1 ? 200 : 50);
    while (getButton() != -1){
      delay(8);
      if (b >= LED_BRIGHTNESS_100) k = -1;
      if (b <= 0) k = 1;
      b += k;
      whiteLED(b);
    }
    if (j == 1) blueLED(0); else greenLED(0);
    delay(40);
    in = (in << 1) + j;
  }
  pinMode(BTN_IN1_PIN, OUTPUT);
  pinMode(BTN_IN2_PIN, OUTPUT);
  return in;
}

// show num
void showNum(int num, int outputBits)
{
  if (outputBits > 4 && cmds[0] == "MODE_INPUT") outputBits += 4;
  delay(100);
  whiteLED(LED_BRIGHTNESS_50);
  delay(100);
  for (int i = outputBits; i > 0; i--)
  {
    int j = (num >> (i - 1)) % 2;
    if (j == 1) blueLED(1); else greenLED(1);
    tone(SPK_PIN, 880, j == 1 ? 200 : 50);
    delay(200);
    if (j == 1) blueLED(0); else greenLED(0);
    delay(100);
  }
  delay(100);
  whiteLED(0);
  delay(100);
}

// mode select
void setMode()
{
  mode = getInput(MODE_BITS);
  showNum(mode, MODE_BITS);
}

// reset
// void(* resetFunc) (void) = 0;
void resetFunc(){
  digitalWrite(RESET_PIN, LOW);
  pinMode(RESET_PIN, OUTPUT);
}

void setup()
{
  initAutoCommandUtil();
  tone(SPK_PIN, 880, 200);
  Serial1.begin(9600);
  setMode();
  pushButton(Button::B, 200, 3);
  delay(400);

  // 初めの卵が出現するまで走り回る
  switch (mode)
  {
    case 0: // A連打
      break;
    case 1: // 自動孵化(全自動)
      moveToInitialPlayerPosition();
      runAround(TIME_TO_HATCHING_SEC / 4);
      break;
    case 2: // 自動孵化(ループのみ)
      moveToInitialPlayerPosition();
      break;
    case 3: // 自動ワット稼ぎ
      pushButton(Button::A, 1000);
      break;
    case 4: // 自動レイド
      break;
    case 5: // 自動IDくじ
      break;
    case 6: // 自動IDくじ・掘り出し物市・マニア
      break;
    case 7: // 自動日付変更(無限)
      goToTimeSetting();
      day1day30();
      break;
    case 8:
      break;
    case 9: // 自動ワット稼ぎ(回数指定)
      v0 = getInput(8);
      showNum(v0, 8);
      pushButton(Button::A, 1000);
      for (v1 = 0; v1 < v0; v1++) {
        execWattGainSequence();
      }
      break;
    case 10: // 自動日付変更(10日単位回数指定)
      v0 = getInput(8) * 10;
      showNum(v0, 12);
      goToTimeSetting();
      day1day30();
      for (v1 = 1; v1 < v0; v1++) {
        delay(50);
        day1day30Multi();
      }
      ID2();
      break;
    case 11: // 自動3日目探索
      break;
    case 12: // 自動4日目探索
      break;
    case 13: // 化石
      v0 = getInput(2);
      showNum(v0, 2);
      break;
    case 14: // 外部コントロール
      /*String serialStr = */Serial1.readStringUntil('\n');
      break;
    // case 15: // コントローラーテスト
    //   break;
    default:
      resetFunc();  //call reset
      break;
  }
}

void loop()
{
  switch (mode)
  {
    case 0:
      pushButton(Button::A, 30, 10000);
      break;
    case 1:
      execHatchingSequence();
      break;
    case 2:
      runAround(30);
      break;
    case 3:
      execWattGainSequence();
      break;
    case 4:
      execRaidBattleSequence();
      break;
    case 5:
      ID();
      changeDate();
      ID2();
      break;
    case 6:
      ID();
      changeDate();
      ID2();
      break;
    case 7:
    case 8:
      delay(50);
      day1day30Multi();
      break;
    case 11:
    case 12:
      pushButton(Button::A, 1000);
      for (v0 = 8; v0 < mode; v0++)
      {
        execWattGainSequence();
      }
      tone(SPK_PIN, 1760, 150);
      delay(200);
      tone(SPK_PIN, 1760, 150);
      delay(200);
      tone(SPK_PIN, 1760, 150);
      v1 = getInput(1);
      restartApp();
      if (v1 == 1) {
        pushButton(Button::A, 1000);
        execWattGainSequence();
        pushButton(Button::B, 30, 2);
        delay(1000);
        pushButton(Button::X, 1000);
        pushButton(Button::R, 1000);
        tone(SPK_PIN, 1760, 150);
        delay(200);
        tone(SPK_PIN, 880, 150);
        delay(200);
        tone(SPK_PIN, 440, 150);
        v1 = getInput(1);
        if(v1 == 1) {
          pushButton(Button::A, 3000);
        }else{
          resetFunc();
        }
      }
      break;
    case 13:
      for (v1 = 0; v1 < 30; v1++) {
        pushButton(Button::A, 100, 7);
        delay(350);
        if ((v0 >> 1) % 2 == 1) pushHatButton(Hat::DOWN, 80);
        pushButton(Button::A, 750, 1);
        if ((v0 >> 0) % 2 == 1) pushHatButton(Hat::DOWN, 80);
        pushButton(Button::A, 35, 12);
        pushButton(Button::B, 35, 156);
        delay(60);
      }
      notificationSound();
      v1 = getInput(1);
      if (v1 == 0) {
        restartApp();
      }else{
        resetFunc();
      }
      break;
    case 14:
      // データを受信した場合にのみ，データを送信する
      if (Serial1.available() > 0) {
        v0 = -1;
        // 受信したデータの1バイトを読み取る
        String serialStr = Serial1.readStringUntil('\n');
        split(serialStr, ',', cmds);
        // 受信したデータを出力する
        Serial1.print(serialStr);
        if (cmds[0] == "bY") { v0 = 0; v1 = Button::Y; }
        if (cmds[0] == "bB") { v0 = 0; v1 = Button::B; }
        if (cmds[0] == "bA") { v0 = 0; v1 = Button::A; }
        if (cmds[0] == "bX") { v0 = 0; v1 = Button::X; }
        if (cmds[0] == "bL") { v0 = 0; v1 = Button::L; }
        if (cmds[0] == "bR") { v0 = 0; v1 = Button::R; }
        if (cmds[0] == "bZL") { v0 = 0; v1 = Button::ZL; }
        if (cmds[0] == "bZR") { v0 = 0; v1 = Button::ZR; }
        if (cmds[0] == "b-") { v0 = 0; v1 = Button::MINUS; }
        if (cmds[0] == "b+") { v0 = 0; v1 = Button::PLUS; }
        if (cmds[0] == "bLC") { v0 = 0; v1 = Button::LCLICK; }
        if (cmds[0] == "bRC") { v0 = 0; v1 = Button::RCLICK; }
        if (cmds[0] == "bHOME") { v0 = 0; v1 = Button::HOME; }
        if (cmds[0] == "bCAP") { v0 = 0; v1 = Button::CAPTURE; }
        if (cmds[0] == "rxL") { v0 = 2; rx_per = -100; }
        if (cmds[0] == "rxH") { v0 = 2; rx_per = 100; }
        if (cmds[0] == "ryL") { v0 = 2; ry_per = -100; }
        if (cmds[0] == "ryH") { v0 = 2; ry_per = 100; }
        if (cmds[0] == "lxL") { v0 = 2; lx_per = -100; }
        if (cmds[0] == "lxH") { v0 = 2; lx_per = 100; }
        if (cmds[0] == "lyL") { v0 = 2; ly_per = -100; }
        if (cmds[0] == "lyH") { v0 = 2; ly_per = 100; }
        if (cmds[0] == "RbY") { v0 = 1; v1 = Button::Y; }
        if (cmds[0] == "RbB") { v0 = 1; v1 = Button::B; }
        if (cmds[0] == "RbA") { v0 = 1; v1 = Button::A; }
        if (cmds[0] == "RbX") { v0 = 1; v1 = Button::X; }
        if (cmds[0] == "RbL") { v0 = 1; v1 = Button::L; }
        if (cmds[0] == "RbR") { v0 = 1; v1 = Button::R; }
        if (cmds[0] == "RbZL") { v0 = 1; v1 = Button::ZL; }
        if (cmds[0] == "RbZR") { v0 = 1; v1 = Button::ZR; }
        if (cmds[0] == "Rb-") { v0 = 1; v1 = Button::MINUS; }
        if (cmds[0] == "Rb+") { v0 = 1; v1 = Button::PLUS; }
        if (cmds[0] == "RbLC") { v0 = 1; v1 = Button::LCLICK; }
        if (cmds[0] == "RbRC") { v0 = 1; v1 = Button::RCLICK; }
        if (cmds[0] == "RbHOME") { v0 = 1; v1 = Button::HOME; }
        if (cmds[0] == "RbCAP") { v0 = 1; v1 = Button::CAPTURE; }
        if (cmds[0] == "RrxL") { v0 = 2; rx_per = 0; }
        if (cmds[0] == "RrxH") { v0 = 2; rx_per = 0; }
        if (cmds[0] == "RryL") { v0 = 2; ry_per = 0; }
        if (cmds[0] == "RryH") { v0 = 2; ry_per = 0; }
        if (cmds[0] == "RlxL") { v0 = 2; lx_per = 0; }
        if (cmds[0] == "RlxH") { v0 = 2; lx_per = 0; }
        if (cmds[0] == "RlyL") { v0 = 2; ly_per = 0; }
        if (cmds[0] == "RlyH") { v0 = 2; ly_per = 0; }
        if (cmds[0] == "til") {
          v0 = 2;
          lx_per = cmds[1].toInt();
          ly_per = cmds[2].toInt();
          rx_per = cmds[3].toInt();
          ry_per = cmds[4].toInt();
        }
        if (cmds[0] == "Rtil") {
          v0 = 2;
          lx_per = 0;
          ly_per = 0;
          rx_per = 0;
          ry_per = 0;
        }
        switch (v0)
        {
          case 0:
            SwitchController().pressButton((Button)v1);
            break;
          case 1:
            SwitchController().releaseButton((Button)v1);
            break;
          case 2:
            SwitchController().setStickTiltRatio(lx_per, ly_per, rx_per, ry_per);
            break;
          default:
            break;
        }
      }
      break;
    case 15:
      pushButton(Button::A, 500);
      pushButton(Button::B, 500);
      pushButton(Button::X, 500);
      pushButton(Button::Y, 500);
      pushButton(Button::L, 500);
      pushButton(Button::R, 500);
      pushButton(Button::ZL, 500);
      pushButton(Button::ZR, 500);
      pushButton(Button::PLUS, 500);
      pushButton(Button::MINUS, 500);
      pushButton(Button::LCLICK, 500);
      pushButton(Button::RCLICK, 500);
      pushHatButton(Hat::UP, 500);
      pushHatButton(Hat::UP_RIGHT, 500);
      pushHatButton(Hat::RIGHT, 500);
      pushHatButton(Hat::RIGHT_DOWN, 500);
      pushHatButton(Hat::DOWN, 500);
      pushHatButton(Hat::DOWN_LEFT, 500);
      pushHatButton(Hat::LEFT, 500);
      pushHatButton(Hat::LEFT_UP, 500);
      pushHatButton(Hat::CENTER, 500);
      pushHatButtonContinuous(Hat::UP, 2000);
      pushHatButtonContinuous(Hat::UP_RIGHT, 2000);
      pushHatButtonContinuous(Hat::RIGHT, 2000);
      pushHatButtonContinuous(Hat::RIGHT_DOWN, 2000);
      pushHatButtonContinuous(Hat::DOWN, 2000);
      pushHatButtonContinuous(Hat::DOWN_LEFT, 2000);
      pushHatButtonContinuous(Hat::LEFT, 2000);
      pushHatButtonContinuous(Hat::LEFT_UP, 2000);
      pushHatButtonContinuous(Hat::CENTER, 2000);
      tiltJoystick(100, 0, 0, 0, 2000);
      tiltJoystick(0, 100, 0, 0, 2000);
      tiltJoystick(0, 0, 100, 0, 2000);
      tiltJoystick(0, 0, 0, 100, 2000);
      pushButton(Button::HOME, 500);
      pushButton(Button::CAPTURE, 500);
      resetFunc();  //call reset
    case 16:
      fortniteCreativeLeave();
    default:
      resetFunc();  //call reset
      break;
  }
}
