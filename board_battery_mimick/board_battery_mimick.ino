#include <SPI.h>
#include <nRF24L01.h>
#include "RF24.h"


// Defining struct to hold stats 
struct board_data {
  byte slip_code;
  long battery_val;
};

RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

bool recievedData = false;
uint32_t lastTimeReceived = 0;

// Pin defination
const byte red_led = 5;
const byte yellow_led = 6;
const byte green_led = 4;
const byte error_code = 2;
const int bat_lvl = A1;
short motorSpeed;
int timeoutMax = 500;

struct board_data data;
unsigned long lastDataCheck;

void setup() {
    Serial.begin(115200);

    radio.begin();
    radio.enableAckPayload();
    radio.enableDynamicPayloads();
    radio.openReadingPipe(1, pipe);
    radio.startListening();

    pinMode(red_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    pinMode(error_code, INPUT_PULLUP);
}

void loop() {

  get_Board_Data();

  // If transmission is available
  if (radio.available()){
    // The next time a transmission is received on pipe, the data in gotByte will be sent back in the acknowledgement (this could later be changed to data from VESC!)
    radio.writeAckPayload(pipe, &data, sizeof(data));

    // Read the actual message
    radio.read(&motorSpeed, sizeof(motorSpeed));
    recievedData = true;
  }

  if (recievedData == true){
    // A speed is received from the transmitter (remote).
    lastTimeReceived = millis();
    recievedData = false;
    digitalWrite(yellow_led, LOW);
    // Write the speed.
    if(motorSpeed == 512){
      digitalWrite(green_led, HIGH);
      digitalWrite(red_led, LOW); 
    }
    else if(motorSpeed != 512){
      digitalWrite(green_led, LOW);
      digitalWrite(red_led, HIGH); 
    }
    
  }
  else if ((millis() - lastTimeReceived) > timeoutMax){
    // No speed is received within the timeout limit.
    digitalWrite(yellow_led, HIGH);
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, LOW); 
  }
}

void get_Board_Data(){
  if (millis() - lastDataCheck >= 250) {
    lastDataCheck = millis();
//    data.slip_code = digitalRead(error_code); 
//    Serial.println(data.slip_code);
//    data.battery_val = analogRead(A1);
  }
}
