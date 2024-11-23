#include <zephyr/logging/log.h>
#include "battery_monitoring.h"

LOG_MODULE_REGISTER(BAT_MONITORING, LOG_LEVEL_DBG);

analog_digital_converter adc_battery;
gpio_dt_spec adc0_ch3_enable_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(adc0_ch3_enable_pin), gpios);

battery_monitoring::battery_monitoring(analog_digital_converter &obj) : obj_ref(obj)
{
    ;
}

void battery_monitoring::init()
{
    obj_ref.init(0, &adc0_ch3_enable_pin);
}

uint32_t battery_monitoring::measure()
{
    measurement_voltage = obj_ref.read_voltage() * BATTERY_ADC_RATIO;
    return measurement_voltage;
} 

uint32_t battery_monitoring::read_last_measurement()
{
    return measurement_voltage;
} 

void main_thread()
{
    battery_monitoring battery_monitoring(adc_battery);
    battery_monitoring.init();
    while(1)
    {
        LOG_DBG("Battery measurement voltage: %d mV", battery_monitoring.measure());
        k_sleep(K_MSEC(BAT_MEASUREMENT_INTERVAL_MS));
    }
}

K_THREAD_DEFINE(battery_measurement_thread, BAT_MEASUREMENT_THREAD_STACK_SIZE, 
                main_thread, NULL, NULL, NULL, 
                BAT_MEASUREMENT_THREAD_PRIORITY, 0, 0);