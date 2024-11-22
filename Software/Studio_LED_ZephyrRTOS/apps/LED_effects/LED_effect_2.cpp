#include "LED_effect_2.h"

effect_2_single_color::effect_2_single_color(LED_matrix &matrix) : matrix_ref(matrix)
{
    ;
}

void effect_2_single_color::run(uint8_t r, uint8_t g, uint8_t b)
{
    matrix_ref.fill(r, g, b);
}