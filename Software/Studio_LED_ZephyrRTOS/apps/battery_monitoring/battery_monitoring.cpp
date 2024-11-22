#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>
#include <battery_monitoring.h>

#define DT_SPEC_AND_COMMA(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
        DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
    };



void ADC_test()
{
    gpio_dt_spec ADC_read_enable_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(adc_read_enable), gpios);
    gpio_pin_configure_dt(&ADC_read_enable_pin, GPIO_OUTPUT_HIGH);

    int err;
    uint32_t count = 0;
    uint16_t buf;
    struct adc_sequence sequence = {
        .buffer = &buf,
        /* buffer size in bytes, not number of samples */
        .buffer_size = sizeof(buf),
    };

    /* Configure channels individually prior to sampling. */
    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
    {
        if (!adc_is_ready_dt(&adc_channels[i]))
        {
            printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
            return;
        }

        err = adc_channel_setup_dt(&adc_channels[i]);
        if (err < 0)
        {
            printk("Could not setup channel #%d (%d)\n", i, err);
            return;
        }
    }

    printk("ADC reading[%u]:\n", count++);
    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
    {
        int32_t val_mv;

        printk("- %s, channel %d: ",
               adc_channels[i].dev->name,
               adc_channels[i].channel_id);

        gpio_pin_set_dt(&ADC_read_enable_pin, 0);
        (void)adc_sequence_init_dt(&adc_channels[i], &sequence);

        err = adc_read_dt(&adc_channels[i], &sequence);
        if (err < 0)
        {
            printk("Could not read (%d)\n", err);
            continue;
        }
        gpio_pin_set_dt(&ADC_read_enable_pin, 1);

        val_mv = (int32_t)buf;

        printk("%" PRId32, val_mv);
        err = adc_raw_to_millivolts_dt(&adc_channels[i], &val_mv);
        /* conversion to mV may not be supported, skip if not */
        if (err < 0)
        {
            printk(" (value in mV not available)\n");
        }
        else
        {
            printk(" = %" PRId32 " mV\n", val_mv);
            printk("battery voltage %d mV\n", val_mv*2);
        }
    }
}