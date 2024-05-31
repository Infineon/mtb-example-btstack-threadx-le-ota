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
 * This file contains Bluetooth® configuration settings for platform
 * TODO: This should be a platform-specific file.
 *
 */

#ifdef COMPONENT_OTA_BLUETOOTH

#include "app_bt_cfg.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"

#ifndef COMPONENT_H1_CP
/* Configure UART for Bluetooth® HCI control */
const cybt_platform_config_t app_bt_platform_cfg_settings =
{
    .hci_config =
    {
#if ( (defined (CYW20829A0LKML) || defined (CYW20829B0LKML) || defined (CYW89829B01MKSBG)) || (defined (BLESS_PORTING_LAYER)) )
        .hci_transport = CYBT_HCI_IPC,
#else
        .hci_transport = CYBT_HCI_UART,

        .hci =
        {
            .hci_uart =
            {
                .uart_tx_pin = CYBSP_BT_UART_TX,
                .uart_rx_pin = CYBSP_BT_UART_RX,
                .uart_rts_pin = CYBSP_BT_UART_RTS,
                .uart_cts_pin = CYBSP_BT_UART_CTS,

                .baud_rate_for_fw_download = 115200,
                .baud_rate_for_feature     = 115200,

                .data_bits = 8,
                .stop_bits = 1,
                .parity = CYHAL_UART_PARITY_NONE,
                .flow_control = WICED_TRUE
            }
        }
#endif
    },

#if ((defined (CYW20829A0LKML) || defined (CYW20829B0LKML) || defined (CYW89829B01MKSBG)) || (defined (BLESS_PORTING_LAYER)))
    .controller_config =
    {
        .bt_power_pin      = NC,
        #if defined(CY_CFG_PWR_SYS_IDLE_MODE) && \
                   ((CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_SLEEP) || \
                   (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_DEEPSLEEP))
        .sleep_mode = { .sleep_mode_enabled = 1 },
        #else
        .sleep_mode = { .sleep_mode_enabled = 0 },
        #endif
    },
#else
    .controller_config =
    {
        .bt_power_pin      = CYBSP_BT_POWER,
        .sleep_mode =
        {
            .sleep_mode_enabled   = true,
            .device_wakeup_pin    = CYBSP_BT_DEVICE_WAKE,
            .host_wakeup_pin      = CYBSP_BT_HOST_WAKE,
            .device_wake_polarity = CYBT_WAKE_ACTIVE_LOW,
            .host_wake_polarity   = CYBT_WAKE_ACTIVE_LOW
        }
    },
#endif

    .task_mem_pool_size    = 2048
};
#endif /* COMPONENT_H1_CP */
#endif /* COMPONENT_OTA_BLUETOOTH */

/* [] END OF FILE */
