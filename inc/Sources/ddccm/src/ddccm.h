/*******************************************************************************
 * FILE: ddccm.h
 *
 * DESCRIPTION:
 *
 *  DDC Card Manager header.
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

#ifndef _DDCCM_H_
#define _DDCCM_H_

/* DDCCM version #'s */
#define DDCCM_VER   3
#define DDCCM_REV   13
#define DDCCM_REL   6
#define DDCCM_BLD   1

#include "include/ddc_types.h"
#include "ddccm_os.h"

#define DDCCM_MAX_NUM_DEVS          8
#define DDCCM_MAX_DEVICES           32
#define DDCCM_MAX_LINE_BUFFER_LEN   256

#define MAX_DEV_1553                41      /* max number                       */
#define MAX_DEV_429                 7       /* of devices                       */
#define MAX_DEV_SYNCHRO             4       /* supported                        */
#define MAX_DEV_POWER               0       /* per family                       */
#define MAX_NUM_DEV                 32      /* max number of devices            */
#define MAX_DEV_NAME_LEN            32      /* device name string length        */
#define MAX_DRIVER_NAME_LEN         64      /* driver name string length        */
#define MAX_DRIVER_VERSION_LEN      32      /* driver version string length     */
#define MAX_CFG_FILE_NAM_LEN        64      /* config file string length        */
#define NOT_SET                     255     /* indicates device number not set  */


#define DDCCM_FLASH_TYPE__FIRMWARE                  0
#define DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE     1

/* TX Inhibit / BC Disable Items */
#define TX_INH_BC_DISABLE_MASK					    0x0100


/* device family */
enum
{
    FAMILY_MIL_STD_1553 = 0,
    FAMILY_ARINC_429,
#ifdef LINUX
    FAMILY_SYNCHRO,
    FAMILY_POWER,
#endif /* LINUX */
    MAX_NUM_FAMILY
};

/* This is the interface types of the various cards that DDC produces */
typedef enum
{
    DDC_BUS_TYPE_ISA = 0,
    DDC_BUS_TYPE_PCI,
    DDC_BUS_TYPE_USB,
    DDC_BUS_TYPE_PCMCIA,
    DDC_BUS_TYPE_CARDBUS,
    DDC_BUS_TYPE_VME,
    DDC_BUS_TYPE_VXI,
    DDC_BUS_TYPE_MXI,
    DDC_BUS_TYPE_FIBRE,
    DDC_BUS_TYPE_FIREWIRE,
    DDC_BUS_TYPE_RS232,
    DDC_BUS_TYPE_PARALLEL,
    DDC_BUS_TYPE_REMOTE

} DDC_BUS_TYPE;

/* card list element */
typedef struct _DDCCM_DEVICE_TYPE
{
    DDC_BUS_TYPE btBusType;
    U32BIT u32DevID;
    U32BIT u32Bus;
    U32BIT u32Slot;
    U32BIT u32Chan;
    U32BIT u32FamilyNumber;
    U8BIT u32LogDevNum;
    U8BIT u8NumChannels;
    BOOLEAN bLegacyDriver;
    BOOLEAN bFirmwareMismatch;
    BOOLEAN bEepMismatch;
    BOOLEAN bBackup;
    char szDevName[MAX_DEV_NAME_LEN];
    char szDriver[MAX_DRIVER_NAME_LEN];
    char szDiscreteDriver[MAX_DRIVER_NAME_LEN];
    char szDriverVersion[MAX_DRIVER_VERSION_LEN];
    U32BIT u32FirmwareVer;
    U32BIT u32InternalFirmwareVer;
    U32BIT u32RecFwVer;
    U32BIT u32RecEepVer;
    U32BIT u32DataArchiveNumber;
    U32BIT u32Capabilities;

    /* remote information */
    U32BIT u32RemoteDevice;
    U32BIT u32RemoteLDN;
    U32BIT u32IpAddr;
    char szIpAddr[20];

    FLASH_INFO      sFlashInfo;
    ENHANCED_CAPABILITY_INFO sEnhancedCardCapabilities;

} DDCCM_DEVICE_TYPE;


/* contains all necessary information about a specific device */
typedef struct _DEVICE_INFORMATION_ENTRY_TYPE
{
    char szDriver[MAX_DRIVER_NAME_LEN];
    DEV_INFO sDevInfo;
    DDC_HANDLE hDriver;             /* handle to the driver */
    U32BIT u32StartAddress;         /* flash start address */
    U32BIT u32SectorCount;          /* flash sector count */
    U32BIT u32MinSectorSize;        /* flash sector size */
    U32BIT u32DataArchiveNumber;    /* data archive number */
    ENHANCED_CAPABILITY_INFO sEnhancedCardCapabilities;          /* capabilities of the device */
    HWVERSIONINFO   sHwVersionInfo; /* hardware version information */
    BOOLEAN bBackup;                /* indicates the device booting from backup flash */
    BOOLEAN bIsFlashable;           /* indicates this device can be reflashed */
    BOOLEAN bValidatedDevice;       /* indicates this device was found in the lookup table */
    BOOLEAN bFirmwareMismatch;      /* indicates the firmware on the device does not match the version found in the lookup table */
    BOOLEAN bEepMismatch;           /* indicates the EEPROM on the device does not match the version found in the lookup table */
    BOOLEAN bIsTxInhibitBcDisableCapable; /* indicates this device is Tx Inhibit BC Disable capable */
    U32BIT u32RecFwVer;             /* recommended firmware version xxyy */
    U32BIT u32RecEepVer;            /* recommended EEPROM version xxyy */

} DEVICE_INFORMATION_ENTRY_TYPE;


/* list of devices */
typedef struct _DEVICE_INFORMATION_TYPE
{
    U16BIT u16NumberOfDevices;                              /* number of devices found */
    DEVICE_INFORMATION_ENTRY_TYPE entry[DDCCM_MAX_DEVICES];       /* device information list */

} DEVICE_INFORMATION_TYPE;



/* card information */
typedef struct _CARD_INFO_TYPE
{
    U32BIT u32DevID;
    DDC_BUS_TYPE btBusType;
    U8BIT u8NumChans;
    char  szDevName[MAX_DEV_NAME_LEN];

} CARD_INFO_TYPE;


/* device list element on linux machine */
typedef struct _REMOTE_DEV_INFO
{
    char szDevName[20];
    U32BIT u32DevID;
    U32BIT u32Chan;
    U32BIT u32BusType;
    U32BIT u32FamilyNum;      /* EMACE , E2MA or ACEX */
    U8BIT u32LogDevNum;
    U32BIT u32ModelNum;

} REMOTE_DEV_INFO;


/* product family information */
typedef struct _FAMILY_INFO_TYPE
{
    unsigned char  u8MaxDevTypes;
    unsigned char  u8LogNumBase;
    unsigned char  u8LogNumMax;
    char szCfgFileName[MAX_CFG_FILE_NAM_LEN];

} FAMILY_INFO_TYPE;


#ifndef LINUX
/* exported functions to 1553/429 library */
extern void ddccmGet1553DevInfo
(
    S16BIT DevNum,
    U16BIT *u16NumLogicalDevs,
    DDCCM_DEVICE_TABLE *sLogicalDeviceTable
);

extern void ddccmGet429DevInfo
(
    S16BIT DevNum,
    U16BIT *u16NumLogicalDevs,
    DDCCM_DEVICE_TABLE *sLogicalDeviceTable
);

#endif /* LINUX not defined */

#endif /* _DDCCM_H_ */

