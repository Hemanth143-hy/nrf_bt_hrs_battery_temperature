/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLX_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_PLX_H_

/**
 * @brief pusle oximeter service 
 * @defgroup bt_pusle oximeter service (PLX)
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */





#ifdef __cplusplus
extern "C" {
#endif

/** @brief Notify sp02 measurement.
 *
 * This will send a GATT notification to all current subscribers.
 *
 *  @param HR & SP02 AND  The heartrate measurement in beats per minute.
 *
 *  @return Zero in case of success and error code in case of error.
 */
int bt_plx_notify(uint16_t sp02);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_HRS_H_ */
