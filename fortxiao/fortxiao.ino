#include "Adafruit_TinyUSB.h"
#include "auto_command_util_xiao.h"
#include "switch_controller_xiao.h"

int LENGTH = 3600 / 10;
int lp = 0;

void fortniteCreativeLeave()
{
  digitalWrite(13, LOW);
  pushButton(Button::X, 200, 2);
  pushButton(Button::X, 3000);
  pushButton(Button::A, 300, 9);
  pushButton(Button::CAPTURE, 300);
  digitalWrite(13, HIGH);
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
        case 2:
          pushButton(Button::X, 200);
          break;
        case 3:
        case 4:
        case 5:
          pushButton(Button::A, 200);
          break;
        case 6:
          tiltJoystick(0, 0, 100 - random(90), 0, 200);
          break;
        case 7:
          tiltJoystick(0, 0, -100 + random(90), 0, 200);
          break;
        case 8:
          pushButton(Button::B, 200);
          break;
        default:
          delay(250);
      }
    }
    digitalWrite(13, HIGH);
  }
  if (lp % 2 == 0){
    pushButton(Button::PLUS, 400);
    pushHatButton(Hat::RIGHT, 400, 4);
    pushHatButton(Hat::UP, 400);
    pushButton(Button::A, 300, 5);
    pushButton(Button::A, 17000);
  } else {
    digitalWrite(13, LOW);
    pushButton(Button::HOME, 700);
    pushButton(Button::X, 700);
    pushButton(Button::A, 100, 30);
    digitalWrite(13, HIGH);
    delay(30000);
    digitalWrite(13, LOW);
    pushButton(Button::A, 100, 30);
    digitalWrite(13, HIGH);
    delay(30000);
  }
  digitalWrite(13, LOW);
  for (int i = 0; i < 10; i++) {
    pushButton(Button::A, 400);
    pushButton(Button::B, 400);
  }
  digitalWrite(13, HIGH);
  pushButton(Button::B, 1000);
  lp++;
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
