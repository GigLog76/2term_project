#include <Keypad.h> 
#include <Beeper.h>
#include <Blinker.h>

Beeper buz(10);
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
{'D','C','B','A'}, 
{'#','9','6','3'},
{'0','8','5','2'},
{'*','7','4','1'}
};

uint8_t hexaTone[ROWS][COLS] = {
  {10, 100, 400, 320}, 
  {250, 175, 440, 360},  
  {300, 225, 500, 380},
  {350, 275, 550, 1000}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {9, 8, 7, 6}; 

//передаем все эти данные библиотеке:
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);



void setup(){
  Serial.begin(9600);

}

void loop() {
  char key = customKeypad.getKey();
  if (key) {
    Serial.println(key);
    // Найти координаты нажатой кнопки
    for (byte r = 0; r < ROWS; r++) {
      for (byte c = 0; c < COLS; c++) {
        if (hexaKeys[r][c] == key) {
          buz.beepNote(hexaTone[r][c], 1,100 ); 
          break;
        }
      }
    }
  }
  buz.tick();
}
