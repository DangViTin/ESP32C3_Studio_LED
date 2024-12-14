#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include "cooling_system.h"

LOG_MODULE_REGISTER(COOLING_SYS, LOG_LEVEL_DBG);

pulse_width_modulation pwm;
pwm_dt_spec pwm_fan = PWM_DT_SPEC_GET(DT_ALIAS(pwm_fan));

static class cooling_system cooling_system(pwm);

cooling_system::cooling_system(pulse_width_modulation &obj) : obj_ref(obj)
{
    ;
}

void cooling_system::init()
{
    obj_ref.init(&pwm_fan);
}

void cooling_system::set_freq_and_duty(uint32_t freq_hz, float duty_cycle)
{
    this->freq_hz = freq_hz;
    this->duty_cycle = duty_cycle;
}

void cooling_system::apply_freq_and_duty()
{
    obj_ref.set_freq_hz(freq_hz);
    obj_ref.set_duty(duty_cycle);
    obj_ref.apply_setting();
}

void cooling_system::set_mode(cooling_system_mode mode)
{
    is_auto_mode = mode;
}

cooling_system_mode cooling_system::get_mode()
{
    return is_auto_mode;
}

void set_fan_mode(cooling_system_mode mode)
{
    cooling_system.set_mode(mode);
}

void set_fan_speed(uint32_t freq_hz, float duty_cycle)
{
    if (cooling_system.get_mode() == auto_mode)
    {
        LOG_DBG("Cooling system in auto mode, reject changing speed");
        return;
    }
    LOG_DBG("Cooling system freq = %d, duty = %0.2f%%", freq_hz, (double)duty_cycle);
    cooling_system.set_freq_and_duty(freq_hz, duty_cycle);
}

void cooling_system_thread_main()
{
    cooling_system.init();
    while(1)
    {
        // No thermal sensor, auto mode will not be implemented, fixed value for fan speed
        if (cooling_system.get_mode() == auto_mode) 
        {
            cooling_system.set_freq_and_duty(100, 50);
            cooling_system.apply_freq_and_duty();
        }
        else
        {
            cooling_system.apply_freq_and_duty();
        }
        k_sleep(K_MSEC(COOLING_SYS_INTERVAL_MS));
    }
}

K_THREAD_DEFINE(cooling_system_thread, COOLING_SYS_THREAD_STACK_SIZE, 
                cooling_system_thread_main, NULL, NULL, NULL, 
                COOLING_SYS_THREAD_PRIORITY, 0, 0);