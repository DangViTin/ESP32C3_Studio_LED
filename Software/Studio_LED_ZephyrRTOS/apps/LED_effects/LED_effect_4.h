#ifndef LED_EFFECT_4_H
#define LED_EFFECT_4_H

#include "LED_matrix.h"

#define MAX_PIXEL                   40
#define MAX_START_DELAY_TIME_MS     5000
#define MIN_DURATION_TIME_MS        5000
#define MAX_DURATION_TIME_MS        12000
struct RGB
{
    float r;
    float g;
    float b;
};

struct LED_pixel_struct
{
    float r_delta;
    float g_delta;
    float b_delta;
    RGB rgb_value;
    uint32_t start_time, end_time, duration;
    uint8_t pos_x, pos_y;
    uint8_t done_flag;
    uint8_t enable_flag;
};
class effect_4_random_pixel
{
    public:
        effect_4_random_pixel(LED_matrix &matrix); // Constructor accepts reference
        void run(uint16_t timeout_sec = 3600, uint16_t time_each_step = 10);
        
    private:
        LED_pixel_struct LED_pixel[MAX_PIXEL] = {0};
        int8_t pos[MAX_PIXEL*2] = {-1};

        uint8_t is_LED_available(int8_t pos_x, int8_t pos_y);
        void clear();

        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
