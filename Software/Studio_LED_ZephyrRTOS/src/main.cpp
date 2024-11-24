#include <zephyr/kernel.h>
#include "LED_matrix.h"
#include "LED_effect_1.h"
#include "LED_effect_2.h"
#include "LED_effect_3.h"
#include "LED_effect_4.h"
#include "LED_effect_5.h"
#include "LED_effect_6.h"
#include "digital_clock.h"

#include "cooling_system.h"

LED_matrix my_LED_matrix;
effect_1_clock my_effect_1_clock(my_LED_matrix);
effect_2_single_color my_effect_2_single_color(my_LED_matrix);
effect_3_single_color_rainbow my_effect_3_single_color_rainbow(my_LED_matrix);
effect_4_random_pixel my_effect_4_random_pixel(my_LED_matrix);
effect_5_bouncing_ball my_effect_5_bouncing_ball(my_LED_matrix);
effect_6_meteor my_effect_6_meteor(my_LED_matrix);

int main(void)
{
	// k_sleep(K_MSEC(2000));

	my_LED_matrix.init();
	my_LED_matrix.power_enable();
	my_LED_matrix.clear();

	set_fan_mode(manual_mode);
	set_fan_speed(100, 99);

	while(1)
	{
		// my_effect_1_clock.run(h, m, s);

		my_effect_2_single_color.run(0, 0, 255);
		// k_sleep(K_MSEC(500));

		// my_effect_3_single_color_rainbow.run(200, 50);
		
		// my_effect_4_random_pixel.run(3600, 10);

		// my_effect_5_bouncing_ball.run(10, 80);

		// my_effect_6_meteor.run(10, 40);
	}
	return 0;
}