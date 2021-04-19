#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


RF24 radio(9,10);
const uint64_t pipe[1] = {0xF0F0F0F0E1LL};

int throttle = 0;
byte buzzer_flag = 0;
unsigned long lastTransmission;
unsigned long buzzer_trigger_time;

byte dead_man_pin = 4;
byte buzzer_pin = 3;
int throttle_pin = A3;



struct boardData {
  long fl;
  long fr;
  long rl;
  long rr;
  byte slip;
  byte eject;
};

struct boardData data;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup(){
//  Serial.begin(9600);
  u8g2.begin();
  radio.begin();
  delay(100);
  
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.stopListening();
  radio.openWritingPipe(pipe[0]);
  radio.setRetries(15,15);

  pinMode(dead_man_pin, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);
 }

 void loop(){
  read_remote_vals();
  send_data_to_board();
  display_data();
}


void read_remote_vals(){
  if(digitalRead(dead_man_pin) == 0){
    throttle = analogRead(throttle_pin);
  }

  else{
    throttle = 6969;
  }
  
  
}



void send_data_to_board(){
  if (millis() - lastTransmission >= 100) {
    if(radio.write(&throttle,sizeof(throttle))){
      lastTransmission = millis();
      if(radio.isAckPayloadAvailable()){
        radio.read(&data,sizeof(data));
      }
    throttle = throttle + 1;
    }  
  }

  if(buzzer_flag != 1 && data.slip == 0 || data.eject == 0 || millis() - lastTransmission >=1000){
    buzzer_flag = 1;
    buzzer_trigger_time = millis();
  }

  if(buzzer_flag == 1 && millis() - buzzer_trigger_time < 200){
    tone(buzzer_pin, 1000);
  }

  else if (buzzer_flag == 1 && millis() - buzzer_trigger_time > 200){
    noTone(buzzer_pin);
    buzzer_flag = 0;
  }

  
//  if(millis() - lastTransmission >=1000){
//    tone(buzzer_pin, 1000);
//  }
//  else if((millis() - lastTransmission < 1000)){
//    noTone(buzzer_pin);  
//  }
}



void display_data(){
//    Serial.print("Sent: ");
//    Serial.println(throttle);
//    Serial.print("FL: ");
//    Serial.println(data.fl);
//    Serial.print("FR: ");
//    Serial.println(data.fr);
//    Serial.print("RL: ");
//    Serial.println(data.rl);
//    Serial.print("RR: ");
//    Serial.println(data.rr);
//    Serial.print("SLIP: ");
//    Serial.println(data.slip);
//    Serial.print("Eject: ");
//    Serial.println(data.eject);
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_micro_tr); // choose a suitable font
    u8g2.setCursor(0,8);
    u8g2.print(throttle);
    u8g2.setCursor(0,16);
    u8g2.print("fl: ");
    u8g2.print(data.fl);
    u8g2.print("  fr: ");
    u8g2.print(data.fr);
    u8g2.setCursor(0,24);
    u8g2.print("rl: ");
    u8g2.print(data.rl);
    u8g2.print("  rr: ");
    u8g2.print(data.rr);
    u8g2.setCursor(0,30);
    u8g2.print("sl: ");
    u8g2.print(data.slip);
    u8g2.print("  ej: ");
    u8g2.print(data.eject);
    u8g2.sendBuffer();          // transfer internal memory to the display

}
