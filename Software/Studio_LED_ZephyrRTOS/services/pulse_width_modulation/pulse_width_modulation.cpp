#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

#include "pulse_width_modulation.h"

LOG_MODULE_REGISTER(PWM, LOG_LEVEL_DBG);

void pulse_width_modulation::init(pwm_dt_spec *pwm_pin)
{
	if (!pwm_is_ready_dt(pwm_pin))
    {
		LOG_ERR("Error: PWM device %s is not ready", pwm_pin->dev->name);
		return;
	}
    this->pwm_pin = pwm_pin;
}

void pulse_width_modulation::set_freq_hz(uint32_t freq_hz)
{
    this->freq_hz = freq_hz;
}

void pulse_width_modulation::set_duty(float duty_cycle)
{
    if (duty_cycle <= 100 && duty_cycle >= 0)
        this->duty_cycle = duty_cycle;
    else
        LOG_ERR("Error: PWM duty (%.2f) is not valid, range (0 - 100) ", (double)duty_cycle);
}

void pulse_width_modulation::disable()
{
    pwm_set_dt(pwm_pin, PWM_HZ(10), 0);
    is_enable = 0;
}

void pulse_width_modulation::apply_setting()
{
    pwm_set_dt(pwm_pin, PWM_HZ(freq_hz), (duty_cycle/100) * PWM_HZ(freq_hz));
    is_enable = 1;
}