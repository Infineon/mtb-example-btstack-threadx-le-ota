/*******************************************************************************
 * File Name:   main.c
 *
 * Description: This code example showcases how to perform an LE over-the-air (OTA) 
 * update using the CYW955913EVK-01 Evaluation Kit. 
 *
 * Related Document: See README.md
 *
 *
 *******************************************************************************
 * Copyright 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 *******************************************************************************/

/*******************************************************************************
 * Header Files
 *******************************************************************************/
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "led_task.h"
#include "wiced_result.h"
#include "wiced_bt_stack.h"
#include "GeneratedSource/cycfg_gatt_db.h"
#include "GeneratedSource/cycfg_bt_settings.h"
#include "app_bt_gatt_handler.h"
/* OTA API */
#include "cy_ota_api.h"
#include "ota_context.h"
/* Storage specific */
#include "cy_ota_storage_api.h"
/*******************************************************************************
 * Macros
 *******************************************************************************/

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
__attribute__((aligned(8)))
uint8_t led_task_stack[LED_TASK_STACK_SIZE] = {0};

/**
 * @brief network parameters for OTA
 */
cy_ota_network_params_t ota_test_network_params = {CY_OTA_CONNECTION_UNKNOWN};

/**
 * @brief aAgent parameters for OTA
 */
cy_ota_agent_params_t ota_test_agent_params = {0};

/**
 * @brief Storage interface APIs for storage operations.
 */
cy_ota_storage_interface_t ota_interfaces =
    {
        .ota_file_open = cy_ota_storage_open,
        .ota_file_read = cy_ota_storage_read,
        .ota_file_write = cy_ota_storage_write,
        .ota_file_close = cy_ota_storage_close,
        .ota_file_verify = cy_ota_storage_verify,
        .ota_file_validate = cy_ota_storage_image_validate,
        .ota_file_get_app_info = cy_ota_storage_get_app_info};

/*******************************************************************************
 * Function Prototypes
 *******************************************************************************/

/*******************************************************************************
 * Function Definitions
 *******************************************************************************/

/*******************************************************************************
 * Function Name: main()
 ********************************************************************************
 * Summary:
 * This is the main function for CPU. It...
 *    1. Initializes the BSP
 *    2. Enables Global interrupt
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *
 *******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* default for all logging to WARNING */
    result = cy_log_init(CY_LOG_DEBUG, NULL, NULL);
    if (result != CY_RSLT_SUCCESS)
    {
        printf("\ncy_log_init failed with Error : [0x%X] \n", (unsigned int)result);
    }

    /* default for OTA logging to NOTICE */
    cy_ota_set_log_level(CY_LOG_NOTICE);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("\r===================================================="
           "===========\n");
    printf("TEST Application: OTA Update version: %d.%d.%d\n",
           APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_BUILD);
    printf("===============================================================\n\n");

    printf("Calling wiced_bt_stack_init\n");
    wiced_result_t wiced_result = WICED_BT_SUCCESS;
    /* Register call back and configuration with stack */
    wiced_result = wiced_bt_stack_init(app_bt_management_callback, &cy_bt_cfg_settings);
    if (WICED_BT_SUCCESS == wiced_result)
    {
        printf("Bluetooth(r) Stack Initialization Successful\n");
    }
    else
    {
        printf("Bluetooth(r) Stack Initialization failed!! wiced_result 0x%x\n", wiced_result);
    }

    result = cy_rtos_thread_create(&led_task_handle,
                                   &led_task,
                                   "led task",
                                   &led_task_stack,
                                   LED_TASK_STACK_SIZE,
                                   LED_TASK_PRIORITY,
                                   0);

    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

/*******************************************************************************
 * Function Name: init_ota()
 *******************************************************************************
 * Summary:
 *  Initialize and start the OTA update
 *
 * Parameters:
 *  pointer to Application context
 *
 * Return:
 *  cy_rslt_t
 *
 *******************************************************************************/
cy_rslt_t init_ota(ota_app_context_t *ota)
{
    cy_rslt_t result;

    if (ota == NULL || ota->tag != OTA_APP_TAG_VALID)
    {
        cy_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "init_ota() Failed - result: 0x%lx\n", CY_RSLT_OTA_ERROR_BADARG);
        return CY_RSLT_OTA_ERROR_BADARG;
    }

    memset(&ota_test_network_params, 0, sizeof(ota_test_network_params));
    memset(&ota_test_agent_params, 0, sizeof(ota_test_agent_params));

    /* Common Network Parameters */
    ota_test_network_params.initial_connection = ota->connection_type;

    /* The following fields are for MQTT and HTTP use (not Bluetooth�) */
    ota_test_network_params.use_get_job_flow = ota->update_flow;

    ota_test_agent_params.validate_after_reboot = 0;

    result = cy_ota_agent_start(&ota_test_network_params, &ota_test_agent_params, &ota_interfaces, &ota_app.ota_context);
    if (result != CY_RSLT_SUCCESS)
    {
        cy_log_msg(CYLF_MIDDLEWARE, CY_LOG_ERR, "cy_ota_agent_start() Failed - result: 0x%lx\n", result);
        while (true)
        {
            cy_rtos_delay_milliseconds(10);
        }
    }
    cy_log_msg(CYLF_MIDDLEWARE, CY_LOG_INFO, "cy_ota_agent_start() Result: 0x%lx\n", result);

    return result;
}

/* [] END OF FILE */
