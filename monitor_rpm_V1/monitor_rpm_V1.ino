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
  byte slip;
  byte eject;
};


struct boardData data;


void setup(){
  Serial.begin(9600);
  radio.begin();
  delay(100);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.openReadingPipe(1,pipe[2]);
  radio.startListening();
  radio.setRetries(15,15);
}


void loop(){
    if (radio.available()){
      radio.read(&data,sizeof(data));
    }
    
    Serial.print(data.fl);
    Serial.print(",");
    Serial.print(data.fr);
    Serial.print(",");
    Serial.print(data.rl);
    Serial.print(",");
    Serial.println(data.rr);

  
  
 }
