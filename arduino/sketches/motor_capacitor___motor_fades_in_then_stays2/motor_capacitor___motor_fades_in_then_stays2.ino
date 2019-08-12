#include <CapacitiveSensor.h>
CapacitiveSensor cs_6_7 = CapacitiveSensor(6, 7); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;

bool lastReading = false;
bool touching = false;
bool toggled = false;

int motorPin1 = 3;
int motorPin2 = 4;
int readingMin = 20000;
int newSpeed = 0;
int speedIncrement = 0;



void setup() {
  cs_6_7.set_CS_AutocaL_Millis(0xFFFFFFFF); // turns off calibration
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT); //initialize the motorIn1 pin as output
  pinMode(motorPin2, OUTPUT); //initialize the motorIn2 pin as output

}

void loop() {
  long cs = cs_6_7.capacitiveSensor(30); // TODO: figure out proper samples interval, baseline is 200000 ms

  lastReading = touching;
  //newSpeed = speedIncrement;
  //newSpeed = map(speedIncrement,0,255,80,255);
  //Serial.println(newSpeed);
  Serial.println(speedIncrement);

  if (cs > readingMin) {
    speedIncrement += 20;
  }

  if (speedIncrement > 255) {
    speedIncrement = 0;
  }

  if (cs > readingMin) {
    if (touching == false) {
      touching = true;
      Serial.println("touching");
    }
  } else {
    if (touching == true) {
      touching = false;
      Serial.println("not touching");
    }
  }

  if (touching == true && toggled == false && lastReading == false) {
    toggled = true;
    Serial.println("wasn't toggled, turned on");
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite(3, speedIncrement);   //Spins the motor on Channel A at full speed
  }

  else if (touching == true && toggled == true && lastReading == false) {
    toggled = false;
    speedIncrement = 0;
    Serial.println("was toggled, turned off");
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite(3, 0);   //Spins the motor on Channel A at full speed
  }


}
