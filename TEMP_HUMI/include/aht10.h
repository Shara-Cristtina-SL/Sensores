#ifndef AHT10_H
#define AHT10_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições para o AHT10
#define AHT10_ADDR           0x38  // Endereço I2C do AHT10
#define AHT10_CMD_INITIALIZE 0xE1  // Comando para inicialização/calibração
#define AHT10_CMD_MEASURE    0xAC  // Comando para iniciar a medição
#define AHT10_CMD_SOFT_RESET 0xBA  // Comando para resetar o sensor
#define AHT10_STATUS_BUSY_MASK 0x80 // Máscara para verificar se o sensor está ocupado
#define AHT10_STATUS_CAL_MASK  0x08 // Máscara para verificar se o sensor está calibrado

// Configuração I2C padrão
#define DEFAULT_I2C_PORT    i2c0
#define DEFAULT_I2C_SDA_PIN 0
#define DEFAULT_I2C_SCL_PIN 1
#define DEFAULT_I2C_BAUDRATE 100000

// Protótipos de Funções
void aht10_init(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin, uint baudrate);
void aht10_reset(i2c_inst_t *i2c_port);
bool aht10_read_data(i2c_inst_t *i2c_port, float *humidity, float *temperature);

#endif // AHT10_H