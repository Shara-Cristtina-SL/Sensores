#ifndef BH1750_H
#define BH1750_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Endereço padrão do BH1750
#define BH1750_ADDR 0x23

// Comando para modo contínuo de alta resolução
#define BH1750_CONT_HI_RES_MODE 0x10

void bh1750_init(i2c_inst_t *i2c);
float bh1750_read_lux(i2c_inst_t *i2c);

#endif
