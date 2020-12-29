/*******************************************************************************
 * FILE: ddc_udl_driver.c
 *
 * DESCRIPTION:
 *
 *  This file provides driver specific communication routines
 *
 ******************************************************************************
 * Non-Disclosure Statement
 * ------------------------
 * This software is the sole property of Data Device Corporation.  All
 * rights, title, ownership, or other interests in the software remain
 * the property of Data Device Corporation.  This software may be used
 * in accordance with applicable licenses.  Any unauthorized use,
 * duplication, transmission, distribution, or disclosure is expressly
 * forbidden.
 *
 * This non-disclosure statement may not be removed or modified without
 * prior written consent of Data Device Corporation.
 ******************************************************************************
 * Data Device Corporation
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (631) 567-5600
 *
 * Copyright (c) 2014 by Data Device Corporation
 * All Rights Reserved.
 *****************************************************************************/

#include "os/include/ddc_os_types.h"
#include "include/ddc_error_list.h"
#include "include/ddc_device_ids.h"
#include "core/1553/ddc_udl_1553_private.h"
#include "core/1553/ddc_udl_1553_bc_private.h"
#include "core/1553/ddc_udl_1553_mt_private.h"
#include "core/1553/ddc_udl_1553_rt_private.h"
#include "driver_sdk/ddc_udl_private.h"
#include "driver_sdk/ddc_udl_bd_private.h"
#include "driver_sdk/ddc_udl_driver_private.h"
#include "driver_sdk/ddc_udl_hardware_private.h"
#include "driver_sdk/ddc_udl_iodev_private.h"
#include "driver_sdk/ddc_udl_um_private.h"
#include "driver_sdk/debug/ddc_udl_debug_private.h"
#include "os/interface/ddc_udl_os_interrupt_private.h"
#include "os/include/ddc_os_types_private.h"
#include "bus/ddc_udl_bus_private.h"



MODULE_LICENSE("GPL");



/*******************************************************************************
 * Name:    ddcUdlOsInit
 *
 * Description:
 *      This function should be called from the main driver initialization
 *      routine.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
S16BIT ddcUdlOsInit
(
    void
)
{
    int result;

#if 0 /* DEBUG */
{
    int i;

    /* set all trace levels for all modules */
    for (i=0; i<DDC_DBG_MODULE_MAX; i++)
    {
        ddcUdlDebugSetTraceLevel(i, DDC_DBG_ALL);
    }

    /* clear out individual modules */
    ddcUdlDebugClearTraceLevel(DDC_DBG_MODULE_BUS, DDC_DBG_ALL);
    ddcUdlDebugClearTraceLevel(DDC_DBG_MODULE_MT, DDC_DBG_MT_CH10_DATA_PKT);
    ddcUdlDebugClearTraceLevel(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_CntrlRoutines);
    ddcUdlDebugClearTraceLevel(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1);
}
#endif /* DEBUG */

    DDC_DBG_PRINT(DDC_DBG_MODULE_DRIVER, DDC_DBG_DRIVER_DRIVER_ENTRY, "ENTER\n");

    ddcUdlInitDeviceCount();

    result = ddcUdlBusInit();

    if (result != DDC_UDL_ERROR__SUCCESS)
    {
        DDC_DBG_PRINT_INFO("ERROR: DDC ACEX driver NOT installed\n");

        return -ENODEV;
    }

    DDC_DBG_PRINT_INFO("Driver installed, %d boards found\n", ddcUdlGetDeviceCount());

    return result;
}

/*******************************************************************************
 * Name:    ddcUdlOsExit
 *
 * Description:
 *      This function should be called from the main driver exit routine.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
void ddcUdlOsExit
(
    void
)
{
    ddcUdlOsBusExit();
}
