#include <zephyr/kernel.h>
#include "LED_matrix.h"
#include "LED_effect_1.h"
#include "LED_effect_2.h"
#include "LED_effect_3.h"
#include "LED_effect_4.h"
#include "LED_effect_5.h"
#include "LED_effect_6.h"
#include "digital_clock.h"

#include <zephyr/drivers/display.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl_input_device.h>
#include "ui.h"

LED_matrix my_LED_matrix;
effect_1_clock my_effect_1_clock(my_LED_matrix);
effect_2_single_color my_effect_2_single_color(my_LED_matrix);
effect_3_single_color_rainbow my_effect_3_single_color_rainbow(my_LED_matrix);
effect_4_random_pixel my_effect_4_random_pixel(my_LED_matrix);
effect_5_bouncing_ball my_effect_5_bouncing_ball(my_LED_matrix);
effect_6_meteor my_effect_6_meteor(my_LED_matrix);

static const struct device *lvgl_keypad = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));

lv_group_t *screen1_group;
lv_group_t *screen2_group;
lv_group_t *screen3_group;

void set_input_screen_1(lv_event_t * e)
{
	lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen1_group);
}

void set_input_screen_2(lv_event_t * e)
{
	lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen2_group);
}

void set_input_screen_3(lv_event_t * e)
{
	lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen3_group);
}

void screen_3_init_effect(lv_event_t * e)
{
	char buf[25] = {0};
	lv_roller_get_selected_str(ui_effectsRoller, buf, sizeof(buf));
}

void effects_change(lv_event_t * e)
{
	char buf[25] = {0};
	lv_roller_get_selected_str(ui_effectsRoller, buf, sizeof(buf));
	if (!strcmp(buf, "Rainbow"))
	{
	}
	else if (!strcmp(buf, "Meteors"))
	{
	}
	else if (!strcmp(buf, "Twinkling stars"))
	{
	}
	else if (!strcmp(buf, "Bouncing balls"))
	{
	}
	else if (!strcmp(buf, "Digital clock"))
	{
	}
}

int main(void)
{
	// k_sleep(K_MSEC(1000));

	
	my_LED_matrix.init();
	my_LED_matrix.power_enable();
	my_LED_matrix.clear();

	const struct device *display_dev;
	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	display_set_pixel_format(display_dev, PIXEL_FORMAT_MONO10);

	screen1_group = lv_group_create();
	screen2_group = lv_group_create();
	screen3_group = lv_group_create();

	ui_init();


	// Can only add after ui_init
	lv_group_add_obj(screen1_group, ui_nextBtn1);
	lv_group_add_obj(screen1_group, ui_FanSwitch);

	lv_group_add_obj(screen2_group, ui_redSlide);
	lv_group_add_obj(screen2_group, ui_greenSlide);
	lv_group_add_obj(screen2_group, ui_blueSlide);
	lv_group_add_obj(screen2_group, ui_nextBtn2);

	lv_group_add_obj(screen3_group, ui_effectsRoller);
	lv_group_add_obj(screen3_group, ui_nextBtn3);

	lv_task_handler();
	display_blanking_off(display_dev);

	while(1)
	{
		lv_task_handler();
		// my_effect_1_clock.run(h, m, s);

		// my_effect_2_single_color.run(0, 0, 255);

		// my_effect_3_single_color_rainbow.run(200, 50);
		
		// my_effect_4_random_pixel.run(3600, 10);

		// my_effect_5_bouncing_ball.run(10, 80);

		// my_effect_6_meteor.run(10, 40);

		k_sleep(K_MSEC(10));
	}
	return 0;
}