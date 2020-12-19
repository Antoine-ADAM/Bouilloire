#include <DS18B20.h>

DS18B20 ds(2);
const byte PIN_LED_R = 10;
const byte PIN_LED_G = 9;
const byte PIN_LED_B = 11;
const byte PIN_ALIM = 3;
const byte PIN_CHAUF = 4;
const int puissance=1000;
const float calorie=4.18;
const float temperature=60.0;
const float testA=25.0;
const float testB=45.0;
const float correctifMasse=0.82;
const float rendement=0.8;
int startChauffe=0;
int stopChauffe=2000000000;
float tempBase=0.0;
float testATemp=0.0;
int testATime=0;
float masse=0.0;
float temp=0.0;
bool effet = true;
int statue = 0;
void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_ALIM, OUTPUT);
  pinMode(PIN_CHAUF, OUTPUT);
  digitalWrite(PIN_ALIM, LOW);
  digitalWrite(PIN_CHAUF, LOW);
  statue = 0;
  while(true){
    while (ds.selectNext()) {
        temp = ds.getTempC();
        Serial.print(String(millis())+",");
        Serial.print(String(temp));
        if(testATemp==0.0 && temp>testA){
          testATemp=temp;
          testATime=millis();
          Serial.print(",TestA");
        }else if(masse==0.0 && temp>testB && testATemp!=0.0){
          masse=(float)correctifMasse*((float)((float)puissance*(float)(millis()-testATime)/1000.0)/(float)((temp-testATemp)*calorie));
          stopChauffe=round((float)((float)(temperature-tempBase)*(float)calorie*(float)masse)/(float)((float)puissance*(float)rendement)*1000.0)+startChauffe;
          Serial.print(",Masse,"+String(masse)+",stopChauffe,"+String(stopChauffe)+",startChauffe,"+String(startChauffe));
        }
        if ((temp > temperature || millis()>stopChauffe) && statue == 1) {
          Serial.print(",STOP,"+String(stopChauffe));
          statue = 2;
          digitalWrite(PIN_CHAUF, LOW);
        }else if(statue == 0 && millis()>20000){
          Serial.print(",START");
          statue = 1;
          startChauffe=millis();
          tempBase=temp;
          digitalWrite(PIN_CHAUF, HIGH);
        }
        Serial.println("");
    }
  }
}

void loop() {
}
