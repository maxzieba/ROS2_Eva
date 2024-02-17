#include <Servo.h>
#define SERVO_CONTROL_PIN 7
#define SERVO_READ_PIN A0

Servo myservo;
byte packet;

int servo_mech_limits[2] = {0, 180};
int servo_raw_sensor_values[2];

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(SERVO_READ_PIN, INPUT);
  myservo.attach(SERVO_CONTROL_PIN);
  calibrate_servo();
}

void calibrate_servo(){
  for (int i=0;i<2;i++){
    myservo.write(servo_mech_limits[i]);
    servo_raw_sensor_values[i] = analogRead(SERVO_READ_PIN);
    delay(1000);
    Serial.println(servo_raw_sensor_values[i]);
  }
}

float map2(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int read_servo_pos(){
    int raw_value = analogRead(SERVO_READ_PIN);

    if (raw_value < servo_raw_sensor_values[1]){
      raw_value = servo_raw_sensor_values[1];
    }
    if (raw_value > servo_raw_sensor_values[0]){
      raw_value = servo_raw_sensor_values[0];
    }
    float retVal = map2(float(raw_value), servo_raw_sensor_values[0],
                                          servo_raw_sensor_values[1],
                                          servo_mech_limits[0],
                                          servo_mech_limits[1]);                 
    return int(retVal);
}



void loop() {

  String incoming_command;

  if (Serial1.available() > 0){
    int read_value;

    incoming_command = Serial1.readStringUntil('\n');
    Serial.println(incoming_command);

    if (incoming_command.startsWith("W")){  // WRITE
      int value = incoming_command.substring(1).toInt();
      myservo.write(value);
      Serial.println("W");
      Serial1.write("W\n");
    }
    else if (incoming_command.startsWith("R")){
      read_value = read_servo_pos();
      String retVal = incoming_command[0] + String(read_value);
      Serial.println(retVal);
      Serial1.println(retVal + '\n');
    }
    else{
      Serial.println("X");
      Serial1.write("X\n");
    }
  }
  
  delay(10);
}
