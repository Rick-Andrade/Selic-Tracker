#ifndef H_CONNECTIONOPTIONS_H
#define H_CONNECTIONSOPTIONS_H

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <Adafruit_SSD1306.h>
#include "Display.h"


void handleRoot(void);
void handleForm(void);
void handleEsc(void);
void handleNotFound(void);

class CaptivePortal
{
private:
    String m_SSID;
    String m_Password;
public:
    void setSSID(String SSID);
    void setPassword(String Password);
    String getSSID(void) {return m_SSID;};
    String getPassword(void) {return m_Password;};
    void AccessPoint(void);
    void writeInEprom(uint8_t baseAddress, String message);
    String readEEprom(void);
    void clearEEPROM(void);
    void WiFiInit(const char* ssid, const char* password);
    String getSsidFromEEPROM(void);
    String getPasswordFromEEPROM(void);
};


#endif