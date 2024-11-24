#ifndef PULSE_WIDTH_MODULATION_H
#define PULSE_WIDTH_MODULATION_H

#include <zephyr/drivers/pwm.h>

class pulse_width_modulation
{
    public:
        void init(pwm_dt_spec *pwm_pin);
        void set_freq_hz(uint32_t freq_hz);
        void set_duty(float duty_cycle);
        void apply_setting();
        void disable();

    private:
        pwm_dt_spec *pwm_pin;
        uint32_t freq_hz;
        float duty_cycle;
        uint8_t is_enable;
};

#endif