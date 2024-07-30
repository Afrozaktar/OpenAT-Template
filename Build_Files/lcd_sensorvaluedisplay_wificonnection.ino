#include <Wire.h> 
#include <SPI.h>
#include <DFRobot_DHT11.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>

// OLED display parameters
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 sensor parameters
DFRobot_DHT11 DHT;
#define DHT11_PIN 5
#define BUZZER 18
#define LED 26

// Analog pins for sensors
#define MOISTURE_SENSOR_PIN 32
#define LIGHT_SENSOR_PIN 35

// Wi-Fi credentials
const char* ssid = "Afroza's S24";
const char* password = "Shabuj123";

// Create an instance of the web server
WebServer server(80);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Initialize DHT11 sensor
  pinMode(DHT11_PIN, INPUT);

  // Initialize LED and Buzzer
  pinMode(LED, OUTPUT);  
  pinMode(BUZZER, OUTPUT);  

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define web server routes
  server.on("/", HTTP_GET, []() {
    DHT.read(DHT11_PIN);
    int moisturesensorValue = analogRead(MOISTURE_SENSOR_PIN);
    int lightsensorValue = analogRead(LIGHT_SENSOR_PIN);

    // Create plain text response
    String response = "Garden Monitoring\n";
    response += "Temperature: " + String(DHT.temperature) + "C\n";
    response += "Humidity: " + String(DHT.humidity) + "%\n";
    response += "Moisture: " + String((moisturesensorValue == 0) ? "DRY" : "WET") + "\n";
    response += "Light: " + String((lightsensorValue <= 350) ? "DARK" : "LIGHT") + "\n";

    server.send(200, "text/plain", response);
  });

  // Start server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Read sensor data
  DHT.read(DHT11_PIN);
  int moisturesensorValue = analogRead(MOISTURE_SENSOR_PIN);
  int lightsensorValue = analogRead(LIGHT_SENSOR_PIN);

  // Update OLED display
  display.clearDisplay();
  display.setTextSize(2);             
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
  display.setCursor(0, 48);
  display.print("L:");
  if (lightsensorValue <= 350) {
    display.print("DARK");
  } else {
    display.print("LIGHT");
  }
  display.display();

  // Print data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(DHT.temperature);
  Serial.print(" C  Humidity: ");
  Serial.println(DHT.humidity);
  
  Serial.print("Moisture: ");
  Serial.println(moisturesensorValue);
  
  Serial.print("Light: ");
  Serial.println(lightsensorValue);

  delay(1000); // Adjust delay as necessary
}
