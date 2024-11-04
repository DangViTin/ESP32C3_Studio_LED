#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>

#define LOG_LEVEL 4
LOG_MODULE_REGISTER(main);

static const struct gpio_dt_spec LED_enable = GPIO_DT_SPEC_GET(DT_NODELABEL(led_strip_enable), gpios);

#define STRIP_NODE		DT_ALIAS(led_strip)
#define STRIP_NUM_PIXELS	DT_PROP(DT_ALIAS(led_strip), chain_length)

#define LED_UPDATE_DELAY	1000
#define DELAY_TIME K_MSEC(LED_UPDATE_DELAY)

#define RGB(_r, _g, _b) { .r = (_r), .g = (_g), .b = (_b) }

static const struct led_rgb colors[] = {
	RGB(0xff, 0x00, 0x00), /* red */
	RGB(0x00, 0xff, 0x00), /* green */
	RGB(0x00, 0x00, 0xff), /* blue */
};

struct led_rgb pixels[STRIP_NUM_PIXELS] = {0};

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

int main(void)
{
	gpio_pin_configure_dt(&LED_enable, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&LED_enable, 1);

	size_t index = 0;
	int rc;

	if (device_is_ready(strip)) {
		LOG_INF("Found LED strip device %s", strip->name);
	} else {
		LOG_ERR("LED strip device %s is not ready", strip->name);
		return 0;
	}

	while (1) {
		LOG_INF("Displaying on strip");
		for (size_t cursor = 0; cursor < ARRAY_SIZE(pixels); cursor++)
		{
			memcpy(&pixels[cursor], &colors[0], sizeof(struct led_rgb));
		}
			led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
			k_sleep(DELAY_TIME);
		
		for (size_t cursor = 0; cursor < ARRAY_SIZE(pixels); cursor++)
		{
			memcpy(&pixels[cursor], &colors[1], sizeof(struct led_rgb));
		}
			led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
			k_sleep(DELAY_TIME);
		
		for (size_t cursor = 0; cursor < ARRAY_SIZE(pixels); cursor++)
		{
			memcpy(&pixels[cursor], &colors[2], sizeof(struct led_rgb));
		}
			led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
			k_sleep(DELAY_TIME);
	}

	return 0;
}