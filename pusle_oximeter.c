/** @file
 *  @brief HRS Service sample
 */

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr.h>
#include <init.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#define LOG_LEVEL CONFIG_BT_PLX_LOG_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(plx);

#define GATT_PERM_READ_MASK     (BT_GATT_PERM_READ | \
				 BT_GATT_PERM_READ_ENCRYPT | \
				 BT_GATT_PERM_READ_AUTHEN)
#define GATT_PERM_WRITE_MASK    (BT_GATT_PERM_WRITE | \
				 BT_GATT_PERM_WRITE_ENCRYPT | \
				 BT_GATT_PERM_WRITE_AUTHEN)

#ifndef CONFIG_BT_PLX_DEFAULT_PERM_RW_AUTHEN
#define CONFIG_BT_PLX_DEFAULT_PERM_RW_AUTHEN 0
#endif
#ifndef CONFIG_BT_PLX_DEFAULT_PERM_RW_ENCRYPT
#define CONFIG_BT_PLX_DEFAULT_PERM_RW_ENCRYPT 0
#endif
#ifndef CONFIG_BT_PLX_DEFAULT_PERM_RW
#define CONFIG_BT_PLX_DEFAULT_PERM_RW 0
#endif

#define PLX_GATT_PERM_DEFAULT (						\
	CONFIG_BT_PLX_DEFAULT_PERM_RW_AUTHEN ?				\
	(BT_GATT_PERM_READ_AUTHEN | BT_GATT_PERM_WRITE_AUTHEN) :	\
	CONFIG_BT_PLX_DEFAULT_PERM_RW_ENCRYPT ?				\
	(BT_GATT_PERM_READ_ENCRYPT | BT_GATT_PERM_WRITE_ENCRYPT) :	\
	(BT_GATT_PERM_READ | BT_GATT_PERM_WRITE))			\

static uint8_t plx_blsc;
uint8_t sp02 = 22;

static void plxmc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("PLX notifications %s", notif_enabled ? "enabled" : "disabled");
}

static ssize_t read_blsc(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			 void *buf, uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &plx_blsc,
				 sizeof(plx_blsc));
}

/* Heart Rate Service Declaration */
BT_GATT_SERVICE_DEFINE(plx_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_PLX),
	BT_GATT_CHARACTERISTIC(BT_UUID_PLX_FEATURES, BT_GATT_CHRC_READ,
			       BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CHARACTERISTIC(BT_UUID_PLX_SPOT_CHECK_MEASUREMENT, BT_GATT_CHRC_INDICATE,
			       BT_GATT_PERM_NONE, NULL, NULL,NULL),
    BT_GATT_CCC(plxmc_ccc_cfg_changed,
		    PLX_GATT_PERM_DEFAULT),

	BT_GATT_CHARACTERISTIC(BT_UUID_PLX_CONTINUOUS_MEASUREMENT, BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(plxmc_ccc_cfg_changed,
		    PLX_GATT_PERM_DEFAULT),

	
    
	/*
	BT_GATT_CHARACTERISTIC(BT_UUID_HRS_BODY_SENSOR, BT_GATT_CHRC_READ,
			       HRS_GATT_PERM_DEFAULT & GATT_PERM_READ_MASK,
			       read_blsc, NULL, NULL),
	BT_GATT_CHARACTERISTIC(BT_UUID_HRS_CONTROL_POINT, BT_GATT_CHRC_WRITE,
			       HRS_GATT_PERM_DEFAULT & GATT_PERM_WRITE_MASK,
			       NULL, NULL, NULL),*/
);

static int plx_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	plx_blsc = 0x01;

	return 0;
}

int bt_plx_notify(uint16_t sp02)
{
	int rc;
	static uint8_t hrm[2];

	hrm[0] = 0x06; /* uint8, sensor contact */
	hrm[1] = sp02;

	rc = bt_gatt_notify(NULL, &plx_svc.attrs[1], &hrm, sizeof(hrm));

	return rc == -ENOTCONN ? 0 : rc;
}

SYS_INIT(plx_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
