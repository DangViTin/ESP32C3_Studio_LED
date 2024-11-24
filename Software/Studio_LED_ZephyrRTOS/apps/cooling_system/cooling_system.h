#ifndef COOLING_SYSTEM_H
#define COOLING_SYSTEM_H

#include "pulse_width_modulation.h"

#define COOLING_SYS_THREAD_STACK_SIZE   512
#define COOLING_SYS_THREAD_PRIORITY     5
#define COOLING_SYS_INTERVAL_MS         1000

enum cooling_system_mode
{
    auto_mode,
    manual_mode,
};

class cooling_system
{
    public:
        cooling_system(pulse_width_modulation &obj); // Constructor accepts reference
        void init();
        void set_mode(cooling_system_mode mode);
        cooling_system_mode get_mode();
        void set_freq_and_duty(uint32_t freq_hz, float duty_cycle);
        void apply_freq_and_duty();

    private:
        uint32_t freq_hz;
        float duty_cycle;
        cooling_system_mode is_auto_mode = auto_mode;
        pulse_width_modulation &obj_ref; // Member variable to store the reference
};

void set_fan_mode(cooling_system_mode mode);
void set_fan_speed(uint32_t freq_hz, float duty_cycle);

#endif