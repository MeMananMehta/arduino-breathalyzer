#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define OLED_RESET 4
#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

const int ledCount = 5;    // the number of LEDs in the bar graph
int TIME_UNTIL_WARMUP = 10;
unsigned long time;
int h = 0;
int t = 0;


int analogPin = 0;
int val = 0;
Adafruit_SSD1306 display(OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);


int ledPins[] = {
  2, 3, 4, 5, 6 // Here we have the number of LEDs to use in the BarGraph
};



void setup()   {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  dht.begin();
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
}


void loop() {

  delay(100);

  h = dht.readHumidity();
  t = dht.readTemperature();
  val = readAlcohol();
  printTitle();
  printWarming();

  time = millis() / 1000;

  if (time <= TIME_UNTIL_WARMUP)
  {
    time = map(time, 0, TIME_UNTIL_WARMUP, 0, 100);
    display.drawRect(10, 50, 110, 10, WHITE); //Empty Bar
    display.fillRect(10, 50, time, 10, WHITE);

  } else{
    
     //led part
  int ledLevel = map(val,  400, 450, 0, ledCount);


  for (int thisLed = 0; thisLed < ledCount; thisLed++) {

    if (thisLed < ledLevel) {
      digitalWrite(ledPins[thisLed], HIGH);
    }

    else {
      digitalWrite(ledPins[thisLed], LOW);
    }
  }
  
    printTitle();
    printAlcohol(val);
    printAlcoholLevel(val);
  }

  display.display();

 
}


void printTitle()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(22, 0);
  display.println("Breath Analyzer");
}

void printWarming()
{
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("Warming up");
}

void printAlcohol(int value)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.print("Level: ");
  display.println(val);
  display.setTextSize(1);
  Serial.print("Current level: ");
  Serial.println(val);


  //printing dht values on monitor
  if (isnan(t) || isnan(h)) {

    Serial.println("Failed to read from DHT");

  } else {

    display.print("Humidity: ");
    display.print(h);
    display.println(" %\t");
    display.print("Temperature: ");
    display.print(t);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");

  }
  delay(1000);  //Insert a 5sec delay between readings

}

void printAlcoholLevel(int value)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15, 55);

  if (value < 400)
  {
    display.println(" You are sober");
  }
  if (value >= 400 && value < 450)
  {
    display.println(" You had a beer");
  }
  if (value > 450)
  {
    display.println("Waah! Modi ji waah");
  }
}

int readAlcohol()
{
  int val = 0;
  int val1;
  int val2;
  int val3;


  display.clearDisplay();
  val1 = analogRead(analogPin);
  delay(10);
  val2 = analogRead(analogPin);
  delay(10);
  val3 = analogRead(analogPin);

  val = (val1 + val2 + val3) / 3;
  return val;
}
