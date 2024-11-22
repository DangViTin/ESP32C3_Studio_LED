#include <zephyr/device.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/sys/timeutil.h>
#include <zephyr/logging/log.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "digital_clock.h"
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(RTC, LOG_LEVEL_DBG);

static const struct device *rtc_dev = DEVICE_DT_GET(DT_NODELABEL(rtc));



void digital_clock_init()
{
    if (device_is_ready(rtc_dev))
    {
        LOG_DBG("RTC device started!");
    }
    else
    {
        LOG_ERR("RTC device start failed, System halt");
        while(1);
    }
    
    time_t timer_set_value = 1767225595UL;
    struct rtc_time time_set;

	gmtime_r(&timer_set_value, (struct tm *)(&time_set));

	time_set.tm_isdst = -1;
	time_set.tm_nsec = 0;

	/* Set RTC time */
	rtc_set_time(rtc_dev, &time_set);
}

void digital_clock_get_time()
{
    struct rtc_time datetime_get;
    rtc_get_time(rtc_dev, &datetime_get);
    LOG_DBG("%d:%d:%d", datetime_get.tm_hour, datetime_get.tm_min, datetime_get.tm_sec);
}