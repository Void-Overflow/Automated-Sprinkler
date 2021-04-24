#define interrupt_pin_ON 2
#define interrupt_pin_OFF 3

#define relay_1 4
#define relay_2 5
#define relay_3 6

bool state = false;
int sequence_index = 0;

const unsigned long long int interval = 10 * 60000;
unsigned long previousTime = 0;

void InterruptServiceRoutine_ON(){
  state = true;
  sequence_index = 0;
}

void InterruptServiceRoutine_OFF(){
   for(int i = 4; i<= 6; i++){
    digitalWrite(i, LOW);
  }

  state = false;
  sequence_index = 0;
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

void loop() {
  if (state == true){
    unsigned long long currentTime = millis();
    
    if(currentTime - previousTime >= interval || sequence_index == 0){
      switch(sequence_index){
        case 0:
          digitalWrite(relay_1, HIGH);
          sequence_index = 1;
          break;
        case 1:
          digitalWrite(relay_1, LOW);
          digitalWrite(relay_2, HIGH);
          sequence_index = 2;
          break;
        case 2:
          digitalWrite(relay_2, LOW);
          digitalWrite(relay_3, HIGH);
          sequence_index = 3;
          break;
        case 3:
          digitalWrite(relay_3, LOW);
          sequence_index = 0;
          state = false;
          break;
      }
      previousTime = currentTime;
    }
  } else{
     for(int i = 4; i<= 6; i++){
      digitalWrite(i, LOW);
     }
      sequence_index = 0;
      previousTime = 0;
  }
}
