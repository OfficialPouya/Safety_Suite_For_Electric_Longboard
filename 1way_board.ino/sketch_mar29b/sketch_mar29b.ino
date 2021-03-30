#include <SPI.h>  
#include "RF24.h"
unsigned long timer;

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};



byte red_led = 6;
byte yellow_led = 5;
byte green_led = 3;
byte button_pin = 4;

int throttle;

void setup() {
    Serial.begin(115200); 
    delay(1000);
    myRadio.begin();  
    myRadio.setChannel(115); 
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.setDataRate( RF24_250KBPS );
    pinMode(red_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    pinMode(button_pin, INPUT_PULLUP);
    myRadio.openReadingPipe(1, addresses[0]);
    myRadio.startListening();
}

void loop() {
    get_data();
    update_leds();
}

void get_data(){
    if (myRadio.available()){
        while (myRadio.available()){
            myRadio.read( &throttle, sizeof(throttle) );
            Serial.print("Speed: ");
            Serial.println(throttle);
        }
    }
}

void update_leds(){
    if(throttle == 512){
        digitalWrite(green_led, LOW);
        digitalWrite(yellow_led, HIGH);
        digitalWrite(red_led, HIGH);
    }

    else if(throttle < 512){
        digitalWrite(green_led, LOW);
        digitalWrite(yellow_led, LOW);
        digitalWrite(red_led, HIGH);
    }

    else{
        digitalWrite(green_led, HIGH);
        digitalWrite(yellow_led, LOW);
        digitalWrite(red_led, LOW);
    }
}
