#include <string.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/util.h>
#include <math.h>
#include "LED_effect_4.h"

effect_4_random_pixel::effect_4_random_pixel(LED_matrix &matrix) : matrix_ref(matrix)
{
    ;
}

uint8_t effect_4_random_pixel::is_LED_availabe(int8_t pos_x, int8_t pos_y)
{
    for (uint8_t i = 0; i < MAX_PIXEL*2; i+=2)
    {
        if (pos_x == pos[i] && pos_y == pos[i+1])
        {
            return 0;
        }
    }
    return 1;
}

void effect_4_random_pixel::clear()
{
    memset(effect_4_random_pixel::LED_pixel, 0, sizeof(effect_4_random_pixel::LED_pixel));
    memset(effect_4_random_pixel::pos, -1 , sizeof(effect_4_random_pixel::pos));
}

void effect_4_random_pixel::run(uint16_t timeout_sec, uint16_t time_each_step)
{
    matrix_ref.clear();
    for(uint8_t i = 0; i < MAX_PIXEL; i++)
    {
        LED_pixel[i].done_flag = 1;
    }
    
    uint32_t terminate_time = k_uptime_get_32() + timeout_sec*1000;

    while(k_uptime_get_32() <= terminate_time)
    {
        for(uint8_t i = 0; i < MAX_PIXEL; i++)
        {
            // Generate random value for element if it is free
            if (LED_pixel[i].done_flag)
            {
                LED_pixel[i].start_time = k_uptime_get_32() + (sys_rand32_get() % MAX_START_DELAY_TIME_MS);
                do
                {
                    LED_pixel[i].duration = sys_rand32_get() % MAX_DURATION_TIME_MS;
                } 
                while(LED_pixel[i].duration <  MIN_DURATION_TIME_MS);
                LED_pixel[i].end_time = LED_pixel[i].start_time + LED_pixel[i].duration;
                
                // Generate position for LED and check if that position already used
                do
                {
                    LED_pixel[i].pos_x = sys_rand32_get() % (LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL);
                    LED_pixel[i].pos_y = sys_rand32_get() % (LED_PANEL_HEIGHT);
                }
                while(!is_LED_availabe(LED_pixel[i].pos_x, LED_pixel[i].pos_y));

                // Add to list
                pos[i] = LED_pixel[i].pos_x;
                pos[i+1] = LED_pixel[i].pos_y;

                // Generate random value for color
                LED_pixel[i].rgb_value.r = sys_rand32_get() % 0xFF;
                LED_pixel[i].rgb_value.g = sys_rand32_get() % 0xFF;
                LED_pixel[i].rgb_value.b = sys_rand32_get() % 0xFF;
                
                // Calculate 
                LED_pixel[i].r_delta = LED_pixel[i].rgb_value.r / ((LED_pixel[i].duration / 2) / time_each_step);   // duration / 2 because it include fade-in and fade-out time
                LED_pixel[i].g_delta = LED_pixel[i].rgb_value.g / ((LED_pixel[i].duration / 2) / time_each_step);
                LED_pixel[i].b_delta = LED_pixel[i].rgb_value.b / ((LED_pixel[i].duration / 2) / time_each_step);

                // LED value will start from 0
                LED_pixel[i].rgb_value.r = 0;
                LED_pixel[i].rgb_value.g = 0;
                LED_pixel[i].rgb_value.b = 0;

                LED_pixel[i].enable_flag = sys_rand32_get() % 2;    // Let's random to see if this LED can light up :)
                LED_pixel[i].done_flag = 0;
            }
        }

        // Process LED
        for(uint8_t k = 0; k < MAX_PIXEL; k++)
        {
            if (LED_pixel[k].done_flag == 0)
            {
                uint32_t current_time = k_uptime_get_32();
                if (current_time >= LED_pixel[k].end_time)
                {
                    LED_pixel[k].rgb_value.r = 0;
                    LED_pixel[k].rgb_value.g = 0;
                    LED_pixel[k].rgb_value.b = 0;

                    LED_pixel[k].done_flag = 1;
                    pos[k] = -1;
                    pos[k+1] = -1;
                }
                else if (current_time >= LED_pixel[k].start_time + (LED_pixel[k].duration / 2))    // duration / 2 because it include fade-in and fade-out time
                {
                    LED_pixel[k].rgb_value.r = CLAMP(LED_pixel[k].rgb_value.r - LED_pixel[k].r_delta, 0, 0xFF);
                    LED_pixel[k].rgb_value.g = CLAMP(LED_pixel[k].rgb_value.g - LED_pixel[k].g_delta, 0, 0xFF);
                    LED_pixel[k].rgb_value.b = CLAMP(LED_pixel[k].rgb_value.b - LED_pixel[k].b_delta, 0, 0xFF);
                }
                else if (current_time >= LED_pixel[k].start_time)
                {
                    LED_pixel[k].rgb_value.r = CLAMP(LED_pixel[k].rgb_value.r + LED_pixel[k].r_delta, 0, 0xFF);
                    LED_pixel[k].rgb_value.g = CLAMP(LED_pixel[k].rgb_value.g + LED_pixel[k].g_delta, 0, 0xFF);
                    LED_pixel[k].rgb_value.b = CLAMP(LED_pixel[k].rgb_value.b + LED_pixel[k].b_delta, 0, 0xFF);
                }
                if (LED_pixel[k].enable_flag)
                {
                    matrix_ref.set_pixel(LED_pixel[k].pos_x, LED_pixel[k].pos_y, round(LED_pixel[k].rgb_value.r), round(LED_pixel[k].rgb_value.g), round(LED_pixel[k].rgb_value.b));
                }
            }
        }
        led_strip_update_rgb(matrix_ref.strip, matrix_ref.pixels, STRIP_NUM_PIXELS);
        k_sleep(K_MSEC(time_each_step));
    }
    clear();
}