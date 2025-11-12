// Final Module

#include <math.h>
class Motor {
  public:
    float x, y, z;
    int pin;
    float multiplier;

    // Constructor to initialize the variables
    Motor(float x, float y, float z, float multiplier, int pin) {
      this->x = x;
      this->y = y;
      this->z = z;
      this->pin = pin;
      this->multiplier = multiplier;
    }

    // Method to calculate the Euclidean distance between two 3D coordinates
    float calcDistance(float posx, float posy, float posz) {
      float distance = sqrt(pow(posx - x, 2) + pow(posy - y, 2) + pow(posz - z, 2));
      return distance;
    }

    // Method to set the intensity of the motor using PWM
    void setIntensity(float posx, float posy, float posz, float nextmotorX, float nextmotorY, float nextmotorZ) {
      float distance = calcDistance(posx, posy, posz);
      //This is the distance to the next motor
      float d = calcDistance(nextmotorX, nextmotorY, nextmotorZ);
      int intensity = (255 / d) * (d - distance) * (this->multiplier);
      analogWrite(pin, intensity);
      delay(10);
    }
};

Motor motor1(0.0, 0.0, 0.0, 1.0, 3); // Create a Motor object
Motor motor2(0.0, 0.0, 1.0, 1.0, 5); // Create a Motor object
Motor motor3(0.0, 0.0, 2.0, 1.0, 6); // Create a Motor object
Motor motor4(0.0, 0.0, 3.0, 1.0, 9); // Create a Motor object
Motor motor5(0.0, 0.0, 4.0, 1.0, 10); // Create a Motor object

void setup() {
  Serial.begin(230400);

}

void loop() {

  if (Serial.available() > 0) {
    // Read a string from serial input
    String inputString = Serial.readStringUntil('\n');
    float dist = inputString.toFloat(); // Convert the string to a float and assign it to posx
    float disttmp = 0.0;
    // Print the values as a formatted string
    //motor1.setIntensity(0.0,0.0,dist,0.0,0.0,1.0); // Call setIntensity() method on the Motor object
    // motor2.setIntensity(0.0,0.0,dist,0.0,0.0,0.0); // Call setIntensity() method on the Motor object
    if (dist > 0 && dist < 1) {
      disttmp = 0;
      motor1.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 1.0);
      motor2.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 2.0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);

    } else if (dist > 1 && dist < 2) {
      disttmp = 0;
      analogWrite(motor1.pin, 0);
      motor2.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 2.0);
      motor3.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 3.0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);


    } else if (dist > 2 && dist < 3) {
      disttmp = 0;
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      motor3.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 3.0);
      motor4.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 4.0);


    } else if (dist > 3 && dist < 4) {
      disttmp = 0;
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      motor4.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 4.0);
      motor5.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 3.0);

    } else if (dist == 0) {
      analogWrite(motor1.pin, 255);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);
    }
    else if (dist == 1) {
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 255);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);
    }
    else if (dist == 2) {
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 255);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);
    }
    else if (dist == 3) {
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 255);
      analogWrite(motor5.pin, 0);
    }
    else if (dist == 4) {
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 255);
    }
    else {
      // Turn off all motors
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);
    }
  }
  // flush the serial buffer
  Serial.flush();

}
