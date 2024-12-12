#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include "analog_digital_converter.h"

LOG_MODULE_REGISTER(ADC, LOG_LEVEL_INF);

void analog_digital_converter::init(uint8_t dt_adc_channel_idx, gpio_dt_spec *adc_enable_pin)
{
    ADC_channel = adc_channels_list[dt_adc_channel_idx];

    if (adc_enable_pin != NULL)
    {
        enable_pin = adc_enable_pin;
        gpio_pin_configure_dt(enable_pin, GPIO_OUTPUT_HIGH);
    }

    int err;
    /* Configure channels individually prior to sampling. */
    if (!adc_is_ready_dt(&ADC_channel))
    {
        LOG_DBG("ADC controller device %s not ready", ADC_channel.dev->name);
        return;
    }

    err = adc_channel_setup_dt(&ADC_channel);
    if (err < 0)
    {
        LOG_DBG("Could not setup %s, ch %d, err: %d", ADC_channel.dev->name, ADC_channel.channel_id, err);
        return;
    }

    (void)adc_sequence_init_dt(&ADC_channel, &sequence);
}

int32_t analog_digital_converter::read_raw()
{
    int err;

    if (enable_pin)
        gpio_pin_set_dt(enable_pin, 0);

    err = adc_read_dt(&ADC_channel, &sequence);
    if (err < 0)
    {
        LOG_ERR("Could not read %s, ch %d, err = %d", ADC_channel.dev->name, ADC_channel.channel_id, err);
    }

    if (enable_pin)
        gpio_pin_set_dt(enable_pin, 1);

    LOG_DBG("%s, ch %d: raw val: %d", ADC_channel.dev->name, ADC_channel.channel_id, adc_raw_value);
    return adc_raw_value;
}

int32_t analog_digital_converter::read_voltage()
{
    int32_t _adc_voltage_mV_value_;
    read_raw();
    // Using temporary variable
    _adc_voltage_mV_value_ = adc_raw_value;
    adc_raw_to_millivolts_dt(&ADC_channel, &_adc_voltage_mV_value_);
    adc_voltage_mV_value = _adc_voltage_mV_value_;

    LOG_DBG("%s, ch %d: vol val: %d", ADC_channel.dev->name, ADC_channel.channel_id, adc_voltage_mV_value);
    return adc_voltage_mV_value;
}