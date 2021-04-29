byte buzzer_pin = 7;
int tone_val = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {
  tone(buzzer_pin, 2493);
  delay (85);
  noTone(buzzer_pin);
  delay (85);  
  tone(buzzer_pin, 2493);
  delay (85);
  noTone(buzzer_pin);
  delay (85);
  tone(buzzer_pin, 2493);
  delay (85);
  noTone(buzzer_pin);
  
  delay(3000);
  
  tone(buzzer_pin, 2216);
  delay (85);
  noTone(buzzer_pin);
  delay (85);
  tone(buzzer_pin, 1864);
  delay (85);
  noTone(buzzer_pin);
  delay (85);
  tone(buzzer_pin, 1480);
  delay (85);
  noTone(buzzer_pin);
  
  delay(3000);

  tone(buzzer_pin, 1480);
  delay (85);
  noTone(buzzer_pin);
  delay (85);
  tone(buzzer_pin, 1864);
  delay (85);
  noTone(buzzer_pin);
  delay (85);
  tone(buzzer_pin, 2216);
  delay (85);
  noTone(buzzer_pin);

  delay(3000);
  
  tone(buzzer_pin, 659*3);
  delay (85);
  noTone(buzzer_pin);
  delay (85);  
  tone(buzzer_pin, 659*3);
  delay (85);
  noTone(buzzer_pin);
  delay (85);
  tone(buzzer_pin, 659*3);
  delay (85);
  noTone(buzzer_pin);
  
  delay(5000);
}
