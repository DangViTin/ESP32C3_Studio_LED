#ifndef BATTERY_MONITORING_H
#define BATTERY_MONITORING_H

#include "analog_digital_converter.h"

#define BAT_MEASUREMENT_THREAD_STACK_SIZE   1024
#define BAT_MEASUREMENT_THREAD_PRIORITY     5
#define BAT_MEASUREMENT_INTERVAL_MS         1000
#define BATTERY_ADC_RATIO                   2

#define BAT_FULL_VOLTAGE_MV    4200
#define BAT_LOW_VOLTAGE_MV    3600

class battery_monitoring
{
    public:
        battery_monitoring(analog_digital_converter &obj); // Constructor accepts reference
        void init();
        uint32_t measure();
        uint32_t read_last_measurement();
        uint8_t read_battery_percent();
        uint8_t read_is_charging();

    private:
        uint32_t measurement_voltage;
        uint8_t battery_percent;
        uint8_t is_charging;
        analog_digital_converter &obj_ref; // Member variable to store the reference
};

uint32_t get_battery_voltage();
uint8_t get_charging_status();
uint8_t get_battery_percent();

#endif