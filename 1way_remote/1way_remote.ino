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

byte dead_man_pin =4;
byte red_led = 6;
byte yellow_led = 7;
byte green_led = 8;
int throttle = A3;

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
    if (digitalRead(dead_man_pin) == 0){
        throttle = throttle;
        digitalWrite(yellow_led, LOW);
        digitalWrite(red_led, LOW);
        digitalWrite(green_led, HIGH);
    }

    else{
        digitalWrite(yellow_led, HIGH);
        digitalWrite(red_led, LOW);
        digitalWrite(green_led, LOW);
        throttle = 512;
    }
    transmit_to_board();
    update_screen();
}


void transmit_to_board(){
    // Transmit once every 50 millisecond
    if (millis() - lastTransmission >= 500) {
        lastTransmission = millis();
        myRadio.write(&throttle, sizeof(throttle));
    }
    else{
        digitalWrite(yellow_led, LOW);
        digitalWrite(red_led, HIGH);
        digitalWrite(green_led, LOW);  
    }
}



void update_screen(){

    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setCursor(0,12);
    u8g2.print(throttle);
    u8g2.setCursor(0,22);
    u8g2.print(lastTransmission);
    u8g2.sendBuffer();          // transfer internal memory to the display
}
