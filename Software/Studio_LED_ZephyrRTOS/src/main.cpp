#include <zephyr/kernel.h>
#include "LED_matrix.h"
#include "LED_effect_1.h"
#include "LED_effect_2.h"
#include "LED_effect_3.h"
#include "LED_effect_4.h"

LED_matrix my_LED_matrix;
effect_1_clock my_effect_1_clock(my_LED_matrix);
effect_2_single_color my_effect_2_single_color(my_LED_matrix);
effect_3_single_color_rainbow my_effect_3_single_color_rainbow(my_LED_matrix);
effect_4_random_pixel my_effect_4_random_pixel(my_LED_matrix);

int main(void)
{
	my_LED_matrix.init();
	my_LED_matrix.power_enable();

	while(1)
	{
		// my_effect_1_clock.run(NULL, NULL, NULL);
		// k_sleep(K_MSEC(500));

		// my_effect_2_single_color.run(100, 100, 100);
		// k_sleep(K_MSEC(500));

		// my_effect_3_single_color_rainbow.run(100, 50);
		
		my_effect_4_random_pixel.run(999, 10);
	}
	return 0;
}