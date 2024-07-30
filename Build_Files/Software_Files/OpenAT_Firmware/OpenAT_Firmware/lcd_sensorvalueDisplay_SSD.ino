//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <SPI.h>
#include <DFRobot_DHT11.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DFRobot_DHT11 DHT;
#define DHT11_PIN 5
#define BUZZER 18
#define LED 26

void setup()
{
 // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  pinMode(LED, OUTPUT);  
  pinMode(BUZZER, OUTPUT);  
  //lcd.setCursor(0,0);
  //lcd.print("Hello, world!");
  //lcd.setCursor(0,1);
  //lcd.print("Afroza's Arduino!"); 
  Serial.begin(115200);
}
void loop()
{
  DHT.read(DHT11_PIN);
  int moisturesensorValue = analogRead(32);
  int lightsensorValue = analogRead(35);
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("T:");
  display.print(DHT.temperature);
  display.print("C ");
  display.setCursor(0, 16);
  display.print("H:");
  display.print(DHT.humidity);
  display.println("%");

  display.setCursor(0, 32);
  display.print("M:");
  if (moisturesensorValue == 0) {
    display.print("DRY");
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(700);
  } else {
    display.print("WET");
  }

  display.setCursor(0,48);
  display.print("L:");
  if (lightsensorValue <= 350) {
    display.print("DARK");
  } else {
    display.print("LIGHT");
  }
   display.display();


  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  delay(1000);
  
  Serial.print("moisture:");
  Serial.println(moisturesensorValue);
  delay(100);  // delay in between reads for stability

  Serial.print("light:");
  Serial.println(lightsensorValue);
  delay(100);  // delay in between reads for stability
}
