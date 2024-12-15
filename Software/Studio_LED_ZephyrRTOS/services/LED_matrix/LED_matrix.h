#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <zephyr/drivers/led_strip.h>
#include <zephyr/drivers/gpio.h>

#define STRIP_NODE		    DT_ALIAS(led_strip)
#define STRIP_NUM_PIXELS	DT_PROP(DT_ALIAS(led_strip), chain_length)

#define LED_PANEL_WIDTH             8
#define LED_PANEL_HEIGHT            8
#define NUMBER_OF_LED_PANEL         2
#define LEDS_PER_PANEL              64
class LED_matrix
{
    public:
        void init();
        int set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
        int clear_pixel(uint8_t x, uint8_t y);
        void clear_all_pixels();
        int set_array(uint8_t const *array, uint8_t x_len, uint8_t y_len, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
        void fill(uint8_t r, uint8_t g, uint8_t b);
        void clear();

        const struct device *strip;
        struct led_rgb pixels[STRIP_NUM_PIXELS] = {0};

    private:
        struct gpio_dt_spec LED_enable_pin;
        int16_t transform_pixel(int16_t x, int16_t y);
};

#endif
