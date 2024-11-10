#ifndef LED_EFFECT_5_H
#define LED_EFFECT_5_H

#include "LED_matrix.h"

#define MAX_BALL  3

class effect_5_bouncing_ball
{
    public:
        effect_5_bouncing_ball(LED_matrix &matrix); // Constructor accepts reference
        void run(uint16_t timeout_sec = 10000, uint16_t time_each_step = 120);

    private:
        int8_t pos[MAX_BALL*2] = {-1};
        struct RGB
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        struct ball
        {
            RGB rgb_value;
            uint8_t pos_x, pos_y;
            uint8_t speed_point;
            uint8_t speed;  // Larger mean slower
            uint8_t x_direction;
            uint8_t y_direction;
        };

        ball ball[MAX_BALL] = {0};
        void clear();
        uint8_t is_ball_available(int8_t pos_x, int8_t pos_y);
        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
