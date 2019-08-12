#include <CapacitiveSensor.h>
CapacitiveSensor cs_6_7 = CapacitiveSensor(6, 7); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;


bool actuatorOn = false;

int motorPin1 = 3;
int motorPin2 = 4;
int readingMin = 10000 ;
int newSpeed = 0;

void setup() {
  cs_6_7.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT); //initialize the motorIn1 pin as output
  pinMode(motorPin2, OUTPUT); //initialize the motorIn2 pin as output

}

void loop() {
  long cs = cs_6_7.capacitiveSensor(30);

  if (cs > readingMin) {
    if (actuatorOn == false) {
      for (int speedIncrement = 0 ; speedIncrement <= 255; speedIncrement += 5) {
        analogWrite(motorPin1, 200);
        analogWrite(motorPin2, 0);
        speedIncrement += newSpeed;

      }
      analogWrite(motorPin1, 200);
      analogWrite(motorPin2, 0);
      actuatorOn = !actuatorOn;
    }
  }


  if (actuatorOn == true) {
    if (cs > readingMin) {
      analogWrite(motorPin1,200);
      analogWrite(motorPin2, 0);
      actuatorOn = !actuatorOn;
    }
  }


  Serial.println(cs);
}
