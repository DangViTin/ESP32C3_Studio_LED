# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(BOARD esp32c3_devkitm)
set(DTC_OVERLAY_FILE "boards/ESP32C3_Studio_LED.overlay")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(Studio_LED)

target_sources(app PRIVATE src/main.cpp)

add_subdirectory(services/LED_matrix)
add_subdirectory(services/analog_digital_converter)
add_subdirectory(services/pulse_width_modulation)

add_subdirectory(apps/LED_effects)
add_subdirectory(apps/digital_clock)
add_subdirectory(apps/battery_monitoring)
add_subdirectory(apps/cooling_system)