#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

struct package {
  int error_code = 0;
  int speed_val = 0;
  boolean dead_man_switch = false;
};


typedef struct package Package;
Package dataRecieve;
Package dataTransmit;

int button_pin = 2;
int led_pin = 3;
int code_to_send = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  int button_pin = 2;
  int led_pin = 3;
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop() {
  if ( myRadio.available()) {
    while (myRadio.available()){
      myRadio.read( &dataRecieve, sizeof(dataRecieve) );
    }
    
//    Serial.println("Recieved");
//    Serial.println("Dead-Man");
//    Serial.println(dataRecieve.dead_man_switch);
//    Serial.println("Speed");
//    Serial.println(dataRecieve.speed_val);
//    Serial.print("\n");
  }
  delay(200);
  int button_state = 0;
  button_state = digitalRead(button_pin);
  myRadio.stopListening();
  if (button_state == LOW){code_to_send = 1;}
  else{code_to_send = 0;}
  
  dataTransmit.error_code = code_to_send  ;
//  Serial.println("Sent ");
//  Serial.println(dataTransmit.error_code);
//  Serial.print("\n");
  
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}
