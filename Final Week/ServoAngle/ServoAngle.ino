#include <ESP32Servo.h>

#define SERVO_PIN 15 // Define the pin to which the servo is connected

Servo myServo;

void setup() {
  Serial.begin(9600);
  
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2500);
}

void loop() {
  /*for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(3000); // Wait 5 seconds before moving to the next angle
  }*/
  myServo.write(0);
  delay(1000);
  myServo.write(30);
  delay(5000);
}
