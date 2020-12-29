/*******************************************************************************
 * FILE: ddccm_flash.c
 *
 * DESCRIPTION:
 *
 *  This module contains the OS independent functions to flash a device.
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
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "strtable.h"
#include <ctype.h>      /* for isdigit */
#include "include/ddc_types.h"
#include "include/ddc_ioctl.h"
#include "ddccm.h"
#include "ddccm_flash.h"
#include "ddccm_os.h"
#include "ddccm_os_flash.h"

/* ========================================================================== */
/*                                DATA TYPES                                  */
/* ========================================================================== */

#define DATA_BUFFER_BYTE_SIZE               0x1000          /* 4k   */
#define DISPLAY_INTERVAL                    0x8000          /* 32k  */

#define ALL_MODELS                          0xFF
#define DATA_ARCHIVE_NUMBER_LENGTH          5
#define DATA_ARCHIVE_NUMBER_NOT_FOUND       -1

#define BIN_FILE_HEADER_VALUE__FPGA                     0xFFFFFFFF
#define BIN_FILE_HEADER_VALUE_TX_INHIBIT_BC_DISABLE     0xDEADFEED


#define RESPONSE_YES                                1
#define RESPONSE_NO                                 0

#define FLASH_STATUS_WRITE_PROTECT_DISABLED         0
#define FLASH_STATUS_WRITE_PROTECT_ENABLED          1
#define FLASH_STATUS_WRITE_PROTECT_UNKNOWN          -1

#define FLASH_ERASE_SUCCESS                         0
#define FLASH_ERASE_ERROR                           -1

#define FLASH_WRITE_SUCCESS                         0
#define FLASH_WRITE_ERROR                           -1

#define FLASH_VERIFY_ERASE_SUCCESS                  0
#define FLASH_VERIFY_ERASE_ERROR                    -1

#define FLASH_VERIFY_PROGRAMMED_SUCCESS             0
#define FLASH_VERIFY_PROGRAMMED_ERROR               -1

#define ERROR_LEVEL_INFORMATION                     0
#define ERROR_LEVEL_WARNING                         1
#define ERROR_LEVEL_ERROR                           2

/* TX Inhibit / BC Disable Items */#define REG_TYPE_BD_OFFSET                      1
#define REG_BOARD_CAPABILITY_OFFSET				0x01
#define REG_BOARD_TX_INH_BC_DISABLE_OFFSET      0x1C
#define REG_BOARD_429_TX_INHIBIT_OFFSET         0x34
#define TX_INH_429_DISABLE_MASK                 0x0400
#define CONTROL_ADDRESS_HI                      11
#define CONTROL_ADDRESS_LO                      10
#define CONTROL_429_ADDRESS_HI                  21
#define CONTROL_429_ADDRESS_LO                  20
#define CONTROL_TERMINATE_HI                    17
#define CONTROL_TERMINATE_LO                    16
#define CONTROL_TERMINATE_ZERO                  18
#define BYTE_SHIFT                              8

/*******************************************************************************
 * Name:    DisplayMessage
 *
 * Description:
 *      This function will print out a message.
 *
 * In   u8ErrorLevel    error level
 * In   format      format control
 * In   ...         optional arguments
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void DisplayMessage
(
    U8BIT u8ErrorLevel,
    const char* format,
    ...
)
{
    va_list args;

    switch(u8ErrorLevel)
    {
        case ERROR_LEVEL_ERROR:
            printf(IDS_ERROR_CAPTION);
            break;

        case ERROR_LEVEL_WARNING:
            printf(IDS_WARNING_CAPTION);
            break;
    }

    va_start( args, format );
    vprintf( format, args );
    va_end( args );
}

/*******************************************************************************
 * Name:    GetFlashWriteProtection
 *
 * Description:
 *      This function returns the flash write protection status.
 *
 * In   pDeviceInfoEntry - pointer to a specific device in the device info list
 * Out  none
 *
 * Returns: 0 = writeable, 1 = write protected, otherwise error
 ******************************************************************************/
static S16BIT GetFlashWriteProtection
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;
    U32BIT u32WriteProtectionStatus;

    /* get flash write protection status */
    if (!ddccmDeviceIoControl(
            pDeviceInfoEntry->hDriver,
            IOCTL_FLASH_MEM_WRITE_PROTECTED,
            &IoCtlParams,
            sizeof(IoCtlParams),
            &u32WriteProtectionStatus,
            sizeof(u32WriteProtectionStatus),
            &Bytes,
            NULL))
    {
        /* error */
        return FLASH_STATUS_WRITE_PROTECT_UNKNOWN;
    }

    return (S16BIT)(u32WriteProtectionStatus & 0xFF);
}

/*******************************************************************************
 * Name:    EraseFlashSectors
 *
 * Description:
 *      This function erases a specific number of sectors in the flash.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32SectorsToErase   total number of sectors to erase
 * Out  none
 *
 * Returns: 0 = success, otherwise error
 ******************************************************************************/
static S16BIT EraseFlashSectors
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32SectorsToErase
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT u32Result;
    U32BIT Bytes;
    U16BIT i;

    printf(IDS_NEWLINE_X1);

    for (i=0; i < u32SectorsToErase; i++)
    {
        printf(IDS_ERASE_FLASH_PROGRESS, (i+1), (int)u32SectorsToErase);

        /* set the start address of the flash we want to erase */
        IoCtlParams.Param1 = (i * pDeviceInfoEntry->u32MinSectorSize);


        u32Result = ddccmDeviceIoControl(
                pDeviceInfoEntry->hDriver,
                IOCTL_FLASH_MEM_BLOCK_ERASE,
                &IoCtlParams,
                sizeof(IoCtlParams),
                NULL,
                0,
                &Bytes,
                NULL);

        if (u32Result == DDCCM_FAILURE)
        {
            /* error */
            DisplayMessage(ERROR_LEVEL_ERROR, IDS_ERASE_FAILURE, u32Result);
            return FLASH_ERASE_ERROR;
        }
    }

    printf(IDS_NEWLINE_X1);

    return FLASH_ERASE_SUCCESS;
}

/*******************************************************************************
 * Name:    WriteFlashMemory
 *
 * Description:
 *      This function writes a block of data to the flash.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32Address          address of where write to the flash
 * In   u32BytesToWrite     number of bytes to write to the flash
 * In   pBuffer             pointer to data buffer
 * Out  none
 *
 * Returns: 0 = success, otherwise error
 ******************************************************************************/
static S16BIT WriteFlashMemory
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32Address, U32BIT u32BytesToWrite,
    char *pWriteBuffer
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT u32Result;
    U32BIT Bytes;

    IoCtlParams.Param1 = u32Address;
    IoCtlParams.Param2 = u32BytesToWrite;

    u32Result = ddccmDeviceIoControl(
            pDeviceInfoEntry->hDriver,
            IOCTL_FLASH_MEM_WRITE,
            &IoCtlParams,
            sizeof(IoCtlParams),
            pWriteBuffer,
            u32BytesToWrite,
            &Bytes,
            NULL);

    if (u32Result == DDCCM_FAILURE)
    {
        /* error */
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_WRITE_FAILURE, u32Result);
        return FLASH_WRITE_ERROR;
    }

    return FLASH_WRITE_SUCCESS;
}

/*******************************************************************************
 * Name:    VerifyFlashErased
 *
 * Description:
 *      This function verifies that the flash is erased for a given range.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32Address          start address of where verify the flash is erased
 * In   u32BytesToVerify    number of bytes to verify in the flash
 * In   pReadBuffer         buffer to hold the read data that will be verified
 * Out  none
 *
 * Returns: 0 = success, otherwise error
 ******************************************************************************/
static S16BIT VerifyFlashErased
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32Address,
    U32BIT u32BytesToVerify,
    char *pReadBuffer
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT u32Result;
    U32BIT Bytes;
    U32BIT i;

    IoCtlParams.Param1	= u32Address;
    IoCtlParams.Param2 = u32BytesToVerify;

    u32Result = ddccmDeviceIoControl(
            pDeviceInfoEntry->hDriver,
            IOCTL_FLASH_MEM_READ,
            &IoCtlParams,
            sizeof(IoCtlParams),
            pReadBuffer,
            u32BytesToVerify,
            &Bytes,
            NULL);

    if (u32Result == DDCCM_FAILURE)
    {
        /* error */
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_ERASE_VERIFY_FAILURE_IOCTL, u32Result);
        return FLASH_VERIFY_ERASE_ERROR;
    }

    for (i=0; i<u32BytesToVerify; i++)
    {
        if ((U8BIT)pReadBuffer[i] != (U8BIT)FLASH_ERASE_VALUE)
        {
            DisplayMessage(ERROR_LEVEL_ERROR, IDS_ERASE_VERIFY_FAILURE,
                (u32Address + i), pReadBuffer[i], FLASH_ERASE_VALUE);

            return FLASH_VERIFY_ERASE_ERROR;
        }
    }

    return FLASH_VERIFY_ERASE_SUCCESS;
}

/*******************************************************************************
 * Name:    VerifyFlashProgrammed
 *
 * Description:
 *      This function verifies that the flash was programmed correctly for a
 *      given range. The source buffer is compared to the data read from the
 *      flash.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32Address          start address of where verify the flash is erased
 * In   u32BytesToVerify    number of bytes to verify in the flash
 * In   pSourceBuffer       source data that will be used to compare to the read buffer
 * In   pReadBuffer         buffer to hold the read data that will be verified
 * Out  none
 *
 * Returns: 0 = success, otherwise error
 ******************************************************************************/
static S16BIT VerifyFlashProgrammed
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32Address,
    U32BIT u32BytesToVerify,
    const char *pSourceBuffer,
    char *pReadBuffer
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT u32Result;
    U32BIT Bytes;
    U32BIT i;

    IoCtlParams.Param1	= u32Address;
    IoCtlParams.Param2 = u32BytesToVerify;

    u32Result = ddccmDeviceIoControl(
            pDeviceInfoEntry->hDriver,
            IOCTL_FLASH_MEM_READ,
            &IoCtlParams,
            sizeof(IoCtlParams),
            pReadBuffer,
            u32BytesToVerify,
            &Bytes,
            NULL);

    if (u32Result == DDCCM_FAILURE)
    {
        /* error */
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_PROGRAMMED_FAILURE_IOCTL, u32Result);
        return FLASH_VERIFY_PROGRAMMED_ERROR;
    }

    for (i=0; i<u32BytesToVerify; i++)
    {
        if (pReadBuffer[i] != pSourceBuffer[i])
        {
            DisplayMessage(ERROR_LEVEL_ERROR, IDS_PROGRAMMED_FAILURE,
                (u32Address + i), pReadBuffer[i], pSourceBuffer[i]);

            return FLASH_VERIFY_PROGRAMMED_ERROR;
        }
    }

    return FLASH_VERIFY_PROGRAMMED_SUCCESS;
}

/*******************************************************************************
 * Name:    FindDataArchiveNumber
 *
 * Description:
 *      This function tries to find a data archive number within the lookup table.
 *      If a match is found, the index is retuned, otherwise an error is returned.
 *
 * In   pLookupTable            pointer to the head of the lookup table
 * In   szBinFilename           pointer to the name of the bin file
 * Out  none
 *
 * Returns: index location if success, invalid entry position if not found
 ******************************************************************************/
static S32BIT FindDataArchiveNumber(LOOKUP_TABLE_TYPE *pLookupTable, char *szBinFilename)
{
    U32BIT u32BinFilenameLen = 0;
    U32BIT u32DeviceNameLen = 0;
    U16BIT u16EntryIndex;
    U16BIT i;
    char   binFilenameChar;
    char   deviceNameChar;

    /* find the position of the '_' in the filename */
    for (i=0; i<strlen(szBinFilename); i++)
    {
         if (szBinFilename[i] == '_')
         {
             /* update the length */
             u32BinFilenameLen = i;
             break;
         }
    }

    /* was the underscore found? */
    if (i == strlen(szBinFilename))
    {
        /* not found, return error */
        return DATA_ARCHIVE_NUMBER_NOT_FOUND;
    }

    for (u16EntryIndex=0; u16EntryIndex < pLookupTable->u16NumberOfLookupTableEntries; u16EntryIndex++)
    {
        /* get the lengths of the device name string */
        u32DeviceNameLen = (U32BIT)strlen(pLookupTable->entry[u16EntryIndex].szDeviceName);

        /* first make sure the lengths are the same */
        if (u32BinFilenameLen == u32DeviceNameLen)
        {
            /* do the product names match? */
            for (i=0; i<u32BinFilenameLen; i++)
            {
                /* convert characters to lower case */
                binFilenameChar = (char)tolower(szBinFilename[i]);
                deviceNameChar = (char)tolower(pLookupTable->entry[u16EntryIndex].szDeviceName[i]);

                /* do the characters match or not? */
                if (binFilenameChar != deviceNameChar)
                {
                    /* not a match, so exit loop */
                    break;
                }
            }

            /* did we match all the characters? */
            if (i == u32BinFilenameLen)
            {
                return u16EntryIndex;
            }
        }
    }

    return DATA_ARCHIVE_NUMBER_NOT_FOUND;
}

/******************************************************************************
 * Name:    VerifyFile
 *
 * Description:
 *      The function verifies that the file to flash is a valid bin file. It first
 *      checks to see if there the filename is properly formatted. Then it checks
 *      for a valid data archive number in the filename. If a valid number is
 *      found, the number is checked to see if it is in the lookup table. If it
 *      is in the table, it checks to see if the device DA# matches the filenane DA#.
 *
 *      Filename format:
 *
 *          BUXXXXX_REVYY.bin
 *
 *      Where   BUXXXXX     is the product name             length may vary
 *              REVY        is the firmware revision        length may vary
 *              .bin        file extension
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the deviceInfo struct
 * In   pDeviceInfo         pointer to the head of the device info list
 * In   fpBinFile           pointer to the file handle for the bin file
 * In   szBinFilename       pointer to the bin filename
 * In   u32FileSize         size of the bin file in bytes
 * In   u32FlashOperationType   type of flash operation
 * Out  none
 *
 * Returns: 0 = vaild filename, otherwise error value
 *****************************************************************************/
static S16BIT VerifyFile
(
    DEVICE_INFORMATION_ENTRY_TYPE *pDeviceInfoEntry,
    LOOKUP_TABLE_TYPE *pLookupTable,
    FILE *fpBinFile,
    char *szBinFilename,
    U32BIT u32FileSize,
    U32BIT u32FlashOperationType
)
{
    U32BIT u32BinFilenameWithPathLength;
    U32BIT u32BinFilenameLength;
    U32BIT u32BinFilenameStartIndex;
    U32BIT u32Value;
    S32BIT s32EntryIndex;
    U8BIT i;
    BOOLEAN bFound = FALSE;

    /* find the string length of the bin filename without the path */

    /* find last slash in string (if present) */
    for (i=(U8BIT)strlen(szBinFilename); i > 0; i--)
    {
        if ((szBinFilename[i] == '\\') || (szBinFilename[i] == '/'))
        {
            bFound = TRUE;
            break;
        }
    }

    u32BinFilenameWithPathLength = (U32BIT)strlen(szBinFilename);

    /* was the slash found and the position is valid? */
    if ((bFound) && (i < strlen(szBinFilename)))
    {
        /* get the length of the filename without the path */
        u32BinFilenameLength = (U32BIT)strlen(&szBinFilename[i+1]);

        /* set the start index of where the file name begins */
        u32BinFilenameStartIndex = i+1;
    }
    else
    {
        /* there is no slash, so just set the size to the length */
        u32BinFilenameLength = u32BinFilenameWithPathLength;

        /* set the start to the beginning */
        u32BinFilenameStartIndex = 0;
    }

    if (u32FlashOperationType == DDCCM_FLASH_TYPE__FIRMWARE)
    {
        /* check if the name meets the minimum length */
        if (u32BinFilenameLength < MIN_BIN_FILE_NAME_LENGTH)
        {
            /* error, name too short */
            return FLASH_ERROR_VERIFY_FILENAME;
        }
    }

    /* check to see if the bin file header is correct */

    /* read the first 32 bit value from the bin file */
    fread(&u32Value, sizeof(U32BIT), 1, fpBinFile);

    /* set the file pointer back to the start */
    fseek(fpBinFile, 0, SEEK_SET);

    if (u32FlashOperationType == DDCCM_FLASH_TYPE__FIRMWARE)
    {
        if (u32Value != BIN_FILE_HEADER_VALUE__FPGA)
        {
            /* error, header not correct */
            return FLASH_ERROR_VERIFY_FILE_HEADER;
        }
    }
    else if (u32FlashOperationType == DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE)
    {
        if (u32Value != BIN_FILE_HEADER_VALUE_TX_INHIBIT_BC_DISABLE)
        {
            /* error, header not correct */
            return FLASH_ERROR_VERIFY_FILE_HEADER;
        }
    }

    /* check if the filename ends in ".bin" (allow .BIN as well) */
    if ( (szBinFilename[u32BinFilenameWithPathLength-4] != '.') &&
       (((szBinFilename[u32BinFilenameWithPathLength-3] != 'b') && (szBinFilename[u32BinFilenameWithPathLength-3] != 'B')) ||
        ((szBinFilename[u32BinFilenameWithPathLength-2] != 'i') && (szBinFilename[u32BinFilenameWithPathLength-2] != 'I')) ||
        ((szBinFilename[u32BinFilenameWithPathLength-1] != 'n') && (szBinFilename[u32BinFilenameWithPathLength-1] != 'N'))))
    {
        /* error, file extension incorrect */
        return FLASH_ERROR_VERIFY_FILENAME;
    }

    /* get the location of the device in the lookup table for use later */
    s32EntryIndex = FindDataArchiveNumber(pLookupTable, &szBinFilename[u32BinFilenameStartIndex]);

    /* was an entry found or not? */
    if (s32EntryIndex != DATA_ARCHIVE_NUMBER_NOT_FOUND)
    {
        /* does the data archive number associated with the filename match the data archive number for the device? */
        if ((U32BIT)pLookupTable->entry[s32EntryIndex].nDataArchiveNumber == pDeviceInfoEntry->u32DataArchiveNumber)
        {
            /* make sure the size of the bin file is correct */
            if ((U32BIT)(pLookupTable->entry[s32EntryIndex].nFileSize) == u32FileSize)
            {
                /* we have a vaild file for the selected device */
                return FLASH_SUCCESS;
            }
            else
            {
                /* error, the file size is invalid */
                return FLASH_ERROR_VERIFY_FILE_SIZE;
            }
        }
        else
        {
            /* mismatch of archive numbers, this may be ok if the user is booting from backup */
            if (pDeviceInfoEntry->bBackup == TRUE)
            {
                /* the user is probably trying to flash a primary image */

                /* does the device ID of the device match that in the lookup table? */
                if (pDeviceInfoEntry->sDevInfo.dwDeviceID == (U32BIT)pLookupTable->entry[s32EntryIndex].nDeviceID)
                {
                    /* check if the lookup table entry is valid for all models or   */
                    /* if the model numbers match                                   */
                    if ((pLookupTable->entry[s32EntryIndex].nModel == ALL_MODELS) ||
                        (pDeviceInfoEntry->sEnhancedCardCapabilities.modelNumber == pLookupTable->entry[s32EntryIndex].nModel))
                    {
                        /* make sure the file size matches */
                        if ((U32BIT)(pLookupTable->entry[s32EntryIndex].nFileSize) == u32FileSize)
                        {
                            /* this bin file appears to be for this device */
                            return FLASH_SUCCESS;
                        }
                        else
                        {
                            /* error, the file size is invalid */
                            return FLASH_ERROR_VERIFY_FILE_SIZE;
                        }
                    }
                }

                /* error, the device ID's do not match */
                return FLASH_ERROR_VERIFY_MISMATCH_DEVICE_ID;
            }
            else
            {
                /* error, the data archive numbers do not match */
                return FLASH_ERROR_VERIFY_MISMATCH_DA_NUMBER;
            }
        }
    }

    return FLASH_ERROR_VERIFY_FILENAME;
}

/*******************************************************************************
 * Name:    PrintFlashWritingProgress
 *
 * Description:
 *      This function will print out the flash progress.
 *
 * In   u32BytesSent    number of bytes written to the flash
 * In   u32TotalBytes   total number of bytes to be written
 * In   u32Address      start of flash address block being written
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void PrintFlashWritingProgress
(
    U32BIT u32BytesSent,
    U32BIT u32TotalBytes,
    U32BIT u32Address
)
{
    U8BIT u8Percentage;

    if (u32BytesSent > u32TotalBytes)
    {
        u8Percentage = 100;
    }
    else
    {
        u8Percentage = (U8BIT)((u32BytesSent * 100) / u32TotalBytes);
    }

    printf(IDS_PROGRAMMING_PROGRESS, u8Percentage, (int)u32Address);
}

/*******************************************************************************
 * Name:    PrintFlashInformation
 *
 * Description:
 *      This function will print out the flash information.
 *
 * In   szBinFilename       pointer to the name of the bin file
 * In   u32Filesize         size of the bin file in bytes
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void PrintFlashInformation
(
    char *szBinFilename,
    U32BIT u32Filesize,
    DEVICE_INFORMATION_ENTRY_TYPE *pDeviceInfoEntry
)
{
    printf(IDS_NEWLINE_X2);
    printf(IDS_INFO_DEVICE, pDeviceInfoEntry->sHwVersionInfo.szModelName);
    printf(IDS_INFO_FILENAME, szBinFilename);
    printf(IDS_INFO_FILE_SIZE, (int)u32Filesize);
}

/*******************************************************************************
* Name:    IsUserReadyToFlash
*
* Description:
*      Read a character from stdin and return only when a 'y' or 'n' is entered.
*
* In   void
* Out  none
*
* Returns: 0 - NO, 1 - YES
******************************************************************************/
static U8BIT IsUserReadyToFlash
(
    void
)
{
    char buff[8];

    printf(IDS_READY_TO_FLASH);

    for (;;)
    {
        printf(IDS_ENTER_Y_N);

        /* read a string from stdin with buffer length checking */
        fgets(buff, sizeof(buff), stdin);

        /* clear the standard input buffer */
        fflush(stdin);

        /* check for a 'y' or 'n' followed by a '\n' */
        if ((buff[0] == 'y') && (buff[1] == '\n'))
        {
            return RESPONSE_YES;
        }
        else if ((buff[0] == 'n') && (buff[1] == '\n'))
        {
            return RESPONSE_NO;
        }
    }
}

/******************************************************************************
 * Name:    GetFileName
 *
 * Description:
 *      Gets a text string from stdin.
 *
 * In   szFileName      pointer to character buffer
 * In   size            size of szFileName buffer
 *
 * Out  updates szFileName
 *
 * Returns: none
 *****************************************************************************/
static S16BIT GetFileName
(
    char *szFileName,
    size_t size
)
{
    size_t i=0;

    printf(IDS_ENTER_FILENAME);

    fflush(stdin);

    for (;;)
    {
        int ch = fgetc(stdin);

        /* check for the end of user input */
        if ((ch == '\n') || (ch == EOF))
        {
            break;
        }

        /* did we go over the size of the input buffer? */
        if (i < (size - 1))
        {
            szFileName[i++] = (char) ch;
        }
    }

    /* NULL terminate the string */
    szFileName[i] = '\0';

    /* clear the standard input buffer */
    fflush(stdin);

    return FLASH_SUCCESS;
}





/*******************************************************************************
 * Name:    ParseLookupTable
 *
 * Description:
 *      This function parses the lookup table. This file contains a mapping of
 *      devices to data archive numbers.
 *
 *      -----------
 *      File Format
 *      -----------
 *
 *      <data archive number>,<device ID>,<model>,<device name>
 *
 * In   pLookupTable - pointer to the head of the lookup table
 * Out  none
 *
 * Returns: 0 = success, otherwise an error
 ******************************************************************************/
S16BIT ParseLookupTable
(
    LOOKUP_TABLE_TYPE *pLookupTable
)
{
#ifdef DDCCM_USE_FILE_SYSTEM
    FILE *fpLookupTableFile;
    U16BIT u16NumberOfEntries = 0;
    const char *pszLookupTableFilename;
    char szLineBuffer[DDCCM_MAX_LINE_BUFFER_LEN];

    pszLookupTableFilename = GetFlashLookupTableFilename();

    /* open the file */
    fpLookupTableFile = fopen(pszLookupTableFilename, "r");

    if (fpLookupTableFile == NULL)
    {
        /* error opening file (not found?) */
        return FLASH_ERROR_FILE_OPEN;
    }

    pLookupTable->u16NumberOfLookupTableEntries = 0;

    /* read a line at a time until we reach the end of the file */
    while (fgets(szLineBuffer, sizeof(szLineBuffer), fpLookupTableFile) != NULL)
    {
        /* must scan in all items to be a valid entry */
        if (sscanf(szLineBuffer, IDS_LOOKUP_TABLE_SCAN_LINE,
            &pLookupTable->entry[u16NumberOfEntries].nDataArchiveNumber,
            &pLookupTable->entry[u16NumberOfEntries].nDeviceID,
            &pLookupTable->entry[u16NumberOfEntries].nModel,
            &pLookupTable->entry[u16NumberOfEntries].nFileSize,
            &pLookupTable->entry[u16NumberOfEntries].nRecFwVer,
            &pLookupTable->entry[u16NumberOfEntries].nRecEepVer,
            (char *)&pLookupTable->entry[u16NumberOfEntries].szDeviceName) == LOOKUP_TABLE_ENTRY_NUMBER_ELEMENTS)
        {
            /* found all items, add to the list */
            u16NumberOfEntries++;

            if (u16NumberOfEntries == MAX_LOOKUP_TABLE_ENTRIES)
            {
                /* warning, the maximum number of entries have been reached */
                /* stop reading in entries                                  */
                break;
            }
        }
    }

    fclose(fpLookupTableFile);

    /* store the count */
    pLookupTable->u16NumberOfLookupTableEntries = u16NumberOfEntries;

    /* was the file empty or no valid entries? */
    if (pLookupTable->u16NumberOfLookupTableEntries == 0)
        return FLASH_ERROR_PARSE_FILE;

    return FLASH_SUCCESS;
#else
    /* do not use file I/O */
    return FLASH_ERROR_FILE_OPEN;
#endif /* DDCCM_USE_FILE_SYSTEM */
}

/*******************************************************************************
 * Name:    MatchDevices
 *
 * Description:
 *      This function tries to match a device with an entry in the lookup table. If
 *      a match is found, the index is updated for the device entry.
 *
 * In   pDeviceInfo     pointer to the head of the device info list
 * In   pLookupTable    pointer to the head of the lookup table
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
void MatchDevices
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo,
    LOOKUP_TABLE_TYPE *pLookupTable
)
{
    U16BIT u16EntryIndex;
    U16BIT u16DeviceIndex;

    for(u16DeviceIndex=0; u16DeviceIndex < pDeviceInfo->u16NumberOfDevices; u16DeviceIndex++)
    {
        /* check to see if the device could be flashed */
        if (pDeviceInfo->entry[u16DeviceIndex].u32SectorCount > 0)
        {
            pDeviceInfo->entry[u16DeviceIndex].bIsFlashable = TRUE;
        }
        else
        {
            /* not a supported device */
            continue;
        }

        for (u16EntryIndex=0; u16EntryIndex < pLookupTable->u16NumberOfLookupTableEntries; u16EntryIndex++)
        {
            /* do the data archive numbers match? */
            if (pDeviceInfo->entry[u16DeviceIndex].u32DataArchiveNumber == (U32BIT)pLookupTable->entry[u16EntryIndex].nDataArchiveNumber)
            {
                /* Some older drivers may not support reading of the data archive number    */
                /* and will return 00000. Since there are mutiple entries for 00000, we     */
                /* need to match the device ID's as well. The non-zero values should also   */
                /* match.                                                                   */
                if (pDeviceInfo->entry[u16DeviceIndex].sDevInfo.dwDeviceID == (U32BIT)pLookupTable->entry[u16EntryIndex].nDeviceID)
                {
                    /* save the recommended firmware version */
                    pDeviceInfo->entry[u16DeviceIndex].u32RecFwVer = pLookupTable->entry[u16EntryIndex].nRecFwVer;

                    /* check for a firmware mismatch */
                    if ((pDeviceInfo->entry[u16DeviceIndex].sHwVersionInfo.dwHdlVersion != (U32BIT)(pLookupTable->entry[u16EntryIndex].nRecFwVer / 100)) ||
                        (pDeviceInfo->entry[u16DeviceIndex].sHwVersionInfo.dwFwVersion  != (U32BIT)(pLookupTable->entry[u16EntryIndex].nRecFwVer % 100)))
                    {
                        pDeviceInfo->entry[u16DeviceIndex].bFirmwareMismatch = TRUE;
                    }

                    /* now check for a PLX EEPROM mismatch */
                    if (pLookupTable->entry[u16EntryIndex].nRecEepVer != 0)
                    {
                        U32BIT devFwVer = 0;

                        /* convert device FW version from string to the same format as that of the flash.ddc file */
                        devFwVer = pDeviceInfo->entry[u16DeviceIndex].sHwVersionInfo.dwHdlVersion * 100;
                        devFwVer += pDeviceInfo->entry[u16DeviceIndex].sHwVersionInfo.dwFwVersion;

                        if ((U32BIT)(pLookupTable->entry[u16EntryIndex].nRecEepVer) > devFwVer)
                        {
                            pDeviceInfo->entry[u16DeviceIndex].bEepMismatch = TRUE;
                        }
                    }

                    pDeviceInfo->entry[u16DeviceIndex].bValidatedDevice = TRUE;
                    break;
                }
            }
        }
    }
}


/*******************************************************************************
 * Name:    TxInhibitBcDisable_GetCapability
 *
 * Description:
 *      This function verifies card has Tx Inhibit/BC Disable capability.
 *
 * In   pDeviceInfoEntry - pointer to a specific device in the device info list
 * Out  none
 *
 * Returns: 0 = FALSE, 1 = TRUE
 ******************************************************************************/
S16BIT TxInhibitBcDisable_GetCapability
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT *pu32DeviceCapability
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    IoCtlParams.Param1 = REG_BOARD_CAPABILITY_OFFSET;
    IoCtlParams.Param2 = REG_TYPE_BD_OFFSET;
    IoCtlParams.Param3 = sizeof(U32BIT);

    /* get flash write protection status */
    if (!ddccmDeviceIoControl(
        pDeviceInfoEntry->hDriver,
        IOCTL_REG_READ_BLK,
        &IoCtlParams,
        sizeof(IoCtlParams),
        pu32DeviceCapability,
        IoCtlParams.Param3,
        &Bytes,
        NULL))
    {
        /* error */
        return FLASH_DEVICE_ERROR;
    }

    /* Bit 8 of Device Capability Register denotes if Tx Inhibit/BC_Disable supported */
    /* value of 1 indicates capability is supported */
    return FLASH_SUCCESS;
}

/*******************************************************************************
 * Name:    TxInhibitBcDisable_GetFlashStartAddress
 *
 * Description:
 *      This function retrieves Flash start address to write auto init bin file
 *
 * In   pDeviceInfoEntry - pointer to a specific device in the device info list
 * Out  u32StartAddress - Flash start address
 *
 * Returns: SUCCESS or error code
 ******************************************************************************/
static S16BIT TxInhibitBcDisable_GetFlashStartAddress
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT *pu32StartAddress
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    IoCtlParams.Param1 = REG_BOARD_TX_INH_BC_DISABLE_OFFSET;
    IoCtlParams.Param2 = REG_TYPE_BD_OFFSET;
    IoCtlParams.Param3 = sizeof(U32BIT);

    if (!ddccmDeviceIoControl(
        pDeviceInfoEntry->hDriver,
        IOCTL_REG_READ_BLK,
        &IoCtlParams,
        sizeof(IoCtlParams),
        pu32StartAddress,
        IoCtlParams.Param3,
        &Bytes,
        NULL))
    {
        /* error */
        return FLASH_DEVICE_ERROR;
    }

    return FLASH_SUCCESS;
}

/*******************************************************************************
 * Name:    TxInhibitBcDisable_RetriveBrdGlobalBaseAddress
 *
 * Description:
 *      Retrieves base address for Global Board register
 *
 * In   pDeviceInfoEntry - pointer to a specific device in the device info list
 * Out  u32StartAddress - Flash start address
 *
 * Returns: SUCCESS or error code
 ******************************************************************************/
static S16BIT TxInhibitBcDisable_RetriveBrdGlobalBaseAddress
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U16BIT *u16BoardGlobalBaseAddress
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT u32RegAddr;
    U32BIT Bytes;

    IoCtlParams.Channel = 0;
    IoCtlParams.Param1 = DDC_GET_INFO__REG_BASE_ADDR;
    IoCtlParams.Param2 = DDC_GET_INFO_ADDR__BD_GLOBAL;

    if (!ddccmDeviceIoControl(
            pDeviceInfoEntry->hDriver,
            IOCTL_GET_INFO,
            &IoCtlParams,
            sizeof(IoCtlParams),
            &u32RegAddr,
            sizeof(u32RegAddr),
            &Bytes,
            NULL))
    {
        /* error */
        return FLASH_ERROR_REG_READ_ERROR;
    }

    *u16BoardGlobalBaseAddress = (U16BIT) u32RegAddr;

    return FLASH_SUCCESS;
}

/*******************************************************************************
 * Name:    FlashDevice
 *
 * Description:
 *      This function handles the flashing of the device. The user is first
 *      asked to enter the bin filename. The flash is then ersaed, blocks of
 *      data are read from the bin file and flshed to the device. This data is
 *      then verified.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   pLookupTable            pointer to the head of the lookup table
 * In   u32FlashOperationType   type of flash operation
 * Out  none
 *
 * Returns: 0 = success, otherwise error value
 ******************************************************************************/
S16BIT FlashDevice
(
    DEVICE_INFORMATION_ENTRY_TYPE *pDeviceInfoEntry,
    LOOKUP_TABLE_TYPE *pLookupTable,
    U32BIT u32FlashOperationType
)
{
#ifdef DDCCM_USE_FILE_SYSTEM
    FILE *fpBinFile;
    U32BIT u32Result;
    U32BIT u32Filesize;
    U32BIT u32BytesRead;
    U32BIT u32TotalBytesRead;
    U32BIT u32StartAddress;
    U32BIT u32SectorsToErase;
	U32BIT u32DeviceCapability = 0;
    U16BIT u16BoardGlobalBaseAddress = 0;
    U16BIT u16TempHold;
    S16BIT s16WriteProtectionStatus;
    S16BIT s16ReturnResult;
    U8BIT u8Response;
    BOOLEAN bUsingTxInhibit429 = 0;
    char szBinFilename[DDCCM_MAX_LINE_BUFFER_LEN];
    char *pBuffer;
    char *pVerifyBuffer;

    u32TotalBytesRead = 0;

    s16ReturnResult = FLASH_SUCCESS;

    /*------------------------------------------------------*/
    /* verify if the information about the flash is valid   */
    /*------------------------------------------------------*/
    if ((pDeviceInfoEntry->u32MinSectorSize == 0) ||
        (pDeviceInfoEntry->u32SectorCount == 0))
    {
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_FLASH_INFO_ERROR);
        return FLASH_DEVICE_ERROR;
    }

    u32StartAddress = pDeviceInfoEntry->u32StartAddress;

    if (u32FlashOperationType == DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE)
    {
        /* Verify board has Tx Inhibit/BC Disable capability supported */
        if (TxInhibitBcDisable_GetCapability(pDeviceInfoEntry, &u32DeviceCapability) != FLASH_SUCCESS)
        {
            return FLASH_ERROR_CAPABILTY_RETRIEVAL;
        }

        if ((u32DeviceCapability & TX_INH_BC_DISABLE_MASK) != TX_INH_BC_DISABLE_MASK)
        {
            return FLASH_ERROR_CAPABILTY_NOT_SUPPORTED;
        }

        if ((u32DeviceCapability & TX_INH_429_DISABLE_MASK) == TX_INH_429_DISABLE_MASK)
        {
            bUsingTxInhibit429 = 1;
        }

        /* get the start address for the Tx Inhibit/BC Disable area */
        if (TxInhibitBcDisable_GetFlashStartAddress(pDeviceInfoEntry, &u32StartAddress) != FLASH_SUCCESS)
        {
            return FLASH_ERROR_START_ADDRESS;
        }

        /* Bits 32:16 represent 1st byte of word start address of sector where bin file must be written to */
        /* Also need to convert from a word address to a byte address */
        u32StartAddress = (u32StartAddress & (U32BIT)0x00FF0000) * sizeof(U16BIT);

        /* Tx Inhibit/BC Disable register is at offset 0x1C */
        /* The absoulte address is dependent on upon the "Board Global" base address */
        if (TxInhibitBcDisable_RetriveBrdGlobalBaseAddress(pDeviceInfoEntry, &u16BoardGlobalBaseAddress)  != FLASH_SUCCESS)
        {
            return FLASH_ERROR_START_ADDRESS;
        }
    }

    /*------------------------------------------------------*/
    /* is the flash write protected?                        */
    /*------------------------------------------------------*/
    s16WriteProtectionStatus = GetFlashWriteProtection(pDeviceInfoEntry);

    if(s16WriteProtectionStatus != FLASH_STATUS_WRITE_PROTECT_DISABLED)
    {
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_WRITE_PROTECT_ERROR);

        return FLASH_DEVICE_ERROR;
    }

    /*------------------------------------------------------*/
    /* get the file name of the firmware from the user      */
    /*------------------------------------------------------*/
    s16ReturnResult = GetFileName(szBinFilename,sizeof(szBinFilename));

    if (s16ReturnResult != FLASH_SUCCESS)
    {
        return s16ReturnResult;
    }

    /* try to open the file */
    fpBinFile = fopen(szBinFilename, "rb");

    if (fpBinFile == NULL)
    {
        /* error */
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_FILE_OPEN_ERROR, szBinFilename);
        return FLASH_DEVICE_ERROR;
    }

    /* determine the file size in bytes */
    fseek(fpBinFile, 0, SEEK_END);
    u32Filesize = ftell(fpBinFile);

    /* set the file pointer back to the start */
    fseek(fpBinFile, 0, SEEK_SET);

    /*------------------------------------------------------*/
    /*
        - calculate how many flash blocks the file will occupy
        - increment by 1 to take care of file sizes less than size of 1 sector
    */
    /*------------------------------------------------------*/

    u32SectorsToErase = (u32Filesize/(pDeviceInfoEntry->u32MinSectorSize) + 1) ;

    /* verify sectors to erase not larger than actual physical sectors allotted by the firmware */
    if (u32SectorsToErase > pDeviceInfoEntry->u32SectorCount)
    {
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_SECTOR_ERROR, u32SectorsToErase, pDeviceInfoEntry->u32SectorCount);
        fclose(fpBinFile);
        return FLASH_DEVICE_ERROR;
    }

    /*------------------------------------------------------*/
    /* check if data archive number is 00000                */
    /*------------------------------------------------------*/
    if (pDeviceInfoEntry->u32DataArchiveNumber == 0)
    {
        /* since the number is 00000, we can not guarantee the user is using the correct file */
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_DATA_ARCHIVE_WARNING, szBinFilename, pDeviceInfoEntry->sHwVersionInfo.szModelName);
    }
    else
    {
        /*--------------------------------------------------------------------------*/
        /* verify file data archive number matches the device data archive number   */
        /*--------------------------------------------------------------------------*/
        s16ReturnResult = VerifyFile(pDeviceInfoEntry, pLookupTable, fpBinFile, szBinFilename, u32Filesize, u32FlashOperationType);

        switch (s16ReturnResult)
        {
            case FLASH_SUCCESS:
                /* file is for this device, so continue on */
                break;

            case FLASH_ERROR_VERIFY_FILENAME:
                /* let the user know that the file can not be verified that it is for this device */
                DisplayMessage(ERROR_LEVEL_WARNING, IDS_VERIFY_FILENAME_WARNING, szBinFilename, pDeviceInfoEntry->sHwVersionInfo.szModelName);

                /* reset the return result */
                s16ReturnResult = FLASH_SUCCESS;
                break;

            /* error cases: do not allow the bin file to be written */
            case FLASH_ERROR_VERIFY_FILE_SIZE:
                DisplayMessage(ERROR_LEVEL_ERROR, IDS_VERIFY_FILE_SIZE_ERROR);
                fclose(fpBinFile);
                return s16ReturnResult;

            case FLASH_ERROR_VERIFY_MISMATCH_DA_NUMBER:
                DisplayMessage(ERROR_LEVEL_ERROR, IDS_VERIFY_MISMATCH_DA_ERROR);
                fclose(fpBinFile);
                return s16ReturnResult;

            case FLASH_ERROR_VERIFY_MISMATCH_DEVICE_ID:
                DisplayMessage(ERROR_LEVEL_ERROR, IDS_VERIFY_MISMATCH_DEVICE_ID_ERROR);
                fclose(fpBinFile);
                return s16ReturnResult;

            case FLASH_ERROR_VERIFY_UNKNOWN_DA_NUMBER:
                DisplayMessage(ERROR_LEVEL_ERROR, IDS_VERIFY_UNKNOWN_DA_ERROR);
                fclose(fpBinFile);
                return s16ReturnResult;

            case FLASH_ERROR_VERIFY_FILE_HEADER:
                DisplayMessage(ERROR_LEVEL_ERROR, IDS_VERIFY_FILE_HEADER_ERROR);
                fclose(fpBinFile);
                return s16ReturnResult;

            default:
                DisplayMessage(ERROR_LEVEL_ERROR, IDS_VERIFY_UNKNOWN_ERROR, s16ReturnResult);
                fclose(fpBinFile);
                return FLASH_ERROR_UNKNOWN;
        }
    }

    /*------------------------------------------------------*/
    /* display flash information                            */
    /*------------------------------------------------------*/
    PrintFlashInformation(szBinFilename, u32Filesize, pDeviceInfoEntry);

    /*------------------------------------------------------*/
    /* verify user is ready to flash the device             */
    /*------------------------------------------------------*/

    /* make sure user is ready to flash the device */
    u8Response = IsUserReadyToFlash();

    if (u8Response == RESPONSE_NO)
    {
        fclose(fpBinFile);
        return FLASH_ERROR_ABORT;
    }

    /*------------------------------------------------------*/
    /* allocate memory for the data buffers                 */
    /*------------------------------------------------------*/
    pBuffer = (char*) malloc (sizeof(char) * DATA_BUFFER_BYTE_SIZE);

    if (pBuffer == NULL)
    {
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_MEMORY_ALLOCATION_ERROR);
        fclose(fpBinFile);
        return FLASH_DEVICE_ERROR;
    }

    pVerifyBuffer = (char*) malloc (sizeof(char) * DATA_BUFFER_BYTE_SIZE);

    if (pVerifyBuffer == NULL)
    {
        DisplayMessage(ERROR_LEVEL_ERROR, IDS_MEMORY_ALLOCATION_ERROR);
        free(pBuffer);
        fclose(fpBinFile);
        return FLASH_DEVICE_ERROR;
    }

    /*------------------------------------------------------*/
    /* erase the flash                                      */
    /*------------------------------------------------------*/
    u32Result = EraseFlashSectors(pDeviceInfoEntry, u32SectorsToErase);

    if (u32Result != FLASH_ERASE_SUCCESS)
    {
        fclose(fpBinFile);
        return FLASH_DEVICE_ERROR;
    }

    /*------------------------------------------------------*/
    /* write and verify data to the flash                   */
    /*------------------------------------------------------*/
    do
    {
        /* read a block of data from the bin file */
        u32BytesRead = (U32BIT)fread(pBuffer, 1, DATA_BUFFER_BYTE_SIZE, fpBinFile);

        /* only display progress every so often... */
        if ((u32TotalBytesRead % DISPLAY_INTERVAL) == 0)
        {
            PrintFlashWritingProgress(u32TotalBytesRead, u32Filesize, u32StartAddress);
        }

        if (u32FlashOperationType == DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE)
        {
            /* Calculate absoulute address of "Tx Inhibit/BC Disable" register location by adding */
            /* Global Board Base address to the register address offset */
            u16TempHold = REG_BOARD_TX_INH_BC_DISABLE_OFFSET + u16BoardGlobalBaseAddress;

            pBuffer[CONTROL_ADDRESS_HI] = (u16TempHold >> BYTE_SHIFT) & 0x00FF;
            pBuffer[CONTROL_ADDRESS_LO] = u16TempHold & 0x00FF;

            if (bUsingTxInhibit429)
            {
                /* Check to see if using old file format, if so then skip */
                if (u32BytesRead > 0x12)
                {
                    u16TempHold = REG_BOARD_429_TX_INHIBIT_OFFSET + u16BoardGlobalBaseAddress;
                    pBuffer[CONTROL_429_ADDRESS_HI] = (u16TempHold >> BYTE_SHIFT) & 0x00FF;
                    pBuffer[CONTROL_429_ADDRESS_LO] = u16TempHold & 0x00FF;
                }
            }
            else
            {
                pBuffer[CONTROL_TERMINATE_HI] = (char)0xc0;
                pBuffer[CONTROL_TERMINATE_LO] = (char)0xde;
                pBuffer[CONTROL_TERMINATE_ZERO] = 0x00;
                u32BytesRead = 0x12;
                u32Filesize = 0x12;
            }
        }

        /* verify the flash is area is erased */
        u32Result = VerifyFlashErased(pDeviceInfoEntry, u32StartAddress, DATA_BUFFER_BYTE_SIZE, pVerifyBuffer);

        if (u32Result != FLASH_VERIFY_ERASE_SUCCESS)
        {
            /* error */
            s16ReturnResult = FLASH_DEVICE_ERROR;
            break;
        }

        /* write the data to the flash */
        u32Result = WriteFlashMemory(pDeviceInfoEntry, u32StartAddress, u32BytesRead, pBuffer);

        if (u32Result != FLASH_WRITE_SUCCESS)
        {
            /* error */
            s16ReturnResult = FLASH_DEVICE_ERROR;
            break;
        }

        /* verify the data that was written */
        u32Result = VerifyFlashProgrammed(pDeviceInfoEntry, u32StartAddress, u32BytesRead, pBuffer, pVerifyBuffer);

        if (u32Result != FLASH_VERIFY_PROGRAMMED_SUCCESS)
        {
            /* error */
            s16ReturnResult = FLASH_DEVICE_ERROR;
            break;
        }

        /* update counters */
        u32StartAddress += u32BytesRead;
        u32TotalBytesRead += u32BytesRead;

    } while (u32TotalBytesRead < u32Filesize);

    PrintFlashWritingProgress(u32TotalBytesRead, u32Filesize, u32StartAddress);

    /*------------------------------------------------------*/
    /* free up the memory                                   */
    /*------------------------------------------------------*/
    free(pBuffer);
    free(pVerifyBuffer);

    fclose(fpBinFile);

    return s16ReturnResult;
#else
    /* do not use file I/O */
    return FLASH_DEVICE_ERROR;
#endif /* DDCCM_USE_FILE_SYSTEM */
}

/*******************************************************************************
 * Name:    GetDevInfo
 *
 * Description:
 *      This function get the device information.
 *
 * In       hDev        Device handle
 * In/Out   pDevInfo    Pointer to the Device Info structure
 *
 * Returns: Error condition
 ******************************************************************************/
DDCCM_ERROR_TYPE ddccmGetDevInfo
(
    int hDev,
    DEV_INFO *pDevInfo
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    IoCtlParams.Channel = 0;

    if (!ddccmDeviceIoControl(
            hDev,
            IOCTL_GET_DEV_INFO,
            &IoCtlParams,
            sizeof(IoCtlParams),
            pDevInfo,
            sizeof(*pDevInfo),
            &Bytes,
            NULL))
    {
        /* not supported - clear out the information */
        memset((void *)pDevInfo, 0, sizeof(*pDevInfo));
        return DDCCM_FAILURE;
    }

    /* Make sure the device ID is only 16 bits */
    pDevInfo->dwDeviceID &= 0x0000FFFF;

    return DDCCM_SUCCESS;
}

/*******************************************************************************
 * Name:    ddccmGetHwVersionInfo
 *
 * Description:
 *      This function get the hardware version information.
 *
 * In       hDev            Device handle
 * In/Out   pHwVersionInfo  Pointer to the Hardware Version Info structure
 * In       bFamily         Family type
 *
 * Returns: Error condition
 ******************************************************************************/
DDCCM_ERROR_TYPE ddccmGetHwVersionInfo
(
    int hDev,
    PHWVERSIONINFO pHwVersionInfo,
    unsigned char bFamily
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    if (!ddccmDeviceIoControl(
            hDev,
            IOCTL_GET_HW_VERSION_INFO,
            &IoCtlParams,
            sizeof(IoCtlParams),
            pHwVersionInfo,
            sizeof(*pHwVersionInfo),
            &Bytes,
            NULL))
    {
        /* not supported - clear out the information */
        memset((void *)pHwVersionInfo, 0, (int)sizeof(*pHwVersionInfo));
        return DDCCM_FAILURE;
    }

    switch (bFamily)
    {
        case FAMILY_MIL_STD_1553:
        {
            pHwVersionInfo->dwFamilyNumber = DDC_GET_1553_FAMILY_NUMBER(pHwVersionInfo->dwFamilyNumber);
            break;
        }

        case FAMILY_ARINC_429:
        {
            pHwVersionInfo->dwFamilyNumber = DDC_GET_429_FAMILY_NUMBER(pHwVersionInfo->dwFamilyNumber);
            break;
        }
    }

    return DDCCM_SUCCESS;
}

/*******************************************************************************
 * Name:    ddccmGetFlashInfo
 *
 * Description:
 *      This function get the flash information.
 *
 * In       hDev            Device handle
 * In/Out   pFlashInfo      Pointer to the Flash Info structure
 *
 * Returns: Error condition
 ******************************************************************************/
DDCCM_ERROR_TYPE ddccmGetFlashInfo
(
    int hDev,
    FLASH_INFO *pFlashInfo
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    if (!ddccmDeviceIoControl(
            hDev,
            IOCTL_FLASH_MEM_INFO,
            &IoCtlParams,
            sizeof(IoCtlParams),
            pFlashInfo,
            sizeof(*pFlashInfo),
            &Bytes,
            NULL))
    {
        /* not supported - clear out the information */
        memset((void *)pFlashInfo, 0, (int)sizeof(*pFlashInfo));
        return DDCCM_FAILURE;
    }

    return DDCCM_SUCCESS;
}

/*******************************************************************************
 * Name:    ddccmGetEnhancedCapabilities
 *
 * Description:
 *      This function get the enhanced capabilities of the card. If the card
 *      does not support this IOCTL, the standard capabilities IOCTLL will be
 *      called. Note that the standard capabilities structure does not have
 *      programmable channel counts.
 *
 * In       hDev            Device handle
 * In/Out   pCapabilities   Pointer to the card capabilities
 *
 * Returns: Error condition
 ******************************************************************************/
DDCCM_ERROR_TYPE ddccmGetEnhancedCapabilities
(
    int hDev,
    PENHANCED_CAPABILITY_INFO pEnhancedCardCapabilities
)
{
    DDC_IOCTL_PARAMS       IoCtlParams;
    U32BIT u32Capabilities = 0;
    U32BIT Bytes;

    memset((void *)pEnhancedCardCapabilities, 0, (int)sizeof(*pEnhancedCardCapabilities));

    if (!ddccmDeviceIoControl(
            hDev,
            IOCTL_GET_ENHANCED_CAPABILITIES,
            &IoCtlParams,
            sizeof(IoCtlParams),
            pEnhancedCardCapabilities,
            sizeof(*pEnhancedCardCapabilities),
            &Bytes,
            NULL))
    {
        /* not supported, try the standard capabilities IOCTL */
        if (!ddccmDeviceIoControl(
                hDev,
                IOCTL_GET_CAPABILITIES,
                &IoCtlParams,
                sizeof(IoCtlParams),
                &u32Capabilities,
                sizeof(u32Capabilities),
                &Bytes,
                NULL))
        {
            /* not supported */
            return DDCCM_FAILURE;
        }

        pEnhancedCardCapabilities->channelCount1553   =(U8BIT)(((u32Capabilities & 0x02000000) >> 22) | (u32Capabilities & 0x00000007));
        pEnhancedCardCapabilities->channelCount429Rx  = (U8BIT)((u32Capabilities & 0x000000F8) >> 3);
        pEnhancedCardCapabilities->channelCount429Tx  =(U8BIT)(((u32Capabilities & 0x04000000) >> 23) | ((u32Capabilities & 0x00000700) >> 8));
        pEnhancedCardCapabilities->channelCountAIO    = (U8BIT)((u32Capabilities & 0x01F00000) >> 20);
        pEnhancedCardCapabilities->channelCountDIO    = (U8BIT)((u32Capabilities & 0x000F8000) >> 15);
        pEnhancedCardCapabilities->channelCountRS232  = (U8BIT)((u32Capabilities & 0x00001800) >> 11);
        pEnhancedCardCapabilities->channelCountRS485  = (U8BIT)((u32Capabilities & 0x00006000) >> 13);
        pEnhancedCardCapabilities->modelNumber        = (U8BIT)((u32Capabilities & 0xF0000000) >> 28);
    }

    return DDCCM_SUCCESS;
}
