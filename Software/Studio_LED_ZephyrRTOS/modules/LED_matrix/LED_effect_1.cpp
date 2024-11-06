#include "LED_effect_1.h"

effect_1_clock::effect_1_clock(LED_matrix &matrix) : matrix_ref(matrix) 
{
    ;
}

void effect_1_clock::run(uint8_t hr, uint8_t min, uint8_t sec)
{
    matrix_ref.clear();
    matrix_ref.fill(50, 0, 0);
}