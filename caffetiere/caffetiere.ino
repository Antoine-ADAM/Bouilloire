#include <DS18B20.h>

DS18B20 ds(2);
const byte PIN_LED_R = 10;
const byte PIN_LED_G = 9;
const byte PIN_LED_B = 11;
const byte PIN_ALIM = 3;
const byte PIN_CHAUF = 4;
float tempo0 = 0.0;
float temp;
bool effet = true;
int statue = 0;
void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_ALIM, OUTPUT);
  pinMode(PIN_CHAUF, OUTPUT);
  digitalWrite(PIN_ALIM, HIGH);
  digitalWrite(PIN_CHAUF, HIGH);
  displayColor(0, 0, 255);
  statue = 1;
  while (true) {
    while (ds.selectNext()) {
      temp = ds.getTempC();
      Serial.println(temp);
      if (temp > 55.0) {
        statue = 2;
        break;
      }
      if (millis() > 160000) {
        statue = 3;
        break;
      }
      if (temp - tempo0 < 0.10) {
        if (millis() > 20000) {
          statue = 4;
          break;
        }
        if (effet) {
          effet = false;
        } else {
          effet = true;
        }
      } else {
        effet = true;
      }
      tempo0 = temp;
      if (effet) {
        displayColor((byte)constrain(map(temp, 15.0, 56.0, 0, 255), 0, 255), 0, (byte)constrain(map(temp, 15.0, 56.0, 255, 0), 0, 255));
      }else{
        displayColor(0,0,0);
      }
    }
    if (statue != 1)break;
    if (millis() > 160000) {
        statue = 3;
        break;
      }
  }
  digitalWrite(PIN_CHAUF, LOW);
  if (statue == 2) {
    displayColor(0, 255, 0);
    delay(30000);
  }
  if (statue == 3) {
    for (int i = 0; i <= 15; i++) {
      displayColor(255, 255, 0);
      delay(500);
      displayColor(255, 0, 0);
      delay(500);
    }
  }
  if (statue == 4) {
    for (int i = 0; i <= 30; i++) {
      displayColor(255, 0, 255);
      delay(250);
      displayColor(255, 0, 0);
      delay(250);
    }
  }
  displayColor(0, 255, 255);
  digitalWrite(PIN_ALIM, LOW);
  while (true) {
    delay(10000);
  }
}

void loop() {
}

void displayColor(byte r, byte g, byte b) {
  analogWrite(PIN_LED_R, r);
  analogWrite(PIN_LED_G, g);
  analogWrite(PIN_LED_B, b);
}
