#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

struct package {
  int id = 1;
  int error_code = 0;
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
    
    if(dataRecieve.error_code == 1){digitalWrite(led_pin, HIGH);}
    else{digitalWrite(led_pin, LOW);}
    Serial.println("Recieve: ");
    Serial.print("Package:");
    Serial.print(dataRecieve.id);
    Serial.print("\n");
    Serial.println(dataRecieve.error_code);
    Serial.print("\n");
  }
  delay(200);
  int button_state = 0;
  button_state = digitalRead(button_pin);
  myRadio.stopListening();
  if (button_state == LOW){code_to_send = 1;}
  else{code_to_send = 0;}
  
  dataTransmit.id = dataTransmit.id + 1;
  dataTransmit.error_code = code_to_send  ;
  Serial.println("Transmit: ");
  Serial.print("Package:");
  Serial.print(dataTransmit.id);
  Serial.print("\n");
  Serial.println(dataTransmit.error_code);
  Serial.print("\n");
  
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}
