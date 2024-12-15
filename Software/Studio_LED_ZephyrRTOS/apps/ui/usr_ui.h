#ifndef USR_UI_H
#define USR_UI_H

#define USR_UI_INTERVAL_MS          10
#define USR_UI_THREAD_STACK_SIZE    (1024*3)
#define USR_UI_THREAD_PRIORITY      5

#define ROLLER_STR_LENGHT           25
class ui_control
{
    public:
        void new_effect_flag_set();
        void new_effect_flag_clear();
        uint8_t new_effect_flag_read();
        char *roller_str_read();

        uint8_t battery_percent_input;
        uint8_t is_charging;
    private:
        uint8_t new_effect_flag;
        char roller_str[ROLLER_STR_LENGHT];
};

uint8_t new_effect_flag_get();
char *roller_str_get();
void set_battery_icon_percent(uint8_t percent);
void battery_is_charging(uint8_t is_charging);

#endif