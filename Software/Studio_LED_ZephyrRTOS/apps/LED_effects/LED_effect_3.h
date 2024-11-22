#ifndef LED_EFFECT_3_H
#define LED_EFFECT_3_H

#include "LED_matrix.h"

class effect_3_single_color_rainbow
{
    public:
        effect_3_single_color_rainbow(LED_matrix &matrix); // Constructor accepts reference
        void run(uint16_t fade_step = 500, uint16_t time_each_step = 50);

    private:
        struct RGB {
            float r;
            float g;
            float b;
        };
        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
