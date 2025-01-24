#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "pio_matrix.pio.h"

//número de LEDs
#define NUM_PIXELS 25
#define NUM_FRAMES 5
#define ROWS 4
#define COLS 4

//pino de saída
#define OUT_PIN 7

const uint row_pins[ROWS] = {28,27,26,22};  // GPIO para linhas
const uint col_pins[COLS] = {21,20,19,18}; // GPIO para colunas

const char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//iniciar teclado
void init_pins() {
    // Configurar pinos das linhas como saída
    for (int i = 0; i < ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1);
    }

    // Configurar pinos das colunas como entrada com pull-up
    for (int i = 0; i < COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]);
    }
}

char escanearTeclado() {
    for (int row = 0; row < ROWS; row++) {
        // Ativar linha atual
        gpio_put(row_pins[row], 0);

        for (int col = 0; col < COLS; col++) {
            if (!gpio_get(col_pins[col])) { // Verificar se a tecla foi pressionada
                sleep_ms(20); // Debounce
                if (!gpio_get(col_pins[col])) { // Confirmar tecla pressionada
                    gpio_put(row_pins[row], 1);
                    return keys[row][col];
                }
            }
        }

        // Desativar linha atual
        gpio_put(row_pins[row], 1);
    }

    return 0; // Nenhuma tecla pressionada
}

//matriz para criar uma animacao
double desenho4[NUM_FRAMES][NUM_PIXELS] ={ 
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

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) 
    {
            valor_led = matrix_rgb(desenho[24-i], r=0.0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void control_components(char key, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    switch (key) {
        case 'A': // Controla LED verde
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
        case 'C': // Controla LED vermelho
                for (int16_t i = 0; i < NUM_PIXELS; i++) 
                {
                  valor_led = matrix_rgb(b = 0.0, r=0.8, g=0.0);
                  pio_sm_put_blocking(pio, sm, valor_led);
                }
        case 'D':
                for (int16_t i = 0; i < NUM_PIXELS; i++) 
                {
                  valor_led = matrix_rgb(b = 0.0, r=0.0, g=0.5);
                  pio_sm_put_blocking(pio, sm, valor_led);
                }
            break;
        case '#': 
                for (int16_t i = 0; i < NUM_PIXELS; i++) 
                {
                  valor_led = matrix_rgb(b = 1.0, r=1.0, g=1.0);
                  pio_sm_put_blocking(pio, sm, valor_led);
                }
            break;

        case '*':
                printf("HABILITANDO O MODO GRAVAÇÃO\n");
	            reset_usb_boot(0,0); 
                break;

        case '4':
                for (int i = 0; i < NUM_FRAMES; i++) 
                {
                 desenho_pio(desenho4[i], valor_led, pio, sm, r, g, b); // Atualiza a matriz com o quadro atual
                 sleep_ms(500); // Controla o FPS
                }
                break;

        default:
            break;
    }
}

//função principal
int main()
{
    PIO pio = pio0; 
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;

    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    init_pins();

    printf("iniciando a transmissão PIO");
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    //configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    while (true) {

        char key = escanearTeclado();

        control_components( key, valor_led,  pio,  sm,  r,  g,  b);

     }
    
}
