#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

const uint64_t pipe[1] = {0xF0F0F0F0E1LL};
RF24 radio(9,10);



struct boardData {
  long fl;
  long fr;
  long rl;
  long rr;
  float battery;
  byte code;
};


struct boardData data;
unsigned long xx;
long old_fl;
long old_fr;
long old_rl;
long old_rr;
void setup(){
  Serial.begin(9600);
  radio.begin();
  delay(100);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.openReadingPipe(1,pipe[2]);
  radio.startListening();
}


void loop(){
    if (radio.available()){
      radio.read(&data,sizeof(data));
    }
//    if(data.fl > 2000 && data.fr > 2000 && data.rl > 2000 && data.rr > 2000){
//      data.fl = old_fl; 
//      data.fr = old_fr; 
//      data.rl = old_rl; 
//      data.rr = old_rr;   
//    }
//    else if(data.fl < -2000 && data.fr < -2000 && data.rl < -2000 && data.rr < -2000){
//      data.fl = old_fl; 
//      data.fr = old_fr; 
//      data.rl = old_rl; 
//      data.rr = old_rr;  }
//    else{
      Serial.print(data.fl);
      Serial.print(",");
      Serial.print(data.fr);
      Serial.print(",");
      Serial.print(data.rl);
      Serial.print(",");
      Serial.println(data.rr);

//      old_fl = data.fl;
//      old_fr = data.fr;
//      old_rl = data.rl;
//      old_rr = data.rr;
//    }
  
  
 }
