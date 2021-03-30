#include <SPI.h>  
#include "RF24.h"
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

unsigned long lastTransmission;
// typedef struct package Package;
// Package dataTransmit;

byte dead_man_pin = 5;
byte red_led = 2;
byte yellow_led = 2;
byte green_led = 2;
int throttle = A1;
char str[11]; 
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    // Serial.begin(115200);
    u8g2.begin();
    delay(1000);
    pinMode(red_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    pinMode(dead_man_pin, INPUT_PULLUP);
    myRadio.begin();  
    myRadio.setChannel(115); 
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.setDataRate(RF24_250KBPS);    
    myRadio.openWritingPipe(addresses[0]);
}


void loop() {
    // read the value of POT
    throttle = analogRead(A1);
    // If dead man is triggered send value of POT, otherwise send
    // 512 (the middle val of potentiometer)
    if (digitalRead(dead_man_pin) == 0){throttle = throttle;}
    else{throttle = 512;}
    transmit_to_board();
    update_screen();
}


void transmit_to_board(){
    // Transmit once every 50 millisecond
    if (millis() - lastTransmission >= 50) {
        lastTransmission = millis();
        boolean sendSuccess = false;
        // Transmit the speed value (0-1024).
        myRadio.write(&throttle, sizeof(throttle));
    }
}

void update_screen(){
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.drawStr(0,10, throttle);      // write speed value
    u8g2.sendBuffer();          // transfer internal memory to the display
}
