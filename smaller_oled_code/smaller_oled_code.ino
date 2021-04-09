#include <SPI.h>
#include <U8g2lib.h>



U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
  u8g2.begin();
  
}
void loop() {
 
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setCursor(0,12);
    u8g2.print("Time Elapsed:");
    u8g2.setCursor(0,22);
    u8g2.print(millis());
    u8g2.sendBuffer();          // transfer internal memory to the display
}
