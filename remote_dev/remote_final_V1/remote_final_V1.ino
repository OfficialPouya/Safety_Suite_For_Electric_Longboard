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

const uint64_t pipe = 0xE8E8F0F0E1LL; 

unsigned long lastTransmission; // time of last transmition for keeping in track 



// bread board v3 for testing
byte dead_man_pin = 3;
byte speaker = 5;
int throttle_pin = A3; // for bread board v3 using 

int failCount; // how many times we were not able to send data (didnt recieve ack)
int throttle;  // speed of board

// structure for data board sends
struct boardData {
//  long fl;
//  long bl;
//  long fr;
//  long br;
//  byte slip;
//  byte eject;  
};



long frontLeft = -1;
long backLeft= -1; 
long frontRight= -1;
long backRight= -1;
byte slip_code=1;
byte eject_code=1;
int tester_val;

struct boardData data;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {
    u8g2.begin();
    delay(1000);
    
    // setting pin modes
    pinMode(speaker, OUTPUT);
    pinMode(dead_man_pin, INPUT_PULLUP);
    
    // starting radio
    myRadio.begin();  
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.enableAckPayload();
    myRadio.enableDynamicPayloads();   
    myRadio.openWritingPipe(pipe);
}

// calling functions
void loop() { 
    read_remote();
    transmit_to_board();
    read_board_data();
    update_screen();
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
        throttle = 6969; // the val 42069 is intereperated by the board as dead-man is not pressed
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
    if (millis() - lastTransmission >= 150) {
        lastTransmission = millis();
        boolean sendSuccess = false;
        sendSuccess = myRadio.write(&throttle, sizeof(throttle));

        while (myRadio.isAckPayloadAvailable()) {
            myRadio.read(&tester_val, sizeof(tester_val));
        }

        if (sendSuccess == true){
            failCount = 0;
            sendSuccess = false;
        }

        else{
            ++failCount;
        }

        // check how many failed packets we had 
        if (failCount >= 5) {
            eject_code = 0;
        }

        else{
            eject_code = 1;
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
//    frontLeft = data.fl;
//    frontRight = data.fr;
//    backLeft = data.bl;
//    backRight = data.br;
//    slip_code = data.slip;
//// eject_code = data.eject;
//      tester_val = data.tester;
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
    u8g2.setFont(u8g2_font_5x7_tf); // choose a suitable font
    u8g2.setCursor(0,8);
    u8g2.print("Speed: ");
    u8g2.println(throttle);
    u8g2.setCursor(0,15);
    u8g2.print("Tester: ");
    u8g2.print(tester_val);
//    u8g2.print("Slip: ");
//    u8g2.print(slip_code);
//    u8g2.print("   EJ: ");
//    u8g2.println(eject_code);
//    u8g2.setCursor(0,22);
//    u8g2.print("FL: ");
//    u8g2.print(frontLeft);
//    u8g2.print("   FR: ");
//    u8g2.println(frontRight);
//    u8g2.setCursor(0,30);
//    u8g2.print("BL: ");
//    u8g2.print(backLeft);
//    u8g2.print("   BR: ");
//    u8g2.println(backRight);
    u8g2.sendBuffer();          // transfer internal memory to the display
}
