#include <SPI.h>  
#include "RF24.h"
#include <NintendoExtensionCtrl.h>
unsigned long timer;

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

void setup() {
  Serial.begin(115200); 
  delay(1000);
  nchuk.begin();
  while (!nchuk.connect()) {
    Serial.println("Controller not detected!");
    delay(1000);
  }
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
   timer = millis();
//   while (!myRadio.available()){Serial.println("No Connection!");}
   while (myRadio.available() && timer % 50 != 0){
      myRadio.read( &dataRecieve, sizeof(dataRecieve) );
      if(dataRecieve.error_code == 1){digitalWrite(led_pin, HIGH);}
      else{digitalWrite(led_pin, LOW);}
      Serial.println("Recieve: ");
      Serial.println("ERROR CODE:");
      Serial.println(dataRecieve.error_code);
      Serial.print("\n");
  }
    
   if(timer % 50 == 0){
      Serial.println("50 ms has passed");
      boolean success = nchuk.update();
      if (!success) {
        Serial.println("Controller disconnected!");
        delay(1000);
      }
      myRadio.stopListening();
      dataTransmit.dead_man_switch = nchuk.buttonZ(); // reads state of z
      dataTransmit.speed_val = nchuk.joyY(); // sends y value for nunchuck
      myRadio.openWritingPipe(addresses[0]);
      myRadio.write(&dataTransmit, sizeof(dataTransmit));
      myRadio.openReadingPipe(1, addresses[0]);
      myRadio.startListening();
    }
    
}
