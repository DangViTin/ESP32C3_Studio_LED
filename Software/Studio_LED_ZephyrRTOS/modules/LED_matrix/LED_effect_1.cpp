#include "LED_effect_1.h"

effect_1_clock::effect_1_clock(LED_matrix &matrix) : matrix_ref(matrix) 
{
    ;
}

void effect_1_clock::run(uint8_t hr, uint8_t min, uint8_t sec)
{
    matrix_ref.clear();
    // Draw numbers
    matrix_ref.set_array(&font5x8[0][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 0, 2, 0, 80, 80);
    matrix_ref.set_array(&font5x8[1][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 4, 2, 0, 80, 0);
    // matrix_ref.set_array(&font5x8[10][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 7, 2, 255, 0, 255);
    matrix_ref.set_array(&font5x8[2][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 9, 2, 80, 0, 0);
    matrix_ref.set_array(&font5x8[3][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 13, 2, 0, 0, 80);

    led_strip_update_rgb(matrix_ref.strip, matrix_ref.pixels, STRIP_NUM_PIXELS);
}