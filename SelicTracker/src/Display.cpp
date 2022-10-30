#include "Display.h"

// Display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displayInit()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
}

void printCenter(const String buf, int xPosition, int yPosition)
{
  int16_t xPositionAux, yPositionAux;
  uint16_t width, height;
  display.getTextBounds(buf, xPosition, yPosition, &xPositionAux, &yPositionAux, &width, &height); //calc width of new string
  display.setCursor((xPosition - width / 2) + (SCREEN_WIDTH / 2), yPosition);
  display.print(buf);
  display.display();
}