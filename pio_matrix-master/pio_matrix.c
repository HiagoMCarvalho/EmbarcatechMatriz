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
#define FRAMED0 27
#define NUM_FRAMES 5
#define FRAMED2 11
// Buzzer
#define buzzer 17

// Linhas e colunas do teclado matricial
#define rows 4
#define cols 4

// Declaração das matrizes do teclado matricial
const uint colunas[cols] = {21, 20, 19, 18};
const uint linhas[rows] = {28, 27, 26, 22};

const char teclas[rows][cols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

void init_pinos()
{
    // Inicializar as teclas como saída
    for (int l = 0; l < rows; l++)
    {
        gpio_init(linhas[l]);
        gpio_set_dir(linhas[l], GPIO_OUT);
        gpio_put(linhas[l], true);
    }

    // Inicializar as teclas como entrada
    for (int c = 0; c < cols; c++)
    {
        gpio_init(colunas[c]);
        gpio_set_dir(colunas[c], GPIO_IN);
        gpio_pull_up(colunas[c]);
    }
}

char escanear_teclado()
{
    for (int row = 0; row < rows; row++)
    {
        gpio_put(linhas[row], false);

        for (int col = 0; col < cols; col++)
        {
            if (!gpio_get(colunas[col]))
            {
                sleep_ms(20); // Debouncing
                if (!gpio_get(colunas[col]))
                {
                    gpio_put(linhas[row], true);
                    return teclas[row][col];
                }
            }
        }
        gpio_put(linhas[row], true); // Desativar linha atual
    }
    return 0; // Nenhuma tecla pressionada
}

// pino de saída
#define OUT_PIN 7

double desenho0[FRAMED0][NUM_PIXELS] =
{ 
                        {0.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0},
                        
                        {0.0, 0.0, 0.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 1.0, 
                        0.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 1.0},

                        {0.0, 0.0, 1.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0},

                       {0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0},

                        {1.0, 1.0, 1.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 1.0},

                        {1.0, 1.0, 0.0, 1.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 1.0, 1.0},

                        {1.0, 0.0, 1.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 1.0, 1.0},

                        {0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0},

                        {1.0, 1.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 1.0, 
                        0.0, 1.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 0.0, 1.0,
                        1.0, 1.0, 1.0, 0.0, 0.0},

                        {1.0, 1.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 1.0, 0.0, 
                        1.0, 0.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 1.0, 0.0,
                        1.0, 1.0, 0.0, 0.0, 1.0},

                        {1.0, 0.0, 0.0, 1.0, 1.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 1.0, 1.0},

                        {0.0, 0.0, 1.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 1.0, 0.0},

                        {0.0, 1.0, 1.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 1.0, 0.0, 0.0},

                        {1.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 1.0, 0.0, 0.0, 0.0},

                        {1.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0},

                        {0.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 1.0},

                        {0.0, 0.0, 0.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        1.0, 1.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 1.0},

                        {0.0, 0.0, 1.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 1.0, 
                        1.0, 0.0, 0.0, 1.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 1.0, 1.0, 1.0},

                        {0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0},

                        {1.0, 1.0, 1.0, 0.0, 1.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        0.0, 1.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        1.0, 1.0, 1.0, 0.0, 0.0},

                        {1.0, 1.0, 0.0, 1.0, 1.0,
                        0.0, 1.0, 0.0, 0.0, 0.0, 
                        1.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        1.0, 1.0, 0.0, 0.0, 0.0},

                        {1.0, 0.0, 1.0, 1.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0},

                        {0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0, 0.0},

                        {1.0, 1.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0},

                        {1.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0, 0.0},

                        {1.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0, 
                        1.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 0.0},

                        {0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0},
                        
                    };
double desenho1[NUM_FRAMES][NUM_PIXELS] =
{
    {1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1},

    {1, 0, 0, 0, 0,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     0, 0, 0, 0, 1},

    {1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     1, 0, 0, 0, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1},

    {1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1},

    {1, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 0,
     0, 0, 0, 0, 1},
};


double desenho2[FRAMED2][NUM_PIXELS] =
    {
        {1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 1.0, 1.0, 1.0, 0.0},

        {1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 1.0},

        {1.0, 1.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 1.0},

        {1.0, 1.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 1.0, 1.0, 1.0, 0.0},

        {1.0, 1.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 0.0, 0.0},

        {1.0, 1.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 1.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 0.0, 0.0},

        {1.0, 1.0, 1.0, 0.0, 1.0,
         0.0, 1.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 0.0, 0.0},

        {1.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 0.0, 0.0},

        {1.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 1.0, 1.0, 1.0, 0.0}};

double desenho4[NUM_FRAMES][NUM_PIXELS] =
{ 
    {0, 0, 0, 0, 0,
     0, 1, 0, 1, 0,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 0},
                        
    {0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1},

    {1, 1, 1, 1, 1,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1},

    {0, 0, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0,
    0, 0, 0, 1, 0,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 0}

};


double desenho3[NUM_FRAMES][NUM_PIXELS] =
{ 
    {1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     1, 0, 0, 0, 0},
                        
    {1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     1, 0, 0, 0, 0,
     1, 1, 1, 0, 0},

    {1, 0, 1, 0, 0,
     1, 0, 1, 0, 0,
     1, 0, 1, 0, 0,
     1, 0, 1, 0, 0,
     1, 1, 1, 0, 0},

    {1, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     1, 0, 1, 0, 0,
     1, 0, 1, 0, 0,
     1, 1, 1, 0, 0},

    {1, 0, 1, 1, 1,
     1, 0, 1, 0, 1,
     1, 0, 1, 0, 1,
     1, 0, 1, 0, 1,
     1, 1, 1, 0, 1},
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
    //funcao para espelhar os leds (Por padrão as imagens aparecem invertidas)
    int ordem_fisica[NUM_PIXELS] = 
    {
        24, 23, 22, 21, 20, 
        15, 16, 17, 18, 19, 
        14, 13, 12, 11, 10,
        5, 6, 7, 8, 9,     
        4, 3, 2, 1, 0       
    };

    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        int indice_fisico = ordem_fisica[i];
        valor_led = matrix_rgb(desenho[indice_fisico], r = 0.0, g = 0.0); // Define a cor do LED
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void criar_desenho(double desenho[NUM_FRAMES][NUM_PIXELS], uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int i = 0; i < NUM_FRAMES ; i++) { 
        desenho_pio(desenho[i], valor_led, pio, sm, r, g, b);
        sleep_ms(1000);
    }
}

void limpar_desenho(uint32_t valor_led, PIO pio, uint sm) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        valor_led = (0, 0, 0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void executar_tecla(char tecla, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    switch (tecla)
    {
    case '0':
            for (int i = 0; i < FRAMED0 ; i++)
            {
             desenho_pio(desenho0[i], valor_led, pio, sm, r, g, b);
             sleep_ms(1000);
            }
            break;


    case '1':
             criar_desenho(desenho1, valor_led, pio, sm, r, g, b);
             sleep_ms(1000);
             limpar_desenho(valor_led, pio, sm);
            break;
    case '2':

            for (int i = 0; i < FRAMED2 ; i++)
            {
             desenho_pio(desenho2[i], valor_led, pio, sm, r, g, b);
             sleep_ms(1000);
            }

             break;  


    case '3': {
             criar_desenho(desenho3, valor_led, pio, sm, r, g, b);
             sleep_ms(1000);
             limpar_desenho(valor_led, pio, sm);
            break;
        }

    case '4':
             criar_desenho(desenho4, valor_led, pio, sm, r, g, b);
             sleep_ms(1000);
             limpar_desenho(valor_led, pio, sm);
            break;
            
    case 'A': // Desliga os leds
            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
             valor_led = matrix_rgb(b = 0.0, r=0.0, g=0.0);
             pio_sm_put_blocking(pio, sm, valor_led);
            }
            break;
    case 'B': // Controla LED azul
            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
            valor_led = matrix_rgb(b = 1.0, r=0.0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
            }
            break;
    case 'C': // Controla LED vermelho
            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
            valor_led = matrix_rgb(b = 0.0, r=0.8, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
            }
            break;
    case 'D': // Controla LED verde
            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
            valor_led = matrix_rgb(b = 0.0, r=0.0, g=0.5);
            pio_sm_put_blocking(pio, sm, valor_led);
            }
            break;
    case '#': // Controla LED Branco
            for (int16_t i = 0; i < NUM_PIXELS; i++) 
            {
            valor_led = matrix_rgb(b = 1.0, r=1.0, g=1.0);
            pio_sm_put_blocking(pio, sm, valor_led);
            }
            break;
    case '*': // Habilita o modo de gravação 
            printf("HABILITANDO O MODO GRAVAÇÃO\n");
            sleep_ms(1000);
	        reset_usb_boot(0,0); 
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
