#ifndef LED_EFFECT_1_H
#define LED_EFFECT_1_H

#include "LED_matrix.h"

class effect_1_clock
{
    public:
        effect_1_clock(LED_matrix &matrix); // Constructor accepts reference
        void run(uint8_t hr, uint8_t min, uint8_t sec);

    private:
        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
