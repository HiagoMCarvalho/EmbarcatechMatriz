#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// arquivo .pio
#include "pio_matrix.pio.h"

// número de LEDs
#define NUM_PIXELS 25
#define NUM_FRAMES 5
// Buzzer
#define buzzer 17

#define rows 4
#define cols 4

// pino de saída
#define OUT_PIN 7

// Declaração das matrizes de colunas
const uint colunas[cols] = {4, 3, 2, 1};
const uint linhas[rows] = {8, 7, 6, 5};

// vetor para criar imagem na matriz de led - 1
double desenho_teste[NUM_FRAMES][NUM_PIXELS] = {
    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 0.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 1.0},

};

// Vetor para criar imagem na matriz led - 2
double desenho2[NUM_FRAMES][NUM_PIXELS] = {
    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 0.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 0.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 1.0},

    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 0.0, 1.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 1.0},

    {1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 1.0},

    {1.0, 1.0, 1.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 0.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 1.0},

    {1.0, 1.0, 1.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 0.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

};

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{

    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {

        valor_led = matrix_rgb(desenho[24 - i], r = 0.0, g = 0.0); // Definine todos os leds para a cor azul
        pio_sm_put_blocking(pio, sm, valor_led);
    }

    imprimir_binario(valor_led);
}

void executar_tecla(char tecla, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    switch (tecla)
    {
    case '1':

        break;

    case '2':
        for (int i = 0; i < NUM_FRAMES; i++)
        {
            desenho_pio(desenho2[i], valor_led, pio, sm, r, g, b);
            sleep_ms(1000);
        }

        for (int i = 0; i < NUM_PIXELS; i++)
        {
            valor_led = (b = 0, r = 0, g = 0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }

        break;

    case '3':

        break;

    case '4':

        break;

    case '5':

        break;

    case '*':
        printf("HABILITANDO O MODO GRAVAÇÃO\n");
        sleep_ms(1000);
        reset_usb_boot(0, 0);
        break;

    default:

        break;
    }
}

// função principal
int main()
{
    PIO pio = pio0;
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 1, b = 0.0, g = 0.0;

    // coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (ok)
        printf("clock set to %ld\n", clock_get_hz(clk_sys));

    // configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Inicializar os botões do teclado
    init_pinos();

    while (true)
    {
        char tecla = escanear_teclado();

        executar_tecla(tecla, valor_led, pio, sm, r, g, b);
    }
}