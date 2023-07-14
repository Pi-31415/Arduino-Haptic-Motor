#include <math.h>

class Motor {
  public:
    float x, y, z;
    int pin;
    float multiplier;

    Motor(float x, float y, float z, float multiplier, int pin) {
      this->x = x;
      this->y = y;
      this->z = z;
      this->pin = pin;
      this->multiplier = multiplier;
    }

    float calcDistance(float posx, float posy, float posz) {
      float distance = sqrt(pow(posx - x, 2) + pow(posy - y, 2) + pow(posz - z, 2));
      return distance;
    }

    void setIntensity(float posx, float posy, float posz, float nextmotorX, float nextmotorY, float nextmotorZ) {
      float distance = calcDistance(posx, posy, posz);
      float d = calcDistance(nextmotorX, nextmotorY, nextmotorZ);
      int intensity = (255 / d) * (d - distance) * (this->multiplier);
      analogWrite(pin, intensity);
      delay(10);
    }
};

Motor motor1(0.0, 0.0, 0.0, 1.0, 3);
Motor motor2(0.0, 0.0, 1.0, 1.0, 5);
Motor motor3(0.0, 0.0, 2.0, 1.0, 6);
Motor motor4(0.0, 0.0, 3.0, 1.0, 9);
Motor motor5(0.0, 0.0, 4.0, 1.0, 10);
Motor motor6(0.0, 0.0, 5.0, 1.0, 11); // The new Motor object

void setup() {
  Serial.begin(230400);
}

void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    float dist = inputString.toFloat(); 
    float disttmp = 0.0;

    if (dist > 0 && dist < 1) {
      disttmp = 0;
      motor1.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 1.0);
      motor2.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 2.0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);
      analogWrite(motor6.pin, 0); // added for motor6
    } 
    else if (dist > 1 && dist < 2) {
      disttmp = 0;
      analogWrite(motor1.pin, 0);
      motor2.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 2.0);
      motor3.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 3.0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);
      analogWrite(motor6.pin, 0); // added for motor6
    } 
    else if (dist > 2 && dist < 3) {
      disttmp = 0;
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      motor3.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 3.0);
      motor4.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 4.0);
      analogWrite(motor5.pin, 0);
      analogWrite(motor6.pin, 0); // added for motor6
    }
    else if (dist > 3 && dist < 4) {
      disttmp = 0;
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      motor4.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 4.0);
      motor5.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 5.0); // Change to 5.0 from 3.0
      analogWrite(motor6.pin, 0); // added for motor6
    }
    else if (dist > 4 && dist < 5) { // added a new condition for motor6
      disttmp = 0;
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      motor5.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 5.0);
      motor6.setIntensity(0.0, 0.0, dist - disttmp, 0.0, 0.0, 6.0); // Added for motor6
    } 
    else if (dist == 0 || dist == 1 || dist == 2 || dist == 3 || dist == 4 || dist == 5) {
      analogWrite(motor1.pin, (dist == 0)*255);
      analogWrite(motor2.pin, (dist == 1)*255);
      analogWrite(motor3.pin, (dist == 2)*255);
      analogWrite(motor4.pin, (dist == 3)*255);
      analogWrite(motor5.pin, (dist == 4)*255);
      analogWrite(motor6.pin, (dist == 5)*255); // added for motor6
    } 
    else {
      analogWrite(motor1.pin, 0);
      analogWrite(motor2.pin, 0);
      analogWrite(motor3.pin, 0);
      analogWrite(motor4.pin, 0);
      analogWrite(motor5.pin, 0);
      analogWrite(motor6.pin, 0); // added for motor6
    }
  }
  Serial.flush();
}
