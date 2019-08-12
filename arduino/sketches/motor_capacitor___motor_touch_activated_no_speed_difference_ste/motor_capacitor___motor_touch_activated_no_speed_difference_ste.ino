#include <AccelStepper.h>
#include <MultiStepper.h>
#include <CapacitiveSensor.h>
#include <Stepper.h>

CapacitiveSensor cs_6_7 = CapacitiveSensor(2, 4); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;

bool lastReading = false;
bool touching = false;
bool toggled = false;


int readingMin = 500;
int newSpeed = 0;
int speedIncrement = 0;

int lowestSpeed = 0;
int highestSpeed = 250;
int speedIncrementValue = 1;
int stepIncrement = 200;




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
  cs_6_7.set_CS_AutocaL_Millis(0xFFFFFFFF); // turns off calibration
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
  myStepper.setSpeed(100);
}

void loop() {
  long cs = cs_6_7.capacitiveSensor(30); // TODO: figure out proper samples interval, baseline is 200000 ms
  Serial.println(cs);

  if (cs > readingMin) {
    //AccelStepper::enableOutputs();
    myStepper.step(100);
  }

  if (cs < readingMin) {
    myStepper.step(0);
    //AccelStepper::disableOutputs();
  }
}
