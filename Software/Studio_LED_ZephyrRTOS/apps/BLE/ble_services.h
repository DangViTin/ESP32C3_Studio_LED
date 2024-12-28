#ifndef BLE_SERVICES_H
#define BLE_SERVICES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>

#define BT_UUID_CONTROL_VAL         BT_UUID_128_ENCODE(0x0193fe95, 0x239a, 0x7f23, 0x826d, 0x2b53e38f42b5)
#define BT_UUID_RGB_VAL         	BT_UUID_128_ENCODE(0x0193fe95, 0x239a, 0x7f23, 0x826d, 0x2b53e38f42b6)
#define BT_UUID_EFFECTS_VAL  		BT_UUID_128_ENCODE(0x0193fe95, 0x239a, 0x7f23, 0x826d, 0x2b53e38f42b7)
#define BT_UUID_FAN_VAL 			BT_UUID_128_ENCODE(0x0193fe95, 0x239a, 0x7f23, 0x826d, 0x2b53e38f42b9)

#define BT_UUID_CONTROL         	BT_UUID_DECLARE_128(BT_UUID_CONTROL_VAL)
#define BT_UUID_RGB         		BT_UUID_DECLARE_128(BT_UUID_RGB_VAL)
#define BT_UUID_EFFECTS  			BT_UUID_DECLARE_128(BT_UUID_EFFECTS_VAL)
#define BT_UUID_FAN					BT_UUID_DECLARE_128(BT_UUID_FAN_VAL)

#define EFFECTS_MAX_NAME_LEN 		128

typedef void (*RGB_write_cb_t)(uint8_t red, uint8_t green, uint8_t blue);
typedef void (*effects_write_cb_t)(char *effect_name);
typedef void (*effects_read_cb_t)(char *effect_name);
typedef void (*fan_write_cb_t)(uint8_t fan_speed);
typedef void (*battery_read_cb_t)(uint8_t *battery_level);

struct service_cb {
	/** RGB write callback. */
	RGB_write_cb_t RGB_write_cb;
	/** Effects write callback. */
	effects_write_cb_t effects_write_cb;
	/** Effects read callback. */
	effects_read_cb_t effects_read_cb;
	/** Fan write callback. */
	fan_write_cb_t fan_write_cb;
	/** Battery read callback. */
	battery_read_cb_t battery_read_cb;
};

int service_init(struct service_cb *callbacks);
int service_battery_level_notify(uint8_t battery_level);

#ifdef __cplusplus
}
#endif

#endif