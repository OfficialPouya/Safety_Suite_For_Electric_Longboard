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

RF24 myRadio (9, 10); // pins must match
const uint64_t pipe = 0xE8E8F0F0E1LL; 
unsigned long lastTransmission; // time of last transmition for keeping in track 

int throttle_pin = A3;
byte dead_man_pin = 3;

int failCount; // how many times we were not able to send data (didnt recieve ack)
int throttle;  // speed of board

struct recData {
    int val; 
};

struct recData data;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    u8g2.begin();
    delay(1000);
 
    pinMode(dead_man_pin, INPUT_PULLUP);
    
    // starting radio
    myRadio.begin();  
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.enableAckPayload();
    myRadio.enableDynamicPayloads();   
    myRadio.openWritingPipe(pipe);
}

void loop(){
    read_remote();
    transmit_to_board();
    update_screen();
}

void read_remote(){
    // read the value of POT
    throttle = analogRead(throttle_pin);
    
    // If dead man is triggered send value of POT
    if (digitalRead(dead_man_pin) == 0){
        throttle = throttle;
    }

    else{
      throttle = 6969;
    }
}


void transmit_to_board(){
    // Transmit once every 50 millisecond
    if (millis() - lastTransmission >= 50) {
        lastTransmission = millis();
        boolean sendSuccess = false;
        sendSuccess = myRadio.write(&throttle, sizeof(throttle));

        while (myRadio.isAckPayloadAvailable()) {
            myRadio.read(&data, sizeof(data));
        }

        if (sendSuccess == true){
            failCount = 0;
            sendSuccess = false;
        }

        else{
            ++failCount;
        }

        // check how many failed packets we had 

    }
}

void update_screen(){
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setCursor(0,12);
    u8g2.print(throttle);
    u8g2.setCursor(0,32);
    u8g2.print(data.val);
    u8g2.sendBuffer();          // transfer internal memory to the display
}
