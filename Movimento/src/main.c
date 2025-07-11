#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050.h"

#define I2C_PORT i2c0
#define SDA_PIN 0
#define SCL_PIN 1

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    mpu6050_init(I2C_PORT);

    mpu6050_data_t data;

    while (true) {
        if (mpu6050_read(I2C_PORT, &data)) {
            printf("Pitch: %.2f°, Roll: %.2f°\n", data.pitch, data.roll);
            printf("Acc X: %d Y: %d Z: %d | Gyro X: %d Y: %d Z: %d\n\n",
                   data.acc_x, data.acc_y, data.acc_z,
                   data.gyro_x, data.gyro_y, data.gyro_z);
        } else {
            printf("Erro na leitura do MPU6050\n");
        }
        sleep_ms(500);
    }

    return 0;
}
