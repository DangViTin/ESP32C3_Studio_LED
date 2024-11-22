#ifndef LED_EFFECT_6_H
#define LED_EFFECT_6_H

#include "LED_matrix.h"

#define MAX_METEOR          8
#define METEOR_TAIL_LENGTH  6
#define METEOR_FADING_RATIO 0.7

class effect_6_meteor
{
    public:
        effect_6_meteor(LED_matrix &matrix); // Constructor accepts reference
        void run(uint16_t timeout_sec = 10000, uint16_t time_each_step = 40);

    private:

        struct RGB
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        struct meteor
        {
            RGB rgb_value[METEOR_TAIL_LENGTH];
            int8_t pos_x; 
            int8_t pos_y;
            uint8_t done_flag;
        };

        int8_t pos[MAX_METEOR] = {-1};
        meteor meteor[MAX_METEOR] = {0};
        void clear();
        uint8_t is_obj_available(int8_t pos_x);
        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
