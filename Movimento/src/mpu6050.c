#include "mpu6050.h"
#include <math.h>

void mpu6050_init(i2c_inst_t *i2c) {
    uint8_t buf[] = {MPU6050_REG_PWR_MGMT_1, 0x00};  // Wake up sensor
    i2c_write_blocking(i2c, MPU6050_ADDR, buf, 2, false);
    sleep_ms(100);
}

static int16_t read_word(i2c_inst_t *i2c, uint8_t reg) {
    uint8_t buf[2];
    i2c_write_blocking(i2c, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c, MPU6050_ADDR, buf, 2, false);
    return (int16_t)((buf[0] << 8) | buf[1]);
}

bool mpu6050_read(i2c_inst_t *i2c, mpu6050_data_t *data) {
    data->acc_x = read_word(i2c, MPU6050_REG_ACCEL_XOUT_H);
    data->acc_y = read_word(i2c, MPU6050_REG_ACCEL_XOUT_H + 2);
    data->acc_z = read_word(i2c, MPU6050_REG_ACCEL_XOUT_H + 4);

    data->gyro_x = read_word(i2c, MPU6050_REG_ACCEL_XOUT_H + 8);
    data->gyro_y = read_word(i2c, MPU6050_REG_ACCEL_XOUT_H + 10);
    data->gyro_z = read_word(i2c, MPU6050_REG_ACCEL_XOUT_H + 12);

    // Cálculo de ângulos (simplificado, apenas com acelerômetro)
    float ax = data->acc_x / 16384.0f;
    float ay = data->acc_y / 16384.0f;
    float az = data->acc_z / 16384.0f;

    data->pitch = atan2f(ay, sqrtf(ax * ax + az * az)) * 180.0f / M_PI;
    data->roll  = atan2f(-ax, az) * 180.0f / M_PI;

    return true;
}
