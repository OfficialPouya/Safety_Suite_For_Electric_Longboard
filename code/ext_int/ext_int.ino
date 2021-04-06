#include <U8g2lib.h>
bool trigger;
#define NUM_READINGS 1
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
bool flag=false;
unsigned long delta_time_arr [NUM_READINGS];
unsigned long delta_time;
bool rot_dir;
double current_RPM=0;
//bool init=0;
bool has_zero(unsigned long delta_time_arr [], int len){
  for (int i=0;i<len;i++){
    if (delta_time_arr[i]==0) return 1;
  }
  return 0;
}
double average(unsigned long delta_time_arr [], int len){
  double sum=0.00;
  for (int i=0;i<len;i++){
    sum=sum+delta_time_arr[i];
  }
  return(double(sum/double(len)));
}
void setup() {
  u8g2.begin();
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(speedhall, INPUT_PULLUP);
pinMode(rothall, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(speedhall), RPM_update, RISING);
//ADD START CONDITION
cli();//stop interrupts
//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624/20;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
sei();//allow interrupts
}
int multiplier;
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13
//generates pulse wave of frequency 20hz (takes two cycles for full wave- toggle high then toggle low)
  flag=true;
 if(micros()-new_time>1000000){
    current_RPM=60.0/(6*(average(delta_time_arr,NUM_READINGS))/1000000.0);
  }
  else
  {
  current_RPM=0;
  }
  trigger=false;
}

//first few readings

void RPM_update(){
  //Serial.println("Hello");
  flag=false;
  //trigger=true;
  new_time=micros();
  rot_dir= (digitalRead(rothall));
 delta_time=new_time-old_time;
 delta_time_arr[i]=delta_time;
 old_time=new_time;
  i=(i++)%NUM_READINGS;
   //Serial.print("Hello");
  
  //flag=true;
}

void loop() {
  //Serial.println(digitalRead(speedhall));
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(rothall));
  if(flag){
   // if(delta_time<200000){Serial.println(delta_time);}
    if(current_RPM<100000)
    {u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setCursor(0,12);
    u8g2.print("RPM: ");
    u8g2.print(current_RPM);
    u8g2.setCursor(0,24);
    if(rot_dir){
      u8g2.print("Forward");
    }
    else{
       u8g2.print("Reverse");
    }
   
    u8g2.sendBuffer();
    }
   
     
    //Serial.println(current_RPM);
    //Serial.println(rot_dir);
    flag=false;
    }
  
}
