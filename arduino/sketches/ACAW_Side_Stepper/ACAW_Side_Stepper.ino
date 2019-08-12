#include <CapacitiveSensor.h>
#include <Stepper.h>

CapacitiveSensor cs_2_4 = CapacitiveSensor(2, 4); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;

bool lastReading = false;
bool touching = false;
bool toggled = false;


int readingMin = 2000;
int delayIncrement = 600;
int stepIncrement = 60;
int speedIncrement = 100;


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
unsigned long currentTime;
unsigned long lastTime;

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
  
  myStepper.setSpeed(speedIncrement);

}

void loop() {
  currentTime = millis();
  long cs = cs_2_4.capacitiveSensor(30);
  lastReading = touching;

  Serial.println("currentTime: ");
  Serial.println(currentTime);

//  Serial.println(delayIncrement);

  // Step quarter revolution in one direction:
  myStepper.step(stepIncrement);
  delay(random(delayIncrement, delayIncrement + 200));

  //Step quarter revolution in the other direction:
  myStepper.step(-stepIncrement);
  delay(random(delayIncrement, delayIncrement));

  if (cs > readingMin) {
    delayIncrement -= 100;
    //speedIncrement += 1;
  }

  if (delayIncrement < 0) {
    delayIncrement = 0;
  }

    if (toggled == false) {
    delayIncrement = 600;
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
    //speedIncrement = 100;
    stepIncrement = 60;
    Serial.println("was toggled, turned on");
  }

  else if (touching == true && toggled == true && lastReading == false) {
    toggled = false;
    delayIncrement = 600;
    //speedIncrement = 1;
    stepIncrement = 0;
    Serial.println("was toggled, turned off");
  }
  lastTime = millis();
  Serial.println("lastTime: ");
  Serial.println(lastTime);
  Serial.println("delta: ");
  Serial.println(lastTime-currentTime);
  Serial.println("--------------------------");
}
