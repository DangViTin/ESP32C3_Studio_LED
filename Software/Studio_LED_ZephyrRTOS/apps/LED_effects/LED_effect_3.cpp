#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <math.h>
#include "LED_effect_3.h"

effect_3_single_color_rainbow::effect_3_single_color_rainbow(LED_matrix &matrix) : matrix_ref(matrix)
{
    // matrix_ref.clear();
}

void effect_3_single_color_rainbow::run(uint16_t fade_step, uint16_t time_each_step)
{
    // matrix_ref.clear();
    RGB rainbow_colors[] = {
        {255, 0, 0},    // Red
        {255, 165, 0},  // Orange
        {255, 255, 0},  // Yellow
        {0, 255, 0},    // Green
        {0, 0, 255},    // Blue
        {75, 0, 130},   // Indigo
        {238, 130, 238} // Violet
    };
    RGB current_color, next_color;
    uint8_t array_size = ARRAY_SIZE(rainbow_colors);

    float fade_step_per_color = fade_step * 1.0 / array_size;

    for (uint8_t i = 0; i < array_size; i++)
    {
        current_color = rainbow_colors[i];
        next_color = rainbow_colors[i + 1 >= array_size ? 0 : (i + 1)];

        float r_delta = (next_color.r - current_color.r) / fade_step_per_color;
        float g_delta = (next_color.g - current_color.g) / fade_step_per_color;
        float b_delta = (next_color.b - current_color.b) / fade_step_per_color;

        for (uint8_t k = 0; k < round(fade_step_per_color); k++)
        {
            matrix_ref.fill(round(current_color.r), round(current_color.g), round(current_color.b));

            current_color.r = CLAMP(current_color.r + r_delta, 0, 255);
            current_color.g = CLAMP(current_color.g + g_delta, 0, 255);
            current_color.b = CLAMP(current_color.b + b_delta, 0, 255);

            k_sleep(K_MSEC(time_each_step));
        }
    }

    matrix_ref.fill(round(current_color.r), round(current_color.g), round(current_color.b));
}