#include <mpu6050.h>
#include <Arduino.h>

void mpu6050::init(mpu_sensitivity sensi)
{
  Wire.begin();

  _writeI2cReg(0x6B, 0); // reset chip
  _writeI2cReg(0x1C, (uint8_t)sensi); // ACCEL_CONFIG
  _writeI2cReg(0x1B, 0x10); //GYRO_CONFIG reg (1000deg/s full scale)
}

mpu6050::imuReadings mpu6050::getValues()
{
  // return if no data ready
  imuReadings rc;

  Wire.beginTransmission(_i2c_addr);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(_i2c_addr, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  for (uint8_t i = 0; i < 3; i++)
  {
    rc.acceleration[i] = _getAccVal();
  }

  // Calculating Roll and Pitch from the accelerometer data
  rc.xAccAngle = (atan(rc.acceleration[1] / sqrt(pow(rc.acceleration[0], 2)
   + pow(rc.acceleration[2], 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  rc.yAccAngle = (atan(-1 * rc.acceleration[0] / sqrt(pow(rc.acceleration[1], 2)
   + pow(rc.acceleration[2], 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)

  // === Read gyroscope data === //
  float elapsedTime = _getElapsedMs() / 1000;
  _lastReadTimestamp = millis();

  _requestReadI2c(0x43, 6); // accel data

  for (uint8_t i = 0; i < 3; i++)
  {
    rc.gyro[i] = _getGyroVal();
  }

  // Correct the outputs with the calculated error values
  rc.gyro[0] += 0.56; // GyroErrorX ~(-0.56)
  rc.gyro[1] -= 2; // GyroErrorY ~(2)
  rc.gyro[2] += 0.79; // GyroErrorZ ~ (-0.8)

  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  float gyroAngleX = gyroAngleX + rc.gyro[0] * elapsedTime; // deg/s * s = deg
  float gyroAngleY = gyroAngleY + rc.gyro[1] * elapsedTime;
  float yaw =  yaw + rc.gyro[2] * elapsedTime;

  // Complementary filter - combine acceleromter and gyro angle values
  float roll = 0.96 * gyroAngleX + 0.04 * rc.xAccAngle;
  float pitch = 0.96 * gyroAngleY + 0.04 * rc.yAccAngle;

  // Print the values on the serial monitor
  Serial.print(roll);
  Serial.print("/");
  Serial.print(pitch);
  Serial.print("/");
  Serial.println(yaw);

  return rc;
}

void mpu6050::_requestReadI2c(uint8_t reg, uint8_t size)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(reg); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(_i2c_addr, size, true);
}

void mpu6050::_writeI2cReg(uint8_t reg, uint8_t data)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}

uint32_t mpu6050::_getElapsedMs()
{
    uint32_t now = millis();
    if (now < _lastReadTimestamp) {
        return now + (UINT32_MAX - _lastReadTimestamp);
    } else {
        return now - _lastReadTimestamp;
    }
}
