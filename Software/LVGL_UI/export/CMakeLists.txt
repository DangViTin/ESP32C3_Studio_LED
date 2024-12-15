SET(SOURCES ui_comp_batterybar.c
    ui_comp_nextbtn.c
    ui_comp.c
    ui_main.c
    ui_solidColor.c
    ui_effects.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_bluetooth_disconnected_png.c
    ui_img_bluetooth_connected_png.c
    ui_img_50_png.c
    ui_img_charge_png.c
    ui_img_8x8_dot_png.c
    ui_img_25_png.c
    ui_img_75_png.c
    ui_img_100_png.c
    ui_img_0_png.c)

add_library(ui ${SOURCES})
