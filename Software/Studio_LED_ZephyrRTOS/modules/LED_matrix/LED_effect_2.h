#ifndef LED_EFFECT_2_H
#define LED_EFFECT_2_H

#include "LED_matrix.h"

class effect_2_single_color
{
    public:
        effect_2_single_color(LED_matrix &matrix); // Constructor accepts reference
        void run(uint8_t r, uint8_t g, uint8_t b);

    private:
        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
