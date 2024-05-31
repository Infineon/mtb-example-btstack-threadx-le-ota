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
 * Description: This file consists of the function prototypes that are
 *              necessary for developing the Bluetooth® applications with GATT Server
 *              callbacks.
 *
 */

#ifndef __APP_BT_GATT_HANDLER_H__
#define __APP_BT_GATT_HANDLER_H__

#ifdef COMPONENT_OTA_BLUETOOTH

/* *****************************************************************************
 *                              INCLUDES
 * ****************************************************************************/
#include "wiced_bt_gatt.h"
#include "cybsp_types.h"
#include "GeneratedSource/cycfg_pins.h"

/* *****************************************************************************
 *                              FUNCTION DECLARATIONS
 * ****************************************************************************/
#ifndef CY_BT_MTU_SIZE
#define CY_BT_MTU_SIZE CY_BT_RX_PDU_SIZE
#endif

wiced_result_t app_bt_management_callback(wiced_bt_management_evt_t event,
                                          wiced_bt_management_evt_data_t *p_event_data);

cy_rslt_t cy_ota_ble_check_build_vs_configurator( void );

void ota_initialize_default_values(void);

#endif      /* COMPONENT_OTA_BLUETOOTH */

#endif      /* __APP_BT_GATT_HANDLER_H__ */

/* [] END OF FILE */
