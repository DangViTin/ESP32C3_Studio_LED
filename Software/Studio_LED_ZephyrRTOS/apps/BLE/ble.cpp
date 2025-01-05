#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/settings/settings.h>

#include "ble_services.h"
#include "ble.h"

#include "battery_monitoring.h"

LOG_MODULE_REGISTER(usr_BLE, LOG_LEVEL_DBG);

uint8_t btn_state = 0;

class ble_connection_status_class
{
private:
    bool is_connected;

public:
    void set_connection_status();
    void clear_connection_status();
    int ble_remove_all_bonded_devices();
    bool get_connection_status() const;
};

ble_connection_status_class ble_connection_status;

void ble_connection_status_class::set_connection_status()
{
    is_connected = true;
}

void ble_connection_status_class::clear_connection_status()
{
    is_connected = false;
}

int ble_connection_status_class::ble_remove_all_bonded_devices()
{
    int err = bt_unpair(BT_ID_DEFAULT, BT_ADDR_LE_ANY);
    if (err)
        LOG_ERR("Cannot delete bond (err: %d)", err);
    return err;
}

bool ble_connection_status_class::get_connection_status() const
{
    return is_connected;
}

bool is_ble_connected()
{
    return ble_connection_status.get_connection_status();
}

int ble_remove_all_bonded_devices()
{
    return ble_connection_status.ble_remove_all_bonded_devices();
}

// Config advertising parameters and advertising data
#define BT_LE_ADV_CONN_NO_ACCEPT_LIST BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONN,        \
                                                      BT_GAP_ADV_FAST_INT_MIN_2, \
                                                      BT_GAP_ADV_FAST_INT_MAX_2, NULL)

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, (sizeof(DEVICE_NAME) - 1)),
};

// Config scan response data
static unsigned char url_data[] = URL_DATA;

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),
};

static void advertise(struct k_work *work)
{
    // Add some delay to make sure free connection objects available.
    k_sleep(K_MSEC(100));

    int err = 0;
    err = bt_le_adv_start(BT_LE_ADV_CONN_NO_ACCEPT_LIST, ad, ARRAY_SIZE(ad),
                          sd, ARRAY_SIZE(sd));
    if (err)
    {
        LOG_ERR("Advertising failed to start (err %d)", err);
    }
}
K_WORK_DEFINE(advertise_work, advertise);

/****** Authentication callbacks ******/
static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
    LOG_DBG("Passkey for %s: %06u", addr, passkey);
}

static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
    LOG_DBG("Pairing cancelled: %s", addr);
}

static struct bt_conn_auth_cb conn_auth_callbacks = {
    .passkey_display = auth_passkey_display,
    .passkey_entry = NULL,
    .cancel = auth_cancel,
};

/****** Pairing callbacks ******/
static void print_bond_list(const struct bt_bond_info *info, void *user_data)
{
    int *bond_cnt = (int *)user_data;
    if ((*bond_cnt) < 0)
    {
        return;
    }
    LOG_DBG("Bond list No.%d: %02X:%02X:%02X:%02X:%02X:%02X", *bond_cnt,
            info->addr.a.val[0],
            info->addr.a.val[1],
            info->addr.a.val[2],
            info->addr.a.val[3],
            info->addr.a.val[4],
            info->addr.a.val[5]);
    (*bond_cnt)++;
}

static void pairing_complete(struct bt_conn *conn, bool bonded)
{
    LOG_DBG("Pairing Complete");
    int bond_cnt = 0;
    bt_foreach_bond(BT_ID_DEFAULT, print_bond_list, &bond_cnt);
    LOG_DBG("Number of bonds: %d/%d", bond_cnt, CONFIG_BT_MAX_PAIRED);
}

static void pairing_failed(struct bt_conn *conn, enum bt_security_err reason)
{
    LOG_ERR("Pairing Failed (%d). Disconnecting.", reason);
    bt_conn_disconnect(conn, BT_HCI_ERR_AUTH_FAIL);
}

static struct bt_conn_auth_info_cb auth_cb_info = {
    .pairing_complete = pairing_complete,
    .pairing_failed = pairing_failed,
};

/****** Connection callbacks ******/
static void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (err)
    {
        LOG_ERR("Connection error %d", err);
        return;
    }
    LOG_DBG("Connected");
    ble_connection_status.set_connection_status();

    struct bt_conn_info info;
    err = bt_conn_get_info(conn, &info);
    if (err)
    {
        LOG_ERR("bt_conn_get_info() returned %d", err);
        return;
    }
    double connection_interval = info.le.interval * 1.25; // in ms
    uint16_t supervision_timeout = info.le.timeout * 10;  // in ms
    LOG_DBG("Connection parameters: interval %.2f ms, latency %d intervals, timeout %d ms", connection_interval, info.le.latency, supervision_timeout);

    int bond_cnt = 0;
    bt_foreach_bond(BT_ID_DEFAULT, print_bond_list, &bond_cnt);
    LOG_DBG("Number of bonds: %d/%d", bond_cnt, CONFIG_BT_MAX_PAIRED);
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_DBG("Disconnected. Reason %d", reason);
    ble_connection_status.clear_connection_status();
    k_work_submit(&advertise_work);
}

static void on_le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout)
{
    double connection_interval = interval * 1.25; // in ms
    uint16_t supervision_timeout = timeout * 10;  // in ms
    LOG_DBG("Connection parameters updated: interval %.2f ms, latency %d intervals, timeout %d ms", connection_interval, latency, supervision_timeout);
}

static void on_security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
    if (!err)
        LOG_DBG("Security changed: %s level %u", addr, level);
    else
        LOG_DBG("Security failed: %s level %u err %d", addr, level, err);
}

static void on_le_data_len_updated(struct bt_conn *conn, struct bt_conn_le_data_len_info *info)
{
    uint16_t tx_len = info->tx_max_len;
    uint16_t tx_time = info->tx_max_time;
    uint16_t rx_len = info->rx_max_len;
    uint16_t rx_time = info->rx_max_time;
    LOG_DBG("Data length updated. Length %d/%d bytes, time %d/%d us", tx_len, rx_len, tx_time, rx_time);
}

static void on_le_phy_updated(struct bt_conn *conn, struct bt_conn_le_phy_info *param)
{
    if (param->tx_phy == BT_CONN_LE_TX_POWER_PHY_1M)
    {
        LOG_DBG("PHY updated. New PHY: 1M");
    }
    else if (param->tx_phy == BT_CONN_LE_TX_POWER_PHY_2M)
    {
        LOG_DBG("PHY updated. New PHY: 2M");
    }
    else if (param->tx_phy == BT_CONN_LE_TX_POWER_PHY_CODED_S8)
    {
        LOG_DBG("PHY updated. New PHY: Long Range");
    }
}

struct bt_conn_cb connection_callbacks = {
    .connected = on_connected,
    .disconnected = on_disconnected,
    .le_param_updated = on_le_param_updated,
    .security_changed = on_security_changed,
    .le_phy_updated = on_le_phy_updated,
    .le_data_len_updated = on_le_data_len_updated,
};

void ble_thread_main(void)
{
    int err;

    bt_conn_auth_cb_register(&conn_auth_callbacks);
    bt_conn_auth_info_cb_register(&auth_cb_info);
    bt_conn_cb_register(&connection_callbacks);

    err = bt_enable(NULL);
    if (err)
        LOG_ERR("Bluetooth initialized failed");

    settings_load();

    // err = service_init(&service_callback);
    if (err)
        LOG_ERR("Failed to init Service (err:%d)", err);

    ble_connection_status.ble_remove_all_bonded_devices();

    k_work_submit(&advertise_work);

    while (1)
    {
        service_battery_level_notify(get_battery_percent());
        k_sleep(K_MSEC((int)BLE_INTERVAL_MS));
    }
}

K_THREAD_DEFINE(ble_thread, BLE_THREAD_STACK_SIZE,
                ble_thread_main, NULL, NULL, NULL,
                BLE_THREAD_PRIORITY, 0, BLE_INTERVAL_START_DELAY);