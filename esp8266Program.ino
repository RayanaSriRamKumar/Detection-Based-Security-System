#include <ESP8266WiFi.h>
#include <Servo.h>
const int servoPin = D2;
const int irSensorPin = D7;
const int trigPin = D1;
const int echoPin = D6;
const int buzzerPin = D3; // Buzzer pin
const int flameSensorPin = D4; // Flame sensor pin
const int eyeBlinkSensorPin = A0; // Eye blink sensor pin
const int redLEDPin = D5; // Red LED pin for eye blink indication
Servo myServo;
bool isRotating = false;
void setup() {
Serial.begin(115200);
// Initialize Servo
myServo.attach(servoPin);
myServo.write(90); // Stop the servo initially

// Initialize IR Sensor
pinMode(irSensorPin, INPUT);
// Initialize Ultrasonic Sensor
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
// Initialize Buzzer
pinMode(buzzerPin, OUTPUT);
digitalWrite(buzzerPin, LOW); // Ensure buzzer is off initially
// Initialize Flame Sensor
pinMode(flameSensorPin, INPUT);
// Initialize Eye Blink Sensor
pinMode(eyeBlinkSensorPin, INPUT);
// Initialize Red LED
pinMode(redLEDPin, OUTPUT);
digitalWrite(redLEDPin, LOW); // Ensure LED is off initially
}
void loop() {
// Check flame sensor
bool flameDetected = digitalRead(flameSensorPin) == LOW;

// Read the eye blink sensor value
int eyeBlinkValue = analogRead(eyeBlinkSensorPin);
bool eyeBlinkDetected = eyeBlinkValue > 500; // Example threshold, adjust as needed

if (flameDetected) {
// Flame detected
Serial.println("Flame detected!");
digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
} else if (eyeBlinkDetected) {
// Eye blink detected
Serial.println("Eye blink detected!");
digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
digitalWrite(redLEDPin, HIGH); // Turn on the red LED
} else {
// No flame or eye blink detected, continue with other sensors
// Read the IR sensor
int irState = digitalRead(irSensorPin);
if (irState == LOW) {
// Object detected by IR sensor
Serial.println("Object detected by IR sensor");

// Start rotating the servo motor
isRotating = true;

// Measure distance using the ultrasonic sensor
int distance = measureDistance();
if (distance <= 50 && distance > 0) { // Example threshold of 50 cm
digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
} else {
digitalWrite(buzzerPin, LOW); // Turn off the buzzer
}
} else {
// No object detected
Serial.println("No object detected by IR sensor");
isRotating = false;

// Turn off the buzzer
digitalWrite(buzzerPin, LOW);
}

if (isRotating) {
// Rotate the servo motor (for continuous rotation servos)
myServo.write(0); // Full speed in one direction
} else {
// Stop the servo motor
myServo.write(90); // Stop
}
}

// Turn off the red LED if no eye blink is detected
if (!eyeBlinkDetected) {
digitalWrite(redLEDPin, LOW);
}

delay(100); // Wait for 100ms
}

int measureDistance() {
// Clear the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Set the trigPin on HIGH state for 10 microseconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Read the echoPin
long duration = pulseIn(echoPin, HIGH);
// Calculate the distance
int distance = duration * 0.0344 / 2; // Distance in cm

// Print the distance
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");

return distance;
}
