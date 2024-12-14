// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: UI

#include "ui.h"


// COMPONENT batteryBar

lv_obj_t * ui_batteryBar_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_batteryBar;
    cui_batteryBar = lv_bar_create(comp_parent);
    lv_bar_set_value(cui_batteryBar, 25, LV_ANIM_OFF);
    lv_bar_set_start_value(cui_batteryBar, 0, LV_ANIM_OFF);
    lv_obj_set_width(cui_batteryBar, 80);
    lv_obj_set_height(cui_batteryBar, 4);
    lv_obj_set_x(cui_batteryBar, -1);
    lv_obj_set_y(cui_batteryBar, 11);
    lv_obj_set_align(cui_batteryBar, LV_ALIGN_CENTER);
    lv_obj_clear_flag(cui_batteryBar, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE);     /// Flags
    lv_obj_set_style_bg_color(cui_batteryBar, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_batteryBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(cui_batteryBar, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(cui_batteryBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(cui_batteryBar, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(cui_batteryBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(cui_batteryBar, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_batteryBar, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_t ** children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_BATTERYBAR_NUM);
    children[UI_COMP_BATTERYBAR_BATTERYBAR] = cui_batteryBar;
    lv_obj_add_event_cb(cui_batteryBar, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_batteryBar, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_batteryBar_create_hook(cui_batteryBar);
    return cui_batteryBar;
}
