#include <SPI.h>  
#include "RF24.h"
#include <NintendoExtensionCtrl.h>
RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};
Nunchuk nchuk;
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
  nchuk.begin();
  while (!nchuk.connect()) {
    Serial.println("Controller not detected!");
    delay(1000);
  }
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
    
    if(dataRecieve.error_code == 1){digitalWrite(led_pin, HIGH);}
    else{digitalWrite(led_pin, LOW);}
//    Serial.println("Recieve: ");
//    Serial.println(dataRecieve.error_code);
//    Serial.print("\n");
  }
  delay(200);
  boolean success = nchuk.update();
  if (!success) {  // Ruh roh
    Serial.println("Controller disconnected!");
    delay(1000);
  }
  else{
    myRadio.stopListening();

    // bool val for dead man 0 not pressed ; 1 is pressed
    // speed should range from 0-255 but only able to read from 32-213
    
    dataTransmit.dead_man_switch = nchuk.buttonZ(); // reads state of z
    dataTransmit.speed_val = nchuk.joyY(); // sends y value for nunchuck
    Serial.print("SENT");
    Serial.print("Dead Man:");
    Serial.println(dataTransmit.dead_man_switch);
    Serial.print("Speed:");
    Serial.println(dataTransmit.speed_val);
    Serial.print("\n");
  }

  
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
 
}
