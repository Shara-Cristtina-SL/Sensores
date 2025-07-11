#ifndef MPU6050_H
#define MPU6050_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Endereço padrão
#define MPU6050_ADDR 0x68

// Registradores
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B

typedef struct {
    int16_t acc_x, acc_y, acc_z;
    int16_t gyro_x, gyro_y, gyro_z;
    float pitch, roll;
} mpu6050_data_t;

void mpu6050_init(i2c_inst_t *i2c);
bool mpu6050_read(i2c_inst_t *i2c, mpu6050_data_t *data);

#endif
