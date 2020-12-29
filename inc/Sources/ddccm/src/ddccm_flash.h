/*******************************************************************************
 * FILE: ddc_flash.h
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

#ifndef _DDC_FLASH_H_
#define _DDC_FLASH_H_

#include "ddccm.h"
#include "ddccm_os.h"
#include "include/ddc_ioctl.h"

/* ========================================================================== */
/*                                DATA TYPES */
/* ========================================================================== */

/* function return values */
#define FLASH_SUCCESS                           0
#define FLASH_DEVICE_ERROR                      -10
#define FLASH_DEVICE_ABORT                      -11
#define FLASH_ERROR_FILE_OPEN                   -21
#define FLASH_ERROR_FIND_DEVICES                -22
#define FLASH_ERROR_PARSE_FILE                  -23
#define FLASH_ERROR_PARAMETER                   -24
#define FLASH_ERROR_ABORT                       -25
#define FLASH_ERROR_VERIFY_MISMATCH_DA_NUMBER   -30
#define FLASH_ERROR_VERIFY_MISMATCH_DEVICE_ID   -31
#define FLASH_ERROR_VERIFY_UNKNOWN_DA_NUMBER    -32
#define FLASH_ERROR_VERIFY_FILENAME             -33
#define FLASH_ERROR_VERIFY_FILE_SIZE            -34
#define FLASH_ERROR_VERIFY_FILE_HEADER          -35
#define FLASH_ERROR_START_ADDRESS				-36
#define FLASH_ERROR_CAPABILTY_RETRIEVAL			-37
#define FLASH_ERROR_CAPABILTY_NOT_SUPPORTED     -38
#define FLASH_ERROR_REG_READ_ERROR              -39
#define FLASH_ERROR_UNKNOWN                     -1000

/* limits */
#define MAX_LOOKUP_TABLE_ENTRIES                256
#define MAX_DEVICE_NAME_LEN                     32
#define MIN_BIN_FILE_NAME_LENGTH                15


#define FLASH_ERASE_VALUE                       0xFF


/* ========================================================================== */

/* lookup table file structure */
typedef struct _LOOKUP_TABLE_ENTRY_TYPE
{
    /* the following are int's because they are used with sscanf */
    int     nDataArchiveNumber;                 /* 1 - Data Archive Number */
    int     nDeviceID;                          /* 2 - Device ID */
    int     nModel;                             /* 3 - Model */
    int     nFileSize;                          /* 4 - bin file size in bytes */
    int     nRecFwVer;                          /* 5 - recommended Firmware ver */
    int     nRecEepVer;                         /* 6 - recommended EEPROM ver */
    char    szDeviceName[MAX_DEVICE_NAME_LEN];  /* 7 - Device Name String */

} LOOKUP_TABLE_ENTRY_TYPE;


/* represents the number of items in the LOOKUP_TABLE_TYPE */
/* structure that will be parsed from the file */
#define LOOKUP_TABLE_ENTRY_NUMBER_ELEMENTS      7


/* lookup table structure */
typedef struct _LOOKUP_TABLE_TYPE
{
    U16BIT u16NumberOfLookupTableEntries;                       /* number of entries in the lookupTable list */
    LOOKUP_TABLE_ENTRY_TYPE entry[MAX_LOOKUP_TABLE_ENTRIES];    /* lookup table list */

} LOOKUP_TABLE_TYPE;



/* ========================================================================== */
/*                            FUNCTION PROTOTYPES */
/* ========================================================================== */
extern S16BIT ParseLookupTable
(
    LOOKUP_TABLE_TYPE *pLookupTable
);

extern void MatchDevices
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo, 
    LOOKUP_TABLE_TYPE *pLookupTable
);

S16BIT TxInhibitBcDisable_GetCapability
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT *pu32DeviceCapability
);

extern S16BIT FlashDevice
(
    DEVICE_INFORMATION_ENTRY_TYPE *pDeviceInfoEntry, 
    LOOKUP_TABLE_TYPE *pLookupTable,
    U32BIT u32FlashOperationType
);

extern DDCCM_ERROR_TYPE ddccmGetDevInfo
(
    int hDev,
    DEV_INFO *pDevInfo
);

extern DDCCM_ERROR_TYPE ddccmGetFlashInfo
(
    int hDev, 
    FLASH_INFO *pFlashInfo
);

extern DDCCM_ERROR_TYPE ddccmGetHwVersionInfo
(
    int hDev,
    PHWVERSIONINFO pHwVersionInfo,
    unsigned char bFamily
);

extern DDCCM_ERROR_TYPE ddccmGetEnhancedCapabilities
(
    int hDev, 
    PENHANCED_CAPABILITY_INFO pEnhancedCardCapabilities
);

#endif /* _DDC_FLASH_H_ */
