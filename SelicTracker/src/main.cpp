#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SSID "XXXXX"
#define PASSWD "XXXXX"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define BUZZER 18

// Display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// HTTP object
HTTPClient http;

char json[1000] = {0};

const char* date[10] = {0};
const char* value[10] = {0};

StaticJsonDocument<768> doc;

void printCenter(const String buf, int x, int y)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
  display.setCursor((x - w / 2) + (128 / 2), y);
  display.print(buf);
  display.display();
}

void resultOfGet(String msg)
{
    memset(json,0,sizeof(json));
    msg.toCharArray(json, 1000);
    deserializeJson(doc, json);

    static uint8_t i = 0;
    
    for (JsonObject item : doc.as<JsonArray>())
    {
      if (i <= 9)
      {
        date[i] = item["data"];
        value[i] = item["valor"];
        i++;
      }
    }

      i = 0;

      Serial.print("Last atualization in ");
      Serial.println(date[9]);
      display.clearDisplay();
      display.setTextSize(1);
      printCenter("Last atualization in: ", 0, 0);
      display.setTextSize(1);
      printCenter(String(date[9]), 0, 9);

      Serial.print("Value (%): ");
      Serial.println(value[9]);
      display.println();
      display.println();
      display.setTextSize(1);
      printCenter("Value: ", 0, 25);
      display.setTextSize(2);
      printCenter(String(value[9]) + "%", 0, 40);  
}

void setup(){

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Connecting to WiFi...");
  display.display();

  WiFi.begin(SSID,PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Conected");

  display.println("Connected to: ");
  display.print(SSID);
  display.display();
  delay(1500);
  display.clearDisplay();
  display.display();

//Go to arduinojson.org/v6/assistant to calcule the capacity
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(8) + 146;
  DynamicJsonDocument doc(capacity);

  pinMode(BUZZER, OUTPUT);
}

void loop()
{
/**
 * @brief Begin the target URL, take the response and close the connection
 * 
 */
    http.begin("https://api.bcb.gov.br/dados/serie/bcdata.sgs.4189/dados/ultimos/10?formato=json");

    int httpCode = http.GET();
    if (httpCode > 0) { 
        String payload = http.getString();
        resultOfGet(payload);
    }
    else {
        Serial.println("Request Fail");
    }
    http.end();

    if (atof(value[9]) < 14)
    {
      uint8_t count = 0;

      while(count != 6)
      {
        digitalWrite(BUZZER, !digitalRead(BUZZER));
        vTaskDelay(pdMS_TO_TICKS(2000));
        count ++;
      }
    }

    vTaskDelay(pdMS_TO_TICKS(60000));
}