#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;

void setup() {

#if RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.
  
  oled.clear();
  oled.setFont(Adafruit5x7);
  oled.set1X();
}

void loop() {  
  double val = 420.69;
  oled.print("Speed: ");
  oled.println(val);
  oled.print("Slip: ");
  oled.print("NO");
  oled.print("   EJ: ");
  oled.println("NO");
  oled.print("FL: ");
  oled.print(3000);
  oled.print("   FR: ");
  oled.println(3000);
  oled.print("BL: ");
  oled.print(3000);
  oled.print("   BR: ");
  oled.println(3000);
  delay(1000);
  oled.clear();
}
