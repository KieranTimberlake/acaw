#include <CapacitiveSensor.h>
#include <Stepper.h>

#include <AccelStepper.h>

CapacitiveSensor cs_2_4 = CapacitiveSensor(2, 4); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;

bool lastReading = false;
bool touching = false;
bool toggled = false;
bool pos = true;

int readingMin = 2000;
int delayIncrement = 600;
int touchIncrement = 50;
int stepIncrement = 90;
int speedIncrement = 100;
int loopInterval = 10;
int maxDelay = 600;

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

unsigned long timeElapsed = 0;
unsigned long timeElapsedTouch = 0;

void setup() {
  cs_2_4.set_CS_AutocaL_Millis(0xFFFFFFFF); // turns off calibration
  Serial.begin(9600);

  //Set the PWM and brake pins so that the direction pins can be used to control the motor:
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);

  digitalWrite(pwmA, LOW);
  digitalWrite(pwmB, LOW);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);

  myStepper.setSpeed(speedIncrement);
}

void loop() {
  delay(loopInterval);
  timeElapsed += loopInterval;
  timeElapsedTouch += loopInterval;
  long cs = cs_2_4.capacitiveSensor(30);
  lastReading = touching;

  if (cs > readingMin) {
    touching = true;
    Serial.println("touching");
  } else {
    touching = false;
  }

  if (touching == true && toggled == false && lastReading == false) {
    toggled = true;
    digitalWrite(pwmA, HIGH);
    digitalWrite(pwmB, HIGH);
    digitalWrite(brakeA, LOW);
    digitalWrite(brakeB, LOW);
    Serial.println("was off, turned on");
  }

  else if (touching == true && toggled == true && lastReading == false) {
    toggled = false;
    digitalWrite(pwmA, LOW);
    digitalWrite(pwmB, LOW);
    digitalWrite(brakeA, LOW);
    digitalWrite(brakeB, LOW);
    delayIncrement = maxDelay;
    Serial.println("was on, turned off");
  }  

  // randomize for less 'mechanical' sound = clank... clank, clank!
  if (timeElapsed >= delayIncrement && toggled) {
    // Step quarter revolution in one direction, switch every other
      Serial.println(timeElapsed);
     Serial.println("delay ended");
    if ( pos ) {
      myStepper.step(stepIncrement);
      pos = false;
    } else {
      myStepper.step(-stepIncrement);
      pos = true;
    }

    timeElapsed = 0;
    Serial.println("moved");
  }

  if (timeElapsedTouch >= touchIncrement && toggled and touching) {
    delayIncrement -= 10;
    Serial.println("decrease delay");
    if (delayIncrement < loopInterval) {
      delayIncrement = loopInterval;
    }
    timeElapsedTouch = 0;
  }

  if (toggled == false) {
    delayIncrement = maxDelay;
  }



}
