#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "bh1750.h"

// Canal I2C usado
#define I2C_PORT i2c0

// Pinos I2C do Pico
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1

// Endereço do sensor BH1750
#define BH1750_ADDR 0x23
#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10

int main() {
    stdio_init_all();  // Inicializa USB serial
    sleep_ms(2000);    // Tempo para a USB conectar

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN , GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN );

    bh1750_init(I2C_PORT);
    printf("BH1750 inicializado!\n");

    while (true) {
        float lux = bh1750_read_lux(I2C_PORT);
        if (lux >= 0) {
            printf("Luminosidade: %.2f lux\n", lux);
        } else {
            printf("Erro na leitura do sensor\n");
        }
        sleep_ms(1000);
    }
}