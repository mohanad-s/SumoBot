
#define proximity_MR 15
#define proximity_ML A0
#define proximity_FR 0  //RX1
#define proximity_FL 4
#define proximity_RR 3
#define proximity_RL 2
#define sharpIR_Front A1
#define sharpIR_Right A2
#define sharpIR_Left A3

#define trigPin 14 // Trigger
#define echoPin 16 // Echo
long duration, cm, inches;

#define ENA 10
#define IN1 8 // Motor A
#define IN2 7 // Motor A
#define IN3 6 // Motor B
#define IN4 5 // Motor B
#define ENB 9

double currentTime, previousTime, previousBackingTime;
#define backwardTiming 600
#define rotate180Degrees 1400
#define rotate90Degrees (rotate180Degrees / 2)
#define rotate45Degrees (rotate90Degrees / 2)

void setup() {
  Serial.begin(9600);
  pinMode(proximity_MR, INPUT);
  pinMode(proximity_ML, INPUT);
  pinMode(proximity_FR, INPUT);
  pinMode(proximity_FL, INPUT);
  pinMode(proximity_RR, INPUT);
  pinMode(proximity_RL, INPUT);
  pinMode(trigPin, OUTPUT);//Define inputs and outputs
  pinMode(echoPin, INPUT); //Define inputs and outputs

  // Set all the motor control pins to outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // wait for 5 seconds after powring up
  delay(5000);
}

void loop() {
  currentTime = millis();
  //  displaySensorsReadings();
  
  Strategy_1_Wandering();
  
  //  previousTime = currentTime;
}
void Strategy_1_Wandering() {
  while (digitalRead(proximity_MR) || digitalRead(proximity_ML)
         || digitalRead(proximity_FR) || digitalRead(proximity_FL)) {
    currentTime = millis();
    if (readUltrasonic() > 50) {
      motorForward(70);
    } else if (readUltrasonic() <= 60) {
      motorForward(250);
    }
    previousTime = currentTime;
  }
  if (!digitalRead(proximity_MR) || !digitalRead(proximity_ML)
      || !digitalRead(proximity_FR) || !digitalRead(proximity_FL)) {
    while ((currentTime - previousTime) <= backwardTiming) {
      currentTime = millis();
      if(!digitalRead(proximity_RR) || !digitalRead(proximity_RL)){
        previousBackingTime = currentTime;
        while((currentTime - previousTime) <= rotate90Degrees){
          currentTime = millis();
          rotateLeft(250);
        }
      }
      motorBackward(250);
    }
    previousTime = currentTime;
    while ((currentTime - previousTime) <= rotate180Degrees) {
      currentTime = millis();
      rotateRight(250);
    }
  }
}

// display serial monitor values from sensors
void displaySensorsReadings() {
  Serial.print("currentTime: "); Serial.print(currentTime);
  Serial.print("previousTime: "); Serial.print(previousTime);
  Serial.print("\tDistance: "); Serial.print(readUltrasonic());
  Serial.print("\tFR: "); Serial.print(!digitalRead(proximity_FR));
  Serial.print("\tMR: "); Serial.print(!digitalRead(proximity_MR));
  Serial.print("\tML: "); Serial.print(!digitalRead(proximity_ML));
  Serial.print("\tFL: "); Serial.print(!digitalRead(proximity_FL));
  Serial.print("\tRR: "); Serial.print(!digitalRead(proximity_RR));
  Serial.print("\tRL: "); Serial.print(!digitalRead(proximity_RL));
  Serial.print("\tIR_F: "); Serial.print(readSharpSensors(A1));
  Serial.print("\tIR_R: "); Serial.print(readSharpSensors(A2));
  Serial.print("\tIR_L: "); Serial.println(readSharpSensors(A3));
  delay(100);
}

long readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 50000);// Read the signal from the sensor
  //  pinMode(echoPin, INPUT);
  cm = (duration / 2) / 29.1; // Divide by 29.1 or multiply by 0.0343
  inches = (duration / 2) / 74; // Divide by 74 or multiply by 0.0135
  return cm;
}

uint16_t readSharpSensors(int pinNumber) {
  uint16_t value = analogRead (pinNumber);
  uint16_t range = get_gp2d12 (value);
  return range;
}
uint16_t get_gp2d12 (uint16_t value) {
  if (value < 10) value = 10;
  return ((67870.0 / (value - 3.0)) - 40.0);
}

// functions lets you control spinning the motors
void motorForward(int Speed) {
  // For PWM maximum possible values are 0 to 255
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  // Turn on motor A & B
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void motorBackward(int Speed) {
  // For PWM maximum possible values are 0 to 255
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  // Turn on motor A & B
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void motorStop() {
  // For PWM maximum possible values are 0 to 255
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  // Turn on motor A & B
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void rotateLeft(int Speed) {
  // For PWM maximum possible values are 0 to 255
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  // Turn on motor A & B
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void rotateRight(int Speed) {
  // For PWM maximum possible values are 0 to 255
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  // Turn on motor A & B
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
