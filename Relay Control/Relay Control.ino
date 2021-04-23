#define interrupt_pin_ON 2
#define interrupt_pin_OFF 3

#define relay_1 4
#define relay_2 5
#define relay_3 6

bool state = false;

bool relay_1_state = false;
bool relay_2_state = false;
bool relay_3_state = false;

int interval = 600000;
int previousTime = 0;

void InterruptServiceRoutine_ON(){
  state = true;
}

void InterruptServiceRoutine_OFF(){
   for(int i = 4; i<= 6; i++){
    digitalWrite(i, LOW);
  }
  state = false;
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
    int currentTime = millis();

    if(currentTime - previousTime >= interval){

      previousTime = currentTime;
    }
  } else{
    previousTime = 0;
  }
}
