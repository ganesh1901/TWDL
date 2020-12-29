/******************************************************************************
* Data Device Corporation
*
* Filename: strtable.h
*
*
* Copyright (c) 2009 Data Device Corporation
******************************************************************************
* Non-Disclosure Statement
* ------------------------
* This software is the sole property of Data Device Corporation. All
* rights, title, ownership, or other interests in the software remain
* the property of Data Device Corporation. This software may be used
* in accordance with applicable licenses. Any unauthorized use,
* duplication, transmission, distribution, or disclosure is expressly
* forbidden.
*
* This non-disclosure statement may not be removed or modified without
* prior written consent of Data Device Corporation.
******************************************************************************
* Module Description:
*
* The purpose of this module is define the strings used in the flash appcation.
*
*****************************************************************************/
#ifndef __STRTABLE_H__
#define __STRTABLE_H__

#define IDS_NEWLINE_X1                          "\n"
#define IDS_NEWLINE_X2                          "\n\n"
#define IDS_PRESS_A_KEY                         "\n\nPress <ENTER> to continue..."
#define IDS_ERROR_CAPTION                       "\n\n(Error) "
#define IDS_WARNING_CAPTION                     "\n\n(Warning) "
#define IDS_DEVICE_WARNING                      "\n" \
                                                "   ===============\n" \
                                                "   !!! Warning !!!\n" \
                                                "   ===============\n" \
                                                "\n" \
                                                "   BIN file specified can not be guaranteed that it is for this device!\n" \
                                                "   Are you sure you want to flash the selected device?\n"

#define IDS_DEVICE_ERROR_HEADER                 "\n" \
                                                "   ===============\n" \
                                                "   !!!  ERROR  !!!\n" \
                                                "   ===============\n" \
                                                "\n"
#define IDS_DEVICE_ERROR_WITH_NAME              "   BIN file specified is for a %s device.\n"
#define IDS_DEVICE_ERROR_WITHOUT_NAME           "   The file can not be flashed to this device.\n"

#define IDS_INFO_DEVICE                         "   Device:                 %s\n"
#define IDS_INFO_FILENAME                       "   File Name:              %s\n"
#define IDS_INFO_FILE_SIZE                      "   File Size:              %d bytes\n"
#define IDS_PROGRAMMING_PROGRESS                "   Programming %3d%%   0x%08X\n"
#define IDS_READY_TO_FLASH                      "\n" \
                                                "   Are you sure you want to flash the selected device?\n" \
                                                "\n"
#define IDS_ENTER_Y_N                           "   Enter 'y' or 'n': "
#define IDS_ENTER_FILENAME                      "Enter the filename to program: "
#define IDS_UNABLE_TO_OPEN_DRIVER               "Unable to open driver for device %s\n"
#define IDS_PID                                 "pid_"
#define IDS_DEV                                 "dev_"
#define IDS_ERASE_FLASH_PROGRESS                "   Erasing Flash Sector: %2d / %d\n"
#define IDS_ERASE_FAILURE                       "Failed to erase the flash (%d)\n"
#define IDS_WRITE_FAILURE                       "Failed to write to flash (%d)\n"
#define IDS_ERASE_VERIFY_FAILURE_IOCTL          "Failed to verify to flash was erased (%d)\n"
#define IDS_ERASE_VERIFY_FAILURE                "Flash Not Erased - Address: 0x%08X   Read: 0x%02X   Expected: 0x%02X\n\n"
#define IDS_PROGRAMMED_FAILURE_IOCTL            "Failed to verify to flash was programmed (%d)\n"
#define IDS_PROGRAMMED_FAILURE                  "Flash Not Programmed - Address: 0x%08X   Read: 0x%02X   Expected: 0x%02X\n\n"
#define IDS_FLASH_INFO_ERROR                    "Flash information not valid\n"
#define IDS_WRITE_PROTECT_ERROR                 "Flash memory is write protected.\n" \
                                                "\n" \
                                                "   Please refer to the user's manual for your device\n" \
                                                "   on how to configure the flash write enable jumper.\n"
#define IDS_FILE_OPEN_ERROR                     "Could not open \"%s\"\n"
#define IDS_SECTOR_ERROR                        "Required sector count too large.\n" \
                                                "\n" \
                                                "           Sectors required....... %d\n" \
                                                "           Flash Sector Count..... %d\n\n"
#define IDS_DATA_ARCHIVE_WARNING                "BIN file specified can not be guaranteed that it is for this device!\n" \
                                                "Verify the file chosen is correct.\n" \
                                                "\n" \
                                                "Bin File:   %s\n" \
                                                "Device:     %s\n"
#define IDS_VERIFY_FILENAME_WARNING             "The bin file specified can not be verified that it is for this device.\n" \
                                                "Verify the file chosen is correct.\n" \
                                                "\n" \
                                                "Bin File:   %s\n" \
                                                "Device:     %s\n"
#define IDS_VERIFY_FILE_SIZE_ERROR              "The bin file size is incorrect!\n"
#define IDS_VERIFY_MISMATCH_DA_ERROR            "The bin file specified is not for this device!\n"
#define IDS_VERIFY_MISMATCH_DEVICE_ID_ERROR     "The bin file specified is not for this device!\n"
#define IDS_VERIFY_UNKNOWN_DA_ERROR             "The bin file specified is for an unknon device!\n"
#define IDS_VERIFY_FILE_HEADER_ERROR            "The bin file specified is invalid!\n"
#define IDS_VERIFY_UNKNOWN_ERROR                "Unknown error (%d)!\n"
#define IDS_MEMORY_ALLOCATION_ERROR             "Memory allocation failed\n"
#define IDS_LOOKUP_TABLE_SCAN_LINE              "%d,%x,%x,%d,%d,%d,%s"

#endif /* __STRTABLE_H__ */
