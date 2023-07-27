// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

Adafruit_BMP280 bmp; // I2C

// Replace with your network credentials
const char* ssid = "CDV";
const char* password = "060609143";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readBMP280Temperature() {
  // Read temperature as Celsius (the default)
  float t = bmp.readTemperature();
  // Convert temperature to Fahrenheit
  //t = 1.8 * t + 32;
  if (isnan(t)) {    
    Serial.println("Failed to read from BMP280 sensor!");
    return "";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readBMP280Altitude() {
  float p = bmp.readAltitude() / 100.0F;
  if (isnan(p)) {
    Serial.println("Failed to read from BMP280 sensor!");
    return "";
  }
  else {
    Serial.println(p);
    return String(p);
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  bool status; 
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID); 
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/altitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBMP280Altitude().c_str());
  });
  
  // Start server
  server.begin();
}
 
void loop(){
  
}
