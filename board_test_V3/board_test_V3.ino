#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

const uint64_t pipe[1]= {0xF0F0F0F0E1LL};
RF24 radio(9,10);

int pot_pin = A3;
byte slip_pin = 4;
byte eject_pin = 3;
byte buzzer_pin = 2;
int throttle; 
unsigned long lastMsg;

struct boardData{
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
  radio.openReadingPipe(1,pipe[0]);
  radio.startListening();
  radio.setRetries(15,15);

  pinMode(slip_pin, INPUT_PULLUP);
  pinMode(eject_pin, INPUT_PULLUP);
  pinMode(buzzer_pin, OUTPUT);
}

void loop(){
  read_board_vals();
  read_remote_send_board();
  print_data();
}

void read_board_vals(){
  int val = analogRead(pot_pin);
  data.fl = 100+val;
  data.fr = 200+val;
  data.rl = 300+val;
  data.rr = 400+val;
  if(digitalRead(slip_pin)== 0){
  data.slip = 0;
  }
  else{
    data.slip = 1;
  }
  
  if(digitalRead(eject_pin)== 0){
  data.eject = 0;
  }
  else{
    data.eject = 1;
  }
}

void read_remote_send_board(){
    if (radio.available()){
    radio.writeAckPayload( 1, &data, sizeof(data) );
    radio.read( &throttle,sizeof(throttle) );
    lastMsg = millis();
    }

    if(millis() - lastMsg > 1000){tone(buzzer_pin, 10);} 
    else{noTone(buzzer_pin);}
    
}

void print_data(){
    Serial.print("Throttle: ");
    Serial.println(throttle); 
}
