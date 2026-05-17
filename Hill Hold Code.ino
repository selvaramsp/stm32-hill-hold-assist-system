Code:

#include <Wire.h>
#include <Servo.h>
#include <math.h>

#define ADXL_ADDR 0x53
Servo brakeServo;

const int clutchPin = PA0;
const int accelPin  = PA1;
const int motor1    = PB10;
const int motor2    = PB11;
const int servoPin  = PA8;

int16_t ax, ay, az;
float slopeAngle = 0;
bool brakeApplied = false;

void ADXL_ReadXYZ() {
  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL_ADDR, 6, true);
  ax = (Wire.read() | (Wire.read() << 8));
  ay = (Wire.read() | (Wire.read() << 8));
  az = (Wire.read() | (Wire.read() << 8));
}

void Motor_Stop() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
}

void Motor_Forward() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
}

void setup() {
  Wire.begin();
  Wire.setClock(100000);

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  brakeServo.attach(servoPin);

  // ADXL345 init
  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(0x2D); Wire.write(0x00); Wire.endTransmission();
  delay(20);
  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(0x2D); Wire.write(0x08); Wire.endTransmission();
  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(0x31); Wire.write(0x08); Wire.endTransmission();

  Motor_Stop();
  brakeServo.write(0);
}

void loop() {
  int clutchVal = analogRead(clutchPin);
  int accelVal  = analogRead(accelPin);
  float clutchPct = (clutchVal / 4095.0) * 100.0;
  float accelPct  = (accelVal / 4095.0) * 100.0;

  ADXL_ReadXYZ();
  float xg = ax * 0.0039;
  float zg = az * 0.0039;
  slopeAngle = atan2f(xg, zg) * (180.0 / 3.1416);

  // --- Hill-hold logic ---
  if (fabs(slopeAngle) > 25.0) {
    if (clutchPct < 10 && accelPct < 10) {
      brakeServo.write(100);   // apply brake
      Motor_Stop();
      brakeApplied = true;
    } else if (brakeApplied && (clutchPct > 15 || accelPct > 15)) {
      brakeServo.write(0);     // release brake
      Motor_Forward();
      brakeApplied = false;
    }
  } else {
    // ---- FLAT road condition ----
    if (clutchPct < 10 && accelPct < 10) {
      Motor_Stop();            // stay idle on flat if no pedals
      brakeServo.write(0);     // ensure brake released
    } else {
      Motor_Forward();         // move if any pedal pressed
      brakeServo.write(0);
    }
    brakeApplied = false;
  }

  delay(150);
}