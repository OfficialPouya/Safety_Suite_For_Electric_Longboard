#include <SPI.h>  
#include "RF24.h"
#include <nRF24L01.h>
unsigned long timer;

RF24 myRadio (9, 10);
const uint64_t pipe[1] = {0xF0F0F0F0E1LL};

bool recievedData = false;
byte button_pin = 4;
int sending = 666;
int reading = 999;
void setup() {
    Serial.begin(115200); 
    pinMode(button_pin, INPUT_PULLUP);
    delay(1000);

    myRadio.begin();  
    myRadio.setAutoAck(true);
    myRadio.enableAckPayload();
    myRadio.enableDynamicPayloads();
    myRadio.openReadingPipe(1, pipe[0]);
    myRadio.startListening();
}


void loop() {
  if(digitalRead(button_pin) == 1){sending = 1;}
  else{sending = 0;}
  if (myRadio.available()){
            myRadio.writeAckPayload(1, sending, sizeof(sending));
            myRadio.read(&reading, sizeof(reading) );
    }
   Serial.print("Value Sent: ");
   Serial.println(sending);
   Serial.print("Value Read: ");
   Serial.println(reading);
}
