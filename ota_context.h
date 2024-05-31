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

/******************************************************************************
 * File Name:   ota_context.h
 *
 * Description: Definitions and data structures for the OTA example application
 *
 * Related Document: See Readme.md
 *
 *******************************************************************************/

#ifndef OTA_CONTEXT_H_
#define OTA_CONTEXT_H_

#include "cy_ota_api.h"

#ifdef COMPONENT_OTA_BLUETOOTH
#include "wiced_bt_types.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_ble.h"
#ifdef COMPONENT_OTA_BLUETOOTH_SECURE
#include "ota_ecc_pp.h"
#ifndef COMPONENT_H1_CP
#include "sha256.h"
#endif /* COMPONENT_H1_CP */
#endif /* COMPONENT_OTA_BLUETOOTH_SECURE */
#endif /* COMPONENT_OTA_BLUETOOTH */

#ifdef USE_EEPROM_TO_STORE_BOND_INFO
#include "cy_em_eeprom.h"
#endif

/******************************************************
 *                     Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define CY_OTA_BLE_TEMP_BUFFER_SIZE (512)

#define OTA_APP_TAG_VALID (0x51EDBA15)
#define OTA_APP_TAG_INVALID (0xDEADBEEF)

/* OTA response buffer size */
#define CY_OTA_BT_RSP_BUFFER_SIZE (64)

/******************************************************
 *                   Enumerations
 ******************************************************/

#ifdef USE_EEPROM_TO_STORE_BOND_INFO
/* EEPROM Configuration details. */
#define EEPROM_SIZE (sizeof(bondinfo_t))
#define SIMPLE_MODE (0u)
#define WEAR_LEVELLING_FACTOR (2u)
#define REDUNDANT_COPY (1u)
#define BLOCKING_WRITE (1u)
#endif /* USE_EEPROM_TO_STORE_BOND_INFO */

/* Logical Start of Emulated EEPROM and location of structure elements. */
#define LOGICAL_EEPROM_START (0u)
#define EEPROM_SLOT_DATA (LOGICAL_EEPROM_START)
#define EEPROM_IDENTITY_KEYS_START (EEPROM_SLOT_DATA + sizeof(bondinfo.slot_data))
#define EEPROM_LINK_KEYS_START (EEPROM_IDENTITY_KEYS_START + sizeof(wiced_bt_local_identity_keys_t))
#define GET_ADDR_FOR_DEVICE_KEYS(x) (EEPROM_LINK_KEYS_START + (x * sizeof(wiced_bt_device_link_keys_t)))

/* enum for slot_data structure */
enum
{
    NUM_BONDED,
    NEXT_FREE
};

/* Macro for Number of devices allowed to bond */
#define BOND_MAX 4u

/* Structure to store info that goes into EEPROM - it holds the number of bonded devices, remote keys and local keys */
#pragma pack(1)
typedef struct bondinfo_s
{
    uint16_t slot_data[2];
#ifdef COMPONENT_OTA_BLUETOOTH
    wiced_bt_device_link_keys_t link_keys[BOND_MAX];
    wiced_bt_local_identity_keys_t identity_keys;
#endif
} bondinfo_t;
#pragma pack()

/******************************************************
 *            OTA example app type definitions
 ******************************************************/

typedef struct
{
    uint32_t tag;

    cy_ota_context_ptr ota_context;
    cy_ota_connection_t connection_type;

#ifdef COMPONENT_OTA_BLUETOOTH
#ifdef USE_EEPROM_TO_STORE_BOND_INFO
    /* eeprom context info */
    cy_stc_eeprom_context_t Em_EEPROM_context;
#endif

    uint16_t bt_conn_id;                       /* Host Bluetooth® Connection ID */
    uint8_t bt_peer_addr[BD_ADDR_LEN];         /* Host Bluetooth® address */
    wiced_bt_ble_conn_params_t bt_conn_params; /* Bluetooth® connection parameters */
    uint16_t bt_config_descriptor;             /* Bluetooth® configuration to determine if Device sends Notification/Indication */
#endif

    /* function / document replacement info - these variables are for the command console for setting up
     * MQTT & HTTP - not necessary for example app creation
     */
    uint8_t connected; /* 0 = not connected, 1 = connected to AP */

#if defined(COMPONENT_OTA_HTTP) || defined(COMPONENT_OTA_MQTT)

#if defined(CYBSP_WIFI_CAPABLE)
    /* AP connection info */
    cy_wcm_connect_params_t wifi_conn_param;
    cy_wcm_ip_address_t ip_address;
#endif

#if defined(CYBSP_ETHERNET_CAPABLE)
    cy_ecm_ip_address_t ip_address;
    cy_ecm_interface_t eth_idx;
#endif

#endif

#ifdef COMPONENT_OTA_MQTT
    /* Which MQTT service are we using */
    OTA_MQTT_SERVICE_T mqtt_service;

    /* Custom MQTT Broker connection */
    char MQTT_broker[OTA_MAX_SERVER_NAME_LEN + 1];
    uint32_t MQTT_port;

    /* Which MQTT certificates are we using */
    OTA_MQTT_SERVICE_T mqtt_certificates;
#endif

#ifdef COMPONENT_OTA_HTTP
    /* Initial HTTP Server connection */
    char HTTP_server[OTA_MAX_SERVER_NAME_LEN + 1];
    uint32_t HTTP_port;
#endif

    /* start OTA transaction using TLS */
    uint8_t start_TLS; /* 0 = non-TLS, 1 = TLS */

    /* Use Job flow */
    cy_ota_update_flow_t update_flow;

    /* Send Result  */
    uint8_t do_not_send_result; /* 0 = send result, 1 = DO NOT send result */

    /* Reboot when OTA is complete */
    uint8_t reboot_at_end; /* 0 = do NOT reboot, 1 = reboot */

    /* Callback replacement settings */
    uint8_t callback_replacement[CY_OTA_NUM_STATES]; /* if 1, replace callback */
    cy_ota_callback_results_t callback_settings[CY_OTA_NUM_STATES][CY_OTA_LAST_REASON];

    uint8_t replace_job_request;     /* 1 = replace outgoing JSON doc with bad doc           */
    uint8_t replace_job_con_dis;     /* 1 = replace Job connect & disconnect functions       */
    uint8_t replace_job_get_doc;     /* 1 = replace Job get document function                */
    uint8_t replace_job_parse;       /* 1 = replace Job Parse function                       */
    uint8_t replace_job_redirect;    /* 1 = replace Job redirect function                    */
    uint8_t replace_storage;         /* 1 = replace Storage open / write / close functions   */
    uint8_t replace_data_con_dis;    /* 1 = replace Data connect & disconnect functions      */
    uint8_t replace_data_get;        /* 1 = replace Data Get function                        */
    uint8_t replace_verify;          /* 1 = replace verify downloaded data function          */
    uint8_t replace_result_redirect; /* 1 = replace result redirect function                 */
    uint8_t replace_result_con_dis;  /* 1 = replace result connect & disconnect functions    */
    uint8_t replace_result_send;     /* 1 = replace result send function                     */
    uint8_t replace_result_response; /* 1 = replace result get response function             */

    uint32_t blinky_led_state; /* 0 = off, 1 = on blinky LED state     */

} ota_app_context_t;

/******************************************************
 *               Variable Definitions
 ******************************************************/

extern ota_app_context_t ota_app;

/******************************************************
 *               Function Declarations
 ******************************************************/

cy_rslt_t init_ota(ota_app_context_t *ota);

void ota_initialize_default_values(void);

#endif /* #define OTA_CONTEXT_H_ */
