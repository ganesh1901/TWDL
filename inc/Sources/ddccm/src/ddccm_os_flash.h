/*******************************************************************************
 * FILE: ddccm_os_flash.h
 *
 * DESCRIPTION:
 *
 *  The purpose of this module is define the data type and structures used
 *  for flashing.
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

#ifndef _DDCCM_OS_FLASH_H_
#define _DDCCM_OS_FLASH_H_

#include "ddccm.h"
#include "include/ddc_types.h"

/* ========================================================================== */
/*                            FUNCTION PROTOTYPES                             */
/* ========================================================================== */

extern BOOLEAN ddccmDeviceIoControl
(
    DDC_HANDLE hDriver,
    U32BIT u32IoCtlCode,
    DDC_IOCTL_PARAMS *pIoCtlParams,
    U32BIT u32sIoCtlParamsSize,
    VOID* pOutBuffer,
    U32BIT u32OutBufferSize,
    U32BIT *pu32BytesReturned,
    VOID* pOpt
);

extern S32BIT ddccmOsFindDevices
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo,
    BOOL bFlashableOnly
);

extern void ddccmOsFreeDevices
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo
);

extern const char *GetFlashLookupTableFilename
(
    void
);


#endif /* _DDCCM_OS_FLASH_H_ */
