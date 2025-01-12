#ifndef BLE_H
#define BLE_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEVICE_NAME "Studio LED"

// https://github.com/DangViTin
#define URL_DATA {0x17, '/', '/', 'g', 'i', 't', 'h', 'u', 'b', '.', 'c', 'o', 'm', '/', 'D', 'a', 'n', 'g', 'V', 'i', 'T', 'i', 'n'}

#define BLE_THREAD_STACK_SIZE (1024 * 3)
#define BLE_THREAD_PRIORITY 5
#define BLE_INTERVAL_START_DELAY 500
#define BLE_INTERVAL_MS (CONFIG_BT_PERIPHERAL_PREF_MIN_INT + CONFIG_BT_PERIPHERAL_PREF_MAX_INT)



bool is_ble_connected();
int ble_remove_all_bonded_devices();
uint32_t ble_get_pass_code();

#ifdef __cplusplus
}
#endif

#endif