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
#include "ble.h"
#include "ble_services.h"

#include "cooling_system.h"

LED_matrix my_LED_matrix;
effect_1_clock usr_effect_1_clock(my_LED_matrix);
effect_2_single_color usr_effect_2_single_color(my_LED_matrix);
effect_3_single_color_rainbow usr_effect_3_single_color_rainbow(my_LED_matrix);
effect_4_random_pixel usr_effect_4_random_pixel(my_LED_matrix);
effect_5_bouncing_ball usr_effect_5_bouncing_ball(my_LED_matrix);
effect_6_meteor usr_effect_6_meteor(my_LED_matrix);

enum ble_control_LED_flag_t
{
	NONE,
	RGB_MODE,
	EFFECTS_MODE
} ble_control_LED_flag;

char ble_control_effect_name[EFFECTS_MAX_NAME_LEN];
uint8_t ble_control_new_effect_flag = 0;
uint8_t ble_control_RGB[3] = {0};

/****** App callbacks ******/
static void usr_RGB_r_cb(uint8_t red, uint8_t green, uint8_t blue)
{
	ble_control_LED_flag = RGB_MODE;
	ble_control_RGB[0] = red;
	ble_control_RGB[1] = green;
	ble_control_RGB[2] = blue;
}

static void usr_effect_w_cb(char *effect_name)
{
	ble_control_LED_flag = EFFECTS_MODE;
	memset(ble_control_effect_name, 0, sizeof(ble_control_effect_name));
	strcpy(ble_control_effect_name, effect_name);
	ble_control_new_effect_flag = 1;
}

static void usr_fan_speed_r_cb(uint8_t fan_speed)
{
	set_fan_mode(manual_mode);
	set_fan_speed(300, fan_speed);
}

static void usr_battery_value_r_cb(uint8_t *battery_level)
{
	*battery_level = get_battery_percent();
}

static void usr_effect_r_cb(char *effect_name)
{
	strcpy(effect_name, "Rainbow\nMeteors\nStars\nBalls\nClock");
}

static struct service_cb service_callback = {
	.RGB_write_cb = usr_RGB_r_cb,
	.effects_write_cb = usr_effect_w_cb,
	.effects_read_cb = usr_effect_r_cb,
	.fan_write_cb = usr_fan_speed_r_cb,
	.battery_read_cb = usr_battery_value_r_cb,
};

int main(void)
{
	my_LED_matrix.init();
	my_LED_matrix.clear();
	service_init(&service_callback);

	while (1)
	{
		// Solid RGB screen
		if (lv_scr_act() == ui_solidColor)
		{
			set_battery_icon_percent(get_battery_percent());
			battery_is_charging(get_charging_status());
			usr_effect_2_single_color.run(
				lv_slider_get_value(ui_redSlide) * 225 / 100,
				lv_slider_get_value(ui_greenSlide) * 225 / 100,
				lv_slider_get_value(ui_blueSlide) * 255 / 100);
			k_sleep(K_MSEC(10));
		}

		// Effect screen
		else if (lv_scr_act() == ui_effects)
		{
			set_battery_icon_percent(get_battery_percent());
			battery_is_charging(get_charging_status());
			if (!strcmp(roller_str_get(), "Rainbow"))
			{
				usr_effect_3_single_color_rainbow.run(500, 20);
			}
			else if (!strcmp(roller_str_get(), "Meteors"))
			{
				if (new_effect_flag_get())
				{
					usr_effect_6_meteor.init();
				}
				usr_effect_6_meteor.run(40);
			}
			else if (!strcmp(roller_str_get(), "Stars"))
			{
				if (new_effect_flag_get())
				{
					usr_effect_4_random_pixel.init();
				}
				usr_effect_4_random_pixel.run(10);
			}
			else if (!strcmp(roller_str_get(), "Balls"))
			{
				if (new_effect_flag_get())
				{
					usr_effect_5_bouncing_ball.init();
				}
				usr_effect_5_bouncing_ball.run(80);
			}
			else if (!strcmp(roller_str_get(), "Clock"))
			{
				usr_effect_1_clock.run(11, 11, 11);
				k_sleep(K_MSEC(10));
			}
			else
			{
				k_sleep(K_MSEC(10));
			}
		}

		// main screen
		else
		{
			set_battery_icon_percent(get_battery_percent());
			battery_is_charging(get_charging_status());
			if (!is_ble_connected())
			{
				usr_effect_2_single_color.run(0, 0, 0);
				k_sleep(K_MSEC(10));
				continue;
			}

			switch (ble_control_LED_flag)
			{
			case RGB_MODE:
				usr_effect_2_single_color.run(ble_control_RGB[0], ble_control_RGB[1], ble_control_RGB[2]);
				k_sleep(K_MSEC(10));
				break;

			case EFFECTS_MODE:
				if (!strcmp(ble_control_effect_name, "Rainbow"))
				{
					usr_effect_3_single_color_rainbow.run(500, 20);
				}
				else if (!strcmp(ble_control_effect_name, "Meteors"))
				{
					if (ble_control_new_effect_flag || new_effect_flag_get())
					{
						usr_effect_6_meteor.init();
						ble_control_new_effect_flag = 0;
					}
					usr_effect_6_meteor.run(40);
				}
				else if (!strcmp(ble_control_effect_name, "Stars"))
				{
					if (ble_control_new_effect_flag || new_effect_flag_get())
					{
						usr_effect_4_random_pixel.init();
						ble_control_new_effect_flag = 0;
					}
					usr_effect_4_random_pixel.run(10);
				}
				else if (!strcmp(ble_control_effect_name, "Balls"))
				{
					if (ble_control_new_effect_flag || new_effect_flag_get())
					{
						usr_effect_5_bouncing_ball.init();
						ble_control_new_effect_flag = 0;
					}
					usr_effect_5_bouncing_ball.run(80);
				}
				else if (!strcmp(ble_control_effect_name, "Clock"))
				{
					usr_effect_1_clock.run(11, 11, 11);
					k_sleep(K_MSEC(10));
				}
				else
				{
					usr_effect_2_single_color.run(0, 0, 0);
					k_sleep(K_MSEC(10));
				}
				break;

			default:
				usr_effect_2_single_color.run(0, 0, 0);
				k_sleep(K_MSEC(10));
			}
		}
	}
	return 0;
}