#ifndef DIGITAL_ANALOG_CONVERTER_H
#define DIGITAL_ANALOG_CONVERTER_H

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>

#define DT_SPEC_AND_COMMA(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
const uint8_t adc_number_of_channels = DT_PROP_LEN(DT_PATH(zephyr_user), io_channels);
const adc_dt_spec adc_channels_list[] = {DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)};

class analog_digital_converter
{
public:
    // adc_enable_pin is not required, NULL if not needed
    void init(uint8_t dt_adc_channel_idx, gpio_dt_spec *adc_enable_pin = NULL);
    int32_t read_raw();
    int32_t read_voltage();

private:
    adc_dt_spec ADC_channel;
    gpio_dt_spec *enable_pin;
    int32_t adc_raw_value;
    int32_t adc_voltage_mV_value;

    struct adc_sequence sequence =
    {
        .buffer = &adc_raw_value,
        /* buffer size in bytes, not number of samples */
        .buffer_size = sizeof(adc_raw_value),
    };
};
#endif