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
	while(1)
	{
		lv_bar_set_value(ui_batteryBar, get_battery_percent(), LV_ANIM_ON);
		lv_bar_set_value(ui_batteryBar2, get_battery_percent(), LV_ANIM_ON);

		if (!strcmp(roller_str, "Rainbow"))
		{
			my_effect_3_single_color_rainbow.run(200, 50);
		}
		else if (!strcmp(roller_str, "Meteors"))
		{
			my_effect_6_meteor.run(10, 40);
		}
		else if (!strcmp(roller_str, "Twinkling stars"))
		{
			my_effect_4_random_pixel.run(3600, 10);
		}
		else if (!strcmp(roller_str, "Bouncing balls"))
		{
			my_effect_5_bouncing_ball.run(10, 80);
		}
		else if (!strcmp(roller_str, "Digital clock"))
		{
			// my_effect_1_clock.run(h, m, s);
		}
		else
		{
			my_effect_2_single_color.run(0, 0, 0);
		}

		k_sleep(K_MSEC(10));
	}
	return 0;
}