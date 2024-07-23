//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 5

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.setCursor(0,0);
  //lcd.print("Hello, world!");
  //lcd.setCursor(0,1);
  //lcd.print("Afroza's Arduino!"); 
  Serial.begin(115200);
}
void loop()
{
  DHT.read(DHT11_PIN);
  int moisturesensorValue = analogRead(A1);
  int lightsensorValue = analogRead(A0);

  lcd.clear();
  // delay in between reads for stability
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(DHT.temperature);
  lcd.print("C");
  lcd.print(" ");
  lcd.print("H:");
  lcd.print(DHT.humidity);
  lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print("M:");
 // lcd.print(moisturesensorValue);
  if (moisturesensorValue == 0) {
    lcd.print("DRY");
  } else {
    lcd.print("WET");
  }
  
  lcd.print(" ");

  lcd.setCursor(6,1);
  lcd.print("L:");
  //lcd.print(lightsensorValue);
  if (lightsensorValue <= 350) {
    lcd.print("DARK");
  } else {
    lcd.print("LIGHT");
  }

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
