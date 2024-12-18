#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/led_strip.h>

#include "LED_matrix.h"

LOG_MODULE_REGISTER(LED_matrix, LOG_LEVEL_DBG);

void LED_matrix::init()
{
    strip = DEVICE_DT_GET(STRIP_NODE);

    static_assert(STRIP_NUM_PIXELS == (LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL) * LED_PANEL_HEIGHT, "Number of LED in device tree is different with number of LED in matrix");

    if (device_is_ready(strip))
    {
		LOG_DBG("Found LED strip device %s", strip->name);
	}
    else
    {
		LOG_ERR("LED strip device %s is not ready", strip->name);
	}
}

//                  Panel 1                                     Panel 0
//     +---+---+---+---+---+---+---+---+           +---+---+---+---+---+---+---+---+
//  7  |63 |   |   |   |   |   |<- |56 |      +--- |63 |   |   |   |   |   |<- |56 | 
//     +---+---+---+---+---+---+---+---+      |    +---+---+---+---+---+---+---+---+
//  6  |   |   |   |   |   |   |   |   |      |    |   |   |   |   |   |   |   |   | 
//     +---+---+---+---+---+---+---+---+      |    +---+---+---+---+---+---+---+---+
//  5  |   |   |   |   |   |   |   |   |      |    |   |   |   |   |   |   |   |   | 
//     +---+---+---+---+---+---+---+---+      |    +---+---+---+---+---+---+---+---+
//  4  |   |   |   |   |   |   |   |   |      |    |   |   |   |   |   |   |   |   | 
//     +---+---+---+---+---+---+---+---+      |    +---+---+---+---+---+---+---+---+
//  3  |   |   |   |   |   |   |   |   |      |    |   |   |   |   |   |   |   |   | 
//     +---+---+---+---+---+---+---+---+      |    +---+---+---+---+---+---+---+---+
//  2  |   |   |   |   |   |   |   |   |      |    |   |   |   |   |   |   |   |   | 
//     +---+---+---+---+---+---+---+---+      |    +---+---+---+---+---+---+---+---+
//  1  |   |   |   |   |   |   |   |   |      |    |   |   |   |   |   |   |   |   | 
//     +---+---+---+---+---+---+---+---+      |    +---+---+---+---+---+---+---+---+
//  0  | 7 |   |   |   |   |   |<- |0  | <----+    | 7 |   |   |   |   |   |<- | 0 | <- Input signal
//     +---+---+---+---+---+---+---+---+           +---+---+---+---+---+---+---+---+
//       0   1   2   3   4   5   6   7               8   9  10  11  12  13  14  15

int16_t LED_matrix::transform_pixel(int16_t x, int16_t y)
{
    if (x < 0 || y < 0)
        return -1;
    
    if ((x >= LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL) || (y >= LED_PANEL_HEIGHT))
        return -1;

    uint8_t panel_number;
    panel_number = (NUMBER_OF_LED_PANEL - 1) - (x / LED_PANEL_WIDTH);
    return (panel_number * LEDS_PER_PANEL) + (LED_PANEL_WIDTH * y) + (LED_PANEL_WIDTH - (x % LED_PANEL_WIDTH)) - 1;
}
   
int LED_matrix::set_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
    int16_t LED_pos = transform_pixel(x, y);
    if (LED_pos >= STRIP_NUM_PIXELS || LED_pos < 0)
        return 0;

    pixels[LED_pos].r = r;
    pixels[LED_pos].g = g;
    pixels[LED_pos].b = b;
    return 1;
}

int LED_matrix::clear_pixel(uint8_t x, uint8_t y)
{
    uint16_t LED_pos = transform_pixel(x, y);
    if (LED_pos >= STRIP_NUM_PIXELS)
        return 0;

    memset(&pixels[LED_pos], 0, sizeof(pixels[LED_pos]));
    return 1;
}

void LED_matrix::clear_all_pixels()
{
    memset(pixels, 0, sizeof(pixels));
}

int LED_matrix::set_array(uint8_t const *array, uint8_t x_len, uint8_t y_len, uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
    // Check if array fit the LED panel
    if (x + x_len > LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL)
        return -1;
    if (y + y_len > LED_PANEL_HEIGHT)
        return -1;
    
    // set the array
    for (int8_t i = y_len - 1; i >= 0; i--)
    {
        for (int8_t k = x_len - 1 ; k >=0; k--)
        {
            if ((array[i] >> k) & 0x01)
            {
                set_pixel(x + x_len - 1 - k, y + y_len - 1 - i, r, g, b);
            }
            else
            {
                clear_pixel(x + x_len - 1 - k, y + y_len - 1 - i);
            }
        }
    }
    return 0;
}

void LED_matrix::fill(uint8_t r, uint8_t g, uint8_t b)
{
    for (uint8_t y = 0; y < LED_PANEL_HEIGHT; y++)
    {
        for(uint8_t x = 0; x < LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL; x++)
        {
            set_pixel(x, y, r, g, b);
        }
    }
    led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
}

void LED_matrix::clear()
{
    for (uint8_t y = 0; y < LED_PANEL_HEIGHT; y++)
    {
        for(uint8_t x = 0; x < LED_PANEL_WIDTH * NUMBER_OF_LED_PANEL; x++)
        {
            set_pixel(x, y, 0, 0, 0);
        }
    }
    led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
}