#include <Arduino.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include "main.h"

#define BUZZER 18

// HTTP object
HTTPClient http;

char json[1000] = {0};

const char* date[10] = {0};
const char* value[10] = {0};

StaticJsonDocument<768> doc;

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

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);

  CaptivePortal cpTest;

  displayInit();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Access the IP 8.8.4.4");
  display.display();

  cpTest.AccessPoint();

  Serial.print("EEPROM content: ");
  Serial.println(cpTest.readEEprom());

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