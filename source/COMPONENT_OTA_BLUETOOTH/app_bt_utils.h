/*
 * Copyright 2024, Cypress Semiconductor Corporation (an Infineon company)
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Description: This file consists of the utility function declarations that will
 *              help debugging and developing the applications easier with much more
 *              meaningful information.
 */

#ifndef __APP_BT_UTILS_H__
#define __APP_BT_UTILS_H__

#ifdef COMPONENT_OTA_BLUETOOTH

/*******************************************************************************
 *                                INCLUDES
 ******************************************************************************/
#include "wiced_bt_dev.h"
#include "wiced_bt_gatt.h"

/*******************************************************************************
 *                                Constants
 ******************************************************************************/
#define CASE_RETURN_STR(const)          case const: return #const;

#define FROM_BIT16_TO_8(val)            ( (uint8_t)( ( (val) >> 8 ) & 0xff) )

#define BT_ADDR_FORMAT                  "%02X:%02X:%02X:%02X:%02X:%02X"

/*******************************************************************************
 *                              FUNCTION DECLARATIONS
 ******************************************************************************/
const char *app_get_bt_event_name(wiced_bt_management_evt_t event);

const char *app_get_bt_advert_mode_name(wiced_bt_ble_advert_mode_t mode);

const char *app_get_gatt_disconn_reason_name(wiced_bt_gatt_disconn_reason_t reason);

const char *app_get_gatt_status_name(wiced_bt_gatt_status_t status);

const char *get_bt_smp_status_name(wiced_bt_smp_status_t status);

#endif      /* COMPONENT_OTA_BLUETOOTH   */


#endif      /* __APP_BT_UTIS_H__ */

/* [] END OF FILE */
