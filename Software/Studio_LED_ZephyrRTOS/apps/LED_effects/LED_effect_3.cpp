#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <math.h>
#include "LED_effect_3.h"

effect_3_single_color_rainbow::effect_3_single_color_rainbow(LED_matrix &matrix) : matrix_ref(matrix)
{
}

void effect_3_single_color_rainbow::run(uint16_t fade_step, uint16_t time_each_step)
{
    static RGB current_color = {0};
    RGB next_color = {0};
    uint8_t array_size = ARRAY_SIZE(rainbow_colors);

    float fade_step_per_color = fade_step * 1.0 / array_size;

    float r_delta = 0;
    float g_delta = 0;
    float b_delta = 0;

    static uint8_t i = 0, k = 0;
    if (i == 0 && k == 0)
    {
        current_color = rainbow_colors[0];
    }

    if (i < array_size)
    {
        next_color = rainbow_colors[i + 1 >= array_size ? 0 : (i + 1)];

        r_delta = (next_color.r - rainbow_colors[i].r) / fade_step_per_color;
        g_delta = (next_color.g - rainbow_colors[i].g) / fade_step_per_color;
        b_delta = (next_color.b - rainbow_colors[i].b) / fade_step_per_color;

        if (k < round(fade_step_per_color))
        {
            matrix_ref.fill(round(current_color.r), round(current_color.g), round(current_color.b));

            current_color.r = CLAMP(current_color.r + r_delta, 0, 255);
            current_color.g = CLAMP(current_color.g + g_delta, 0, 255);
            current_color.b = CLAMP(current_color.b + b_delta, 0, 255);

            k++;
            k_sleep(K_MSEC(time_each_step));
        }
        else
        {
            k = 0;
            i++;
            current_color = rainbow_colors[i];
        }
    }
    else
    {
        matrix_ref.fill(round(current_color.r), round(current_color.g), round(current_color.b));
        i = 0;
    }
}