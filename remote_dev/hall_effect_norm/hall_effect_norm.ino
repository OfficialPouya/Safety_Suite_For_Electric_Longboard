
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
//if(throttle_read > max_val){max_val = throttle_read;}
//if(throttle_read < min_val){min_val = throttle_read;}

if(throttle_read > 125){throttle  = map(throttle_read, 125, 1024, 129, 255);}
else if(throttle_read > 50 && throttle_read <= 115){throttle  = map(throttle_read, 50, 115, 100, 127);}
else if(throttle_read > 35 && throttle_read <= 50){throttle  = map(throttle_read, 35, 50, 70, 99);}
else if(throttle_read > 30 && throttle_read <= 35){throttle  = map(throttle_read, 30, 35, 50, 69);}
else if(throttle_read > 25 && throttle_read <= 30){throttle  = map(throttle_read, 25, 30, 40, 49);}
else if(throttle_read > 23 && throttle_read <= 25){throttle  = map(throttle_read, 23, 25, 30, 39);}
else if(throttle_read >= 21 && throttle_read <= 23){throttle  = map(throttle_read, 20, 23, 0, 30);}
else if(throttle_read < 21){throttle = 0;}
else{ throttle = 128; }

Serial.println(throttle);
//Serial.println(throttle_read);

delay(50);
}
