//--------------------------
// DEFINING THE PINS
//--------------------------

// sensor inputs
int leftSensorPin = A1;
int rightSensorPin = A3;

// motors
int leftMotorPin = 9;
int rightMotorPin = 7;



void setup() {
  // jap200
  // initialise communication at 9600 baud rate
  Serial.begin(9600);

  // setting up the motor pins
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

}

void loop() {

  // reading the peak values from the two sensors
  long leftSensor = analogRead(leftSensorPin);
  long rightSensor = analogRead(rightSensorPin);

  //defining the variables
  int leftDutyCycle = 0;
  int rightDutyCycle = 0;
  long calculation = leftSensor-rightSensor;
  long sensorDifference = calculation;
  char direction[8] = "FORWARD";

  int targetDutyCycle = 65;
  float kDiff = 0.05;
  int differenceThreshold = 150; // used to prevent the mouse from 'zig zagging'



  //--------------------------
  // CALCULATING MOTOR OUTPUT
  //--------------------------

  if (sensorDifference > differenceThreshold) {
    // higher voltage at left sensor -> turning left
    // (making the right motor spin faster than the left motor)
    strcpy(direction, "LEFT");

    leftDutyCycle = targetDutyCycle - kDiff*sensorDifference;
    rightDutyCycle = targetDutyCycle + kDiff*sensorDifference;

  } else if (sensorDifference < -differenceThreshold) {
    // higher voltage at right sensor -> turning right
    // (making the left motor spin faster than the right motor)
    strcpy(direction, "RIGHT");

    leftDutyCycle = targetDutyCycle + kDiff*abs(sensorDifference);
    rightDutyCycle = targetDutyCycle - kDiff*abs(sensorDifference);

  } else {
    //continue forward
    strcpy(direction, "FORWARD");

    leftDutyCycle = targetDutyCycle;
    rightDutyCycle = targetDutyCycle;
  }



  // setting the speed of the motors
  analogWrite(leftMotorPin, leftDutyCycle);
  analogWrite(rightMotorPin, rightDutyCycle);



  //--------------------------
  // USER INFO
  // printing helpful information for the user to easily review
  // (only use one at a time - can't use both so comment the other one)
  //--------------------------

  // -> Serial monitor analysis
  Serial.print("Left Motor: ");
  Serial.print(leftDutyCycle);
  Serial.print(",    Right Motor: ");
  Serial.print(rightDutyCycle);
  Serial.print(",    Direction:");
  Serial.print(direction);
  Serial.print(",    Difference:");
  Serial.print(sensorDifference);
  Serial.println(".");

  // -> Serial plotter analysis
  // Serial.println(leftDutyCycle);
  // Serial.println(rightDutyCycle);
  // Serial.prinln(sensorDifference);


  delay(50); //delay before taking new readings

}

