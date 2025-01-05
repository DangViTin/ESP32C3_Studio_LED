#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/display.h>
#include <lvgl_input_device.h>
#include "ui.h"
#include "usr_ui.h"

LOG_MODULE_REGISTER(USR_UI, LOG_LEVEL_DBG);

static const struct device *lvgl_keypad = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));

static class ui_control ui_control;

lv_group_t *screen1_group;
lv_group_t *screen2_group;
lv_group_t *screen3_group;

void screen_1_start_load(lv_event_t *e)
{
    ui_control.new_effect_flag_set();
    lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen1_group);
}

void screen_2_start_load(lv_event_t *e)
{
    lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen2_group);
}

void screen_3_start_load(lv_event_t *e)
{
    ui_control.new_effect_flag_set();
    lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), screen3_group);
    lv_roller_get_selected_str(ui_effectsRoller, ui_control.roller_str_read(), ROLLER_STR_LENGHT);
}

void scr3_roller_key_right(lv_event_t *e)
{
    ui_control.new_effect_flag_set();
    lv_roller_get_selected_str(ui_effectsRoller, ui_control.roller_str_read(), ROLLER_STR_LENGHT);
}

void scr3_roller_key_left(lv_event_t *e)
{
    ui_control.new_effect_flag_set();
    lv_roller_get_selected_str(ui_effectsRoller, ui_control.roller_str_read(), ROLLER_STR_LENGHT);
}

void screen_3_start_unload(lv_event_t *e)
{
    memset(ui_control.roller_str_read(), 0, ROLLER_STR_LENGHT);
}

//////////////////////////////////////////////////////////////////

void ui_control::new_effect_flag_set()
{
    new_effect_flag = 1;
}

void ui_control::new_effect_flag_clear()
{
    new_effect_flag = 0;
}

uint8_t ui_control::new_effect_flag_read()
{
    return new_effect_flag;
}

char *ui_control::roller_str_read()
{
    return roller_str;
}

char *roller_str_get()
{
    return ui_control.roller_str_read();
}

uint8_t new_effect_flag_get()
{
    uint8_t value = ui_control.new_effect_flag_read();
    ui_control.new_effect_flag_clear();
    return value;
}

void set_battery_icon_percent(uint8_t percent)
{
    ui_control.battery_percent_input = percent;
}

void battery_is_charging(uint8_t is_charging)
{
    ui_control.is_charging = is_charging;
}

void set_battery_icon()
{
    if (ui_control.is_charging)
    {
        lv_obj_clear_flag(ui_charge1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_charge2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_charge3, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(ui_charge1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_charge2, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_charge3, LV_OBJ_FLAG_HIDDEN);
    }

    if (ui_control.battery_percent_input < 5)
    {
        lv_img_set_src(ui_battery1, &ui_img_0_png);
        lv_img_set_src(ui_battery2, &ui_img_0_png);
        lv_img_set_src(ui_battery3, &ui_img_0_png);
    }
    else if (ui_control.battery_percent_input < 25)
    {
        lv_img_set_src(ui_battery1, &ui_img_25_png);
        lv_img_set_src(ui_battery2, &ui_img_25_png);
        lv_img_set_src(ui_battery3, &ui_img_25_png);
    }
    else if (ui_control.battery_percent_input < 50)
    {
        lv_img_set_src(ui_battery1, &ui_img_50_png);
        lv_img_set_src(ui_battery2, &ui_img_50_png);
        lv_img_set_src(ui_battery3, &ui_img_50_png);
    }
    else if (ui_control.battery_percent_input < 75)
    {
        lv_img_set_src(ui_battery1, &ui_img_75_png);
        lv_img_set_src(ui_battery2, &ui_img_75_png);
        lv_img_set_src(ui_battery3, &ui_img_75_png);
    }
    else
    {
        lv_img_set_src(ui_battery1, &ui_img_100_png);
        lv_img_set_src(ui_battery2, &ui_img_100_png);
        lv_img_set_src(ui_battery3, &ui_img_100_png);
    }
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
        k_sleep(K_MSEC(USR_UI_INTERVAL_MS));
        set_battery_icon();
        lv_task_handler();
    }
}

K_THREAD_DEFINE(usr_ui_thread, USR_UI_THREAD_STACK_SIZE,
                usr_ui_thread_main, NULL, NULL, NULL,
                USR_UI_THREAD_PRIORITY, 0, 0);