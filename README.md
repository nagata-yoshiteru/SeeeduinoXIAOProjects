# Seeeduino XIAO Projects

## Setup

### Add Seeeduino to your Arduino IDE
- Click on File > Preference, and fill Additional Boards Manager URLs with the url below:
https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json
- Click Tools-> Board-> Boards Manager..., print keyword "Seeeduino XIAO" in the searching blank. Here comes the "Seeed SAMD Boards". Install it.

### Add TinyUSB Library
- Download whole repository from https://github.com/adafruit/Adafruit_TinyUSB_Arduino
- Click Sketch -> Include Library -> Add .ZIP Library and select the downloaded file.

### (Optional) Modify Device ID for Nintendo Switch
- Open `C:\Users\`**`UserName`**`\AppData\Local\Arduino15\packages\Seeeduino\hardware\samd\`**`Version`**`\boards.txt`
- Modify following line:
    ```
    seeed_XIAO_m0.build.vid=0x2886
    seeed_XIAO_m0.build.pid=0x802F
    ```
    ⇒
    ```
    seeed_XIAO_m0.build.vid=0x0f0d
    seeed_XIAO_m0.build.pid=0x0092
    ```
