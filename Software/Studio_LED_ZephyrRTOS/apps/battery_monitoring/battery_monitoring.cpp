#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include "battery_monitoring.h"
#include "math.h"

LOG_MODULE_REGISTER(BAT_MONITORING, LOG_LEVEL_INF);

analog_digital_converter adc_battery;
gpio_dt_spec adc0_ch3_enable_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(adc0_ch3_enable_pin), gpios);
gpio_dt_spec charging_status_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(charger_status_pin), gpios);

static class battery_monitoring battery_monitoring(adc_battery);

battery_monitoring::battery_monitoring(analog_digital_converter &obj) : obj_ref(obj)
{
    ;
}

void battery_monitoring::init()
{
    obj_ref.init(0, &adc0_ch3_enable_pin);
    gpio_pin_configure_dt(&charging_status_pin, GPIO_INPUT);
}

uint32_t battery_monitoring::measure()
{
    measurement_voltage = obj_ref.read_voltage() * BATTERY_ADC_RATIO;
    LOG_DBG("Battery measurement voltage: %d mV", measurement_voltage);
    return measurement_voltage;
}

uint32_t battery_monitoring::read_last_measurement()
{
    return measurement_voltage;
}

uint8_t battery_monitoring::read_is_charging()
{
    is_charging = gpio_pin_get_dt(&charging_status_pin);
    LOG_DBG("Battery charging status: %d", is_charging);
    return is_charging;
}

uint8_t battery_monitoring::read_battery_percent()
{
    if (measurement_voltage >= BAT_FULL_VOLTAGE_MV)
    {
        battery_percent = 100;
    }
    else if (measurement_voltage <= BAT_LOW_VOLTAGE_MV)
    {
        battery_percent = 0;
    }
    else
    {
        battery_percent = (uint8_t) round((measurement_voltage - BAT_LOW_VOLTAGE_MV) * 100.0 / (BAT_FULL_VOLTAGE_MV - BAT_LOW_VOLTAGE_MV));
    }

    LOG_DBG("Battery percent: %d %%", battery_percent);
    return battery_percent;
}

uint32_t get_battery_voltage()
{
    return battery_monitoring.read_last_measurement();
}

uint8_t get_charging_status()
{
    return battery_monitoring.read_is_charging();
}

uint8_t get_battery_percent()
{
    return battery_monitoring.read_battery_percent();
}

void battery_measurement_thread_main()
{
    battery_monitoring.init();
    while (1)
    {
        battery_monitoring.measure();
        k_sleep(K_MSEC(BAT_MEASUREMENT_INTERVAL_MS));
    }
}

K_THREAD_DEFINE(battery_measurement_thread, BAT_MEASUREMENT_THREAD_STACK_SIZE,
                battery_measurement_thread_main, NULL, NULL, NULL,
                BAT_MEASUREMENT_THREAD_PRIORITY, 0, 0);