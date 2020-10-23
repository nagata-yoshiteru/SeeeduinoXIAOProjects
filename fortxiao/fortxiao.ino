#include "Adafruit_TinyUSB.h"
#include "auto_command_util_xiao.h"
#include "switch_controller_xiao.h"

int LENGTH = 3600 / 10;

void fortniteCreativeLeave()
{
  pushButton(Button::X, 200, 2);
  pushButton(Button::X, 3000);
  pushButton(Button::A, 300, 9);
  pushButton(Button::CAPTURE, 300);
  for (int i = 0; i < LENGTH + 4; i++)
  {
    delay(9750);
    digitalWrite(13, LOW);
    int j = random(10);
    if (i % LENGTH == 3) {
      pushButton(Button::CAPTURE, 200);
    } else {
      switch (j) {
        case 0:
        case 1:
          pushButton(Button::X, 200);
          break;
        case 2:
        case 3:
          pushButton(Button::A, 200);
          break;
        case 4:
          pushButton(Button::B, 200);
          break;
        //case 5:
          //pushButton(Button::ZR, 200);
          //break;
        case 6:
          tiltJoystick(0, 0, 100, 0, 200);
          break;
        case 7:
          tiltJoystick(0, 0, -100, 0, 200);
          break;
        default:
          delay(250);
      }
    }
    digitalWrite(13, HIGH);
  }
  pushButton(Button::PLUS, 400);
  pushHatButton(Hat::RIGHT, 400, 4);
  pushHatButton(Hat::UP, 400);
  pushButton(Button::A, 300, 5);
  pushButton(Button::A, 17000);
  for (int i = 0; i < 7; i++) {
    pushButton(Button::A, 400);
    pushButton(Button::B, 400);
  }
  pushButton(Button::B, 1000);
}

void setup()
{
  initAutoCommandUtil();
  randomSeed(analogRead(11));
  pushButton(Button::B, 200, 3);
  delay(400);
}

void loop()
{
  fortniteCreativeLeave();
}