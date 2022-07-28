//-------Updated by Maya on 28/7/2022----------

// Global variables
unsigned long activtime = 0;
int flag;
int maxIdx;

// Class for motor interface
class Motor {
  private:
    byte pin;
    unsigned long SOA;           // Stimuli Onset Asynchrony in milliseconds
    unsigned long DOS;           // Duration of Signals in milliseconds
    double activeTime; // Time taken for motor to go from minimum to maximum
    unsigned long totalTime;          // Total Time when the motor is active in milliseconds
    unsigned long PA, PS, PD, PE = 0; //   These are points for the vibration cycle
    unsigned long P0 = -1;
    double amplitudeStep = 1; // How much should the amplitude change in 1ms
    double currentIntensity = 0;
    double maxIntensity;

  public:
    static unsigned long currentTime;

    // Constructor 
    Motor(byte pin, unsigned long SOA, unsigned long DOS, double activeTime, double maxIntensity) {
      // Use 'this->' to make the difference between the
      // 'pin' attribute of the class and the
      // local variable 'pin' created from the parameter.
      this->pin = pin;
      this->SOA = SOA;
      this->DOS = DOS;
      this->activeTime = activeTime;
      this->totalTime = DOS + activeTime + activeTime;
      this-> maxIntensity = maxIntensity;
      this->amplitudeStep = maxIntensity / activeTime;
      
      init();
    }

    // Motor Setup
    void init() {
      //   init() is equivalent to setup() for the objects
      pinMode(pin, OUTPUT);
      off();
    }
    
    // Getters and Setters
    unsigned long getPS() {
      return PS;
    }
    unsigned long getPE() {
      return PE;
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

    // Turns off motor (intensity = 0)
    void off() {
      set(0);
      currentIntensity = 0;
    }

    // Vibrate without switching
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
    
    
    // The new and improved vibrate
    void vibrate_end_motor(unsigned long startTime, int& brush_mode) {
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
            if ((brush_mode==2 || brush_mode==1) && currentIntensity<=amplitudeStep && currentIntensity>=0){
              currentIntensity = 0;
              set(0);
              if (brush_mode==2){
                flag *=-1;
              }
              delay(1000);
              activtime = millis();
            }
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

// Initialize static global time tracker
unsigned long Motor::currentTime = 0;

/**************************************************
   Main Program Begins Here
 **************************************************/
 
// read Serial until until_c char found, returns true when found else false
// non-blocking, until_c is returned as last char in String, updates input String with chars read
bool readStringUntil(String& input, char until_c) {
  while (Serial.available()) {
    char c = Serial.read();
    //input += c;
    if (c == until_c) {
      return true;
    }
    else{
      input += c;
    }
  }
  
  return false;
}

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
int SOA = 110;
int DOS = 220;
int activeTime = 10;

Motor Motor1(motor_pin[0], SOA, DOS, activeTime, 255);
Motor Motor2(motor_pin[1], SOA, DOS, activeTime, 255);
Motor Motor3(motor_pin[2], SOA, DOS, activeTime, 255);
Motor Motor4(motor_pin[3], SOA, DOS, activeTime, 255);
Motor Motor5(motor_pin[4], SOA, DOS, activeTime, 255);
Motor Motor6(motor_pin[5], SOA, DOS, activeTime, 255);
Motor Motor7(motor_pin[6], SOA, DOS, activeTime, 255);
Motor Motor8(motor_pin[7], SOA, DOS, activeTime, 255);
Motor Motor9(motor_pin[8], SOA, DOS, activeTime, 255);
Motor Motor10(motor_pin[9], SOA, DOS, activeTime, 255);
Motor Motor11(motor_pin[10], SOA, DOS, activeTime, 255);
Motor Motor12(motor_pin[11], SOA, DOS, activeTime, 255);
Motor Motor13(motor_pin[12], SOA, DOS, activeTime, 255);
Motor Motor14(motor_pin[13], SOA, DOS, activeTime, 255);

// Uncomment for full array of motors
//Motor Motors[] = {Motor1, Motor2, Motor3, Motor4, Motor5 , Motor6, Motor7, Motor8, Motor9, Motor10, Motor11, Motor12, Motor13, Motor14};

// Current array of motors
Motor Motors[] = {Motor1, Motor2, Motor3, Motor4, Motor9, Motor10};

// Global variables
String input;
String current_mode;
int current_int_mode;
int brush_mode;

// Turn off all motors - sanity check for glitches
void flush_motors(){
  for (int i =0; i<=maxIdx; i++){
    Motors[i].off();
  }
}

// Function declaration - brushing functionality
void brush_motors(int& brush_mode);

// Setup and initialize Serial Communication
void setup() {
  Serial.begin(115200);

  // Initial conditions
  maxIdx = 5;
  flag = 1;
  brush_mode =0;
  current_int_mode = -1;
}


// Main Arduino Loop
void loop() {

  // If input is available, read until the newline delimiter
  if (readStringUntil(input, '\n')) { 

    // Print out as sanity check
      Serial.print(F(" got a line of input '")); 
      Serial.print(input); 
      Serial.println("'");

      // Current mode
      current_mode = input;

      // Convert to an integer
      current_int_mode = current_mode.toInt();

      // Flush all motors before changing mode
      flush_motors();

      // Adjust flags if brushing modes actiavted
      if (current_int_mode >maxIdx && current_int_mode <= maxIdx + 3){

          // Brush forwards: flag = 1
          if (current_int_mode== maxIdx+1){
            flag = 1;
            brush_mode = 1;
          }

          // Brush backwards: flag = -1
          else if (current_int_mode == maxIdx+2){
            flag = -1;
            brush_mode =1;
          }

          // Brush back and forth: flag switches internally
          else{
            brush_mode =2;
          }

          // Reset initial activation time
          activtime = millis();
      }

      // Clear input to get ready for next serial read
      input = ""; 
    }

    // Turn all the motors off
    if (current_int_mode == -1){
        flush_motors();
      }
      
   // Vibrate an individual motor
   if (current_int_mode>=0 && current_int_mode <=maxIdx){
    Motors[current_int_mode].vibrate(millis());
   }

   // Activate brushing motion
   if (current_int_mode >maxIdx && current_int_mode <= maxIdx + 3){
    brush_motors(brush_mode);
   }
}

// This brushes in either direction or back nad forth
//    Brush mode 1: brush in one direction
//    Brush mode 2: brush back nad forth
void brush_motors(int& brush_mode){

    // Vibrate forwards
    if (flag ==1){

      // Turn on first motor in sequence
        Motors[0].vibrate(activtime);

        // Subsequent motors turn on according to SOA of previous motor
        for (int i=1; i<maxIdx; i++){
          Motors[i].vibrate(Motors[i-flag].getPS());
        }

        // Turn on last motor and adjust flag based on mode
        Motors[maxIdx].vibrate_end_motor(Motors[maxIdx-flag].getPS(), brush_mode);
      }

     // Vibrate backwards
     if (flag ==-1){

      // Turn on first motor from the opposite side
        Motors[maxIdx].vibrate(activtime);

        // Go back in sequence
        for (int i=maxIdx-1; i>0; i--){
          Motors[i].vibrate(Motors[i-flag].getPS());
        }

        // Turn on last motor and adjust flag based on mode
        Motors[0].vibrate_end_motor(Motors[1].getPS(), brush_mode);
     }
}


// Brushing back and forth
// Brush one direction

// Vibrate in one direction
// add vibrate and update millis() function
//  Need to know which direction you're going
//  Need to update millis, delay but not change flag

// Otherwise for number between 0-13, vibrate current motor with timestamp = millis()
// Problem with delay?? Not getting input at right time at end of the brush?
// Close completely / turn off all function
