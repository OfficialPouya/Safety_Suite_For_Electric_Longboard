#include <U8g2lib.h>
bool trigger;
#define NUM_READINGS 5


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif



U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

int speedhall= 3;
int rothall= 4;
int i=0;

unsigned long old_time;
unsigned long new_time;
unsigned long delta_time_arr [NUM_READINGS];
unsigned long delta_time;
unsigned long eoi_time;

double current_RPM=0;

bool rot_dir;
bool flag=false;
//bool init=0;

/*

*/
bool has_zero(unsigned long delta_time_arr [], int len){
  for (int i=0;i<len;i++){
    if (delta_time_arr[i]==0) return 1;
  }
  return 0;
}

/*

*/
double average(unsigned long delta_time_arr [], int len){
  double sum=0.00;
  for (int i=0;i<len;i++){
    sum=sum+delta_time_arr[i];
  }
  return(double(sum/double(len)));
}


void setup() {
  u8g2.begin();

  pinMode(speedhall, INPUT_PULLUP);
  pinMode(rothall, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(speedhall), RPM_update, RISING);
  cli();  //clear interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624/100;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();  //enable global interrupts
}



int multiplier;
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13
//generates pulse wave of frequency 20hz (takes two cycles for full wave- toggle high then toggle low)
  flag=true;
 //if(micros()-new_time>1000000){
  if(millis()-eoi_time > 650){
    current_RPM = 0;
    for(int w=0; w<NUM_READINGS; w++){
      delta_time_arr[w]=0;
    }
    eoi_time = micros(); 
  }
  else{
    current_RPM=60.0/(double(NUM_READINGS)*6.0*(average(delta_time_arr,NUM_READINGS))/(1000000.0));
  }
  

}


void RPM_update(){
  flag=false;
  new_time=micros();
  rot_dir= (digitalRead(rothall));
  delta_time=new_time-old_time;
  delta_time_arr[i]=delta_time;
  old_time=new_time;
  i = i++;
  i=i%NUM_READINGS;
  eoi_time = millis();
}

void loop() {
  //Serial.println(digitalRead(speedhall));
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(rothall));
  if(flag){
    update_my_screen();
  flag=false;
  }  
}



void update_my_screen(){
  if(current_RPM<100000){
      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_10x20_tr); // choose a suitable font
      u8g2.setCursor(0,13);
      u8g2.print("RPM: ");
      u8g2.print(current_RPM);
      u8g2.setCursor(0,30);
      
       if(current_RPM == 0){
         u8g2.print("Stopped");
       }
       else if(rot_dir){
         u8g2.print("Forward");
       }
       else{
         u8g2.print("Reverse");
       }


//  for testing
/* 
      if(current_RPM == 0){
        u8g2.print(millis());
      }
      else{
        u8g2.print(eoi_time);
      }
*/
      u8g2.sendBuffer();
    }
}
