// Apologies for my spaghetti code
// The bug to fix is to refresh the P0 member in each instance of the Motor clalss
// It is the starting time for the motors
// We need to update it once the motor has done vibrating.

// Ask Pi for the diagram of PA, PS, PD, PE

// Declare Motor as Class
class Motor {
  private:
    byte pin;
    unsigned long SOA;           // Stimuli Onset Asynchrony in milliseconds
    unsigned long DOS;           // Duration of Signals in milliseconds
    double activeTime; // Time taken for motor to go from minimum to maximum
    // voltage in milliseconds
    unsigned long totalTime;          // Total Time when the motor is active in milliseconds
    unsigned long PA, PS, PD, PE = 0; //   These are points for the vibration cycle
    unsigned long P0 = -1;
    double amplitudeStep = 1; // How much should the amplitude change in 1ms
    double currentIntensity = 0;

  public:
    static unsigned long currentTime;
    Motor(byte pin, unsigned long SOA, unsigned long DOS, double activeTime) {
      // Use 'this->' to make the difference between the
      // 'pin' attribute of the class and the
      // local variable 'pin' created from the parameter.
      this->pin = pin;
      this->SOA = SOA;
      this->DOS = DOS;
      this->activeTime = activeTime;
      this->totalTime = DOS + activeTime + activeTime;
      this->amplitudeStep = 255.0 / activeTime;
      init();
    }

    void init() {
      //   init() is equivalent to setup() for the objects
      pinMode(pin, OUTPUT);
      off();
    }
    // Getters and Setters
    unsigned long getPS() {
      return PS;
    }
    unsigned long getCurrentIntensity() {
      unsigned long intIntensity = currentIntensity;
      return intIntensity;
    }

    // This function sets the motor at a particular intensity between 0 and 255
    void set(double intensity) {
      unsigned long intIntensity = intensity;
      analogWrite(pin, intIntensity);
    }
    void off() {
      set(0);
      currentIntensity = 0;
    }
    void vibrate(unsigned long startTime) {
      if (millis() >= startTime) {
        // Set Up
        P0 = startTime;
        PA = P0 + activeTime;
        PS = PA + SOA;
        PD = PA + DOS;
        PE = P0 + totalTime;
        // Prunsigned long Initial Info
        currentTime = millis();
        if (currentTime == startTime) {
          // Uncomment this to debug
          //printInfo();
        }

        if (currentIntensity != 0) {
          // Uncomment this for debugging individual motors in Serial Plotter
          //   Serial.print(currentIntensity);
          //   Serial.print('\n');
        }
        //   Main Vibration Function
        // Increase
        if (currentTime >= P0 && currentTime < PA) {
          currentIntensity += amplitudeStep;
          if (currentIntensity >= 255) {
            currentIntensity = 255;
          }
          set(currentIntensity);
        }

        // Hold Constant

        else if (currentTime >= PA && currentTime < PD) {
          currentIntensity = 255;
          set(currentIntensity);
        }

        // Decrease
        else if (currentTime >= PD && currentTime < PE) {
          currentIntensity -= amplitudeStep;
          if (currentIntensity <= 0) {
            currentIntensity = 0;
          }
          set(currentIntensity);
        }

        else {
          off();
        }
      }
    }
    // Following is for debugging
    void printInfo() {
      Serial.print("-----------------");
      Serial.print('\n');
      Serial.print("PIN : ");
      Serial.print(pin);
      Serial.print('\n');
      Serial.print("SOA : ");
      Serial.print(SOA);
      Serial.print('\n');
      Serial.print("DOS : ");
      Serial.print(DOS);
      Serial.print('\n');
      Serial.print("Active Time : ");
      Serial.print(activeTime);
      Serial.print('\n');
      Serial.print("Total Time : ");
      Serial.print(totalTime);
      Serial.print('\n');
      Serial.print("PA : ");
      Serial.print(PA);
      Serial.print('\n');
      Serial.print("PS : ");
      Serial.print(PS);
      Serial.print('\n');
      Serial.print("PD : ");
      Serial.print(PD);
      Serial.print('\n');
      Serial.print("PE : ");
      Serial.print(PE);
      Serial.print('\n');
      Serial.print("P0 : ");
      Serial.print(P0);
      Serial.print('\n');
      Serial.print("Step : ");
      Serial.print(amplitudeStep);
      Serial.print('\n');
    }
};

/**************************************************
   Main Program Begins Here
 **************************************************/

// Initialize static global time tracker
unsigned long Motor::currentTime = 0;

// Pin Variables for vibration motors.
// This program is fixed for 4 motors
const unsigned long PIN_1 = 31;
const unsigned long PIN_2 = 2;
const unsigned long PIN_3 = 3;
const unsigned long PIN_4 = 4;
const unsigned long PIN_5 = 5;
const unsigned long PIN_6 = 6;
const unsigned long PIN_7 = 7;
const unsigned long PIN_8 = 8;
const unsigned long PIN_9 = 9;
const unsigned long PIN_10 = 10;
const unsigned long PIN_11 = 11;
const unsigned long PIN_12 = 12;
const unsigned long PIN_13 = 13;
const unsigned long PIN_14 = 33;

unsigned long motor_pin[] = {PIN_1, PIN_2, PIN_3, PIN_4, PIN_5, PIN_6, PIN_7, PIN_8, PIN_9, PIN_10, PIN_11, PIN_12, PIN_13, PIN_14};
const unsigned long number_of_modules = sizeof(motor_pin) / sizeof(int);

// Parameters are Motor(byte pin, unsigned long SOA, unsigned long DOS, unsigned long activeTime)
int SOA = 410;
int DOS = 120;
int activeTime = 10;
Motor Motor1(motor_pin[0], SOA, DOS, activeTime);
Motor Motor2(motor_pin[1], SOA, DOS, activeTime);
Motor Motor3(motor_pin[2], SOA, DOS, activeTime);
Motor Motor4(motor_pin[3], SOA, DOS, activeTime);
Motor Motor5(motor_pin[4], SOA, DOS, activeTime);
Motor Motor6(motor_pin[5], SOA, DOS, activeTime);
Motor Motor7(motor_pin[6], SOA, DOS, activeTime);
Motor Motor8(motor_pin[7], SOA, DOS, activeTime);
Motor Motor9(motor_pin[8], SOA, DOS, activeTime);
Motor Motor10(motor_pin[9], SOA, DOS, activeTime);
Motor Motor11(motor_pin[10], SOA, DOS, activeTime);
Motor Motor12(motor_pin[11], SOA, DOS, activeTime);
Motor Motor13(motor_pin[12], SOA, DOS, activeTime);
Motor Motor14(motor_pin[13], SOA, DOS, activeTime);

// Following is the array of motors for easier access
Motor Motors[] = {Motor1, Motor2, Motor3, Motor4, Motor5 , Motor6, Motor7, Motor8, Motor9, Motor10, Motor11, Motor12, Motor13, Motor14};

// Initialize Serial Communication
void setup() {
  Serial.begin(115200);
}

unsigned long activtime = 0;
// Main Arduino Loop
void loop() {
  //  Loop every 10 seconds
  if (millis() % 10000 == 0) {
    activtime = millis();
  }


//
//  Motors[5].vibrate(activtime);
//  Motors[3].vibrate(Motors[5].getPS());
//  Motors[2].vibrate(Motors[3].getPS());
//  Motors[1].vibrate(Motors[2].getPS());
//  Motors[0].vibrate(Motors[1].getPS());

//
//  Motors[5].vibrate(activtime);
//  Motors[4].vibrate(Motors[5].getPS());
//  Motors[3].vibrate(Motors[4].getPS());
//  Motors[2].vibrate(Motors[3].getPS());
//  Motors[1].vibrate(Motors[2].getPS());
//  Motors[0].vibrate(Motors[1].getPS());
//

  //  Motors[0].vibrate(Motors[1].getPS());

//    Motors[0].vibrate(activtime);
//    Motors[1].vibrate(Motors[0].getPS());
//    Motors[2].vibrate(Motors[1].getPS());
//    Motors[1].vibrate(Motors[2].getPS());
//    Motors[0].vibrate(Motors[1].getPS());


  //
  //  Motors[13].vibrate(activtime);
  //  Motors[2].vibrate(Motors[3].getPS());
  //  Motors[1].vibrate(Motors[2].getPS());
  //  Motors[0].vibrate(Motors[1].getPS());
  //
//
//      Motors[0].vibrate(activtime);
//      Motors[1].vibrate(Motors[0].getPS());
//      Motors[2].vibrate(Motors[1].getPS());
//      Motors[3].vibrate(Motors[2].getPS());
//      Motors[4].vibrate(Motors[3].getPS());
//      Motors[5].vibrate(Motors[4].getPS());
//      Motors[6].vibrate(Motors[5].getPS());
//      Motors[7].vibrate(Motors[6].getPS());
//      Motors[8].vibrate(Motors[7].getPS());
//      Motors[9].vibrate(Motors[8].getPS());
//      Motors[10].vibrate(Motors[9].getPS());
//      Motors[11].vibrate(Motors[10].getPS());
//      Motors[12].vibrate(Motors[11].getPS());
//      Motors[13].vibrate(Motors[12].getPS());
}
