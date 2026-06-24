#include <Keypad.h>
#include <Beeper.h>
#include <U8x8lib.h>

Beeper buz(10);

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};

int hexaTone[ROWS][COLS] = {
  {10, 100, 400, 320},
  {250, 175, 440, 360},
  {300, 225, 500, 380},
  {350, 275, 550, 1000}
};

byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

int pinLength = 0;
int maxAttempts = 0;
int attemptsLeft = 0;
String pinCode = "";
String inputBuffer = "";
byte state = 0;

void setup() {
  Serial.begin(9600);
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  u8x8.drawString(0, 0, "==== Log In ====");
  u8x8.drawString(0, 2, "Code length(1-8)");
  u8x8.drawString(0, 3, "To confirm: #");
  u8x8.drawString(0, 4, "Reset lenght: *");
}

void loop() {
  char key = customKeypad.getKey();
  if (key) {
    buz.beepNote(800, 1, 30);
    if (key >= '0' && key <= '9') {
      inputBuffer += key;
      u8x8.drawString(0, 5, inputBuffer.c_str());
    }
    else if (key == '#') {
      if (inputBuffer.length() == 0) {
        u8x8.drawString(0, 6, "Empty!        ");
        return;
      }
      switch (state) {
        case 0:
          pinLength = inputBuffer.toInt();
          if (pinLength < 1 || pinLength > 8) {
            u8x8.drawString(0, 6, "ERROR! 1-8   ");
            inputBuffer = "";
            return;
          }
          u8x8.clearDisplay();
          u8x8.drawString(0, 0, "Length accepted");
          u8x8.drawString(0, 2, "Attempts (1-99)");
          u8x8.drawString(0, 5, "");
          state = 1;
          break;
        case 1:
          maxAttempts = inputBuffer.toInt();
          if (maxAttempts < 1 || maxAttempts > 99) {
            u8x8.drawString(0, 6, "ERROR! 1-99 ");
            inputBuffer = "";
            return;
          }
          attemptsLeft = maxAttempts;
          u8x8.clearDisplay();
          u8x8.drawString(0, 0, "Atempts accepted");
          u8x8.drawString(0, 2, "Enter PIN-code");
          u8x8.drawString(0, 3, "Length: ");
          u8x8.drawString(7, 3, String(pinLength).c_str());
          u8x8.drawString(0, 5, "");
          state = 2;
          break;
        case 2:
          if (inputBuffer.length() != pinLength) {
            u8x8.drawString(0, 6, "MISMATCH!  ");
            inputBuffer = "";
            return;
          }
          pinCode = inputBuffer;
          u8x8.clearDisplay();
          u8x8.drawString(0, 0, "Code Saved!");
          u8x8.drawString(0, 2, "= Log In System=");
          u8x8.drawString(0, 3, " ");
          u8x8.drawString(0, 4, "Enter the code:");
          u8x8.drawString(0, 5, "Attempts: ");
          u8x8.drawString(9, 5, String(attemptsLeft).c_str());
          u8x8.drawString(0, 6, "");
          state = 3;
          break;
        case 3:
         /* if (inputBuffer.length() != pinLength) {
            u8x8.drawString(0, 6, "WRONG LENGTH");
            inputBuffer = "";
            return;
          }*/
          if (inputBuffer == pinCode) {
            u8x8.clearDisplay();
            u8x8.drawString(0, 2, "* WELCOME BACK *");
            buz.beepNote(1000, 1, 500);
            state = 4;
          } else {
            attemptsLeft--;
            u8x8.drawString(0, 4, "Attempts:       ");
            u8x8.drawString(9, 4, String(attemptsLeft).c_str());
            u8x8.drawString(0, 6, "WRONG!      ");
            if (attemptsLeft <= 0) {
              u8x8.clearDisplay();
              u8x8.drawString(0, 2, "ACCESS IS CLOSED");
              buz.beepNote(200, 1, 500);
              state = 4;
            }
          }
          break;
      }
      inputBuffer = "";
      u8x8.drawString(0, 5, "                ");
    }
    else if (key == '*') {
      inputBuffer = "";
      u8x8.drawString(0, 5, "                ");
      u8x8.drawString(0, 6, "Cleared       ");
    }
  }
  buz.tick();
}
