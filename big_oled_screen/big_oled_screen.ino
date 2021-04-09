#include <SPI.h>
#include <Wire.h>

#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 Display(OLED_RESET);

void setup()   {
  Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  Display.clearDisplay();
  Display.display();
  delay (1000);
}


void loop() {
  Display.setTextSize(1.5);
  Display.clearDisplay();
  Display.setTextColor(WHITE);
  Display.setCursor(0, 0);
  Display.println("Time Elapsed");
  Display.setTextColor(WHITE);
  Display.setCursor(0, 19);
  Display.println(millis());
  Display.display();
   
}
