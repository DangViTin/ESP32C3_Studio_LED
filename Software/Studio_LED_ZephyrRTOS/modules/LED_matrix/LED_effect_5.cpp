#include "LED_effect_5.h"
#include <string.h>
#include <zephyr/random/random.h>

effect_5_bouncing_ball::effect_5_bouncing_ball(LED_matrix &matrix) : matrix_ref(matrix) 
{
    ;
}

void effect_5_bouncing_ball::clear()
{
    memset(effect_5_bouncing_ball::ball, 0, sizeof(effect_5_bouncing_ball::ball));
    memset(effect_5_bouncing_ball::pos, -1 , sizeof(effect_5_bouncing_ball::pos));
}

uint8_t effect_5_bouncing_ball::is_ball_available(int8_t pos_x, int8_t pos_y)
{
    for (uint8_t i = 0; i < (MAX_BALL*2) - 1; i+=2)
    {
        if (pos_x == pos[i] && pos_y == pos[i+1])
        {
            return 0;
        }
    }
    return 1;
}

void effect_5_bouncing_ball::run(uint16_t timeout_sec, uint16_t time_each_step)
{
    // Clear all old effects
    matrix_ref.clear_all_pixels();

    // Init balls value
    for (uint8_t i = 0; i < MAX_BALL; i++)
    {
        // Generate position for balls and check if that position already used
        do
        {
            ball[i].pos_x = sys_rand32_get() % (LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL);
            ball[i].pos_y = sys_rand32_get() % LED_PANEL_HEIGHT;
        }
        while (!is_ball_available(ball[i].pos_x, ball[i].pos_y));

        ball[i].rgb_value.r = sys_rand32_get() % 0xFF;
        ball[i].rgb_value.g = sys_rand32_get() % 0xFF;
        ball[i].rgb_value.b = sys_rand32_get() % 0xFF;
        ball[i].x_direction = sys_rand32_get() % 2;
        ball[i].y_direction = sys_rand32_get() % 2;
        ball[i].speed = (sys_rand32_get() % 4) + 1;

        matrix_ref.set_pixel(ball[i].pos_x, ball[i].pos_y, ball[i].rgb_value.r, ball[i].rgb_value.g, ball[i].rgb_value.b);
    }

    led_strip_update_rgb(matrix_ref.strip, matrix_ref.pixels, STRIP_NUM_PIXELS);
    k_sleep(K_MSEC(time_each_step));

    // Process balls
    uint32_t terminate_time = k_uptime_get_32() + timeout_sec*1000;
    while(k_uptime_get_32() <= terminate_time)
    {
        for (uint8_t i = 0; i < MAX_BALL; i++)
        {
            matrix_ref.clear_pixel(ball[i].pos_x, ball[i].pos_y);

            if (ball[i].pos_x == (LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL) - 1)
            {
                ball[i].x_direction = 0;
            }
            else if (ball[i].pos_x == 0)
            {
                ball[i].x_direction = 1;
            }

            if (ball[i].pos_y == LED_PANEL_HEIGHT - 1)
            {
                ball[i].y_direction = 0;
            }
            else if (ball[i].pos_y == 0)
            {
                ball[i].y_direction = 1;
            }

            ball[i].pos_x += (ball[i].x_direction > 0 ? 1 : -1);
            ball[i].pos_y += (ball[i].y_direction > 0 ? 1 : -1);

            matrix_ref.set_pixel(ball[i].pos_x, ball[i].pos_y, ball[i].rgb_value.r, ball[i].rgb_value.g, ball[i].rgb_value.b);
        }

        led_strip_update_rgb(matrix_ref.strip, matrix_ref.pixels, STRIP_NUM_PIXELS);
        k_sleep(K_MSEC(time_each_step));
    }
    clear();
}