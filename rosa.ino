#define STEER_PIN 34    
#define THROTTLE_PIN 35 

// Your updated motor pins
#define RPWM 22 
#define RLPWM 23
#define LPWM 19 
#define LLPWM 21 

// Cache variables to smooth over missed pulses during diagonal moves
long lastThrottle = 1500; 
long lastSteer = 1500;
int errorCounter = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("--- Rosa System Online ---");

  pinMode(STEER_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);

  // NEW API: ledcAttach(pin, frequency, resolution)
  ledcAttach(LPWM, 1000, 8); 
  ledcAttach(LLPWM, 1000, 8); 
  ledcAttach(RPWM, 1000, 8); 
  ledcAttach(RLPWM, 1000, 8); 

  stopMotors();
}

void loop() {
  // Reduced timeout so the loop doesn't hang
  long rawSteer = pulseIn(STEER_PIN, HIGH, 21000);
  long rawThrottle = pulseIn(THROTTLE_PIN, HIGH, 21000);

  bool signalValid = true;

  // 1. Update cache ONLY if we get a good reading
  if (rawThrottle > 800 && rawThrottle < 2200) {
    lastThrottle = rawThrottle;
  } else {
    signalValid = false;
  }

  if (rawSteer > 800 && rawSteer < 2200) {
    lastSteer = rawSteer;
  } else {
    signalValid = false;
  }

  // 2. Failsafe: Only stop if we miss 10 pulses in a row
  if (!signalValid) {
    errorCounter++;
    if (errorCounter > 10) {
      Serial.println("SIGNAL LOST: Check Receiver Wiring!");
      stopMotors();
      return; 
    }
  } else {
    errorCounter = 0; 
  }

  // 3. Map values using the cached signals
  int throttle = map(lastThrottle, 1000, 2000, -255, 255);
  int steering = map(lastSteer, 1000, 2000, -255, 255);

  // Deadzone
  if (abs(throttle) < 25) throttle = 0;
  if (abs(steering) < 25) steering = 0;

  // 4. Mixing Logic
  int leftSpeed = throttle + steering;
  int rightSpeed = throttle - steering;

  // 5. Proportional Scaling (The magic for diagonal movement)
  // Instead of just constraining and losing steering at max throttle, 
  // this scales both wheels down proportionally so the turn still happens.
  int maxVal = max(abs(leftSpeed), abs(rightSpeed));
  if (maxVal > 255) {
    leftSpeed = (leftSpeed * 255) / maxVal;
    rightSpeed = (rightSpeed * 255) / maxVal;
  }

  Serial.print("IN: T:"); Serial.print(lastThrottle); 
  Serial.print(" S:"); Serial.print(lastSteer);
  Serial.print(" | OUT: L:"); Serial.print(leftSpeed);
  Serial.print(" R:"); Serial.println(rightSpeed);

  motorDrive(LPWM, LLPWM, leftSpeed);
  motorDrive(RPWM, RLPWM, rightSpeed);
  
  delay(5); 
}

void motorDrive(uint8_t fwd_pin, uint8_t rev_pin, int speed) {
  // Safety: Always write the opposite pin LOW before writing the active pin HIGH
  if (speed > 0) {
    ledcWrite(rev_pin, 0);
    ledcWrite(fwd_pin, speed);
  } else if (speed < 0) {
    ledcWrite(fwd_pin, 0);
    ledcWrite(rev_pin, abs(speed));
  } else {
    ledcWrite(fwd_pin, 0);
    ledcWrite(rev_pin, 0);
  }
}

void stopMotors() {
  ledcWrite(LPWM, 0); ledcWrite(LLPWM, 0);
  ledcWrite(RPWM, 0); ledcWrite(RLPWM, 0);
}