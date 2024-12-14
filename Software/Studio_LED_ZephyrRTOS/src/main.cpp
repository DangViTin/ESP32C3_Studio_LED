#include <zephyr/kernel.h>
#include "LED_matrix.h"
#include "LED_effect_1.h"
#include "LED_effect_2.h"
#include "LED_effect_3.h"
#include "LED_effect_4.h"
#include "LED_effect_5.h"
#include "LED_effect_6.h"
#include "digital_clock.h"

#include "ui.h"
#include "usr_ui.h"

#include "battery_monitoring.h"

LED_matrix my_LED_matrix;
effect_1_clock my_effect_1_clock(my_LED_matrix);
effect_2_single_color my_effect_2_single_color(my_LED_matrix);
effect_3_single_color_rainbow my_effect_3_single_color_rainbow(my_LED_matrix);
effect_4_random_pixel my_effect_4_random_pixel(my_LED_matrix);
effect_5_bouncing_ball my_effect_5_bouncing_ball(my_LED_matrix);
effect_6_meteor my_effect_6_meteor(my_LED_matrix);

int main(void)
{
	my_LED_matrix.init();
	my_LED_matrix.power_enable();
	my_LED_matrix.clear();

	k_sleep(K_MSEC(100));
	while (1)
	{
		// Solid RGB screen
		if (lv_scr_act() == ui_solidColor)
		{
			set_battery_icon_percent(get_battery_percent());
			my_effect_2_single_color.run(
				lv_slider_get_value(ui_redSlide) * 225 / 100,
				lv_slider_get_value(ui_greenSlide) * 225 / 100,
				lv_slider_get_value(ui_blueSlide) * 255 / 100);
			k_sleep(K_MSEC(10));
		}

		// Effect screen
		else if (lv_scr_act() == ui_effects)
		{
			set_battery_icon_percent(get_battery_percent());
			if (!strcmp(roller_str_get(), "Rainbow"))
			{
				my_effect_3_single_color_rainbow.run(500, 20);
			}
			else if (!strcmp(roller_str_get(), "Meteors"))
			{
				if (new_effect_flag_get())
				{
					my_effect_6_meteor.init();
				}
				my_effect_6_meteor.run(40);
			}
			else if (!strcmp(roller_str_get(), "Stars"))
			{
				if (new_effect_flag_get())
				{
					my_effect_4_random_pixel.init();
				}
				my_effect_4_random_pixel.run(10);
			}
			else if (!strcmp(roller_str_get(), "Balls"))
			{
				if (new_effect_flag_get())
				{
					my_effect_5_bouncing_ball.init();
				}
				my_effect_5_bouncing_ball.run(80);
			}
			else if (!strcmp(roller_str_get(), "Clock"))
			{
				my_effect_1_clock.run(11, 11, 11);
				k_sleep(K_MSEC(10));
			}
		}

		// main screen
		else
		{
			set_battery_icon_percent(get_battery_percent());
			my_effect_2_single_color.run(0, 0, 0);
			k_sleep(K_MSEC(10));
		}

		k_sleep(K_MSEC(1));
	}
	return 0;
}