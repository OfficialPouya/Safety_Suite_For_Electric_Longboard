
int throttle;
int throttle_read;
int  my_speed;
int diff; 
int max_val;
int min_val; 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
throttle_read = analogRead(A0);
if(throttle_read > max_val){max_val = throttle_read;}
if(throttle_read < min_val){min_val = throttle_read;}

if(throttle_read > 250){
  throttle  = map(throttle_read, 250, 1020, 129, 255);
}

else if (throttle_read < 230){
  throttle  = map(throttle_read, 30, 230, 0, 127);
}

else{ throttle = 128; }

Serial.println(throttle);

delay(50);
}
