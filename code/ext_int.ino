int speedhall= 3;
int rothall= 4;
int i=0;
unsigned long old_time;
unsigned long new_time;
bool flag=false;
unsigned long delta_time_arr [12];
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
  flag=false;
  multiplier=(rot_dir)?1:-1;
  //if(has_zero(delta_time_arr,6)) current_RPM=0;
  current_RPM=60.0/(3*(average(delta_time_arr,12))/1000000.0);
}

//first few readings

void RPM_update(){
  //Serial.println("Hello");
  flag=true;
  new_time=micros();
  rot_dir= (digitalRead(rothall));
 delta_time=new_time-old_time;
 delta_time_arr[i]=delta_time;
 old_time=new_time;
  i=(i++)%10;
   //Serial.print("Hello");
  
  //flag=true;
}
void loop() {
  //Serial.println(digitalRead(speedhall));
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(rothall));
  if(flag){
   // if(delta_time<200000){Serial.println(delta_time);}
    if(current_RPM<100000){Serial.println(current_RPM);}
    {Serial.println(rot_dir);}
     
    //Serial.println(current_RPM);
    //Serial.println(rot_dir);
    flag=false;
    }
  
}
