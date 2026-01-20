#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define TRIG_PIN 2
#define ECHO_PIN 3

#define LED_VERDE     10
#define LED_AMARILLO  11
#define LED_ROJO      12

typedef enum {
    STATE_LEJOS,
    STATE_MEDIO,
    STATE_CERCA
} state_t;

float medir_distancia() {
    gpio_put(TRIG_PIN, 0);
    sleep_us(2);
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    while (!gpio_get(ECHO_PIN));
    absolute_time_t start = get_absolute_time();

    while (gpio_get(ECHO_PIN));
    absolute_time_t end = get_absolute_time();

    int64_t tiempo = absolute_time_diff_us(start, end);
    return tiempo / 58.0f; // cm
}

void apagar_leds() {
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_AMARILLO, 0);
    gpio_put(LED_ROJO, 0);
}

int main() {
    stdio_init_all();

    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    gpio_init(LED_VERDE);
    gpio_init(LED_AMARILLO);
    gpio_init(LED_ROJO);

    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_AMARILLO, GPIO_OUT);
    gpio_set_dir(LED_ROJO, GPIO_OUT);

    state_t estado = STATE_LEJOS;

    while (1) {
        float distancia = medir_distancia();

        /* Transición de estado */
        if (distancia < 10.0)
            estado = STATE_CERCA;
        else if (distancia < 30.0)
            estado = STATE_MEDIO;
        else
            estado = STATE_LEJOS;

        /* Máquina de estados */
        apagar_leds();

        switch (estado) {
            case STATE_LEJOS:
                gpio_put(LED_VERDE, 1);
                break;

            case STATE_MEDIO:
                gpio_put(LED_AMARILLO, 1);
                break;

            case STATE_CERCA:
                gpio_put(LED_ROJO, 1);
                break;
        }

        sleep_ms(200);
    }
}
