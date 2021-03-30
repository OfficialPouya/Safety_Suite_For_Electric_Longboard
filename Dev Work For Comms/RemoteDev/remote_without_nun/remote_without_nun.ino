#include <SPI.h>
#include "RF24.h"
#include <nRF24L01.h>

// Defining struct to hold stats 
struct board_data {
  byte slip_code;
  long battery_val;
};



struct board_data data;

// Defining Variables
short throttle;

// Pin defination
const byte red_led = 2;
const byte yellow_led = 3;
const byte green_led = 4;
const byte dead_man_switch = 5;
const byte speaker = 6;
const int speed_wheel = A1;

// Defining variables for NRF24 communication
bool connected = false;
short failCount;
const uint64_t pipe = 0xE8E8F0F0E1LL; // If you change the pipe, you will need to update it on the receiver to.
unsigned long lastTransmission;

// Instantiating RF24 object for NRF24 communication
RF24 radio(9, 10);


void setup() {
    Serial.begin(115200);
    
    // Setting Pin Modes
    pinMode(dead_man_switch, INPUT_PULLUP);
    pinMode(red_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    pinMode(speaker, OUTPUT);

    // Start radio communication
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.enableAckPayload();
    radio.enableDynamicPayloads();
    radio.openWritingPipe(pipe);
}


void loop() {
    throttle = analogRead(A1);
    // Use throttle and trigger to drive motors
    if (digitalRead(dead_man_switch) == 0){throttle = throttle;}
    // 512 is the middle position of potentiometer
    else{throttle = 512;}
    // Transmit to receiver
//    Serial.println(throttle);
    transmit_to_board();
}


void transmit_to_board(){
  
  // Transmit once every 50 millisecond
  if (millis() - lastTransmission >= 50) {
    lastTransmission = millis();
    boolean sendSuccess = false;
    
    // Transmit the speed value (0-1024).
    sendSuccess = radio.write(&throttle, sizeof(throttle));
    
//     // Listen for an acknowledgement reponse (return of VESC data).
//     while (radio.isAckPayloadAvailable()) {
//         radio.read(&data, sizeof(data));
// //        Serial.println(data.battery_val);
//     }
//         if (sendSuccess == true){
//             // Transmission was a succes
//             failCount = 0;
//             sendSuccess = false;
//              Serial.println("Transmission Success");
//              digitalWrite(speaker, LOW);
//         }

//         else {
//             // Transmission was not a succes
//             ++failCount;
//             Serial.println("Transmission Failed");
//             digitalWrite(speaker, HIGH);
//         }

//         // If lost more than 5 transmissions, we can assume that connection is lost.
//         if (failCount < 10) {
//             connected = true;
//         } 
//         else {
//             connected = false;
//         }
    }
}
