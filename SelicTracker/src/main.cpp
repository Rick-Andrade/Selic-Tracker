#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"

#define SSID "XXXXX"
#define PASSWD "xxxx"

// HTTP object
HTTPClient http;

char json[1000] = {0};

StaticJsonDocument<768> doc;

void resultOfGet(String msg){
    memset(json,0,sizeof(json));
    msg.toCharArray(json, 1000);
    deserializeJson(doc, json);

    static uint8_t i = 0;
    const char* date[10] = {0};
    const char* value[10] = {0};
    
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

      Serial.print("Value (%): ");
      Serial.println(value[9]);
    
}

void setup(){

  Serial.begin(115200);


  WiFi.begin(SSID,PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Conected");

//Go to arduinojson.org/v6/assistant to calcule the capacity
  const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(8) + 146;
  DynamicJsonDocument doc(capacity);
}

void loop() {
//Begin the target URL, take the response and close the connection
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
    vTaskDelay(pdMS_TO_TICKS(60000));
}