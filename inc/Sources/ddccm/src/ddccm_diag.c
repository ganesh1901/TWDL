/*******************************************************************************
 * FILE: ddccm_diag.c
 *
 * DESCRIPTION:
 *
 *  DDC Card Manager diagnostic routines.
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

#define SELECTION_EXIT                                      0
#define MAX_ROWS                                            8
#define MAX_COLS                                            4

#define REG_TYPE_ABSOLUTE_ADDR                              0
#define REG_TYPE_BD_OFFSET                                  1
#define REG_TYPE_BC_OFFSET                                  2
#define REG_TYPE_MRT_OFFSET                                 3
#define REG_TYPE_MTI_OFFSET                                 4
#define REG_TYPE_1553_OFFSET                                5
#define REG_TYPE_EI_OFFSET                                  6
#define REG_TYPE_REPLAY_OFFSET                              7
#define REG_TYPE_TRG_OFFSET                                 8


#define UM_DEVICE_ID_CAPABILITIES                           0x0100
#define UM_DEVICE_ID_BRD_GLOBAL                             0x0200
#define UM_DEVICE_ID_MIL_STD_1553                           0x0300
#define UM_DEVICE_ID_ARINC_429_GLOBAL                       0x0400
#define UM_DEVICE_ID_ARINC_429_TX                           0x0500
#define UM_DEVICE_ID_ARINC_429_RX                           0x0600
#define UM_DEVICE_ID_IRIG_B                                 0x0700
#define UM_DEVICE_ID_MIL_STD_1553_SF                        0x0800
#define UM_DEVICE_ID_MIL_STD_1553_MF                        0x0900
#define UM_DEVICE_ID_MIO_UART                               0x0A00
#define UM_DEVICE_ID_MIO_CAST_GLOBAL_UART                   0x0A10
#define UM_DEVICE_ID_MIO_CAST_ASYNC_UART                    0x0A20
#define UM_DEVICE_ID_MIO_CAST_SYNC_ASYNC_UART               0x0A30
#define UM_DEVICE_ID_ARINC_429_GLOBAL_TX_MF                 0x0B00
#define UM_DEVICE_ID_ARINC_429_GLOBAL_RX_MF                 0x0B10
#define UM_DEVICE_ID_ARINC_429_TX_MF                        0x0C00
#define UM_DEVICE_ID_ARINC_429_RX_MF                        0x0D00
#define UM_DEVICE_ID_CAN_BUS                                0x0E00
#define UM_DEVICE_ID_ARINC_717_GLOBAL                       0x0F00
#define UM_DEVICE_ID_ARINC_717_RX_TX                        0x1000

#define UM_COMPONENTS_ID_CAPABILITIES                       0x0101

#define UM_COMPONENTS_ID_BRD_GLOBAL                         0x0201
#define UM_COMPONENTS_ID_BRD_RESETS                         0x0202
#define UM_COMPONENTS_ID_BRD_MEM_CONFIG                     0x0203

#define UM_COMPONENTS_ID_BRD_GLOBAL_VOLT_MON_X8             0x0220

#define UM_COMPONENTS_ID_BRD_GLOBAL_THERMAL_MON             0x0230

#define UM_COMPONENTS_ID_QPRIME_GENERAL                     0x0210
#define UM_COMPONENTS_ID_QPRIME_DMA                         0x0211
#define UM_COMPONENTS_ID_QPRIME_BIST                        0x0212

#define UM_COMPONENTS_ID_MIL_STD_1553_SF_GLOBAL             0x0301
#define UM_COMPONENTS_ID_MIL_STD_1553_SF_BCI                0x0302
#define UM_COMPONENTS_ID_MIL_STD_1553_SF_RTX                0x0303
#define UM_COMPONENTS_ID_MIL_STD_1553_SF_MTIE               0x0304
#define UM_COMPONENTS_ID_MIL_STD_1553_SF_IMP                0x0305

#define UM_COMPONENTS_ID_ARINC_429_GLOBAL_TX                0x0401
#define UM_COMPONENTS_ID_ARINC_429_GLOBAL_RX                0x0402

#define UM_COMPONENTS_ID_ARINC_429_TX                       0x0501

#define UM_COMPONENTS_ID_ARINC_429_RX                       0x0601

#define UM_COMPONENTS_ID_IRIG_B_RX                          0x0701
#define UM_COMPONENTS_ID_IRIG_B_TX                          0x0702

#define UM_COMPONENTS_ID_MIL_STD_1553_MF_GLOBAL             0x0901
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_BCI                0x0902
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_RTX                0x0903
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_MTIE               0x0904
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_IMP                0x0905
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_IMP_BC             0x0906
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_IMP_MRT            0x0907
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_EI                 0x0908
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_REPLAY		        0x0909
#define UM_COMPONENTS_ID_MIL_STD_1553_MF_TRIGGERS           0x090A

#define UM_COMPONENTS_ID_MIO_UART                           0x0A01
#define UM_COMPONENTS_ID_MIO_CAST_UART_GLOBAL               0x0A11
#define UM_COMPONENTS_ID_MIO_CAST_UART_SERIAL               0x0A21
#define UM_COMPONENTS_ID_MIO_CAST_UART_HDLC                 0x0A31
#define UM_COMPONENTS_ID_MIO_CAST_UART_SDLC                 0x0A32
#define UM_COMPONENTS_ID_MIO_CAST_UART_ASYNC                0x0A33

#define UM_COMPONENTS_ID_ARINC_429_GLOBAL_TX_MF             0x0B01
#define UM_COMPONENTS_ID_ARINC_429_GLOBAL_RX_MF             0x0B11

#define UM_COMPONENTS_ID_ARINC_429_TX_MF                    0x0C01
#define UM_COMPONENTS_ID_ARINC_429_RX_MF                    0x0D01

#define UM_COMPONENTS_ID_CAN_BUS                            0x0E01

#define UM_COMPONENTS_ID_ARINC_717_PROG_CH_GLOBAL           0x0F01

#define UM_COMPONENTS_ID_ARINC_717_PROG_CH                  0x1001


typedef struct _UM_ROM_LOOKUP_TABLE
{
    char name[64];
    U16BIT id;

} UM_ROM_LOOKUP_TABLE;

UM_ROM_LOOKUP_TABLE umRomLookupTable[] =
{
    { "UM_DEVICE_ID_CAPABILITIES", UM_DEVICE_ID_CAPABILITIES },
    { "UM_DEVICE_ID_BRD_GLOBAL", UM_DEVICE_ID_BRD_GLOBAL },
    { "UM_DEVICE_ID_MIL_STD_1553", UM_DEVICE_ID_MIL_STD_1553 },
    { "UM_DEVICE_ID_ARINC_429_GLOBAL", UM_DEVICE_ID_ARINC_429_GLOBAL },
    { "UM_DEVICE_ID_ARINC_429_TX", UM_DEVICE_ID_ARINC_429_TX },
    { "UM_DEVICE_ID_ARINC_429_RX", UM_DEVICE_ID_ARINC_429_RX },
    { "UM_DEVICE_ID_IRIG_B", UM_DEVICE_ID_IRIG_B },
    { "UM_DEVICE_ID_MIL_STD_1553_SF", UM_DEVICE_ID_MIL_STD_1553_SF },
    { "UM_DEVICE_ID_MIL_STD_1553_MF", UM_DEVICE_ID_MIL_STD_1553_MF },
    { "UM_DEVICE_ID_MIO_UART", UM_DEVICE_ID_MIO_UART },
    { "UM_DEVICE_ID_ARINC_429_GLOBAL_TX_MF", UM_DEVICE_ID_ARINC_429_GLOBAL_TX_MF },
    { "UM_DEVICE_ID_ARINC_429_GLOBAL_RX_MF", UM_DEVICE_ID_ARINC_429_GLOBAL_RX_MF },
    { "UM_DEVICE_ID_ARINC_429_TX_MF", UM_DEVICE_ID_ARINC_429_TX_MF },
    { "UM_DEVICE_ID_ARINC_429_RX_MF", UM_DEVICE_ID_ARINC_429_RX_MF },
    { "UM_DEVICE_ID_CAN_BUS", UM_DEVICE_ID_CAN_BUS },
    { "UM_DEVICE_ID_ARINC_717_GLOBAL", UM_DEVICE_ID_ARINC_717_GLOBAL },
    { "UM_DEVICE_ID_ARINC_717_RX_TX", UM_DEVICE_ID_ARINC_717_RX_TX },
    { "CAPABILITIES_REG_BASE_ADDR_INST", UM_COMPONENTS_ID_CAPABILITIES },
    { "BRD_GLOBAL_GENERAL_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_BRD_GLOBAL },
    { "BRD_GLOBAL_RESETS_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_BRD_RESETS },
    { "BRD_GLOBAL_SW_LOCK_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_BRD_MEM_CONFIG },
    { "BRD_GLOBAL_VOLT_MON_X8_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_BRD_GLOBAL_VOLT_MON_X8 },
    { "BRD_GLOBAL_THERMAL_MON_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_BRD_GLOBAL_THERMAL_MON },
    { "QPRIME_GENERAL_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_QPRIME_GENERAL },
    { "QPRIME_DMA_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_QPRIME_DMA },
    { "QPRIME_BIST_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_QPRIME_BIST },
    { "MIL_STD_1553_GLOBAL_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_SF_GLOBAL },
    { "MIL_STD_1553_BCi_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_SF_BCI },
    { "MIL_STD_1553_MRTi_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_SF_RTX },
    { "MIL_STD_1553_MTiE_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_SF_MTIE },
    { "MIL_STD_1553_IMP_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_SF_IMP },
    { "ARINC_429_GLOBAL_TX_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_GLOBAL_TX },
    { "ARINC_429_GLOBAL_RX_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_GLOBAL_RX },
    { "ARINC_429_TX_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_TX },
    { "ARINC_429_RX_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_RX },
    { "IRIG_B_RX_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_IRIG_B_RX },
    { "IRIG_B_TX_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_IRIG_B_TX },
    { "MIL_STD_1553_MF_GLOBAL_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_GLOBAL },
    { "MIL_STD_1553_MF_BCI_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_BCI },
    { "MIL_STD_1553_MF_RTX_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_RTX },
    { "MIL_STD_1553_MF_MTIE_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_MTIE },
    { "MIL_STD_1553_MF_IMP_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_IMP },
    { "MIL_STD_1553_MF_IMP_BC_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_IMP_BC },
    { "MIL_STD_1553_MF_IMP_MRT_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_IMP_MRT },
    { "MIL_STD_1553_MF_EI_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_EI },
    { "MIL_STD_1553_MF_REPLAY_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_REPLAY	 },
    { "MIL_STD_1553_MF_TRIGGERS_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIL_STD_1553_MF_TRIGGERS },
    { "MIO_UART_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_MIO_UART },
    { "UM_DEVICE_ID_MIO_CAST_GLOBAL_UART", UM_DEVICE_ID_MIO_CAST_GLOBAL_UART },
    { "UM_COMPONENTS_ID_MIO_UART", UM_COMPONENTS_ID_MIO_UART },
    { "UM_COMPONENTS_ID_MIO_CAST_UART_GLOBAL", UM_COMPONENTS_ID_MIO_CAST_UART_GLOBAL},
    { "UM_COMPONENTS_ID_MIO_CAST_UART_SERIAL", UM_COMPONENTS_ID_MIO_CAST_UART_SERIAL},
    { "UM_COMPONENTS_ID_MIO_CAST_UART_HDLC", UM_COMPONENTS_ID_MIO_CAST_UART_HDLC },
    { "UM_COMPONENTS_ID_MIO_CAST_UART_SDLC", UM_COMPONENTS_ID_MIO_CAST_UART_SDLC },
    { "UM_COMPONENTS_ID_MIO_CAST_UART_ASYNC", UM_COMPONENTS_ID_MIO_CAST_UART_ASYNC },
    { "UM_DEVICE_ID_MIO_CAST_ASYNC_UART", UM_DEVICE_ID_MIO_CAST_ASYNC_UART },
    { "UM_DEVICE_ID_MIO_CAST_SYNC_ASYNC_UART", UM_DEVICE_ID_MIO_CAST_SYNC_ASYNC_UART },
    { "ARINC_429_GLOBAL_TX_MF_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_GLOBAL_TX_MF },
    { "ARINC_429_GLOBAL_RX_MF_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_GLOBAL_RX_MF },
    { "ARINC_429_TX_MF_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_TX_MF },
    { "ARINC_429_RX_MF_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_429_RX_MF },
    { "CAN_BUS_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_CAN_BUS },
    { "ARINC_717_PROG_CH_GLOBAL_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_717_PROG_CH_GLOBAL },
    { "ARINC_717_PROG_CH_%s_BASE_ADDR_INST", UM_COMPONENTS_ID_ARINC_717_PROG_CH },
};

#define UM_MAX_NUM_COMPONENTS                               40      /* total component number per device */
#define UM_MAX_NUM_INSTANCES                                50      /* total channel number per FPGA */
#define UM_MAX_NUM_DEVICES                                  20      /* total device number per card */

typedef struct _UM_COMPONENT_INFO
{
    U32BIT  umComponentType;
    char    umName[64];
    U32BIT  umComponentRev;
    U32BIT  umComponentRegSize;
    U32BIT  umRegBaseAddr[UM_MAX_NUM_INSTANCES];

} UM_COMPONENT_INFO, *PUM_COMPONENT_INFO;


typedef struct _UM_DEVICE_INFO
{
    U32BIT  umDevType;
    char    umName[64];
    U32BIT  umDevRev;
    U32BIT  umDevMemSize;   /* in 32bit */
    U32BIT  umDevNumInstances;
    U32BIT  umMemBaseAddr[UM_MAX_NUM_INSTANCES]; /* in 32bit */
    U32BIT  umDevNumComponents;
    UM_COMPONENT_INFO umComponentInfo[UM_MAX_NUM_COMPONENTS];

} UM_DEVICE_INFO, *PUM_DEVICE_INFO;


typedef struct _UM_INFO
{
    char    s8BrdModelNum[10];
    U32BIT  u32DataArcNum;
    U32BIT  firmwareRelVersion;
    U32BIT  firmwareIntVersion;
    U32BIT  numDevices;
    UM_DEVICE_INFO umDeviceInfo[UM_MAX_NUM_DEVICES];

} UM_INFO, *PUM_INFO;

#define ACCESS_TYPE_REG             0
#define ACCESS_TYPE_MEM             1

#define REG_MEM_ADDR_BLOCK_SIZE     0x20

#define REG_TYPE_ABSOLUTE_ADDR      0
#define REG_TYPE_BD_OFFSET          1
#define REG_TYPE_BC_OFFSET          2
#define REG_TYPE_MRT_OFFSET         3
#define REG_TYPE_MTI_OFFSET         4
#define REG_TYPE_1553_OFFSET        5
#define REG_TYPE_EI_OFFSET          6
#define REG_TYPE_REPLAY_OFFSET      7
#define REG_TYPE_TRG_OFFSET         8

static U32BIT u32Selection = 0;

static U32BIT u32DumpRegStartAddr = 0x00000000;
static U32BIT u32SingleRegStartAddr = 0x00000000;

static U32BIT u32DumpMemStartAddr = 0x00000000;
static U32BIT u32SingleMemStartAddr = 0x00000000;

static U32BIT u32BcRawDecodeStartAddr = 0x00000000;
static U32BIT u32BcRawDecodeEndAddr = 0x00000000;

static char regOffsetTypeStr[][16] =
{
    { "0 - Absolute" },
    { "1 - Board" },
    { "2 - BC" },
    { "3 - MRT" },
    { "4 - MTI" },
    { "5 - 1553" },
    { "6 - EI" },
    { "7 - Replay" },
    { "8 - Trigger" },
};

/* ========================================================================== */
/*                            FUNCTION PROTOTYPES                             */
/* ========================================================================== */
extern void ddccmPressAKey
(
    void
);

extern U8BIT Get32BitVal
(
    U32BIT *val
);

static void ddccmDiagDumpUmRom
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
);

static void ddccmDiagDumpRegMem
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    int nAccessType
);

static void ddccmDiagDumpRegistersMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
);

static void ddccmDiagReadWriteRegisterMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
);

static void ddccmDiagDumpMemoryMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
);

static void ddccmDiagReadWriteMemoryMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
);

static void ddccmDiagBcOpcodeDecoderMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
);

/*******************************************************************************
 * Name:    PrintDiagDeviceList
 *
 * Description:
 *      This function will print out all devices found to the display.
 *
 * In   pDeviceInfo             pointer to the head of the device info list
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void PrintDiagDeviceList
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo
)
{
	U16BIT i;

    printf("----------------------------------------------------------------\n");
    printf(" Device  Name            Model  FW ID\n");
    printf("----------------------------------------------------------------\n");

    for (i=0; i<pDeviceInfo->u16NumberOfDevices; i++)
    {
        printf("   ");
        printf(" %-2d", i + 1); /* display index # starting at 1 */
        printf("   %-16s  %-2d   %05d  %s",
            pDeviceInfo->entry[i].sHwVersionInfo.szModelName,
            pDeviceInfo->entry[i].sEnhancedCardCapabilities.modelNumber,
            pDeviceInfo->entry[i].u32DataArchiveNumber,
            (pDeviceInfo->entry[i].bBackup == TRUE) ? "(Backup)" : "");
        printf("\n");
    }

    printf("----------------------------------------------------------------\n");
    printf("\n");
}

/*******************************************************************************
 * Name:    GetDiagDeviceSelection
 *
 * Description:
 *      This function will ask the user to select a device. The selection returned
 *      will always be a valid value.
 *
 * In   pDeviceInfo             pointer to the head of the device info list
 * Out  none
 *
 * Returns: valid user selection
 ******************************************************************************/
static U32BIT GetDiagDeviceSelection
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo
)
{
    U32BIT u32Value;
    U8BIT u8Valid;
    char ch;

    printf("\n");

    /* the input buffer contains leftover data from entering the last selection, so clear it out */
    while (((ch = getchar()) != '\n') && (ch != (char)EOF));

    /* loop until we get a valid selection */
    for (;;)
    {
        PrintDiagDeviceList(pDeviceInfo);

        printf("\nEnter the number of the device to use (0 to exit): ");

        u8Valid = Get32BitVal(&u32Value);

        /* make sure the value is valid and the selection is in the rage of device numbers */
        if ((u8Valid) && (u32Value <= pDeviceInfo->u16NumberOfDevices))
        {
            break;
        }

        printf("\n\n  *** Invalid Entry, please try again ***\n\n");
    }

    return u32Value;
}

/*******************************************************************************
 * Name:    ddccmDiagMenu
 *
 * Description:
 *      This function sets the logical device number of a particular device
 *      or channel.
 *
 * In   u8Family - which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
void ddccmDiagMenu
(
    U8BIT u8Family
)
{
    /* list of information for all devices */
    DEVICE_INFORMATION_TYPE deviceInfo;

    S32BIT nError;
    char szStringIn[80] = "";

    /*------------------------------------------------------*/
    /* find all known devices                               */
    /*------------------------------------------------------*/
    nError = ddccmOsFindDevices(&deviceInfo, FALSE);
    if (nError)
    {
        printf("\n\n(Error %d) Failure to detect cards\n", (int)nError);
        ddccmPressAKey();
        return;
    }

    /* were there any devices found? */
    if (deviceInfo.u16NumberOfDevices == 0)
    {
        printf("\n\n[Error) *** No devices found! ***\n");
        ddccmPressAKey();
        return;
    }

    /*------------------------------------------------------*/
    /* get the selection from the user                      */
    /*------------------------------------------------------*/
    u32Selection = GetDiagDeviceSelection(&deviceInfo);

    switch (u32Selection)
    {
        case SELECTION_EXIT:
        {
            ddccmOsFreeDevices(&deviceInfo);
            return;
        }

        default:
        {
            break;
        }
    }

    do
    {
        printf("\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| [DIAGNOSTICS] MAIN                                              |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| b  (b)c opcode decoder                                          |\n");
        printf("| g  read/write re(g)isters                                       |\n");
        printf("| r  dump (r)egisters                                             |\n");
        printf("| e  read/write m(e)mory                                          |\n");
        printf("| m  dump (m)emory                                                |\n");
        printf("| u  dump (u)mrom                                                 |\n");
        printf("| x  e(x)it application                                           |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("\n Enter command: ");

        scanf("%s", szStringIn);

        printf("\n");

        if (!strcmp(szStringIn, "g"))
        {
            /* reset start addr */
            u32SingleRegStartAddr = 0x00000000;
            ddccmDiagReadWriteRegisterMenu(&deviceInfo.entry[u32Selection-1]);
        }
        else if (!strcmp(szStringIn, "e"))
        {
            u32SingleMemStartAddr = 0x00000000;
            ddccmDiagReadWriteMemoryMenu(&deviceInfo.entry[u32Selection-1]);
        }
        else if (!strcmp(szStringIn, "m"))
        {
            /* reset start addr */
            u32DumpMemStartAddr = 0x00000000;
            ddccmDiagDumpMemoryMenu(&deviceInfo.entry[u32Selection-1]);
        }
        else if (!strcmp(szStringIn, "r"))
        {
            /* reset start addr */
            u32DumpRegStartAddr = 0x00000000;
            ddccmDiagDumpRegistersMenu(&deviceInfo.entry[u32Selection-1]);
        }
        else if (!strcmp(szStringIn, "b"))
        {
            ddccmDiagBcOpcodeDecoderMenu(&deviceInfo.entry[u32Selection-1]);
        }
        else if (!strcmp(szStringIn, "u"))
        {
            ddccmDiagDumpUmRom(&deviceInfo.entry[u32Selection-1]);
        }
        else if (!strcmp(szStringIn, "x"))
        {
            /* quit */
            ddccmOsFreeDevices(&deviceInfo);
            break;
        }
        else
        {
                printf("\n    Invalid command!\n");
        }

    } while (1);
}

/*******************************************************************************
 * Name:    ddccmDiagReadReg
 *
 * Description:
 *      This function reads a register value from the specified address.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32Addr             absolute address
 * In   u32RegOffsetType    offset type
 * Out  pu32Data            data value read
 *
 * Returns:
 ******************************************************************************/
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

static S16BIT ddccmDiagReadReg
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32Addr,
    U32BIT u32RegOffsetType,
    U32BIT *pu32Data
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    IoCtlParams.Channel = 0;
    IoCtlParams.Param1 = u32Addr;
    IoCtlParams.Param2 = u32RegOffsetType;
    IoCtlParams.Param3 = sizeof(*pu32Data);

    /* get flash write protection status */
    if (!ddccmDeviceIoControl(
        pDeviceInfoEntry->hDriver,
        IOCTL_REG_READ_BLK,
        &IoCtlParams,
        sizeof(IoCtlParams),
        pu32Data,
        IoCtlParams.Param3,
        &Bytes,
        NULL))
    {
        /* error */
        return DDCCM_FAILURE;
    }

    return DDCCM_SUCCESS;
}

/*******************************************************************************
 * Name:    ddccmDiagWriteReg
 *
 * Description:
 *      This function writes a register value from the specified address.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32Addr             absolute address
 * In   u32RegOffsetType    offset type
 * Out  u32Data             data value to write
 *
 * Returns:
 ******************************************************************************/
static S16BIT ddccmDiagWriteReg
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32Addr,
    U32BIT u32RegOffsetType,
    U32BIT u32Data
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    IoCtlParams.Channel = 0;
    IoCtlParams.Param1 = u32Addr;
    IoCtlParams.Param2 = u32RegOffsetType;
    IoCtlParams.Param3 = sizeof(u32Data);

    /* get flash write protection status */
    if (!ddccmDeviceIoControl(
        pDeviceInfoEntry->hDriver,
        IOCTL_REG_WRITE_BLK,
        &IoCtlParams,
        sizeof(IoCtlParams),
        &u32Data,
        IoCtlParams.Param3,
        &Bytes,
        NULL))
    {
        /* error */
        return DDCCM_FAILURE;
    }

    return DDCCM_SUCCESS;
}

/*******************************************************************************
 * Name:    ddccmDiagReadMem
 *
 * Description:
 *      This function reads a memory value from the specified address.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32Addr             absolute address
 * Out  pu32Data            data value read
 *
 * Returns:
 ******************************************************************************/
static S16BIT ddccmDiagReadMem
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32Addr,
    U32BIT *pu32Data
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    IoCtlParams.Channel = 0;
    IoCtlParams.Param1 = u32Addr;
    IoCtlParams.Param2 = 1; /* 32 bit mode access */
    IoCtlParams.Param3 = sizeof(*pu32Data);

    /* get flash write protection status */
    if (!ddccmDeviceIoControl(
        pDeviceInfoEntry->hDriver,
        IOCTL_MEM_READ_BLK,
        &IoCtlParams,
        sizeof(IoCtlParams),
        pu32Data,
        IoCtlParams.Param3,
        &Bytes,
        NULL))
    {
        /* error */
        return DDCCM_FAILURE;
    }

    return DDCCM_SUCCESS;
}

/*******************************************************************************
 * Name:    ddccmDiagWriteMem
 *
 * Description:
 *      This function writes a memory value from the specified address.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   u32Addr             absolute address
 * Out  pu32Data            data value write
 *
 * Returns:
 ******************************************************************************/
static S16BIT ddccmDiagWriteMem
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    U32BIT u32Addr,
    U32BIT u32Data
)
{
    DDC_IOCTL_PARAMS IoCtlParams;
    U32BIT Bytes;

    IoCtlParams.Channel = 0;
    IoCtlParams.Param1 = u32Addr;
    IoCtlParams.Param2 = 1; /* 32 bit mode access */
    IoCtlParams.Param3 = sizeof(u32Data);

    /* get flash write protection status */
    if (!ddccmDeviceIoControl(
        pDeviceInfoEntry->hDriver,
        IOCTL_MEM_WRITE_BLK,
        &IoCtlParams,
        sizeof(IoCtlParams),
        &u32Data,
        IoCtlParams.Param3,
        &Bytes,
        NULL))
    {
        /* error */
        return DDCCM_FAILURE;
    }

    return DDCCM_SUCCESS;
}

/*******************************************************************************
 * Name:    ddccmDiagDumpUmRom
 *
 * Description:
 *      This function decodes and prints the UMROM table.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagDumpUmRom
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    UM_INFO umInfo;
    U32BIT u32Addr = 0x00000000;
    U32BIT u32Data;
    U16BIT numDevicesIndex;

    /* make sure this is an ACEX device */
    if ((pDeviceInfoEntry->sHwVersionInfo.dwFamilyNumber != ACEX) &&
        (pDeviceInfoEntry->sHwVersionInfo.dwFamilyNumber != DDCFAMILY_429X))
    {
        printf("\n\n *** ERROR: This is not an ACEX device ***\n\n");
        return;
    }

    /* 1.2.1	Capabilities Board Model Num Registers (0x00-0x01) */
    ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &u32Data);
    u32Addr++;

    umInfo.s8BrdModelNum[0] = (S8BIT)(u32Data >> 24) & 0x00ff;
    umInfo.s8BrdModelNum[1] = (S8BIT)(u32Data >> 16) & 0x00ff;
    umInfo.s8BrdModelNum[2] = (S8BIT)(u32Data >> 8 ) & 0x00ff;
    umInfo.s8BrdModelNum[3] = (S8BIT)(u32Data      ) & 0x00ff;

    ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &u32Data);
    u32Addr++;

    umInfo.s8BrdModelNum[4] = (S8BIT)(u32Data >> 24) & 0x00ff;
    umInfo.s8BrdModelNum[5] = (S8BIT)(u32Data >> 16) & 0x00ff;
    umInfo.s8BrdModelNum[6] = (S8BIT)(u32Data >> 8 ) & 0x00ff;
    umInfo.s8BrdModelNum[7] = (S8BIT)(u32Data      ) & 0x00ff;
    umInfo.s8BrdModelNum[8] = 0;

    printf("#======================================================================================================\n");
    printf("# %-24s%s (%s)\n", "Model Number:", pDeviceInfoEntry->sHwVersionInfo.szModelName, umInfo.s8BrdModelNum);

    /* 1.2.2	Capabilities Data Archive Number Register(0x02) */
    ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &umInfo.u32DataArcNum);
    u32Addr++;

    printf("# %-24s%d\n", "Data Archive Number:", umInfo.u32DataArcNum);

    /* 1.2.3	Capabilities Firmware Release Version Register (0x03) */
    ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &umInfo.firmwareRelVersion);
    u32Addr++;

    /* 1.2.4	Capabilities Firmware Internal Version Register(0x04) */
    ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &umInfo.firmwareIntVersion);
    u32Addr++;

    printf("# %-24s%d.%d\n", "Firmware Revision:", umInfo.firmwareRelVersion, umInfo.firmwareIntVersion);

    /* 1.2.5	Capabilities Number of Devices Register (0x05) */
    ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &umInfo.numDevices);
    u32Addr++;

    /* printf("\nNumber of Devices: %d\n\n", umInfo.numDevices); */

    printf("#======================================================================================================\n");
    printf("#   %-60s", "Name");
    printf("%-16s", "Address");
    printf("   %-6s%-4s", "Type", "Rev");
    printf("Size\n");
    printf("#======================================================================================================\n");

    /* read device information */
    for (numDevicesIndex = 0; numDevicesIndex < umInfo.numDevices; numDevicesIndex++)
    {
        PUM_DEVICE_INFO tmpUmDevicePtr;
        PUM_COMPONENT_INFO tmpUmComponentPtr;
        U16BIT numComponentsIndex;
        U16BIT u16Index;
        int i;

        /* 1.2.6	Per Device: Device Type Register */
        tmpUmDevicePtr = &umInfo.umDeviceInfo[numDevicesIndex];

        ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmDevicePtr->umDevType);
        u32Addr++;

        /* mask off delimiter for now*/
        tmpUmDevicePtr->umDevType = tmpUmDevicePtr->umDevType & 0x0000FFFF;

        sprintf(tmpUmDevicePtr->umName, "*** UNKNOWN DEVICE *** (ID %04X)", tmpUmDevicePtr->umDevType);

        for (i=0; i < (sizeof(umRomLookupTable)/sizeof(umRomLookupTable[0])); i++)
        {
            if (umRomLookupTable[i].id == tmpUmDevicePtr->umDevType)
            {
                sprintf(tmpUmDevicePtr->umName,"%s", umRomLookupTable[i].name);
                break;
            }
        }

        /* 1.2.7	Per Device: Device Revision Register */
        ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmDevicePtr->umDevRev);
        u32Addr++;

        /* 1.2.8	Per Device: Number of Instances Register */
        ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmDevicePtr->umDevNumInstances);
        u32Addr++;

        /* 1.2.9	Per Device: Memory Size Register */
        ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmDevicePtr->umDevMemSize);
        u32Addr++;

        for(u16Index=0; u16Index < tmpUmDevicePtr->umDevNumInstances; ++u16Index)
        {
            char csName[32];
            char csFullName[128];

            sprintf(csName, tmpUmDevicePtr->umName, "MEM");
            sprintf(csFullName, "%s_%d ", csName, u16Index);
            printf("UBA %-60s", csFullName);

            /* 1.2.10	Per Device: Per Instance: Memory Base Address Register */
            ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmDevicePtr->umMemBaseAddr[u16Index]);
            u32Addr++;

            printf("0x%08X        ", tmpUmDevicePtr->umMemBaseAddr[u16Index]);
            printf("# MEM  ");
            printf("%-3d 0x%-8X\n", tmpUmDevicePtr->umDevRev, tmpUmDevicePtr->umDevMemSize);
        }

        /* 1.2.11	Per Device: Number of Components Register */
        ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmDevicePtr->umDevNumComponents);
        u32Addr++;

        /* printf("umInitialize: Device Num Components is %d.\n", tmpUmDevicePtr->umDevNumComponents); */

        for (numComponentsIndex=0; numComponentsIndex < tmpUmDevicePtr->umDevNumComponents; ++numComponentsIndex)
        {
            tmpUmComponentPtr = &tmpUmDevicePtr->umComponentInfo[numComponentsIndex];

            /* 1.2.12	Per Device: Per Component: Component Type Register */
            ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmComponentPtr->umComponentType);
            u32Addr++;

            tmpUmComponentPtr->umComponentType &= 0x0000FFFF; /* mask off delimiter for now */

            sprintf(tmpUmComponentPtr->umName, "*** UNKNOWN COMPONENT *** (ID %04X)", tmpUmComponentPtr->umComponentType); /* add count of unknowns */

            for (i=0; i < (sizeof(umRomLookupTable)/sizeof(umRomLookupTable[0])); i++)
            {
                if (umRomLookupTable[i].id == tmpUmComponentPtr->umComponentType)
                {
                    sprintf(tmpUmComponentPtr->umName,"%s", umRomLookupTable[i].name);
                    break;
                }
            }

            /* 1.2.13	Per Device: Per Component: Component Revision Register */
            ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmComponentPtr->umComponentRev);
            u32Addr++;

            /* printf("umInitialize: Component Rev is %X.\n",tmpUmComponentPtr->umComponentRev); */

            /* 1.2.14	Per Device: Per Component: Register Set Size Register */
            ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmComponentPtr->umComponentRegSize);
            u32Addr++;

            /* printf("umInitialize: Component Reg Size is %X.\n",tmpUmComponentPtr->umComponentRegSize); */

            for (u16Index=0; u16Index < tmpUmDevicePtr->umDevNumInstances; u16Index++)
            {
                char csName[32];
                char csFullName[128];

                sprintf(csName, tmpUmComponentPtr->umName, "REG");
                sprintf(csFullName, "%s_%d", csName, u16Index);

                printf("UBA %-60s", csFullName);

                /* 1.2.15	Per Device: Per Component: Per Instance: Register Base Address Register */
                ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &tmpUmComponentPtr->umRegBaseAddr[u16Index]);
                u32Addr++;

                /* printf("umInitialize: Reg Base Address is %X.\n",tmpUmComponentPtr->umRegBaseAddr[u16Index]); */

                printf("0x%08X        ", tmpUmComponentPtr->umRegBaseAddr[u16Index]);
                printf("# REG  ");
                printf("%-3d 0x%-8X\n", tmpUmComponentPtr->umComponentRev, tmpUmComponentPtr->umComponentRegSize);
            }
        }
    }
}

/*******************************************************************************
 * Name:    ddccmDiagDumpRegistersMenu
 *
 * Description:
 *      This function prints the dump registers menu.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagDumpRegistersMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    char szStringIn[80] = "";

    do
    {
        printf("\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| [DIAGNOSTICS] MAIN -> DUMP REGISTERS                            |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| a  set start (a)ddr                              0x%08X     |\n", u32DumpRegStartAddr);
        printf("| n  dump registers at from (n)ext address block                  |\n");
        printf("| p  dump registers at from (p)revious address block              |\n");
        printf("| s  dump registers at (s)ame address block                       |\n");
        printf("| x  e(x)it to previous menu                                      |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("\n Enter command: ");

        scanf("%s", szStringIn);

        printf("\n");

        if (!strcmp(szStringIn, "a"))
        {
            printf("\n");
            printf(" Current Address............................: %08X\n", u32DumpRegStartAddr);
            printf(" Enter the new register start address in hex: ");

            scanf("%x", &u32DumpRegStartAddr);
        }
        else if (!strcmp(szStringIn, "n"))
        {
            /* check if less then the max addr block*/
            if (u32DumpRegStartAddr <= (0xFFFFFFFF - REG_MEM_ADDR_BLOCK_SIZE))
            {
                u32DumpRegStartAddr += REG_MEM_ADDR_BLOCK_SIZE;
            }

            ddccmDiagDumpRegMem(pDeviceInfoEntry, ACCESS_TYPE_REG);
        }
        else if (!strcmp(szStringIn, "p"))
        {
            /* make sure we do not go below zero */
            if (u32DumpRegStartAddr < REG_MEM_ADDR_BLOCK_SIZE)
            {
                /* just set the start to zero */
                u32DumpRegStartAddr = 0x00000000;
            }
            else
            {
                u32DumpRegStartAddr -= REG_MEM_ADDR_BLOCK_SIZE;
            }

            ddccmDiagDumpRegMem(pDeviceInfoEntry, ACCESS_TYPE_REG);
        }
        else if (!strcmp(szStringIn, "s"))
        {
            ddccmDiagDumpRegMem(pDeviceInfoEntry, ACCESS_TYPE_REG);
        }
        else if (!strcmp(szStringIn, "x"))
        {
            /* quit to previous menu */
            return;
        }
        else
        {
                printf("\n    Invalid command!\n");
        }

    } while (1);
}

/*******************************************************************************
 * Name:    ddccmDiagReadWriteRegisterMenu
 *
 * Description:
 *      This function prints the dump registers menu.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagReadWriteRegisterMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    U32BIT u32RegOffsetType = REG_TYPE_ABSOLUTE_ADDR;
    U32BIT u32Data;
    int i;
    char szStringIn[80] = "";

    u32RegOffsetType = REG_TYPE_ABSOLUTE_ADDR;

    do
    {
        printf("\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| [DIAGNOSTICS] MAIN -> READ/WRITE REGISTER                       |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| a  set start (a)ddr                           0x%08X        |\n", u32SingleRegStartAddr);
        printf("| t  set the register offset (t)ype             %-13s     |\n", regOffsetTypeStr[u32RegOffsetType]);
        printf("| r  (r)ead register at current address                           |\n");
        printf("| w  (w)rite register at current address                          |\n");
        printf("| x  e(x)it to previous menu                                      |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("\n Enter command: ");

        scanf("%s", szStringIn);

        printf("\n");


        if (!strcmp(szStringIn, "a"))
        {
            printf("\n");
            printf(" Current Address............................: %08X\n", u32SingleRegStartAddr);
            printf(" Enter the new register start address in hex: ");

            scanf("%x", &u32SingleRegStartAddr);
        }
        else if (!strcmp(szStringIn, "t"))
        {
            printf(" Enter the new Register Offset Type (or a user defined type):\n");
            printf("   %s\n", regOffsetTypeStr[0]);
            printf("   %s\n", regOffsetTypeStr[1]);
            printf("   %s\n", regOffsetTypeStr[2]);
            printf("   %s\n", regOffsetTypeStr[3]);
            printf("   %s\n", regOffsetTypeStr[4]);
            printf("   %s\n", regOffsetTypeStr[5]);
            printf("   %s\n", regOffsetTypeStr[6]);
            printf("   %s\n", regOffsetTypeStr[7]);
            printf("   %s\n", regOffsetTypeStr[8]);
            printf("\n");
            printf(" Current Type..: %s\n", regOffsetTypeStr[u32RegOffsetType]);
            printf(" Enter New Type: ");

            scanf("%d", &u32RegOffsetType);
        }
        else if (!strcmp(szStringIn, "r"))
        {
            ddccmDiagReadReg(pDeviceInfoEntry, u32SingleRegStartAddr, u32RegOffsetType, &u32Data);

            printf("===================================================================\n");
            printf("Address      HEX        DEC              ASCII\n");
            printf("===================================================================\n");

            printf("0x%08X:  ", u32SingleRegStartAddr);
            printf("%08X   ", u32Data);
            printf("%-12u   ", u32Data);
            printf("  ");

            /* print ASCII */
            for (i=3; i>=0; i--)
            {
                char ch;

                /*                              shift # bits per byte */
                ch = (char)((u32Data >> (i * 8)) & 0xFF);

                /* is printable? */
                if ((ch >= 0x20) && (ch <= 0x7E))
                {
                    printf("%c", ch);
                }
                else
                {
                    printf(".");
                }
            }
            printf("\n");
            printf("===================================================================\n");
        }
        else if (!strcmp(szStringIn, "w"))
        {
            printf("\n");
            printf(" Enter value to write to \"%s\" at offset 0x%08X in hex: ", regOffsetTypeStr[u32RegOffsetType], u32SingleRegStartAddr);

            scanf("%x", &u32Data);

            ddccmDiagWriteReg(pDeviceInfoEntry, u32SingleRegStartAddr, u32RegOffsetType, u32Data);
        }
        else if (!strcmp(szStringIn, "x"))
        {
            /* quit to previous menu */
            return;
        }
        else
        {
            printf("\n    Invalid command!\n");
        }

    } while (1);
}


/*******************************************************************************
 * Name:    ddccmDiagDumpRegMem
 *
 * Description:
 *      This function dumps the reg/mem data starting at the start addr specified
 *      using the 'a' command or 0x00000000 if not specified.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * In   nAccessType         type to access - reg or mem
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagDumpRegMem
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    int nAccessType
)
{
    U32BIT u32Addr = 0x00000000;
    U32BIT u32Data[4] = { 0x00000000 };
    int row;
    int col;

    if (nAccessType == ACCESS_TYPE_REG)
    {
        u32Addr = u32DumpRegStartAddr;
    }
    else
    {
        u32Addr = u32DumpMemStartAddr;
    }

    printf("===================================================================\n");
    printf("Address      Data (HEX)                            ASCII\n");
    printf("===================================================================\n");

    for (row=0; row<MAX_ROWS; row++)
    {
        printf("0x%08X:  ", u32Addr);

        /* print hex */
        for (col=0; col<MAX_COLS; col++)
        {
            if (nAccessType == ACCESS_TYPE_REG)
            {
                if (ddccmDiagReadReg(pDeviceInfoEntry, u32Addr, REG_TYPE_ABSOLUTE_ADDR, &u32Data[col]) == DDCCM_SUCCESS)
                {
                    printf("%08X ", u32Data[col]);
                }
                else
                {
                    /* error */
                    printf("-------- ");
                }
            }
            else
            {
                if (ddccmDiagReadMem(pDeviceInfoEntry, u32Addr, &u32Data[col]) == DDCCM_SUCCESS)
                {
                    printf("%08X ", u32Data[col]);
                }
                else
                {
                    /* error */
                    printf("-------- ");
                }
            }

            u32Addr++;
        }

        printf("  ");

        /* print ASCII */
        for (col=0; col<MAX_COLS; col++)
        {
            int i;
            for (i=3; i>=0; i--)
            {
                char ch;

                /*                              shift # bits per byte */
                ch = (char)((u32Data[col] >> (i * 8)) & 0xFF);

                /* is printable? */
                if ((ch >= 0x20) && (ch <= 0x7E))
                {
                    printf("%c", ch);
                }
                else
                {
                    printf(".");
                }
            }
        }

        printf("\n");
    }

    printf("===================================================================\n");
}

/*******************************************************************************
 * Name:    ddccmDiagDumpMemoryMenu
 *
 * Description:
 *      This function prints the dump memory menu.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagDumpMemoryMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    char szStringIn[80] = "";

    do
    {
        printf("\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| [DIAGNOSTICS] MAIN -> DUMP MEMORY                                |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| a  set start (a)ddr                              0x%08X     |\n", u32DumpMemStartAddr);
        printf("| n  dump memory at from (n)ext address block                     |\n");
        printf("| p  dump memory at from (p)revious address block                 |\n");
        printf("| s  dump memory at (s)ame address block                          |\n");
        printf("| x  e(x)it to previous menu                                      |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("\n Enter command: ");

        scanf("%s", szStringIn);

        printf("\n");

        if (!strcmp(szStringIn, "a"))
        {
            printf("\n");
            printf(" Current Address..........................: %08X\n", u32DumpMemStartAddr);
            printf(" Enter the new memory start address in hex: ");

            scanf("%x", &u32DumpMemStartAddr);
        }
        else if (!strcmp(szStringIn, "n"))
        {
            /* check if less then the max addr block*/
            if (u32DumpMemStartAddr <= (0xFFFFFFFF - REG_MEM_ADDR_BLOCK_SIZE))
            {
                u32DumpMemStartAddr += REG_MEM_ADDR_BLOCK_SIZE;
            }

            ddccmDiagDumpRegMem(pDeviceInfoEntry, ACCESS_TYPE_MEM);
        }
        else if (!strcmp(szStringIn, "p"))
        {
            /* make sure we do not go below zero */
            if (u32DumpMemStartAddr < REG_MEM_ADDR_BLOCK_SIZE)
            {
                /* just set the start to zero */
                u32DumpMemStartAddr = 0x00000000;
            }
            else
            {
                u32DumpMemStartAddr -= REG_MEM_ADDR_BLOCK_SIZE;
            }

            ddccmDiagDumpRegMem(pDeviceInfoEntry, ACCESS_TYPE_MEM);
        }
        else if (!strcmp(szStringIn, "s"))
        {
            ddccmDiagDumpRegMem(pDeviceInfoEntry, ACCESS_TYPE_MEM);
        }
        else if (!strcmp(szStringIn, "x"))
        {
            /* quit to previous menu */
            return;
        }
        else
        {
            printf("\n    Invalid command!\n");
        }

    } while (1);
}

/*******************************************************************************
 * Name:    ddccmDiagReadWriteMemoryMenu
 *
 * Description:
 *      This function prints the dump memory menu.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagReadWriteMemoryMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    U32BIT u32Data;
    int i;
    char szStringIn[80] = "";

    do
    {
        printf("\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| [DIAGNOSTICS] MAIN -> READ/WRITE MEMORY                         |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| a  set start (a)ddr                           0x%08X        |\n", u32SingleMemStartAddr);
        printf("| r  (r)ead memory at current address                             |\n");
        printf("| w  (w)rite memory at current address                            |\n");
        printf("| x  e(x)it to previous menu                                      |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("\n Enter command: ");

        scanf("%s", szStringIn);

        printf("\n");


        if (!strcmp(szStringIn, "a"))
        {
            printf("\n");
            printf(" Current Address..........................: %08X\n", u32SingleMemStartAddr);
            printf(" Enter the new memory start address in hex: ");

            scanf("%x", &u32SingleMemStartAddr);
        }
        else if (!strcmp(szStringIn, "r"))
        {
            ddccmDiagReadMem(pDeviceInfoEntry, u32SingleMemStartAddr, &u32Data);

            printf("===================================================================\n");
            printf("Address      HEX        DEC              ASCII\n");
            printf("===================================================================\n");

            printf("0x%08X:  ", u32SingleMemStartAddr);
            printf("%08X   ", u32Data);
            printf("%-12u   ", u32Data);
            printf("  ");

            /* print ASCII */
            for (i=3; i>=0; i--)
            {
                char ch;

                /*                              shift # bits per byte */
                ch = (char)((u32Data >> (i * 8)) & 0xFF);

                /* is printable? */
                if ((ch >= 0x20) && (ch <= 0x7E))
                {
                    printf("%c", ch);
                }
                else
                {
                    printf(".");
                }
            }
            printf("\n");
            printf("===================================================================\n");
        }
        else if (!strcmp(szStringIn, "w"))
        {
            printf("\n Enter value to write to address 0x%08X in hex: ", u32SingleMemStartAddr);

            scanf("%x", &u32Data);

            ddccmDiagWriteMem(pDeviceInfoEntry, u32SingleMemStartAddr, u32Data);
        }
        else if (!strcmp(szStringIn, "x"))
        {
            /* quit to previous menu */
            return;
        }
        else
        {
            printf("    Invalid command!\n");
        }

    } while (1);
}

/* ========================================================================== */
/* ========================================================================== */
/* ========================================================================== */
/* BC OPCODE DECODER */
/* ========================================================================== */
/* ========================================================================== */
/* ========================================================================== */

#define REG_TYPE_BC_OFFSET                  2

#define REG_BC_INITIAL_INSTRUCTION_POINTER_REGISTER     6

#define ADDR_TYPE_DEFAULT                   0
#define ADDR_TYPE_SHARED_MEMORY_ADDRESS     1
#define ADDR_TYPE_COMPARE_MEM_ADDR          2
#define ADDR_TYPE_MESSAGE_POINTER           3
#define ADDR_TYPE_DATA_BLOCK_POINTER        4

#define PARAMETER_STRING_LEN                18
#define SHARED_MEMORY_ADDRESS_MASK          0x000FFFFF

#define DECODE_TYPE_SMART                   0
#define DECODE_TYPE_RAW                     1


#define OP_CODE_PARITRY_FIELD               0
#define OP_CODE_OPC_FIELD                   1
#define OP_CODE_CONDITION_FIELD             2
#define OP_CODE_MAX_FIELD                   3

#define OP_CODE_PARITRY_MASK                0x8000
#define OP_CODE_OPC_MASK_SFP                0x7E00  /* flexcore is 6 bits */
#define OP_CODE_OPC_MASK_FLEX               0x7C00  /* flexcore is 6 bits */
#define OP_CODE_SHIFT_FLEX                  10
#define OP_CODE_SHIFT_SFP                   9
#define OP_CODE_CONDITION_MASK              0x002F   /* 32 bit core */
#define OP_CODE_CCIN_MASK                   0x0010   /* Invert Condition code - 32 bit core  */
#define OP_CODE_OKEY_MASK                   0x000001C0
#define OP_CODE_OKEY                        0x00000140

/* OP Codes */
#define OP_CODE_INVALID                     0x01
#define OP_CODE_XEQ                         0x01
#define OP_CODE_JMP                         0x02
#define OP_CODE_CAL                         0x03
#define OP_CODE_RTN                         0x04
#define OP_CODE_IRQ                         0x06
#define OP_CODE_HLT                         0x07
#define OP_CODE_DLY                         0x08
#define OP_CODE_WFT                         0x09
#define OP_CODE_CFT                         0x0A
#define OP_CODE_CMT                         0x0B
#define OP_CODE_FLG                         0x0C
#define OP_CODE_LTT                         0x0D
#define OP_CODE_LFT                         0x0E
#define OP_CODE_SFT                         0x0F
#define OP_CODE_PTT                         0x10
#define OP_CODE_PBS                         0x11
#define OP_CODE_PSI                         0x12
#define OP_CODE_PSM                         0x13
#define OP_CODE_WTG                         0x14
#define OP_CODE_XQF                         0x15
#define OP_CODE_GRP                         0x16
#define OP_CODE_RDM                         0x17
#define OP_CODE_CPM                         0x18
#define OP_CODE_INC                         0x19
#define OP_CODE_DEC                         0x1A
#define OP_CODE_WRM                         0x1B
#define OP_CODE_LIA                         0x1C
#define OP_CODE_PTU                         0x1D
#define OP_CODE_WFV                         0x1E
#define OP_CODE_AMT                         0x21
#define OP_CODE_CMM                         0x22
#define OP_CODE_LWT                         0x23
#define OP_CODE_LIB                         0x24
#define OP_CODE_MAX                         OP_CODE_LWT

typedef struct
{
    U8BIT u8OpCode;
    BOOLEAN bHasConditional;
    char opCodeString[8];
    char opCodeDescString[32];

} TYPE_OP_CODE_LOOKUP;


TYPE_OP_CODE_LOOKUP aOpCodeLookupTable[] =
{
    { OP_CODE_XEQ,  1,  " XEQ  ",  " Execute Msg             " },
    { OP_CODE_JMP,  1,  " JMP  ",  " Jump                    " },
    { OP_CODE_CAL,  1,  " CAL  ",  " Call Subroutine         " },
    { OP_CODE_RTN,  1,  " RTN  ",  " Return                  " },
    { OP_CODE_IRQ,  1,  " IRQ  ",  " IRQ Request             " },
    { OP_CODE_HLT,  1,  " HLT  ",  " Halt                    " },
    { OP_CODE_DLY,  1,  " DLY  ",  " Delay                   " },
    { OP_CODE_WFT,  1,  " WFT  ",  " Execute Message         " },
    { OP_CODE_CFT,  0,  " CFT  ",  " Compare to Frame Timer  " },
    { OP_CODE_CMT,  0,  " CMT  ",  " Compare to Msg Timer    " },
    { OP_CODE_FLG,  0,  " FLG  ",  " GP Flag Bits            " },
    { OP_CODE_LTT,  1,  " LTT  ",  " Load Time Tag Counter   " },
    { OP_CODE_LFT,  1,  " LFT  ",  " Load Frame Timer        " },
    { OP_CODE_SFT,  1,  " SFT  ",  " Start Frame Timer       " },
    { OP_CODE_PTT,  1,  " PTT  ",  " Push Time Tag Reg       " },
    { OP_CODE_PBS,  1,  " PBS  ",  " Push Block Status Word  " },
    { OP_CODE_PSI,  1,  " PSI  ",  " Push Immediate Value    " },
    { OP_CODE_PSM,  1,  " PSM  ",  " Push (Indirect) Memory  " },
    { OP_CODE_WTG,  1,  " WTG  ",  " Wait For Ext. Trigger   " },
    { OP_CODE_XQF,  0,  " XQF  ",  " Execute and Flip        " },
    { OP_CODE_GRP,  1,  " GRP  ",  " Group Condtion Codes    " },
    { OP_CODE_RDM,  1,  " RDM  ",  " Read Memory             " },
    { OP_CODE_CPM,  1,  " CPM  ",  " Compare Memory          " },
    { OP_CODE_INC,  1,  " INC  ",  " Increment Memory        " },
    { OP_CODE_DEC,  1,  " DEC  ",  " Decrement Memory        " },
    { OP_CODE_WRM,  1,  " WRM  ",  " Write Memory            " },
    { OP_CODE_LIA,  1,  " LIA  ",  " Load Immediate Accum A  " },
    { OP_CODE_PTU,  1,  " PTU  ",  " Push GPQ TimeTag upper  " },
    { OP_CODE_WFV,  1,  " WFV  ",  " Wait Frame Timer Value  " },
    { OP_CODE_AMT,  1,  " AMT  ",  " Async Message Trigger   " },
    { OP_CODE_CMM,  1,  " CMM  ",  " Compare Memory Masked   " },
    { OP_CODE_LIB,  1,  " LIB  ",  " Load Immediate ACC B    " },
    { OP_CODE_LWT,  1,  " LWT  ",  " Load Watchdog Timer     " },
};

/* Condition Codes */
#define CONDITION_CODE_GP0                  0x00
#define CONDITION_CODE_GP1                  0x01
#define CONDITION_CODE_GP2                  0x02
#define CONDITION_CODE_GP3                  0x03
#define CONDITION_CODE_GP4                  0x04
#define CONDITION_CODE_GP5                  0x05
#define CONDITION_CODE_GP6                  0x06
#define CONDITION_CODE_GP7                  0x07
#define CONDITION_CODE_NO_RESP              0x08
#define CONDITION_CODE_FMT_ERR              0x09
#define CONDITION_CODE_GOOD_BAD_BLK_XFER    0x0A
#define CONDITION_CODE_MASKED_STATUS_SET    0x0B
#define CONDITION_CODE_GOOD_BAD_MESSAGE     0x0C
#define CONDITION_CODE_RETRY1               0x0D
#define CONDITION_CODE_RETRY2               0x0E
#define CONDITION_CODE_ALWAYS               0x0F
#define CONDITION_CODE_GP8                  0x11
#define CONDITION_CODE_GP9                  0x12
#define CONDITION_CODE_GP10                 0x13
#define CONDITION_CODE_GP11                 0x14
#define CONDITION_CODE_GP12                 0x15
#define CONDITION_CODE_GP13                 0x16
#define CONDITION_CODE_GP14                 0x17
#define CONDITION_CODE_GROUP_COND_CODE      0x18

/* MPTR - Message Pointer */
#define MPTR_FORCE_FULLY_TRANSFER_MASK      0x00000000
#define MPTR_COMMAND_DATA_SELECT_MASK       0xC0000000
#define MPTR_POST_IF_INVALID_MASK           0x20000000
#define MPTR_32_BIT_DATA_SUPPORT_MASK       0x10000000
#define MPTR_DO_NOT_POST_MASK               0x08000000
#define MPTR_CONTROL_BLOCK_ADDRESS_MASK     0x0007FFFF
#define MPTR_XQF_FILP_ADDRESS_MASK          0x00000010

#define MPTR_COMMAND_AND_DATA               0x00000000
#define MPTR_COMMAND_ONLY                   0x40000000
#define MPTR_DATA_ONLY                      0x80000000

/* DBP - Data Block Pointer */
#define DBP_ADDRESS_MASK                    0x0007FFFF

/* BC */
#define BC_MESSGE_BLOCK_SIZE                0x10 /* # DWORDS */
#define BC_DATA_BLOCK_SIZE                  0x10 /* # DWORDS */

#define BC_MESSGE_BLOCK_AREA_MASK           0x0000001F
#define BC_MESSGE_BLOCK_AREA_A              0x00
#define BC_MESSGE_BLOCK_AREA_B              0x01

#define BC_MESSGE_BLOCK_BC_CONTROL_WORD         0x00
#define BC_MESSGE_BLOCK_1553_COMMAND_WORD       0x01
#define BC_MESSGE_BLOCK_DATA_BLOCK_POINTER      0x02
#define BC_MESSGE_BLOCK_TIME_TO_NEXT_MESSAGE    0x03
#define BC_MESSGE_BLOCK_TIME_TAG                0x04
#define BC_MESSGE_BLOCK_BC_BLOCK_STATUS_WORD    0x05
#define BC_MESSGE_BLOCK_LOOPBACK_WORD           0x06
#define BC_MESSGE_BLOCK_RT_STATUS_WORD          0x07
#define BC_MESSGE_BLOCK_2ND_1553_COMMAND_WORD   0x08
#define BC_MESSGE_BLOCK_2ND_RT_STATUS_WORD      0x09


static char aBcMsgBlockItemStrings[][64] =
{
    { "BC Control Word" },
    { "1553 Command Word" },
    { "Data Block Pointer" },
    { "Time-to-Next-Message" },
    { "Time Tag" },
    { "BC Block Status Word" },
    { "Loopback Word" },
    { "RT Status Word" },
    { "2nd 1553 Command Word" },
    { "2nd RT Status Word" },
    { "Reserved" },
    { "Reserved" },
    { "Reserved" },
    { "Reserved" },
    { "Reserved" },
    { "Reserved" },
};

typedef struct
{
    U8BIT u8DecodeType;
    U32BIT u8OpCode;
    U32BIT u32StartAddr;
    U32BIT u32EndAddr;
    U8BIT u8OpCodeType;
    BOOLEAN bInRange;

} TYPE_BC_OPCODE_DECODE_INFO;

struct TYPE_BC_OPCODE_DECODE_LIST
{
    TYPE_BC_OPCODE_DECODE_INFO info;
    struct TYPE_BC_OPCODE_DECODE_LIST *pNext;
};


static struct TYPE_BC_OPCODE_DECODE_LIST *m_pDecodedList = NULL;

static U8BIT m_u8DecodeType = DECODE_TYPE_SMART;


/*============================================================================*/
/*============================================================================*/
/*                            Queue Functions                                 */
/*============================================================================*/
/*============================================================================*/

static void RemoveAll(struct TYPE_BC_OPCODE_DECODE_LIST *pList)
{
    struct TYPE_BC_OPCODE_DECODE_LIST *pCurrent = pList;
    struct TYPE_BC_OPCODE_DECODE_LIST *pNext;

    while (pCurrent != NULL)
    {
        pNext = pCurrent->pNext;
        free(pCurrent);
        pCurrent = pNext;
    }
}

void AddTail
(
    struct TYPE_BC_OPCODE_DECODE_LIST **pHead,
    TYPE_BC_OPCODE_DECODE_INFO *pInfo
)
{
    struct TYPE_BC_OPCODE_DECODE_LIST *pCurrent;
    struct TYPE_BC_OPCODE_DECODE_LIST *pNew;

    pNew = malloc(sizeof(*pNew));
    pNew->info = *pInfo;
    pNew->pNext = NULL;

    /* is this an empty list? */
    if (*pHead == NULL)
    {
        *pHead = pNew;
        return;
    }

    /* find the end of the list */
    pCurrent = *pHead;

    while (pCurrent)
    {
        if (pCurrent->pNext == NULL)
        {
            pCurrent->pNext = pNew;
            return;
        }

        pCurrent = pCurrent->pNext;
    }
}

/******************************************************************************
 * Name:          DecodeOpCode
 *
 * Description:   Decodes long words to 1553 OP codes
 *
 * In             BOOLEAN debug, if set TRUE will display result
 * Out            N/A
 *****************************************************************************/
BOOLEAN DecodeOpCode
(
    U32BIT address,
    U32BIT u32BcParam47_32_BCInst,
    U32BIT u32BcParam31_0,
    TYPE_BC_OPCODE_DECODE_INFO *pBcOpcodeStatus
)
{
    int i;
    int oneBitCount;
    unsigned int opCode = OP_CODE_INVALID;
    unsigned int conditionCode;
    BOOLEAN         validOpcode;
    unsigned int bitValue;
    char opCodeString[32];
    char parameterString[64];
    char opCodeDescString[32];
    char conditionCodeString[128];
    char parityCodeString[20];
    char outString[400];
    char addressString[16];
    char valueString[16];
    int opCodeIndex;
    int loop;
    BOOLEAN bOpCodeMatch = FALSE;

    /*
    *
    *                                                    BC Instruction List
    *                                                   +---------------------+
    * Message Control/Status Block                      | OP Code             |--> BC Instruction List
    * +---+----------------------------------------+    +---------------------+    Pointer Register
    * | 1 | BC Control Word                        |--> | Parameter (Pointer) |    Initialize by Register
    * +---+----------------------------------------+    +---------------------+    0D (RD/WR);
    * | 2 | Command Word  (Rx Command RT - to - RT |
    * +---+----------------------------------------+    +------------+
    * | 3 | Data Block Pointer                     |--> | Data Block |
    * +---+----------------------------------------+    |            |
    * | 4 | Time-To-Next Message                   |    +------------+
    * +---+----------------------------------------+
    * | 5 | Block Status Word                      |
    * +---+----------------------------------------+
    * | 6 | Loopback Word                          |
    * +---+----------------------------------------+
    * | 7 | RT Status Word                         |
    * +---+----------------------------------------+
    * | 8 | Command Word  (Rx Command RT - to - RT |
    * +---+----------------------------------------+
    */

    /*
        BC Op Code Format

        +---------+---+---+---+---+---+---+------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        | 32                           16 | 15   |14 |13 |12 |11 |10 |09 |08 |07 |06 |05 |04 |03 |02 |01 |00 |
        +---------+---+---+---+---+---+---+------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        |      BC Parameter[47:32]        |Odd   |Op Code Field          | 1 | 0 | 1 |Ext|Condition Code     |
        |                                 |Parity|                       |   OKEY    |CC |Field              |
        +---------+---+---+---+---+---+---+------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

        +---------+---+---+---+---+---+---+------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        | 32                           16 | 15                                                            00 |
        +---------+---+---+---+---+---+---+------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        |                                    BC Parameter[31:0]                                              |
        +---------+---+---+---+---+---+---+------+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    */

    /* Decode bit fields */

    /* -------------------------------------- */
    /* decode OPC Field */
    /* -------------------------------------- */
    if ((u32BcParam47_32_BCInst & OP_CODE_OKEY_MASK) == OP_CODE_OKEY)
    {
        validOpcode = TRUE;
        opCode = (u32BcParam47_32_BCInst & OP_CODE_OPC_MASK_SFP) >> OP_CODE_SHIFT_SFP;

        /* Verify Parity is Odd */
        bitValue = 0x00001;
        oneBitCount = 0;

        for (i=0; i < 16; i++)
        {
            if (bitValue & u32BcParam47_32_BCInst)
            {
                oneBitCount++;
            }
            bitValue <<= 1;
        }

        if (oneBitCount % 2)
        {
            strcpy(parityCodeString, "");
        }
        else
        {
            strcpy(parityCodeString, "  (Parity Error)");
        }

        bOpCodeMatch = FALSE;

        for (loop=0; loop<sizeof(aOpCodeLookupTable)/sizeof(aOpCodeLookupTable[0]); loop++)
        {
            if (opCode == aOpCodeLookupTable[loop].u8OpCode)
            {
                strcpy(opCodeString, aOpCodeLookupTable[loop].opCodeString);
                strcpy(opCodeDescString, aOpCodeLookupTable[loop].opCodeDescString);
                opCodeIndex = loop;
                bOpCodeMatch = TRUE;
                break;
            }
        }

        if (bOpCodeMatch == FALSE)
        {
            /* no match in table */
            sprintf(opCodeString, "---");
            sprintf(opCodeDescString, "Unknown Opcode (0x%02X)", opCode);
            validOpcode = FALSE;
        }
    }
    else /* not an Opcode */
    {
        /* OKEY not present */
        validOpcode = FALSE;
    }


    /* -------------------------------------- */
    /* decode Condition Field */
    /* -------------------------------------- */
    if (validOpcode)
    {
        /* process parameter */

        pBcOpcodeStatus->u8OpCodeType = ADDR_TYPE_DEFAULT;
        pBcOpcodeStatus->u8OpCode = opCode;

        switch (opCode)
        {
            case OP_CODE_JMP:
            case OP_CODE_CAL:
            case OP_CODE_DEC:
            case OP_CODE_INC:
            case OP_CODE_RDM:
            case OP_CODE_WRM:
            case OP_CODE_PSM:
            {
                /* SMA (Shared Memory Address) Parameter */
                sprintf(parameterString, "%X", u32BcParam31_0 & SHARED_MEMORY_ADDRESS_MASK);
                pBcOpcodeStatus->u8OpCodeType = ADDR_TYPE_SHARED_MEMORY_ADDRESS;
                pBcOpcodeStatus->u32StartAddr = (u32BcParam31_0 & SHARED_MEMORY_ADDRESS_MASK);
                break;
            }

            case OP_CODE_XEQ:
            case OP_CODE_XQF:
            {
                /* MPTR */
/* #define MPTR_FORCE_FULLY_TRANSFER_MASK      0x00000000 */
/* #define MPTR_COMMAND_DATA_SELECT_MASK       0xC0000000 */
/* #define MPTR_POST_IF_INVALID_MASK           0x20000000 */
/* #define MPTR_32_BIT_DATA_SUPPORT_MASK       0x10000000 */
/* #define MPTR_DO_NOT_POST_MASK               0x08000000 */
/* #define MPTR_CONTROL_BLOCK_ADDRESS_MASK     0x0007FFFF */

                sprintf(parameterString, "%X ", u32BcParam31_0 & MPTR_CONTROL_BLOCK_ADDRESS_MASK);
                if ((u32BcParam31_0 & MPTR_COMMAND_DATA_SELECT_MASK) == MPTR_COMMAND_AND_DATA)
                {
                    strcat(parameterString, "(Cmd & Data)");
                }
                else if ((u32BcParam31_0 & MPTR_COMMAND_DATA_SELECT_MASK) == MPTR_COMMAND_ONLY)
                {
                    strcat(parameterString, "(Cmd Only)");
                }
                else if ((u32BcParam31_0 & MPTR_COMMAND_DATA_SELECT_MASK) == MPTR_DATA_ONLY)
                {
                    strcat(parameterString, "(Data Only)");
                }
                else
                {
                    strcat(parameterString, "(Not Used)");
                }

                pBcOpcodeStatus->u8OpCode = opCode;
                pBcOpcodeStatus->u8OpCodeType = ADDR_TYPE_MESSAGE_POINTER;
                pBcOpcodeStatus->u32StartAddr = (u32BcParam31_0 & SHARED_MEMORY_ADDRESS_MASK);

                if (opCode == OP_CODE_XQF)
                {
                    /* since bit 4 in a XQF opcode gets flipped, we will decode starting at the location */
                    /* when bit 4 is 0 */
                    pBcOpcodeStatus->u32StartAddr &= ~(MPTR_XQF_FILP_ADDRESS_MASK);
                }

                break;
            }

            /* case OP_CODE_IRQ: */
            /* case OP_CODE_DLY: */
            /* case OP_CODE_WFT: */
            /* case OP_CODE_CFT: */
            /* case OP_CODE_CMT: */
            /* case OP_CODE_FLG: */
            /* case OP_CODE_LTT: */
            /* case OP_CODE_LFT: */
            /* case OP_CODE_SFT: */
            /* case OP_CODE_PTT: */
            /* case OP_CODE_PBS: */
            /* case OP_CODE_PSI: */
            /* case OP_CODE_WTG: */
            /* case OP_CODE_GRP: */

            case OP_CODE_CPM:
            {
                sprintf(parameterString, "%X - (Accum A)", u32BcParam31_0);
                pBcOpcodeStatus->u32StartAddr = u32BcParam31_0;
                pBcOpcodeStatus->u8OpCodeType = ADDR_TYPE_COMPARE_MEM_ADDR;
                break;
            }

            case OP_CODE_LIA:
            {
                sprintf(parameterString, "%X", u32BcParam31_0);
                break;
            }

            /* case OP_CODE_PTU: */
            /* case OP_CODE_WFV: */
            /* case OP_CODE_AMT: */
            /* case OP_CODE_CMM: */
            /* case OP_CODE_LIB: */
            /* { */
               /* break; */
            /* } */

            /* case OP_CODE_LWT: */

            /* No Parameter (N/A) */
            case OP_CODE_RTN:
            case OP_CODE_HLT:
            default:
            {
                pBcOpcodeStatus->u8OpCodeType = ADDR_TYPE_DEFAULT;
                strcpy(parameterString, "       ");
                break;
            }
        }

        /* add some padding if needed */
        if (strlen(parameterString) < PARAMETER_STRING_LEN)
        {
            unsigned int j;
            for (j=0; (strlen(parameterString) < PARAMETER_STRING_LEN); j++)
            {
                strcat(parameterString, " ");
            }
        }

        /* does this opcode have a condition code that needs processing? */
        if (aOpCodeLookupTable[opCodeIndex].bHasConditional)
        {
            conditionCode = (u32BcParam47_32_BCInst & OP_CODE_CONDITION_MASK);

            if (conditionCode > 0xF) /* CCEX = 1 check */
            {
                conditionCode = (conditionCode & 0x0F) | 0x10;
            }

            switch (conditionCode)
            {
                /* -------------------------------------- */
                case CONDITION_CODE_GP0:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if lreater than or GP0 flag low");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if less than or GP0 flag high");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_GP1:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if not equal or GP1 flag set low");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if equal or GP1 flag set high");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_GP2:
                case CONDITION_CODE_GP3:
                case CONDITION_CODE_GP4:
                case CONDITION_CODE_GP5:
                case CONDITION_CODE_GP6:
                case CONDITION_CODE_GP7:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if GP_ flag low");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if GP_ flag high");
                    }

                    /* update the GP # */
                    conditionCodeString[5] = '2' + (char)(conditionCode - CONDITION_CODE_GP2);
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_NO_RESP:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if the RT responded to the previous BC 1553 transaction");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if the RT did not respond to the previous BC 1553 transaction");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_FMT_ERR:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if 1553 format error flag set");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if no format error flag set");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_GOOD_BAD_BLK_XFER:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if message has not completed correctly");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if a valid and error-free message has completed and the data is ready to be read by the host");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_MASKED_STATUS_SET:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if masked status bits not set");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if masked status bits set");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_GOOD_BAD_MESSAGE:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if message completes without a format error, loopback test fail, and no response error for the most recent message");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if either a format error, loopback test fail, or no response error for the most recent message");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_RETRY1:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if no retry was initiated on the last message by the Automatic retry engine");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if at least 1 retry was initiated on the last message by the Automatic retry engine");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_RETRY2:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if a second retry was not initiated on the last message by the Automatic retry engine");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if 2 retries were initiated on the last message by the Automatic retry engine");
                    }
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_ALWAYS:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"(Always disabled condition)");
                    }
                    else
                    {
                        strcpy(conditionCodeString,""); /* this is used a lot, so do not bother printing it */
                    }
                    break;
                }
                /* Extended BC Condtion Codes (available in 32-bit Core Only) */

                /* -------------------------------------- */
                case CONDITION_CODE_GP8:
                case CONDITION_CODE_GP9:
                case CONDITION_CODE_GP10:
                case CONDITION_CODE_GP11:
                case CONDITION_CODE_GP12:
                case CONDITION_CODE_GP13:
                case CONDITION_CODE_GP14:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString,"if GP_ flag low");
                    }
                    else
                    {
                        strcpy(conditionCodeString,"if GP_ flag high");
                    }

                    /* update the GP # */
                    conditionCodeString[5] = '8' + (char)(conditionCode - CONDITION_CODE_GP8);
                    break;
                }

                /* -------------------------------------- */
                case CONDITION_CODE_GROUP_COND_CODE:
                /* -------------------------------------- */
                {
                    if (OP_CODE_CCIN_MASK & u32BcParam47_32_BCInst)
                    {
                        strcpy(conditionCodeString," if the GRP condition tests false with any of the conditions testing false");
                    }
                    else
                    {
                        strcpy(conditionCodeString," if the GRP condition tests true with all conditions testing true");
                    }
                    break;
                }

                /* -------------------------------------- */
                default:
                /* -------------------------------------- */
                {
                    strcpy(conditionCodeString,"Unknown Condition");
                }
            }
        }
        else
        {
            /* no condition to print */
            strcpy(conditionCodeString, "");
        }
    }

    sprintf(addressString, "%08X  ", address);
    strcpy(outString,addressString);

    sprintf(valueString, " %08X ", u32BcParam47_32_BCInst);
    strcat(outString, valueString);

    if (validOpcode)
    {
        strcat(outString, opCodeString);
        strcat(outString, parameterString);

        strcat(outString, opCodeDescString);

        strcat(outString,"   ");
        strcat(outString, conditionCodeString);

        strcat(outString, parityCodeString);
    }

    strcat(outString, "\n");

    printf(outString);


    /* print parameter */
    sprintf(addressString, "%08X  ", address+1);
    strcpy(outString,addressString);

    sprintf(valueString, " %08X ", u32BcParam31_0);
    strcat(outString, valueString);

    strcat(outString, "\n");

    printf(outString);

    return 0;
}

/*******************************************************************************
 * Name:    decodeRange
 *
 * Description:
 *      This function decodes the BC opcode items.
 *
 * In   pDecodeItem
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void decodeRange
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry,
    TYPE_BC_OPCODE_DECODE_INFO *pDecodeItem
)
{
    struct TYPE_BC_OPCODE_DECODE_LIST *toDecodeList = NULL;
    struct TYPE_BC_OPCODE_DECODE_LIST *pCurrent = NULL;
    TYPE_BC_OPCODE_DECODE_INFO sBcOpcodeStatus = { 0 };
    U32BIT u32CurrentAddress = 0;
    U32BIT u32BcParam47_32_BCInst;
    U32BIT u32BcParam31_0;
    /* BOOLEAN bOpCodeDecoded = FALSE; */
    BOOLEAN bDecoded = FALSE;
    BOOLEAN bInRange = FALSE;
    int i;
    int msgBlock;
    int dataBlock;

    u32CurrentAddress = pDecodeItem->u32StartAddr;

    /*
        SMART DECODE
         - try to decode from the start address till either:
             * an invalid opcode is encountered
             * the address has already been decoded

        RAW DECODE
          - just decode the address range regardless of what is in memory
     */

    do
    {
        bInRange = FALSE;

        if (pDecodeItem->u8DecodeType == DECODE_TYPE_SMART)
        {
            /* is this address in the already decoded ranges? */
            struct TYPE_BC_OPCODE_DECODE_LIST *pList = m_pDecodedList;

            while (pList != NULL)
            {
                TYPE_BC_OPCODE_DECODE_INFO decodedElement = pList->info;

                /* is the address in the range? */
                if (((u32CurrentAddress) >= decodedElement.u32StartAddr) &&
                    ((u32CurrentAddress) < decodedElement.u32EndAddr))
                {
                    /* this address was already decoded, so stop here */
                    bInRange = TRUE;
                    break;
                }

                pList = pList->pNext;
            }
        }

        if (!bInRange)
        {
            /* not in list, so decode... */

            if (pDecodeItem->u8OpCodeType == ADDR_TYPE_COMPARE_MEM_ADDR)
            {
                U32BIT u32Data;

                /* single memory address */
                ddccmDiagReadMem(pDeviceInfoEntry, u32CurrentAddress, &u32Data);

                printf("%08X   %08X \n", u32CurrentAddress, u32Data);
            }
            else if (pDecodeItem->u8OpCodeType == ADDR_TYPE_MESSAGE_POINTER)
            {
                U32BIT u32DataBlockPointerAddress[2] = { 0 };
                int nMsgBlocksToDecode = 1;

                printf("------------------------------------\n");
                printf("BC Message Block\n");
                printf("------------------------------------\n");

                if (pDecodeItem->u8OpCode == OP_CODE_XQF)
                {
                    /* XEQ message blocks work with 2 areas, so decode both */
                    nMsgBlocksToDecode = 2;
                }

                for (msgBlock=0; (msgBlock < nMsgBlocksToDecode); msgBlock++)
                {
                    for (i=0; i<BC_MESSGE_BLOCK_SIZE; i++)
                    {
                        U32BIT u32Data;
                        char outString[128];

                        /* single memory address */
                        ddccmDiagReadMem(pDeviceInfoEntry, u32CurrentAddress, &u32Data);

                        sprintf(outString, "%08X   %08X   [%c] %02Xh - %s", u32CurrentAddress, u32Data, (msgBlock==0)?'A':'B', i+(0x10*msgBlock), aBcMsgBlockItemStrings[i]);

                        if (i == BC_MESSGE_BLOCK_DATA_BLOCK_POINTER)
                        {
                            char szBuff[32];

                            u32DataBlockPointerAddress[msgBlock] = (u32Data/2);
                            sprintf(szBuff, " (DWORD Addr: %X)", u32DataBlockPointerAddress[msgBlock]);
                            strcat(outString, szBuff);
                        }

                        strcat(outString, "\n");

                        printf(outString);

                        u32CurrentAddress++;
                    }
                }

                /* print out Data Blocks */
                printf("\n");
                printf("------------------------------------\n");
                printf("Data Block\n");
                printf("------------------------------------\n");
                printf("                      Data Words    \n");

                for (dataBlock=0; dataBlock<2; dataBlock++)
                {
                    u32CurrentAddress = u32DataBlockPointerAddress[dataBlock] & DBP_ADDRESS_MASK;

                    /* check for invalid addresses */
                    if (u32CurrentAddress == 0)
                    {
                        /* skip addresses of 0 */
                        continue;
                    }

                    for (i=0; i<BC_DATA_BLOCK_SIZE; i++)
                    {
                        U32BIT u32Data;
                        U16BIT *pu16Data;
                        char outString[128];

                        /* single memory address */
                        ddccmDiagReadMem(pDeviceInfoEntry, u32CurrentAddress, &u32Data);

                        pu16Data = (U16BIT *) &u32Data;

                        sprintf(outString, "%08X   %08X   [%2d] %04X  [%2d] %04X", u32CurrentAddress, u32Data, (i*2), pu16Data[0], ((i*2)+1), pu16Data[1]);
                        strcat(outString, "\n");

                        printf(outString);

                        u32CurrentAddress++;
                    }

                    printf("\n");
                }
            }
            else /* subroutine */
            {
                /* read the intruction and parameter items */
                ddccmDiagReadMem(pDeviceInfoEntry, u32CurrentAddress, &u32BcParam47_32_BCInst);
                ddccmDiagReadMem(pDeviceInfoEntry, (u32CurrentAddress+1), &u32BcParam31_0);

                /* is this a "valid opcode"? */
                if (((u32BcParam47_32_BCInst & OP_CODE_OKEY_MASK) == OP_CODE_OKEY) ||
                    (pDecodeItem->u8DecodeType == DECODE_TYPE_RAW))
                {
                    DecodeOpCode(u32CurrentAddress, u32BcParam47_32_BCInst, u32BcParam31_0, &sBcOpcodeStatus);

                    /* bOpCodeDecoded = TRUE; */

                    /* do not need to follow addresses for raw */
                    if (pDecodeItem->u8DecodeType != DECODE_TYPE_RAW)
                    {
                        /* is this opcode a JMP, CALL, etc.. type call? */
                        if ((sBcOpcodeStatus.u8OpCodeType == ADDR_TYPE_SHARED_MEMORY_ADDRESS) ||
                            (sBcOpcodeStatus.u8OpCodeType == ADDR_TYPE_COMPARE_MEM_ADDR) ||
                            (sBcOpcodeStatus.u8OpCodeType == ADDR_TYPE_MESSAGE_POINTER))
                        {
                            /* is this address in the already decoded ranges? */
                            struct TYPE_BC_OPCODE_DECODE_LIST *pList = toDecodeList;
                            BOOLEAN bInList = FALSE;

                            while (pList != NULL)
                            {
                                TYPE_BC_OPCODE_DECODE_INFO decodedElement = pList->info;

                                /* is the start address in the list? */
                                if (sBcOpcodeStatus.u32StartAddr == decodedElement.u32StartAddr)
                                {
                                    /* this address was already decoded, so stop here */
                                    bInList = TRUE;
                                    break;
                                }

                                pList = pList->pNext;
                            }

                            if (!bInList)
                            {
                                /* add to the to decode list */
                                AddTail(&toDecodeList, &sBcOpcodeStatus);
                            }
                        }
                    }

                    bDecoded = TRUE;
                }
                else
                {
                    if (pDecodeItem->u8DecodeType == DECODE_TYPE_SMART)
                    {
                        /* stop - no more valid opcodes */
                        bDecoded = FALSE;
                    }
                    else
                    {
                        /* TODO - else needed? */
                    }
                }

                u32CurrentAddress += 2;
            }
        }

        if (pDecodeItem->u8DecodeType == DECODE_TYPE_RAW)
        {
            /* did we reach the end address? */
            if ((u32CurrentAddress) >= pDecodeItem->u32EndAddr)
            {
                bInRange = TRUE;
            }
        }

    } while ((!bInRange) && (bDecoded));

    if (!bInRange)
    {
        /* add this range to the decoded list */
        sBcOpcodeStatus.u32StartAddr = pDecodeItem->u32StartAddr;
        sBcOpcodeStatus.u32EndAddr = u32CurrentAddress;
        AddTail(&m_pDecodedList, &sBcOpcodeStatus);

        printf("\n");
    }

    /* loop through all the items in the list (if any) */
    pCurrent = toDecodeList;

    while (pCurrent != NULL)
    {
        decodeRange(pDeviceInfoEntry, &pCurrent->info);

        pCurrent = pCurrent->pNext;
    }

    RemoveAll(toDecodeList);
}

/*******************************************************************************
 * Name:    ddccmDiagBcOpcodeDecode
 *
 * Description:
 *      This function decodes the BC opcode.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagBcOpcodeDecode
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    U32BIT u32BcInitInstPointerAddr = 0;
    TYPE_BC_OPCODE_DECODE_INFO decodeItem;

    printf("+=====================================================================================================\n");
    printf("| BC Opcode Decoder - DDCCM v%d.%d.%d\n", DDCCM_VER, DDCCM_REV, DDCCM_REL);
    printf("|\n");
    printf("| NOTES:\n");
    printf("|\n");
    printf("| * If no specific conditional behavior is detailed in the instruction definition,\n");
    printf("|   then the general behavior of an instruction is to run if the condition tests\n");
    printf("|   TRUE; otherwise if the condition tests FALSE, the instruction is skipped and\n");
    printf("|   BC moves on to the next instruction.\n");
    printf("|\n");
    printf("| * Message block A and B are decoded, even though B may not be valid\n");
    printf("| \n");
    printf("| * All Data Block Pointers are decoded, even though they not be valid\n");
    printf("+=====================================================================================================\n");

    ddccmDiagReadReg(pDeviceInfoEntry, REG_BC_INITIAL_INSTRUCTION_POINTER_REGISTER, REG_TYPE_BC_OFFSET, &u32BcInitInstPointerAddr);

    printf("\n");
    printf("BC Initial Instruction Pointer Register (0x06): 0x%08X\n", u32BcInitInstPointerAddr);
    printf("\n");
    printf("ADDRESS    VALUE     INSTRUCTION & PARAMETER CONDITION\n");
    printf("--------------------------------------------------------------------------------------------------\n");

    /* clear the list first */
    RemoveAll(m_pDecodedList);

    if (m_u8DecodeType == DECODE_TYPE_SMART)
    {
        decodeItem.u32StartAddr = u32BcInitInstPointerAddr;
        decodeItem.u8OpCodeType = ADDR_TYPE_DEFAULT;
        decodeItem.u8DecodeType = DECODE_TYPE_SMART;
    }
    else /* raw */
    {
        decodeItem.u32StartAddr = u32BcRawDecodeStartAddr;
        decodeItem.u32EndAddr = u32BcRawDecodeEndAddr;
        decodeItem.u8OpCodeType = ADDR_TYPE_DEFAULT;
        decodeItem.u8DecodeType = DECODE_TYPE_RAW;

        if (decodeItem.u32StartAddr == decodeItem.u32EndAddr)
        {
            printf("\n ERROR: Start and end addresses are the same\n");
            return;
        }
    }

    decodeRange(pDeviceInfoEntry, &decodeItem);

    RemoveAll(m_pDecodedList);
}

/*******************************************************************************
 * Name:    ddccmDiagBcOpcodeDecoderMenu
 *
 * Description:
 *      This function prints the BC opcode decoder menu.
 *
 * In   pDeviceInfoEntry    pointer to a specific device in the device info list
 * Out  none
 *
 * Returns:
 ******************************************************************************/
static void ddccmDiagBcOpcodeDecoderMenu
(
    DEVICE_INFORMATION_ENTRY_TYPE const *pDeviceInfoEntry
)
{
    char szStringIn[80] = "";

    do
    {
        printf("\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| [DIAGNOSTICS] MAIN -> BC OPCODE DECODER                         |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("| s  (s)mart decode                                               |\n");
        printf("| r  (r)aw decode                                                 |\n");
        printf("| a  st(a)rt address                            0x%08X        |\n", u32BcRawDecodeStartAddr);
        printf("| e  (e)nd address                              0x%08X        |\n", u32BcRawDecodeEndAddr);
        printf("| x  e(x)it to previous menu                                      |\n");
        printf("+-----------------------------------------------------------------+\n");
        printf("\n Enter command: ");

        scanf("%s", szStringIn);

        printf("\n");


        if (!strcmp(szStringIn, "a"))
        {
            printf("\n");
            printf(" Current Start Address.............: %08X\n", u32BcRawDecodeStartAddr);
            printf(" Enter the new start address in hex: ");

            scanf("%x", &u32BcRawDecodeStartAddr);
        }
        else if (!strcmp(szStringIn, "e"))
        {
            printf("\n");
            printf(" Current End Address.............: %08X\n", u32BcRawDecodeEndAddr);
            printf(" Enter the new end address in hex: ");

            scanf("%x", &u32BcRawDecodeEndAddr);
        }
        else if (!strcmp(szStringIn, "s"))
        {
            m_u8DecodeType = DECODE_TYPE_SMART;
            ddccmDiagBcOpcodeDecode(pDeviceInfoEntry);
        }
        else if (!strcmp(szStringIn, "r"))
        {
            m_u8DecodeType = DECODE_TYPE_RAW;
            ddccmDiagBcOpcodeDecode(pDeviceInfoEntry);
        }
        else if (!strcmp(szStringIn, "x"))
        {
            /* quit to previous menu */
            return;
        }
        else
        {
            printf("    Invalid command!\n");
        }

    } while (1);
}
