#include <SPI.h>  
#include "RF24.h"
#include <nRF24L01.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


RF24 myRadio (9, 10);
const uint64_t pipe[1] = {0xF0F0F0F0E1LL};
unsigned long lastTransmission;

byte button = 4;
int failCount;
int val = 69;
int sending;
int got_it = 0;
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    // Serial.begin(115200);
    u8g2.begin();
    delay(1000);
    pinMode(button, INPUT_PULLUP);
    
    myRadio.begin(); 
    myRadio.setAutoAck(true); 
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.enableAckPayload();
    myRadio.enableDynamicPayloads();   
    myRadio.openWritingPipe(pipe[0]);
}

void loop() {
  if (digitalRead(button) == 0){sending = 60;}
  else{sending = 420;}
  
  if (millis() - lastTransmission >= 50) {
    lastTransmission = millis();
    boolean sendSuccess = false;
    sendSuccess = myRadio.write(&sending, sizeof(sending));
  
    if (myRadio.isAckPayloadAvailable()) {
      myRadio.read(val, sizeof(val));
      got_it = 100;
    }
    
    if (sendSuccess == true){
      failCount = 0;
      sendSuccess = false;
    }
    
    else{
      ++failCount;
    }
  }

      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.setCursor(0,12);
      u8g2.print("Sending: ");
      u8g2.print(sending);
      u8g2.setCursor(0,22);
      u8g2.print("Reading: ");
      u8g2.print(val);
      u8g2.setCursor(0,32);
      u8g2.print("GOT ACK: ");
      u8g2.print(got_it);
      u8g2.sendBuffer();          // transfer internal memory to the display
   
  
}
