#include "ConnectionOptions.h"

WiFiClient espClient;
WebServer server(80);

bool serverFlag = true;

String responseHTML =   ""
                        "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Captive Portal</title>"
                        "<style>*{box-sizing: border-box;}  body {background-color: #e1e1e1; width: 50%; padding: 10px 3% 20px; position: relative;}"
                        "#regForm {background-color: #1A1888; margin: 70px auto;  border-radius: 1em; font-family: Raleway; position: absolute; padding: 0px; width: 592px; height: 566px; top: 7%; left: 55%;}"
                        "h3 {text-align: center; color: #B69F3F;}"
                        "h4 {text-align: center; color: #38B6FF; font-family: Raleway;}"
                        "input {padding: 10px; background-color: #0F0E0C; width: 100%; font-size: 17px; font-family: Raleway; border: none; border-bottom: 1px solid #B69F3F; color: #B69F3F;}"
                        "#button {background-color: #0F0E0C; border: 2px solid #B69F3F; border-radius: 2em; width: 150px; height: 50px; font-size: 20px; font-family: Raleway; cursor: pointer; position: absolute; bottom: 10px; left: 37%;}"
                        "#screen {background-color: #0F0E0C; border: 18px solid #000000; position: relative; padding: 25px; width: 592px; height: 357px; margin-top: 79px;}"
                        "#screenDetail {background-color: #0F0E0C; border: 6px solid #B69F3F; margin-top: 0px; position: relative; padding: 50px; width: 169px; height: 115px; top: 50%; left: 36%;}" 
                        "#hole1 {background-color: #e1e1e1; border: 8px solid #A6A8AB; border-radius: 50%; position: absolute; width: 70px; height: 70px; top:0px;}"
                        "#hole2 {background-color: #e1e1e1; border: 8px solid #A6A8AB; border-radius: 50%; position: absolute; width: 70px; height: 70px; top:0px; right: 0px;}"
                        "#hole3 {background-color: #e1e1e1; border: 8px solid #A6A8AB; border-radius: 50%; position: absolute; width: 70px; height: 70px; bottom:0px; left: 0px;}"
                        "#hole4 {background-color: #e1e1e1; border: 8px solid #A6A8AB; border-radius: 50%; position: absolute; width: 70px; height: 70px; bottom:0px; right: 0px;}"
                        "</style></head><p></p>"                            
                        "<body><form id='regForm' action='/action_page.php' method='post'><div class='tab'><br>"
                        "<div id = 'hole1'></div>"
                        "<div id = 'hole2'></div>"
                        "<div id = 'hole3'></div>"
                        "<div id = 'hole4'></div>"
                        "<div id='screen'>"
                        "<p><h4>Insert your network informations</h4></p>"
                        "<p><input name='ssid' size='40' type='ssid' placeholder='SSID'/></p><br>"
                        "<p><input name='password' size='40' type='password' placeholder='Password'/></p><br>"
                        "<input id = 'button' type = 'submit' value = 'Press'>"
                        "</div>"
                        "<div id='screenDetail'></div>"
                        "<br>"
                        "</body></html>";     


void CaptivePortal::WiFiInit (const char* ssid, const char* password)
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
 
void CaptivePortal::setSSID(String SSID)
{
    m_SSID = SSID;
}

void CaptivePortal::setPassword(String Password)
{
    m_Password = Password;
}

void CaptivePortal::AccessPoint()
{
    const byte DNS_PORT = 53;
    
    IPAddress local_IP(8,8,4,4);
    DNSServer dnsServer;  

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP, local_IP, IPAddress (255,255,255,0));
    WiFi.softAP("Selic Tracker");
   
    dnsServer.start(DNS_PORT, "*", local_IP);

    Serial.print("AP IP address: ");
    Serial.println(local_IP);

    server.on("/", handleRoot);
    server.on("/action_page.php", handleForm);
    server.on("/cancel.php", handleEsc);
    server.onNotFound(handleNotFound); 
    server.onNotFound([]()
    {
        server.send(200,"text/html", responseHTML); 
        
    });
    
    server.begin();
    Serial.println("Server started");

    while (serverFlag)
    {
        dnsServer.processNextRequest();
        server.handleClient();
        delay(2);    
    }
 
    server.stop(); 
    server.close();
    Serial.println("Closed server");
    
    WiFi.softAPdisconnect(true);

    server.close();
} 

void CaptivePortal::writeInEprom(uint8_t baseAddress, String message)
{ 
    CaptivePortal::clearEEPROM();

    Serial.println("Writing EEPROM... ");

    for (uint8_t i =0; i < message.length(); i++)
    {
        EEPROM.write(i, message[i]);
    }
    EEPROM.commit();
}

String CaptivePortal::readEEprom(void)
{
    String readEepromAux;

    Serial.println("Reading EEPROM...");

    for (uint8_t i = 0; i < 96; i++)
    {
        readEepromAux += char(EEPROM.read(i));
    }

    return readEepromAux;
}

void CaptivePortal::clearEEPROM(void)
{
    Serial.println("Clearing EEPROM...");

    for (uint8_t i = 0; i < 96; ++i)
    {
        EEPROM.write(i, 0);
    }
}

String CaptivePortal::getSsidFromEEPROM(void)
{
    uint8_t indexOf = CaptivePortal::readEEprom().indexOf('\\');
    String storedSSID = CaptivePortal::readEEprom().substring(2, indexOf);

    return storedSSID;
}

String CaptivePortal::getPasswordFromEEPROM(void)
{
    uint8_t indexOf = CaptivePortal::readEEprom().indexOf('\\');
    String storedPassword = CaptivePortal::readEEprom().substring(indexOf + 1, CaptivePortal::readEEprom().length());

    return storedPassword;
}


 void handleRoot()
{                                          
    server.send(200, "text/html", responseHTML);
}

void handleForm()
{
    CaptivePortal cpLauncher;
    String message;


    cpLauncher.setSSID(server.arg("ssid"));

    cpLauncher.setPassword(server.arg("password"));

    Serial.print("SSID: ");
    Serial.println(cpLauncher.getSSID().c_str());

    Serial.print("Password: ");
    Serial.println(cpLauncher.getPassword().c_str());

    serverFlag = false;

    cpLauncher.WiFiInit(cpLauncher.getSSID().c_str(), cpLauncher.getPassword().c_str());

    display.println("Connected to: ");
    display.print(cpLauncher.getSSID().c_str());
    display.display();
    delay(1500);
    display.clearDisplay();
    display.display();

    message = String(cpLauncher.getSSID().length(), HEX);
    message += String(cpLauncher.getPassword().length(), HEX);

    message += cpLauncher.getSSID();
    message += "\\";
    message += cpLauncher.getPassword();

    Serial.print("Will be write in EPROM: ");
    Serial.println(message); 

    cpLauncher.writeInEprom(0, message);
}

void handleEsc(void)
{
    serverFlag = false;
}

void handleNotFound(void)
{
    String message = "File Not Found\n\n try to connect click <a href='192.168.1.1'>here</a>";
    Serial.println("page not found");
    server.send(404, "text/plain", message);
}


     

    
