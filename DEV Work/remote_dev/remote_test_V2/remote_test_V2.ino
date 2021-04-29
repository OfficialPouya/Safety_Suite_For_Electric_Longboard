#include <SPI.h>  
#include "RF24.h"
#include <nRF24L01.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

RF24 myRadio (9, 10); // pins must match

const uint64_t pipe[1] = {0xF0F0F0F0E1LL};

unsigned long lastTransmission; // time of last transmition for keeping in track 



// bread board v3 for testing
byte dead_man_pin = 4;
byte red_led = 6;
byte yellow_led = 7;
byte green_led = 8;
byte speaker = 5;
byte lost_con = 0;
int throttle_pin = A3; // for bread board v3 using 

int failCount; // how many times we were not able to send data (didnt recieve ack)
int throttle;  // speed of board

// structure for data board sends
struct boardData {
  int batteryLevel;
  byte slip;   
};

struct boardData data;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    u8g2.begin();
    delay(100);
    
    // setting pin modes
    pinMode(red_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    pinMode(speaker, OUTPUT);
    pinMode(dead_man_pin, INPUT_PULLUP);
    delay(100);
    
    // starting radio
    myRadio.begin();
    delay(100);
    myRadio.setAutoAck(true);  
    myRadio.enableAckPayload();
    myRadio.enableDynamicPayloads();   
    myRadio.openWritingPipe(pipe[0]);
    myRadio.setRetries(15,15);
}

// calling functions
void loop() { 
    read_remote();
    transmit_to_board();
    read_board_data();
}


/*
 NAME: read_remote()
 DESCRIPTION: Read values of the controller deadman switch + throttle 
 INPUTS: NONE
 OUTPUTS: LED change 
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: throttle value
 */
void read_remote(){
    // read the value of POT
    throttle = analogRead(throttle_pin);
    
    // If dead man is triggered send value of POT
    if (digitalRead(dead_man_pin) == 0){
        throttle = throttle;
    }

    
    //  otherwise send 512 (the middle val of potentiometer)
    else{
        throttle = 512;
    }
}

/*
 NAME: transmit_to_board()
 DESCRIPTION: Sends data to board AND reads ack + payload from board
 INPUTS: NONE
 OUTPUTS: Speaker change  
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: failCount, last transmition
 */
void transmit_to_board(){
    // Transmit once every 50 millisecond
    if (millis() - lastTransmission >= 50) {
        lastTransmission = millis();
        boolean sendSuccess = false;
        sendSuccess = myRadio.write(&throttle, sizeof(throttle));

        if(myRadio.isAckPayloadAvailable()) {
            myRadio.read(&data, sizeof(data));
        }

        update_screen();
        
        if (sendSuccess == true){
            failCount = 0;
            sendSuccess = false;
        }

        else{
            ++failCount;
        }

        // check how many failed packets we had 
        if (failCount >= 5) {
            lost_con = 1;
        }

        else{
            lost_con = 0;
        }
    }
}



/*
 NAME: read_board_data()
 DESCRIPTION: parsing data board send back
 INPUTS: NONE
 OUTPUTS: Speakers & LEDs could chagne 
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: No actual values changed
 */
void read_board_data(){
    if(data.slip == 0){
        digitalWrite(yellow_led, HIGH);
    }
    else{
        digitalWrite(yellow_led, LOW);
    }

    if(data.batteryLevel > 513){
        digitalWrite(red_led, LOW);
        digitalWrite(green_led, HIGH); 
    }

    else if(data.batteryLevel <513 && data.batteryLevel > 256){
        digitalWrite(red_led, HIGH);
        digitalWrite(green_led, HIGH);
    }

    else{
        digitalWrite(red_led, HIGH);
        digitalWrite(green_led, LOW);
    }
}


/*
 NAME: read_board_data()
 DESCRIPTION: reads 
 INPUTS: NONE
 OUTPUTS: Speaker change  
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: failCount, last transmition
 */
void update_screen(){
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setCursor(0,12);
    u8g2.print(throttle);
    u8g2.setCursor(0,22);
    u8g2.print(data.slip);
    u8g2.setCursor(0,32);
    u8g2.print(data.batteryLevel);
    u8g2.sendBuffer();          // transfer internal memory to the display
}
