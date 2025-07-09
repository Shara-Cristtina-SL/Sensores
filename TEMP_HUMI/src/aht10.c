#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "aht10.h"

// --- Função: aht10_init ---
void aht10_init(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin, uint baudrate) {
    // Inicializa a interface I2C
    i2c_init(i2c_port, baudrate);

    // Configura os pinos GPIO para I2C
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);

    // Habilita pull-ups internos
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    printf("I2C e GPIOs configurados. Tentando resetar AHT10...\n");
    aht10_reset(i2c_port);

    // Envia o comando de inicialização/calibração
    uint8_t init_cmd[3] = {AHT10_CMD_INITIALIZE, 0x08, 0x00};
    int ret = i2c_write_blocking(i2c_port, AHT10_ADDR, init_cmd, 3, false);
    
    if (ret == PICO_ERROR_GENERIC) {
        printf("Erro ao escrever comando de inicializacao para AHT10.\n");
        return;
    }

    sleep_ms(300); // Espera o sensor inicializar e calibrar

    // Verifica o estado de calibração
    uint8_t status;
    i2c_read_blocking(i2c_port, AHT10_ADDR, &status, 1, false);
    if (!(status & AHT10_STATUS_CAL_MASK)) {
        printf("AHT10 NAO CALIBRADO! Tente reiniciar o sistema.\n");
    } else {
        printf("AHT10 inicializado e calibrado com sucesso.\n");
    }
}

// --- Função: aht10_reset ---
void aht10_reset(i2c_inst_t *i2c_port) {
    uint8_t reset_cmd = AHT10_CMD_SOFT_RESET;
    int ret = i2c_write_blocking(i2c_port, AHT10_ADDR, &reset_cmd, 1, false);
    if (ret == PICO_ERROR_GENERIC) {
        printf("Erro ao enviar comando de reset para AHT10.\n");
    }
    sleep_ms(20); // O datasheet recomenda esperar 20ms após o reset
}

// --- Função: aht10_read_data ---
bool aht10_read_data(i2c_inst_t *i2c_port, float *humidity, float *temperature) {
    // 1. Envia o comando de medição
    uint8_t measure_cmd[3] = {AHT10_CMD_MEASURE, 0x33, 0x00};
    int ret = i2c_write_blocking(i2c_port, AHT10_ADDR, measure_cmd, 3, false);
    if (ret == PICO_ERROR_GENERIC) {
        printf("Erro ao enviar comando de medicao para AHT10.\n");
        return false;
    }

    // Espera a conclusão da medição 
    sleep_ms(80);

    // Lê o byte de status
    uint8_t status_byte;
    i2c_read_blocking(i2c_port, AHT10_ADDR, &status_byte, 1, false);

    if (status_byte & AHT10_STATUS_BUSY_MASK) {
        printf("AHT10 Ocupado, nao foi possivel ler os dados.\n");
        return false;
    }

    // Lê os 6 bytes de dados
    uint8_t data[6];
    ret = i2c_read_blocking(i2c_port, AHT10_ADDR, data, 6, false);
    if (ret == PICO_ERROR_GENERIC) {
        printf("Erro ao ler dados do AHT10.\n");
        return false;
    }

    // Processa os dados
    uint32_t raw_humidity = ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | data[3];
    raw_humidity = raw_humidity >> 4;

    uint32_t raw_temperature = ((uint32_t)data[3] & 0x0F) << 16 | ((uint32_t)data[4] << 8) | data[5];

    // Calcula os valores finais
    *humidity = (float)raw_humidity * 100.0f / 1048576.0f;
    *temperature = (float)raw_temperature * 200.0f / 1048576.0f - 50.0f;

    return true;
}