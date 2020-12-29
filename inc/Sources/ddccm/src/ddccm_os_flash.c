/*******************************************************************************
 * FILE: ddccm_os.h
 *
 * DESCRIPTION:
 *
 *  The purpose of this module is to abstract out OS function calls.
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>      /* for open(...)    */
#include <unistd.h>     /* for close(...)   */
#include <sys/ioctl.h>  /* ioctl            */
#include "include/ddc_types.h"
#include "include/ddc_ioctl.h"
#include "ddccm.h"
#include "ddccm_flash.h"
#include "ddccm_os.h"
#include "ddccm_os_flash.h"
#include "strtable.h"


/*============================================================================*/
/*                             LOCAL VARIABLES                                */
/*============================================================================*/
static const char szLookupTableFilenamePath[256] = "/usr/lib/ddc/flash.ddc";

/*******************************************************************************
 * NAME:    ddccmDeviceIoControl
 *
 * DESCRIPTION:
 *
 *      This function converts generic IOCTL call to Linux ioctl.
 *
 * In   hDriver             Handle to the driver
 * In   u32IoCtlCode        IOCTL command
 * In   pIoCtlParams        Pointer to an IOCTL command structure
 * In   u32sIoCtlParamsSize Size of pIoCtlParams in bytes
 * In   pOutBuffer          Output buffer to return data
 * In   u32OutBufferSize    Size of pOutBuffer in bytes
 * In   pu32BytesReturned   Number of bytes returned in pOutBuffer
 * In   pOpt                Not used
 *
 * Out  TRUE=success, FALSE=fail
 ******************************************************************************/
BOOLEAN ddccmDeviceIoControl
(
    DDC_HANDLE hDriver,
    U32BIT u32IoCtlCode,
    DDC_IOCTL_PARAMS *pIoCtlParams,
    U32BIT u32sIoCtlParamsSize,
    VOID* pOutBuffer,
    U32BIT u32OutBufferSize,
    U32BIT *pu32BytesReturned,
    VOID* pOpt
)
{
    DDC_IOCTL_PARAMS sIoCtlParams;

    /* unused parameter */
    DDC_UNREFERENCED_PARAMETER(pOpt);

   /* Clear out possible stale errno to 0 */
    errno = 0;

    if (pIoCtlParams == NULL)
    {
        /* use a local IOCTL to pass info to/from the driver since one was not supplied */
        pIoCtlParams = &sIoCtlParams;
    }

    pIoCtlParams->sIoctlInfo.u32InputBufferLength = u32sIoCtlParamsSize;
    pIoCtlParams->sIoctlInfo.u32OutBufferSize = u32OutBufferSize;
    pIoCtlParams->sIoctlInfo.pOutBuffer = (unsigned long)pOutBuffer;
    pIoCtlParams->sIoctlInfo.pu32BytesReturned = (unsigned long)pu32BytesReturned;

    pIoCtlParams->sIoctlInfo.status = ioctl(hDriver, u32IoCtlCode, pIoCtlParams);

    if (pIoCtlParams->sIoctlInfo.status)
    {
        if (pIoCtlParams->sIoctlInfo.status == -1)
        {
            pIoCtlParams->sIoctlInfo.status = -errno;
        }
        return 0;
    }

    return 1;
}


/*******************************************************************************
 * Name:    ddccmOsFindDevices
 *
 * Description:
 *      This function searches for all DDC devices and fill in a global
 *      information data structure.
 *
 *      Modifies global variables: deviceInfo, nDevices
 *
 * In   pDeviceInfo     pointer to the head of the device info list
 * In   bFlashableOnly  Store only flashable devices or all
 * Out  pDeviceInfo     adds devices found
 *
 * Returns: 0 = success, otherwise error value
 ******************************************************************************/
S32BIT ddccmOsFindDevices
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo,
    BOOL bFlashableOnly
)
{
    FILE            *fpDriverListFile;
    DEV_INFO        sDevInfo;
    FLASH_INFO      sFlashInfo;
    U16BIT u16NumberOfDevices = 0;
    int             nModifyPos;
    int             status = 0;
    unsigned int    i;
    char            szDriverListFilename[32] = IDS_DRIVER_LIST_FILENAME;
    char            chCardRef;
    char            chCardRefMin;
    char            chCardRefMax;
    char            sDevFile[DDCCM_MAX_LINE_BUFFER_LEN];

    unsigned char   bFamily = 0;
    int             E;

    memset((void *)pDeviceInfo, 0, sizeof(DEVICE_INFORMATION_TYPE));

    /* open the driver file list */
    fpDriverListFile = fopen(szDriverListFilename, "r");

    if (fpDriverListFile == NULL)
    {
        /* error opening file (not found?) */
        return FLASH_ERROR_FILE_OPEN;
    }

    /* read a line at a time until we reach the end of the file */
    while (fgets(sDevFile, sizeof(sDevFile), fpDriverListFile) != NULL)
    {
        /* strip trailing newline */
        for (i = 0; i < strlen(sDevFile); i++)
        {
            if ((sDevFile[i] == '\n') || (sDevFile[i] == '\r'))
            {
                sDevFile[i] = '\0';
            }
        }

        if (strstr(sDevFile, "usb") != NULL)
        {
            /* format is /dev/#######dx             */
            sprintf(sDevFile, "%sdx", sDevFile);

            /* loop through device 1, 3, 5, etc...  */
            chCardRefMin = '0';
            chCardRefMax = '0' + DDCCM_MAX_NUM_DEVS;
            nModifyPos = strlen(sDevFile) - 1;
        }
        else
        {
            /* format is /dev/#######x1             */
            sprintf(sDevFile, "%sx1", sDevFile);

            /* loop through device a, b, c, etc...  */
            chCardRefMin = 'a';
            chCardRefMax = 'i'; /* up to h */
            nModifyPos = strlen(sDevFile) - 2;
        }

        /* loop for all possible devices */
        for (chCardRef = chCardRefMin; chCardRef < chCardRefMax; chCardRef++)
        {
            sDevFile[nModifyPos] = chCardRef;

            /* Attempt to open the device.  If it fails to open, it's probably a non-existant card */
            pDeviceInfo->entry[u16NumberOfDevices].hDriver = open(sDevFile, O_RDWR);

            status = 0;

            if (pDeviceInfo->entry[u16NumberOfDevices].hDriver > 0)
            {
                strcpy(pDeviceInfo->entry[u16NumberOfDevices].szDriver, sDevFile);

                /*--------------------------------------------------*/
                /* get bus & port info to serve as unique device id */
                /*--------------------------------------------------*/
                E = ddccmGetDevInfo((int)pDeviceInfo->entry[u16NumberOfDevices].hDriver, &sDevInfo);

                if (E != DDCCM_SUCCESS)
                {
                    /* error, so skip this entry */
                    continue;
                }

                pDeviceInfo->entry[u16NumberOfDevices].sDevInfo = sDevInfo;

                /*--------------------------------------------------*/
                /* get flash info                                   */
                /*--------------------------------------------------*/
                E = ddccmGetFlashInfo((int)pDeviceInfo->entry[u16NumberOfDevices].hDriver, &sFlashInfo);

                if (E != DDCCM_SUCCESS)
                {
                    /* error, so skip this entry */
                    continue;
                }

                if (bFlashableOnly)
                {
                    if (sFlashInfo.SectorCount == 0)
                    {
                        /* we can not add this device since we do not know the flash information, so skip it */
                        continue;
                    }
                }

                pDeviceInfo->entry[u16NumberOfDevices].u32StartAddress  = sFlashInfo.StartAddress;
                pDeviceInfo->entry[u16NumberOfDevices].u32SectorCount   = sFlashInfo.SectorCount;
                pDeviceInfo->entry[u16NumberOfDevices].u32MinSectorSize = sFlashInfo.MinSectorSize;

                /*--------------------------------------------------*/
                /* get capabilities (model number, card type)       */
                /*--------------------------------------------------*/
                E = ddccmGetEnhancedCapabilities((int)pDeviceInfo->entry[u16NumberOfDevices].hDriver, &pDeviceInfo->entry[u16NumberOfDevices].sEnhancedCardCapabilities);

                if (E != DDCCM_SUCCESS)
                {
                    /* error, so skip this entry */
                    continue;
                }

                /* check if the device is booting from backup flash image   */
                /* a backup image will have all channel counts set to 0     */
                if ((pDeviceInfo->entry[u16NumberOfDevices].sEnhancedCardCapabilities.channelCount1553 == 0) &&
                    (pDeviceInfo->entry[u16NumberOfDevices].sEnhancedCardCapabilities.channelCount429Rx == 0) &&
                    (pDeviceInfo->entry[u16NumberOfDevices].sEnhancedCardCapabilities.channelCount429Tx == 0) &&
                    (pDeviceInfo->entry[u16NumberOfDevices].sEnhancedCardCapabilities.channelCount429Prog == 0))
                {
                    pDeviceInfo->entry[u16NumberOfDevices].bBackup = TRUE;
                }
                else
                {
                    pDeviceInfo->entry[u16NumberOfDevices].bBackup = FALSE;
                }

                /*--------------------------------------------------*/
                /* get hardware version information                 */
                /*--------------------------------------------------*/
                E = ddccmGetHwVersionInfo((int)pDeviceInfo->entry[u16NumberOfDevices].hDriver, &pDeviceInfo->entry[u16NumberOfDevices].sHwVersionInfo, bFamily);

                if (E != DDCCM_SUCCESS)
                {
                    /* error, so skip this entry */
                    continue;
                }

                pDeviceInfo->entry[u16NumberOfDevices].u32DataArchiveNumber = pDeviceInfo->entry[u16NumberOfDevices].sHwVersionInfo.dwHdlNumber;

                /* update the count */
                u16NumberOfDevices++;

                if (u16NumberOfDevices == MAX_NUM_DEV)
                {
                    /* max number of devices reached */
                    break;
                }
            }
        }
    }

    fclose(fpDriverListFile);

    pDeviceInfo->u16NumberOfDevices = u16NumberOfDevices;

    if( status )
        return FLASH_DEVICE_ERROR;

    return FLASH_SUCCESS;
}


/*******************************************************************************
 * Name:    ddccmOsFreeDevices
 *
 * Description:
 *      This function frees that devices in the device info structure.
 *
 * In   pDeviceInfo     pointer to the device info list
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
void ddccmOsFreeDevices
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo
)
{
    U16BIT i;

    for (i=0; i<pDeviceInfo->u16NumberOfDevices; i++)
    {
        if (pDeviceInfo->entry[i].hDriver)
        {
             close(pDeviceInfo->entry[i].hDriver);
             pDeviceInfo->entry[i].hDriver = -1;
        }
    }
}

/*******************************************************************************
 * Name:    GetFlashLookupTableFilename
 *
 * Description:
 *      This function returns a pointer a character string which holds the
 *      location of the flash lookup table.
 *
 * In   none
 * Out  none
 *
 * Returns: pointer to the flash lookup table filename
 ******************************************************************************/
const char *GetFlashLookupTableFilename
(
    void
)
{
    return szLookupTableFilenamePath;
}
