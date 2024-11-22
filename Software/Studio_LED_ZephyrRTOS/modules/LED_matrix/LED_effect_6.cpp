#include "LED_effect_6.h"
#include <string.h>
#include <zephyr/random/random.h>

effect_6_meteor::effect_6_meteor(LED_matrix &matrix) : matrix_ref(matrix) 
{
    ;
}

void effect_6_meteor::clear()
{
    memset(effect_6_meteor::pos, -1 , sizeof(effect_6_meteor::pos));
}

uint8_t effect_6_meteor::is_obj_available(int8_t pos_x)
{
    for (uint8_t i = 0; i < MAX_METEOR; i++)
    {
        if (pos_x == pos[i])
        {
            return 0;
        }
    }
    return 1;
}

void effect_6_meteor::run(uint16_t timeout_sec, uint16_t time_each_step)
{
    for(uint8_t i = 0; i < MAX_METEOR; i++)
    {
        meteor[i].done_flag = 1;
    }

    uint32_t terminate_time = k_uptime_get_32() + timeout_sec*1000;
    while(k_uptime_get_32() <= terminate_time)
    {
        // Init meteor value
        for (uint8_t i = 0; i < MAX_METEOR; i++)
        {
            if (!meteor[i].done_flag)
            {
                continue;
            }

            // Generate position for meteor and check if that position already used
            do
            {
                meteor[i].pos_x = sys_rand32_get() % (LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL);
            }
            while (!is_obj_available(meteor[i].pos_x));

            meteor[i].pos_y = (LED_PANEL_HEIGHT - 1) + (sys_rand32_get() % LED_PANEL_HEIGHT);

            // Add to list
            pos[i] = meteor[i].pos_x;

            // Meteor head value
            meteor[i].rgb_value[0].r = sys_rand32_get() % 0xFF;
            meteor[i].rgb_value[0].g = sys_rand32_get() % 0xFF;
            meteor[i].rgb_value[0].b = sys_rand32_get() % 0xFF;

            // Meteor tail value fading out from head color
            for (uint8_t k = 1; k < METEOR_TAIL_LENGTH; k++)
            {
                meteor[i].rgb_value[k].r = meteor[i].rgb_value[k-1].r * METEOR_FADING_RATIO * (1.0 - (1.0 * k / METEOR_TAIL_LENGTH));
                meteor[i].rgb_value[k].g = meteor[i].rgb_value[k-1].g * METEOR_FADING_RATIO * (1.0 - (1.0 * k / METEOR_TAIL_LENGTH));
                meteor[i].rgb_value[k].b = meteor[i].rgb_value[k-1].b * METEOR_FADING_RATIO * (1.0 - (1.0 * k / METEOR_TAIL_LENGTH));
            }

            meteor[i].done_flag = 0;
        }

        // Process
        matrix_ref.clear_all_pixels();

        for (uint8_t i = 0; i < MAX_METEOR; i++)
        {
            // Delete out of bonds obj
            if (meteor[i].pos_y < -(METEOR_TAIL_LENGTH - 1))
            {
                pos[i] = -1;
                meteor[i].done_flag = 1;
                continue;
            }
            
            for (uint8_t k = 0; k < METEOR_TAIL_LENGTH; k++)
            {
                matrix_ref.set_pixel(meteor[i].pos_x, meteor[i].pos_y + k, meteor[i].rgb_value[k].r, meteor[i].rgb_value[k].g, meteor[i].rgb_value[k].b);
            }
            meteor[i].pos_y-- ;


        }

        led_strip_update_rgb(matrix_ref.strip, matrix_ref.pixels, STRIP_NUM_PIXELS);
        k_sleep(K_MSEC(time_each_step));
    }
    clear();
}