#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <U8g2lib.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

unsigned long timer; 

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
  u8g2.begin();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    
    radio.read(&timer, sizeof(timer));
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setCursor(0,12);
    u8g2.print("Time Recieved: ");
    u8g2.setCursor(0,22);
    u8g2.print(timer);
    u8g2.sendBuffer();          // transfer internal memory to the display
  }
}
