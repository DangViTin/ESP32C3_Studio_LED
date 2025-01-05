#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "ble_services.h"

LOG_MODULE_REGISTER(BLE_service, LOG_LEVEL_DBG);

static struct service_cb service_cb;
static bool notify_battery_level_enabled;

int service_init(struct service_cb *callbacks)
{
	if (callbacks)
	{
		service_cb.RGB_write_cb = callbacks->RGB_write_cb;
		service_cb.effects_write_cb = callbacks->effects_write_cb;
		service_cb.effects_read_cb = callbacks->effects_read_cb;
		service_cb.fan_write_cb = callbacks->fan_write_cb;
		service_cb.battery_read_cb = callbacks->battery_read_cb;
	}
	return 0;
}

static ssize_t write_RGB(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
						 uint16_t len, uint16_t offset, uint8_t flags)
{
	LOG_DBG("RGB attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);
	if (len != 3)
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);

	if (offset != 0)
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);

	if (service_cb.RGB_write_cb)
	{
		service_cb.RGB_write_cb(((uint8_t *)buf)[0], ((uint8_t *)buf)[1], ((uint8_t *)buf)[2]);
	}
	return len;
}

static ssize_t write_effect(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
							uint16_t len, uint16_t offset, uint8_t flags)
{
	LOG_DBG("Effect attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);
	char effect_name[EFFECTS_MAX_NAME_LEN] = {0};
	memcpy(effect_name, buf, len);
	
	if (service_cb.effects_write_cb)
	{
		service_cb.effects_write_cb(effect_name);
	}
	return len;
}

static ssize_t read_effect(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
						   uint16_t len, uint16_t offset)
{
	LOG_DBG("Effect attribute read, handle: %u, conn: %p", attr->handle, (void *)conn);

	if (service_cb.effects_read_cb)
	{
		char name[128];
		service_cb.effects_read_cb(name);

		return bt_gatt_attr_read(conn, attr, buf, len, offset, name, strlen(name));
	}
	return 0;
}

static ssize_t write_fan_speed(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
							   uint16_t len, uint16_t offset, uint8_t flags)
{
	LOG_DBG("Fan attribute write, handle: %u, conn: %p", attr->handle, (void *)conn);
	if (len != 1)
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);

	if (offset != 0)
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);

	if (service_cb.fan_write_cb)
	{
		service_cb.fan_write_cb(*((uint8_t *)buf));
	}
	return len;
}

static ssize_t read_battery_level(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
								  uint16_t len, uint16_t offset)
{
	LOG_DBG("Battery attribute read, handle: %u, conn: %p", attr->handle, (void *)conn);

	if (service_cb.battery_read_cb)
	{
		uint8_t bat_level;
		service_cb.battery_read_cb(&bat_level);

		return bt_gatt_attr_read(conn, attr, buf, len, offset, &bat_level, sizeof(bat_level));
	}
	return 0;
}

static void service_ccc_battery_level_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	notify_battery_level_enabled = (value == BT_GATT_CCC_NOTIFY);
}

/* Create and add the CONTROL service to the Bluetooth LE stack [0]*/
BT_GATT_SERVICE_DEFINE(control_service, BT_GATT_PRIMARY_SERVICE(BT_UUID_CONTROL),
					   /* Create and add the RGB characteristic [1][2]*/
					   BT_GATT_CHARACTERISTIC(BT_UUID_RGB, BT_GATT_CHRC_WRITE,
											  BT_GATT_PERM_WRITE_AUTHEN, NULL, write_RGB, NULL),

					   /* Create and add the EFFECTS characteristic [3][4]*/
					   BT_GATT_CHARACTERISTIC(BT_UUID_EFFECTS, BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
											  BT_GATT_PERM_READ_AUTHEN | BT_GATT_PERM_WRITE_AUTHEN, read_effect, write_effect, NULL),

					   /* Create and add the FAN characteristic [5][6]*/
					   BT_GATT_CHARACTERISTIC(BT_UUID_FAN, BT_GATT_CHRC_WRITE,
											  BT_GATT_PERM_WRITE_AUTHEN, NULL, write_fan_speed, NULL),

					   /* Create and add the EFFECTS_READ characteristic [7][8]*/
					   BT_GATT_CHARACTERISTIC(BT_UUID_BAS_BATTERY_LEVEL, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
											  BT_GATT_PERM_READ_AUTHEN, read_battery_level, NULL, NULL),
					   /* Create and add the Client Characteristic Configuration Descriptor [9]*/
					   BT_GATT_CCC(service_ccc_battery_level_cfg_changed, BT_GATT_PERM_WRITE_AUTHEN));

int service_battery_level_notify(uint8_t battery_level)
{
	if (!notify_battery_level_enabled)
	{
		return -EACCES;
	}

	return bt_gatt_notify(NULL, &control_service.attrs[7],
						  &battery_level,
						  sizeof(battery_level));
}