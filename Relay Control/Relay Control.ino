#define interrupt_pin_ON 2
#define interrupt_pin_OFF 3

#define relay_1 4
#define relay_2 5
#define relay_3 6

void InterruptServiceRoutine_ON(){
  for(int i = 4; i<= 6; i++){
    digitalWrite(i, HIGH);
  }
}

void InterruptServiceRoutine_OFF(){
   for(int i = 4; i<= 6; i++){
    digitalWrite(i, LOW);
  }
}

void setup() {
  pinMode(interrupt_pin_ON, INPUT);
  pinMode(interrupt_pin_OFF, INPUT);


  for(int i = 4; i<= 6; i++){
    pinMode(i, OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(interrupt_pin_ON), InterruptServiceRoutine_ON, RISING);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin_OFF), InterruptServiceRoutine_OFF, RISING);
}

void loop() {}
