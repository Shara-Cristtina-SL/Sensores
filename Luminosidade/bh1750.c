#include "bh1750.h"

void bh1750_init(i2c_inst_t *i2c) {
    uint8_t buf[1];
    buf[0] = BH1750_CONT_HI_RES_MODE;
    i2c_write_blocking(i2c, BH1750_ADDR, buf, 1, false);
}

float bh1750_read_lux(i2c_inst_t *i2c) {
    uint8_t buf[2];
    int ret = i2c_read_blocking(i2c, BH1750_ADDR, buf, 2, false);
    if (ret < 0) return -1.0f;

    uint16_t raw = (buf[0] << 8) | buf[1];
    return raw / 1.2f;  // Conversão conforme datasheet
}
