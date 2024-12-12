#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/display.h>
#include <lvgl_input_device.h>
#include "ui.h"
#include "usr_ui.h"

LOG_MODULE_REGISTER(USR_UI, LOG_LEVEL_DBG);

static const struct device *lvgl_keypad = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));

char roller_str[25] = {0};

lv_group_t *screen1_group;
lv_group_t *screen2_group;
lv_group_t *screen3_group;

void set_input_screen_1(lv_event_t *e)
{
    lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen1_group);
}

void set_input_screen_2(lv_event_t *e)
{
    lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen2_group);
}

void set_input_screen_3(lv_event_t *e)
{
    lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen3_group);
}

void screen_3_init_effect(lv_event_t *e)
{
    lv_roller_get_selected_str(ui_effectsRoller, roller_str, sizeof(roller_str));
}

void effects_change(lv_event_t *e)
{
    lv_roller_get_selected_str(ui_effectsRoller, roller_str, sizeof(roller_str));
}

void screen_3_exit_effect(lv_event_t *e)
{
    memset(roller_str, 0, sizeof(roller_str));
}

void usr_ui_thread_main()
{
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

    while (1)
    {
        lv_task_handler();
        k_sleep(K_MSEC(USR_UI_INTERVAL_MS));
    }
}

K_THREAD_DEFINE(usr_ui_thread, USR_UI_THREAD_STACK_SIZE,
                usr_ui_thread_main, NULL, NULL, NULL,
                USR_UI_THREAD_PRIORITY, 0, 0);