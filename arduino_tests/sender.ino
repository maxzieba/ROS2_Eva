#define JOYSTICK_SIGNAL A0
#define READ_LED 3
#define WRITE_LED 5

int value;

enum rw_state{
  write = 0,
  read = 1,
  both_on = 2,
  both_off = 3,
};

void set_rw_led_states(rw_state state){
  if (state == write){
    digitalWrite(WRITE_LED, 100);
    digitalWrite(READ_LED, 0);
  }
  else if (state == read){
    digitalWrite(WRITE_LED, 0);
    digitalWrite(READ_LED, 100);
  }
  else if (state == both_off){
    digitalWrite(WRITE_LED, 0);
    digitalWrite(READ_LED, 0);
  }
  else if (state == both_on){
    digitalWrite(WRITE_LED, 1);
    digitalWrite(READ_LED, 1);
  }
}


void setup() {
  pinMode(JOYSTICK_SIGNAL, INPUT);
  pinMode(READ_LED, OUTPUT);
  pinMode(WRITE_LED, OUTPUT);
  Serial.begin(9600);  // to print in console
  Serial1.begin(9600);  // to communicate with second Arduino
}


void loop() {
  value = analogRead(JOYSTICK_SIGNAL);

  if (Serial1.available() > 0){
    Serial.println(String("Got:") + Serial1.readStringUntil("\n"));
  }
  
  if (value > 600){
    set_rw_led_states(read);
    Serial1.write("RX\n");
  }
  else if (value < 500){
    set_rw_led_states(write);
    Serial1.println("W" + String(int(map(value, 0, 500, 0, 180))) + "\n");
  }  
  else{
    set_rw_led_states(both_off);
  }
  delay(10);
}
