#include <SPI.h>  
#include "RF24.h"
#include <nRF24L01.h>
unsigned long timer;

RF24 myRadio (9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;


struct recData {
    int val;  
};

struct recData data;

bool recievedData = false;
uint32_t lastTimeReceived = 0;

byte button_pin = 7;
int speeder; 
int throttle;

void setup() {
    Serial.begin(115200); 
    pinMode(button_pin, INPUT_PULLUP);
    delay(1000);

    myRadio.begin();  
    myRadio.enableAckPayload();
    myRadio.enableDynamicPayloads();
    myRadio.openReadingPipe(1, pipe);
    myRadio.startListening();
}


void loop() {
  read_button();
  get_data();
  update_screen();
}


void read_button(){
  byte state = digitalRead(button_pin);
  if(state == 1){data.val=10;}
  else{data.val=123;}
}

void get_data(){
   if (myRadio.available()){
            myRadio.writeAckPayload(pipe, &data, sizeof(data));
            myRadio.read(&throttle, sizeof(throttle) );
            speeder = throttle; 
    }
}

void update_screen(){
  Serial.print("Button State: ");
  Serial.print(data.val);
  Serial.print("Throttle: ");
  Serial.print(speeder);
}
