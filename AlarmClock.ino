                                    
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
float p = 3.1415926;
int Seconds, Minutes, Hours, Days, Months, Years;
const int pResistor = A0;
const int buttonPin1 = 2;     
const int buttonPin2 = 3;     
const int speakerPin =  7;    

int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;
int Background = 1;
int TextColor = 1;
int value;
bool TurnOffAlarm = true;
  void drawtext(char *text, uint16_t color) {
    tft.setCursor(20, 60);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.print(text);
}
int ChangeBackground(){
  if (Background == 1){
     tft.fillScreen(ST77XX_BLACK);
    }
    if (Background == 2){
     tft.fillScreen(ST77XX_MAGENTA);
    }
    if (Background == 3){
     tft.fillScreen(ST77XX_BLUE);
    }
     if (Background == 4){
     tft.fillScreen(ST77XX_RED);
    }
    if (Background == 5){
     tft.fillScreen(ST77XX_YELLOW);
     Background-=4;
    }
    if (Background == 6){
     tft.fillScreen(ST77XX_BLACK);
     Background-=5;
    }
  }
  int ChangeTextColor(){
  if (TextColor == 1){
     tft.setTextColor(ST77XX_WHITE);
    }
    if (TextColor == 2){
     tft.setTextColor(ST77XX_MAGENTA);
    }
    if (TextColor == 3){
     tft.setTextColor(ST77XX_BLUE);
    }
     if (TextColor == 4){
     tft.setTextColor(ST77XX_RED);
    }
    if (TextColor == 5){
     tft.setTextColor(ST77XX_YELLOW);
  
    }
    if (TextColor == 6){
     tft.setTextColor(ST77XX_BLACK);
   TextColor-=5;
    }
  }
  RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


 void Time(){
  
  DateTime now = rtc.now();
    tft.print(now.year(), DEC);
    tft.print("/");
    tft.print(now.month(), DEC);
    tft.print('/');
    tft.print(now.day(), DEC);
    tft.print(" ");
    tft.println(daysOfTheWeek[now.dayOfTheWeek()]);
    
    tft.println();
    tft.print(now.hour(), DEC);
    tft.print(':');
    tft.print(now.minute(), DEC);
    tft.print(':');
    tft.print(now.second(), DEC);
    tft.println();
  }
  void MainScreenPrint() {
    tft.setTextWrap(true);
    tft.setCursor(20, 0);
    ChangeBackground();
    ChangeTextColor();
    tft.setTextSize(2);
    tft.println("The Time is");
    tft.setTextSize(2);
    Time();
    tft.setCursor(20,90);
    const uint8_t SAMPLE_NAME [] PROGMEM = {};
}


int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//            mid C  C#   D    D#   E    F    F#   G    G#   A

bool isAlarmOn = true;

void setup(void) {
  
  Serial.begin(9600);
  pinMode(pResistor, INPUT);
  pinMode(speakerPin, OUTPUT);      
   pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);   
  Serial.print("Hello! ST77xx TFT Test");

   tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

 
  Serial.println("Initialized");

  uint16_t time = millis();
  
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(1000);

  tft.fillScreen(ST77XX_BLACK);
  drawtext("My Alarm Clock", ST77XX_WHITE);
  delay(1000);
  
  Serial.println("done");
  delay(1000);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  rtc.adjust(DateTime(2018, 9, 24, 7, 0, 0));
}

int getPhotoSensorConfigValue() {
  return 200;
}

void setAlarm(bool v) {
  isAlarmOn = v;
}

bool shouldAlarmBeActive() {
  if(millis()<15000) {
    return false;
  }
  return isAlarmOn;
}


void loop() {
  value = analogRead(pResistor);
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState1 == HIGH) { 
    Background++;
    ChangeBackground();
    }
  if (buttonState2 == HIGH) { 
    TextColor++;
    ChangeTextColor();
    } 
   MainScreenPrint();
  delay(500);
 uint16_t time = millis();
 if(shouldAlarmBeActive()) {
  Serial.println(value);
  if (TurnOffAlarm == true){
    for (int i = 0; i < numTones; i++) {
      tone(speakerPin, tones[i]);
      delay(500);
      MainScreenPrint();
    }
  }
    
  if (value < getPhotoSensorConfigValue()) {
    TurnOffAlarm = false;
    Serial.println("ALARM TRIGGERED!");
    noTone(speakerPin);
  }
  }
 } 
 





