#include <SPI.h>  
#include "RF24.h"
#include <nRF24L01.h>
unsigned long timer;

RF24 myRadio (9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

struct boardData {
    int batteryLevel; 
    int slip;
};

struct boardData data;

bool recievedData = false;
bool flag = false; 

uint32_t lastTimeReceived = 0;

byte red_led = 6;
byte yellow_led = 7;
byte green_led = 8;
byte button_pin = 4;
int bat_pot = A3; 
int throttle;

void setup() {
    Serial.begin(115200); 
    pinMode(red_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    pinMode(button_pin, INPUT_PULLUP);
    delay(1000);

    myRadio.begin();  
    myRadio.enableAckPayload();
    myRadio.enableDynamicPayloads();
    myRadio.openReadingPipe(1, pipe);
    myRadio.startListening();
}

void loop() {
    flag = false; 
    read_board_values();
    get_data_remote();
    update_leds();
    if(flag){Serial.println(throttle);}
}

void read_board_values(){
    if(digitalRead(button_pin) == 1){
        data.slip = 1;
    }
    else{
        data.slip = 0;
    }
//    Serial.println(data.slip);
    data.batteryLevel = analogRead(bat_pot);
}

void get_data_remote(){
    if (myRadio.available()){
            myRadio.writeAckPayload(pipe, &data, sizeof(data));
            myRadio.read(&throttle, sizeof(throttle) );
            flag = true; 
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
