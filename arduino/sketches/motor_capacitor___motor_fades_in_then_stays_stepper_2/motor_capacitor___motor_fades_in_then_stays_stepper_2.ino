#include <Stepper.h>
#include <CapacitiveSensor.h>
CapacitiveSensor cs_2_4 = CapacitiveSensor(3, 4); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;

bool lastReading = false;
bool touching = false;
bool toggled = false;


int readingMin = 2000;
int newSpeed = 0;
int speedIncrement = 0;


// Define number of steps per revolution:
const int stepsPerRevolution = 200;
// Give the motor control pins names:
const int pwmA = 3;
const int pwmB = 11;
const int brakeA = 9;
const int brakeB = 8;
const int dirA = 12;
const int dirB = 13;
// Initialize the stepper library on the motor shield:
Stepper myStepper = Stepper(stepsPerRevolution, dirA, dirB);

void setup() {
  cs_2_4.set_CS_AutocaL_Millis(0xFFFFFFFF); // turns off calibration
  Serial.begin(9600);

  // Set the PWM and brake pins so that the direction pins can be used to control the motor:
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);

  digitalWrite(pwmA, HIGH);
  digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);

  // Set the motor speed (RPMs):
  myStepper.setSpeed(10);
}

void loop() {
  long cs = cs_2_4.capacitiveSensor(30); // TODO: figure out proper samples interval, baseline is 200000 ms

  lastReading = touching;
  //newSpeed = speedIncrement;
  //newSpeed = map(speedIncrement,0,255,80,255);
  //Serial.println(newSpeed);
  Serial.println(speedIncrement);
  myStepper.setSpeed(speedIncrement);
  myStepper.step(100);

  if (cs > readingMin) {
    speedIncrement += 100;
  }

  if (speedIncrement > 600) {
    speedIncrement = 600;
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
    Serial.println("was toggled, turned on");
  }

  else if (touching == true && toggled == true && lastReading == false) {
    toggled = false;
    speedIncrement = 0;
    Serial.println("was toggled, turned off");
  }


}
