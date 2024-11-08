#include "LED_effect_1.h"
#include <zephyr/kernel.h>

effect_1_clock::effect_1_clock(LED_matrix &matrix) : matrix_ref(matrix) 
{

}

void effect_1_clock::run(uint8_t hr, uint8_t min, uint8_t sec)
{
    // Draw numbers
    matrix_ref.set_array(&font5x8[hr / 10][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 0, 2, CLOCK_LEDS_COLOR);
    matrix_ref.set_array(&font5x8[hr % 10][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 4, 2, CLOCK_LEDS_COLOR);
    matrix_ref.set_array(&font5x8[min / 10][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 9, 2, CLOCK_LEDS_COLOR);
    matrix_ref.set_array(&font5x8[min % 10][0], FONT5X8_WIDHT, FONT5X8_HEIGHT, 13, 2, CLOCK_LEDS_COLOR);

    // Draw sperated mark
    if (sec % 2 == 0)
    {
        matrix_ref.clear_pixel(7, 2);      
    }
    else
    {
        matrix_ref.set_pixel(7, 2, CLOCK_DOT_LEDS_COLOR);
    }

    // 1 day have 1440 minutes.

    float percent_of_the_day_left = (float)(MINUTES_OF_DAY - (hr * 60 + min)) / MINUTES_OF_DAY;
    float leds_remain = (LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL) * percent_of_the_day_left;

    for (uint8_t i = 0; i < (uint8_t)leds_remain; i++)
    {
        matrix_ref.set_pixel(i, 0, 0, BOTTOM_BAR_COLOR_VALUE, 0);
    }

    float decimals_part_of_leds_remain = leds_remain - (uint8_t)leds_remain;
    matrix_ref.set_pixel((uint8_t)leds_remain, 0, 1, (uint8_t)(BOTTOM_BAR_COLOR_VALUE * decimals_part_of_leds_remain), 0);

    for (uint8_t i = (uint8_t)leds_remain + 1; i < LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL; i++)
    {
        matrix_ref.set_pixel(i, 0, 0, 0, 0);
    }

    led_strip_update_rgb(matrix_ref.strip, matrix_ref.pixels, STRIP_NUM_PIXELS);
}