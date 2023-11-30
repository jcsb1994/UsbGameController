#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
// #include <Arduino.h>
#include <Wire.h>

class mpu6050
{
public:
    mpu6050(const uint8_t i2c_addr = 0x68) : _i2c_addr(i2c_addr) {}
    ~mpu6050() {}

    typedef enum {
        MPU_SENSI_2G,
        MPU_SENSI_8G = 0x10
    } mpu_sensitivity;

    struct imuReadings {
        float acceleration[3];
        float xAccAngle;
        float yAccAngle;
        float gyro[3];
        float xGyroAngle;
        float yGyroAngle;
        float yaw;
        float roll;
        float pitch;
    } ;

    void init(mpu_sensitivity sensi = MPU_SENSI_2G);
    
    imuReadings getValues();
private:
    const uint8_t _i2c_addr;
    uint32_t _lastReadTimestamp = 0;

    inline float _getAccVal() { return (Wire.read() << 8 | Wire.read()) >> 14; }
    inline float _getGyroVal() { return (Wire.read() << 8 | Wire.read()) / 131; }
    void _requestReadI2c(uint8_t reg, uint8_t size);
    void _writeI2cReg(uint8_t reg, uint8_t data);
    uint32_t _getElapsedMs();
};

#endif