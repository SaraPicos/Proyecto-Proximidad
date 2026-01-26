/* ============================
 *  LIBRERÍAS
 * ============================ */

/* Librería estándar de entrada/salida  */
#include <stdio.h>

/* Librería base del SDK de Raspberry Pi Pico */
#include "pico/stdlib.h"

/* Control de pines GPIO */
#include "hardware/gpio.h"

/* Manejo de temporización y tiempos absolutos */
#include "hardware/timer.h"


/* ============================
 * Definición de pines
 * ============================ */

/* Pin TRIG del sensor */
#define TRIG_PIN 2

/* Pin ECHO del sensor */
#define ECHO_PIN 3

/* Pines de los LEDs */
#define LED_VERDE     10
#define LED_AMARILLO  11
#define LED_ROJO      12


/* ============================
 * Definición de estados
 * ============================ */

/*
 * Enumeración que representa los estados del sistema
 * según la distancia medida.
 */
 
typedef enum {
    STATE_LEJOS,   /* Coche lejos */
    STATE_MEDIO,   /* Coche a distancia media */
    STATE_CERCA    /* Coche cerca */
} state_t;


/* ============================
 * Función: medir_distancia
 * ============================ */

/*
 * Envía un pulso al sensor y mide el tiempo de retorno.
 *
 * Devuelve: Distancia en centímetros (float).
 */
 
float medir_distancia() {

    /* Asegura que TRIG esté en bajo antes del pulso */
    gpio_put(TRIG_PIN, 0);
    sleep_us(2);

    /* Genera un pulso de 10 us en TRIG */
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    /* Espera hasta que el pin ECHO pase a nivel alto */
    while (!gpio_get(ECHO_PIN));

    /* Registra el tiempo de inicio del pulso ECHO */
    absolute_time_t start = get_absolute_time();

    /* Espera hasta que el pin ECHO vuelva a nivel bajo */
    while (gpio_get(ECHO_PIN));

    /* Registra el tiempo de fin del pulso ECHO */
    absolute_time_t end = get_absolute_time();

    /* Calcula la diferencia de tiempo en microsegundos */
    int64_t tiempo = absolute_time_diff_us(start, end);

    /*
     * Conversión de tiempo a distancia:
     * - El sonido viaja ida y vuelta
     * - 58 us ≈ 1 cm
     */
    return tiempo / 58.0f;
}


/* ============================
 * Función: apagar_leds
 * ============================ */

/*
 * Apaga todos los LEDs del sistema.
 * Se utiliza antes de encender el LED correspondiente
 * al estado actual.
 */
void apagar_leds() {
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_AMARILLO, 0);
    gpio_put(LED_ROJO, 0);
}


/* ============================
 * Función principal
 * ============================ */

int main() {

    /* Inicializa USB stdio (necesario para printf) */
    stdio_init_all();

    /* Configuración del pin TRIG como salida */
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);

    /* Configuración del pin ECHO como entrada */
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    /* Inicialización de los pines de los LEDs */
    gpio_init(LED_VERDE);
    gpio_init(LED_AMARILLO);
    gpio_init(LED_ROJO);

    /* Configuración de los LEDs como salidas */
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_set_dir(LED_AMARILLO, GPIO_OUT);
    gpio_set_dir(LED_ROJO, GPIO_OUT);

    /* Estado inicial del sistema */
    state_t estado = STATE_LEJOS;

    /* Bucle */
    while (1) {

        /* Mide la distancia usando el sensor */
        float distancia = medir_distancia();

        /* Muestra la distancia por consola */
        printf("Distancia: %.1f cm\n", distancia);

        /* ============================
         * Lógica de transición de estados
         * ============================ */

        if (distancia < 10.0f) {
            estado = STATE_CERCA;
        }
        else if (distancia < 30.0f) {
            estado = STATE_MEDIO;
        }
        else {
            estado = STATE_LEJOS;
        }

        /* ============================
         * Máquina de estados (salidas)
         * ============================ */

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

            default:
                apagar_leds();
                break;
        }

        /* Retardo para evitar lecturas excesivas */
        sleep_ms(200);
    }
}


