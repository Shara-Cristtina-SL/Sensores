#include <stdio.h>
#include "pico/stdlib.h"
#include "aht10.h"

int main() {
    stdio_init_all(); // Inicializa comunicação serial (UART) para printf

    printf("Iniciando AHT10 no Pico...\n");

    // Inicializa o sensor AHT10 com configurações padrão
    aht10_init(DEFAULT_I2C_PORT, DEFAULT_I2C_SDA_PIN, DEFAULT_I2C_SCL_PIN, DEFAULT_I2C_BAUDRATE);

    float humidity, temperature;

    while (true) {
        if (aht10_read_data(DEFAULT_I2C_PORT, &humidity, &temperature)) {
            printf("Umidade: %.2f %%RH, Temperatura: %.2f C\n", humidity, temperature);
        } else {
            printf("Falha na leitura do AHT10. Tentando resetar...\n");
            aht10_reset(DEFAULT_I2C_PORT);
            sleep_ms(500);
        }
        sleep_ms(2000); 
    }

    return 0;
}