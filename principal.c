#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "principal.pio.h"

// Definicoes de pinos e quantidade de LEDs
#define PINO_LED 7
#define NUM_LEDS 25

// Pinos para botoes e LED de status
#define LED_STATUS 13
#define BOTAO_UP 5
#define BOTAO_DOWN 6

// Delay para debounce (em microsegundos)
#define DELAY_REBOTE 200000

// Variaveis globais de controle
static volatile int valor_display = 0;
static volatile uint32_t tempo_ultimo_evento = 0;
static volatile bool atualizar_display_flag = false;

// Definicao das cores RGB iniciais
uint8_t cor_r = 1;
uint8_t cor_g = 1;
uint8_t cor_b = 1;

// Padrões para representar numeros na matriz 5x5
bool matriz_numeros[10][NUM_LEDS] = {
    { 0, 1, 1, 1, 0,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      1, 0, 0, 0, 1,
      0, 1, 1, 1, 0 },  // 0

    { 0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0 },  // 1

    { 0, 1, 1, 1, 0,
      0, 0, 1, 0, 0,
      0, 1, 0, 0, 0,
      0, 0, 0, 1, 0,
      0, 0, 1, 1, 0 },  // 2

    { 0, 1, 1, 1, 0,
      0, 0, 0, 1, 0,
      0, 1, 1, 1, 0,
      0, 0, 0, 1, 0,
      0, 1, 1, 1, 0 },  // 3

    { 0, 1, 0, 0, 0,
      0, 0, 0, 1, 0,
      0, 1, 1, 1, 0,
      0, 1, 0, 1, 0,
      0, 1, 0, 1, 0 },  // 4

    { 0, 0, 1, 1, 0,
      0, 0, 0, 1, 0,
      0, 0, 1, 1, 0,
      0, 1, 0, 0, 0,
      0, 1, 1, 1, 0 },  // 5

    { 0, 1, 1, 1, 0,
      0, 1, 0, 1, 0,
      0, 1, 1, 1, 0,
      0, 1, 0, 0, 0,
      0, 1, 1, 1, 0 },  // 6

    { 0, 0, 0, 1, 0,
      0, 0, 1, 0, 0,
      0, 0, 1, 0, 0,
      0, 0, 0, 1, 0,
      0, 1, 1, 1, 0 },  // 7

    { 0, 1, 1, 1, 0,
      0, 1, 0, 1, 0,
      0, 1, 1, 1, 0,
      0, 1, 0, 1, 0,
      0, 1, 1, 1, 0 },  // 8

    { 0, 1, 1, 1, 0,
      0, 0, 0, 1, 0,
      0, 1, 1, 1, 0,
      0, 1, 0, 1, 0,
      0, 1, 1, 1, 0 }   // 9
};

// Conversao RGB para GRB (para LEDs WS2812)
static inline uint32_t converte_grb(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
}

// Envia a cor para o LED
static inline void envia_dado_led(uint32_t cor_grb) {
    pio_sm_put_blocking(pio0, 0, cor_grb << 8u);
}

// Atualiza a matriz de LEDs com o numero atual
void atualiza_display_leds(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t cor_atual = converte_grb(r, g, b);
    for (int i = 0; i < NUM_LEDS; i++) {
        envia_dado_led(matriz_numeros[valor_display][i] ? cor_atual : 0);
    }
}

// Handler para interrupcoes dos botoes
void trata_interrupcao_botoes(uint gpio, uint32_t events) {
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

    if (tempo_atual - tempo_ultimo_evento > DELAY_REBOTE) {
        tempo_ultimo_evento = tempo_atual;

        if (gpio == BOTAO_UP && valor_display < 9) {
            valor_display++;
        } else if (gpio == BOTAO_DOWN && valor_display > 0) {
            valor_display--;
        }

        atualizar_display_flag = true;
    }
}

// Funcao principal
int main() {
    stdio_init_all();

    // Inicializa PIO para controle dos LEDs
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &principal_program);
    principal_program_init(pio, sm, offset, PINO_LED, 800000, false);

    // Configura LED de status
    gpio_init(LED_STATUS);
    gpio_set_dir(LED_STATUS, GPIO_OUT);

    // Configura botoes
    gpio_init(BOTAO_UP);
    gpio_set_dir(BOTAO_UP, GPIO_IN);
    gpio_pull_up(BOTAO_UP);

    gpio_init(BOTAO_DOWN);
    gpio_set_dir(BOTAO_DOWN, GPIO_IN);
    gpio_pull_up(BOTAO_DOWN);

    // Configura interrupcoes
    gpio_set_irq_enabled_with_callback(BOTAO_UP, GPIO_IRQ_EDGE_FALL, true, &trata_interrupcao_botoes);
    gpio_set_irq_enabled_with_callback(BOTAO_DOWN, GPIO_IRQ_EDGE_FALL, true, &trata_interrupcao_botoes);

    // Loop principal
    while (true) {
        gpio_put(LED_STATUS, 1);
        sleep_ms(150);
        gpio_put(LED_STATUS, 0);
        sleep_ms(50);

        if (atualizar_display_flag) {
            atualiza_display_leds(cor_r, cor_g, cor_b);
            atualizar_display_flag = false;
        }
    }

    return 0;
}
