#ifndef LED_EFFECT_3_H
#define LED_EFFECT_3_H

#include "LED_matrix.h"

class effect_3_single_color_rainbow
{
    public:
        effect_3_single_color_rainbow(LED_matrix &matrix); // Constructor accepts reference
        void run(uint16_t fade_step = 200, uint16_t time_each_step = 50);
    private:
        struct RGB {
            float r;
            float g;
            float b;
        };
        
        RGB rainbow_colors[7] = {
        {255, 0, 0},    // Red
        {255, 165, 0},  // Orange
        {255, 255, 0},  // Yellow
        {0, 255, 0},    // Green
        {0, 0, 255},    // Blue
        {75, 0, 130},   // Indigo
        {238, 130, 238} // Violet
        };

        LED_matrix &matrix_ref; // Member variable to store the reference
};

#endif
