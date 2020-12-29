/*******************************************************************************
 * FILE: ddccm.c
 *
 * DESCRIPTION:
 *
 *  DDC Card Manager for Linux, VxWorks, and INTEGRITY.
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

#include <arpa/inet.h>
#include <ctype.h>          /* for isspace */
#include <errno.h>          /* for 'errno' */
#include <fcntl.h>          /* for open(...) */
#include <netdb.h>
#include <netinet/in.h>     /* for inet_pton */
#include <stdio.h>          /* console io */
#include <stdlib.h>         /* for 'atoi()' */
#include <string.h>         /* string functions */
#include <sys/ioctl.h>      /* ioctl */
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>      /* for 'umask' */
#include <unistd.h>         /* for close(...) */
#include <net/if.h>
#include <ifaddrs.h>

#include "ddccm.h"
#include "ddccm_os.h"
#include "ddccm_flash.h"
#include "ddccm_os_flash.h"
#include "include/ddc_error_list.h"

#include "strtable.h"

/* ========================================================================== */
/*                                DATA TYPES                                  */
/* ========================================================================== */

#define DDCCM_EXIT                                                      0xFF

/* message levels */
#define MESSAGE_LEVEL_ERROR                                             0
#define MESSAGE_LEVEL_WARNING                                           1
#define MESSAGE_LEVEL_INFORMATION                                       2

/* error exit conditions */
#define ERROR_EXITING                                                   0
#define ERROR_FOPEN_PROC_BUS_PCI_DEVICES_FAILED                         1
#define ERROR_FSCANF_ON_DATA_OF_PROC_BUS_PCI_DEVICES_FAILED             2
#define ERROR_FGETS_ON_DISCARD_DATA_OF_PROC_BUS_PCI_DEVICES_FAILED      3
#define ERROR_FCLOSE_OF_PROC_BUS_PCI_DEVICES_FAILED                     4
#define ERROR_FOPEN_OF_VAR_LIB_PCMCIA_STAB_FAILED                       5
#define ERROR_READ_OF_VAR_LIB_PCMCIA_STAB_FILE_FAILED                   6
#define ERROR_FCLOSE_OF_VAR_LIB_PCMCIA_STAB_FAILED                      7
#define ERROR_FOPEN_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED              8
#define ERROR_READ_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED               9
#define ERROR_FCLOSE_OF_USR_LIB_DDC_DDCXXXX_CFG_FAILED                  10
#define ERROR_REMOVE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED             11
#define ERROR_FCLOSE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED             12
#define ERROR_CHMOD666_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED           13
#define ERROR_TOO_MANY_DEVICES                                          14
#define ERROR_READ_OF_USR_LIB_DDC_DRIVERS_DDC_FILE_FAILED               15
#define ERROR_NOT_ROOT_USER                                             16
#define ERROR_DEVICE_ALREADY_OPEN                                       17
#define ERROR_FOPEN_OF_PROC_BUS_PCI_DEVICES_FAILED                      18
#define ERROR_READ_OF_PROC_BUS_PCI_DEVICES_FAILED                       19
#define ERROR_CHMOD666_OF_IP_ADDRESS_CFG_FILE_FAILED                    20
#define ERROR_FCLOSE_OF_IP_ADDRESS_CFG_FILE_FAILED                      21


typedef struct _ERROR_MSG_TABLE_TYPE
{
    U8BIT u8ErrorValue;
    U8BIT u8MessageLevel;
    char szError[128];

} ERROR_MSG_TABLE_TYPE;

static const ERROR_MSG_TABLE_TYPE aErrMsg[] =
{
    { ERROR_EXITING,                                                MESSAGE_LEVEL_ERROR,    "Exiting"                                                                   },
    { ERROR_FOPEN_PROC_BUS_PCI_DEVICES_FAILED,                      MESSAGE_LEVEL_ERROR,    "'fopen()' of '/proc/bus/pci/devices' failed"                               },
    { ERROR_FSCANF_ON_DATA_OF_PROC_BUS_PCI_DEVICES_FAILED,          MESSAGE_LEVEL_ERROR,    "'fscanf()' on data of '/proc/bus/pci/devices' failed"                      },
    { ERROR_FGETS_ON_DISCARD_DATA_OF_PROC_BUS_PCI_DEVICES_FAILED,   MESSAGE_LEVEL_ERROR,    "'fgets()' on discard data of '/proc/bus/pci/devices' failed"               },
    { ERROR_FCLOSE_OF_PROC_BUS_PCI_DEVICES_FAILED,                  MESSAGE_LEVEL_ERROR,    "'fclose()' of '/proc/bus/pci/devices' failed"                              },
    { ERROR_FOPEN_OF_VAR_LIB_PCMCIA_STAB_FAILED,                    MESSAGE_LEVEL_ERROR,    "'fopen()' of '/var/lib/pcmcia/stab' failed"                                },
    { ERROR_READ_OF_VAR_LIB_PCMCIA_STAB_FILE_FAILED,                MESSAGE_LEVEL_ERROR,    "Read of '/var/lib/pcmcia/stab' file failed"                                },
    { ERROR_FCLOSE_OF_VAR_LIB_PCMCIA_STAB_FAILED,                   MESSAGE_LEVEL_ERROR,    "'fclose()' of '/var/lib/pcmcia/stab' failed"                               },
    { ERROR_FOPEN_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED,           MESSAGE_LEVEL_ERROR,    "'fopen()' of '/usr/lib/ddc/ddcxxxx.cfg' file failed"                       },
    { ERROR_READ_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED,            MESSAGE_LEVEL_ERROR,    "Read of '/usr/lib/ddc/ddcxxxx.cfg' file failed"                            },
    { ERROR_FCLOSE_OF_USR_LIB_DDC_DDCXXXX_CFG_FAILED,               MESSAGE_LEVEL_ERROR,    "'fclose()' of '/usr/lib/ddc/ddcxxxx.cfg' failed"                           },
    { ERROR_REMOVE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED,          MESSAGE_LEVEL_ERROR,    "'remove()' of '/usr/lib/ddc/ddcxxxx.cfg' file failed"                      },
    { ERROR_FCLOSE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED,          MESSAGE_LEVEL_ERROR,    "'fclose()' of '/usr/lib/ddc/ddcxxxx.cfg' file failed"                      },
    { ERROR_CHMOD666_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED,        MESSAGE_LEVEL_ERROR,    "'chmod(666)' of '/usr/lib/ddc/ddcxxxx.cfg' file failed"                    },
    { ERROR_TOO_MANY_DEVICES,                                       MESSAGE_LEVEL_ERROR,    "Too many devices"                                                          },
    { ERROR_READ_OF_USR_LIB_DDC_DRIVERS_DDC_FILE_FAILED,            MESSAGE_LEVEL_ERROR,    "Read of '/usr/lib/ddc/drivers.ddc' file failed"                            },
    { ERROR_NOT_ROOT_USER,                                          MESSAGE_LEVEL_WARNING,  "You need to have root privlidges to modify device information!"            },
    { ERROR_DEVICE_ALREADY_OPEN,                                    MESSAGE_LEVEL_ERROR,    "Please exit all applications before executing ddccm"                       },
    { ERROR_FOPEN_OF_PROC_BUS_PCI_DEVICES_FAILED,                   MESSAGE_LEVEL_ERROR,    "'fopen()' of '/proc/bus/pci/devices' failed"                               },
    { ERROR_READ_OF_PROC_BUS_PCI_DEVICES_FAILED,                    MESSAGE_LEVEL_ERROR,    "'Read of '/proc/bus/pci/devices' failed"                                   },
    { ERROR_FCLOSE_OF_PROC_BUS_PCI_DEVICES_FAILED,                  MESSAGE_LEVEL_ERROR,    "'fclose() of '/proc/bus/pci/devices' failed"                               },
    { ERROR_CHMOD666_OF_IP_ADDRESS_CFG_FILE_FAILED,                 MESSAGE_LEVEL_ERROR,    "'chmod(666)' of '/usr/lib/ddc/ddccm_ip.cfg' file failed"                   },
    { ERROR_FCLOSE_OF_IP_ADDRESS_CFG_FILE_FAILED,                   MESSAGE_LEVEL_ERROR,    "'fclose() of '/usr/lib/ddc/ddccm_ip.cfg' failed"                           },
};

/* the order of this array must match apCardInfo */
static char szFamilyNames[][32] =
{
    "1553 Data Bus",
    "ARINC 429",
#ifdef LINUX
    "Synchro",
    "Power",
#endif /* LINUX */
};

/* this is the string list of bus types. it must match the DDC_BUS_TYPE enum */
static char szBusType[][16] =
{
    "ISA",
    "PCI",
    "USB",
    "PCMCIA",
    "CARDBUS",
    "VME",
    "VXI",
    "MXI",
    "FIBRE",
    "FIREWIRE",
    "RS232",
    "PARALLEL",
    "REMOTE"
};

#define CFG_FILE_EXTENSION  ".cfg"
#define EXTENDED_FILE_TYPE  "ex"

#define SELECTION_EXIT      0

/* format indexes to the szDeviceSprintfFormat table */
#define FORMAT_PCI_1553     0
#define FORMAT_PCI_429      1
#define FORMAT_PCI_DISC     2

#define FORMAT_USB_1553     3
#define FORMAT_USB_429      4
#define FORMAT_USB_DISC     5

static char szDeviceSprintfFormat[][16] =
{
#ifdef LINUX
    "%s%c%d",   /* PCI 1553 */
    "%s%c",     /* PCI 429 */
    "%s%d",     /* PCI DISC */
    "%s%d",     /* USB 1553 */
    "%sa%d",    /* USB 429 */
    "%sd%d",    /* USB DISC */
#endif /* LINUX */

#ifdef VX_WORKS
    "%s%02d/%c%02d",    /* PCI 1553 */
    "%s%02d/%c%02d",    /* PCI 429 */
    "%s%02d/%c%02d",    /* PCI DISC */
    "%s%02d/%c%02d",    /* USB 1553 */
    "%s%02d/%c%02d",    /* USB 429  */
    "%s%02d/%c%02d",    /* USb DISC */
#endif /* VX_WORKS */

#ifdef INTEGRITY
    "%s%02d%c%02d",     /* PCI 1553 */
    "%s%02d%c%02d",     /* PCI 429 */
    "%s%02d%c%02d",     /* PCI DISC */
#endif /* INTEGRITY */
};


#define MAX_NUM_USB_CHANNELS            4
#define MAX_USB_DEV_FILE_NUM            (MAX_NUM_DEV * MAX_NUM_USB_CHANNELS)
#define MAX_IP_ADDRESSES                20

#define REMOTE_DEVICE_FAMILY_1553       0
#define REMOTE_DEVICE_FAMILY_429        1

/* since html device file name string format is two digits, we can support a max range of 00 to 99 devices */
#define MAX_NUM_HTML_DEVICE_NAMES       100

#define HTML_DEVICE_NAME_BASE           "/usr/lib/ddc/ddccm_device_%02d.htm"
#define HTML_429_DEVICE_NAME_BASE       "/usr/lib/ddc/ddccm_429_device_%02d.htm"
#define HTML_1553_DEVICE_NAME_BASE      "/usr/lib/ddc/ddccm_1553_device_%02d.htm"
#define HTML_ETH_DEVICE_NAME_BASE       "/usr/lib/ddc/ddccm_eth_device_%02d.htm"

/* ========================================================================== */
/*                                  LOCALS                                    */
/* ========================================================================== */

static BOOLEAN bLookupTableAvailable = FALSE;
static BOOLEAN bSkipLookupTable = FALSE;
static LOOKUP_TABLE_TYPE lookupTable;
static BOOLEAN bDdccmExit;
static BOOLEAN bDisplayfindDevicesErrors = FALSE;
static BOOLEAN bAutoEnumDevices = TRUE;

/* number of devices currently installed, and listed    */
/* in the configuration file that are not installed     */
static U8BIT u8NumberOfDevices = 0;

/* Program device list */
static DDCCM_DEVICE_TYPE aDevices[MAX_NUM_DEV];


#ifndef LINUX
static BOOLEAN bPrintCmdLineHelp = FALSE;
#endif /* not defined LINUX */


static char aIpAddressList[MAX_IP_ADDRESSES][16]; /* xxx.xxx.xxx.xxx */
static U8BIT u8IpAddressCount = 0;
static char szIpAddressCfgFileName[MAX_CFG_FILE_NAM_LEN] = "/usr/lib/ddc/ddccm_ip.cfg";


/* ========================================================================== */
/*                              SHARED GLOBALS                                */
/* ========================================================================== */

/* logical device number count (also indicates # of items in the associated logical device table) */
U16BIT ddc1553NumLogicalDevs = 0;
U16BIT ddc429NumLogicalDevs = 0;

#ifndef LINUX
/* global logical device tables  */
DDCCM_DEVICE_TABLE ddccm1553LogicalDeviceTable[DDC_MAX_NUM_DEVICES];
DDCCM_DEVICE_TABLE ddccm429LogicalDeviceTable[DDC_MAX_NUM_DEVICES];
#endif /* not defined LINUX */


/* ========================================================================== */
/*                                  VXWORKS                                   */
/* ========================================================================== */
#ifdef VX_WORKS

#define USB_1553_START_CHANNEL          0
#define USB_1553_END_CHANNEL            MAX_USB_DEV_FILE_NUM
#define USB_1553_CHANNEL_INCREMENT      1

#define USB_429_START_CHANNEL           0
#define USB_429_END_CHANNEL             MAX_USB_DEV_FILE_NUM
#define USB_429_CHANNEL_INCREMENT       1

#define USB_DISC_START_DEVICE           0


#define PCI_1553_START_DEVICE           0
#define PCI_1553_END_DEVICE             31
#define PCI_1553_CHANNEL_INCREMENT      1

#define PCI_429_START_DEVICE            0
#define PCI_429_END_DEVICE              31
#define PCI_429_CHANNEL_INCREMENT       1

/*
    NOTE:   Discrete devices only list start value since there will be
            the same number of discrete devices as 429 or 1553 device types
*/
#define PCI_DISC_START_DEVICE           0 /* DDC TODO */


static char szDevFile1553[][DDCCM_MAX_LINE_BUFFER_LEN] =
{
    { "/ddcacexpci/" },
	{ "/ddcacexusb/" },
    { "/ddce2mapci/" },
    { "/ddcemapci/"  },
    { "" } /* last entry */
};

static char szDevFile429[][DDCCM_MAX_LINE_BUFFER_LEN] =
{
    { "/ddcacexpci/" },
	{ "/ddcacexusb/" },
    { "/ddce2mapci/" },
    { "/ddcemapci/"  },
    { "" } /* last entry */
};

static char (*szDevFile[])[256] =
{
    szDevFile1553,
    szDevFile429,
    NULL
};

#endif /* VX_WORKS */


/* ========================================================================== */
/*                                 INTEGRITY                                  */
/* ========================================================================== */
#ifdef INTEGRITY

#define PCI_1553_START_DEVICE           0
#define PCI_1553_END_DEVICE             31
#define PCI_1553_CHANNEL_INCREMENT      1

#define PCI_429_START_DEVICE            0
#define PCI_429_END_DEVICE              31
#define PCI_429_CHANNEL_INCREMENT       1

/*
    NOTE:   Discrete devices only list start value since there will be
            the same number of discrete devices as 429 or 1553 device types
*/
#define PCI_DISC_START_DEVICE           0 /* DDC TODO */

static char szDevFile1553[][DDCCM_MAX_LINE_BUFFER_LEN] =
{
    { "AcexDev" },
    { "E2maDev" },
    { "EmaDev" },
    { "" } /* last entry */
};

static char szDevFile429[][DDCCM_MAX_LINE_BUFFER_LEN] =
{
    { "AcexDev" },
    { "E2maDev" },
    { "EmaDev" },
    { "" } /* last entry */
};

static char (*szDevFile[])[256] =
{
    szDevFile1553,
    szDevFile429,
    NULL
};

#endif /* INTEGRITY */


/* ========================================================================== */
/*                                  LINUX                                     */
/* ========================================================================== */
#ifdef LINUX

#define USB_1553_START_CHANNEL          0
#define USB_1553_END_CHANNEL            MAX_USB_DEV_FILE_NUM
#define USB_1553_CHANNEL_INCREMENT      4

#define USB_429_START_CHANNEL           2
#define USB_429_END_CHANNEL             MAX_USB_DEV_FILE_NUM
#define USB_429_CHANNEL_INCREMENT       4

#define USB_DISC_START_DEVICE           3

#define PCI_1553_START_DEVICE           'a' /* 'a'    */
#define PCI_1553_END_DEVICE             'q' /* to 'q' */
#define PCI_1553_CHANNEL_INCREMENT       1

#define PCI_429_START_DEVICE            'a' /* 'a'    */
#define PCI_429_END_DEVICE              'q' /* to 'q' */
#define PCI_429_CHANNEL_INCREMENT       1

/*
    NOTE:   Discrete devices only list start value since there will be
            the same number of discrete devices as 429 or 1553 device types
*/
#define PCI_DISC_START_DEVICE           1


/* config file, number of known device types, logical device base   */
/* number and maximum logical device number for each known family   */
static const FAMILY_INFO_TYPE aFamInfo[MAX_NUM_FAMILY] =
{
    { MAX_DEV_1553,     0,  31, "/usr/lib/ddc/ddc1553"      },
    { MAX_DEV_429,      1,  8,  "/usr/lib/ddc/ddc429"       },
    { MAX_DEV_SYNCHRO,  0,  31, "/usr/lib/ddc/ddcsynchro"   },
    { MAX_DEV_POWER,    0,  0,  "/usr/lib/ddc/ddcpower"     }
};

/* these structures associate device id's with bus type, card name  */
/* and number of available channels on that particular card         */

/* Legacy 1553 Cards */
static const CARD_INFO_TYPE aCardInfo1553[MAX_DEV_1553] =
{
   { 0x4DDC0801, DDC_BUS_TYPE_PCI, 1, "BU-65570I1" },
   { 0x4DDC0802, DDC_BUS_TYPE_PCI, 2, "BU-65570I2" },
   { 0x4DDC0881, DDC_BUS_TYPE_PCI, 1, "BU-65570T1" },
   { 0x4DDC0882, DDC_BUS_TYPE_PCI, 2, "BU-65570T2" },
   { 0x4DDC0811, DDC_BUS_TYPE_PCI, 1, "BU-65572I1" },
   { 0x4DDC0812, DDC_BUS_TYPE_PCI, 2, "BU-65572I2" },
   { 0x4DDC0891, DDC_BUS_TYPE_PCI, 1, "BU-65572T1" },
   { 0x4DDC0892, DDC_BUS_TYPE_PCI, 2, "BU-65572T2" },
   { 0x4DDC0000, DDC_BUS_TYPE_PCI, 2, "BU-65549Mx" },
   /* { 0x65553230, DDC_BUS_TYPE_PMCIA, 1, "BU-6555xM2" }, */
};

/* Legacy 429 Cards */
static const CARD_INFO_TYPE aCardInfo429[MAX_DEV_429] =
{
   { 0x4DDC0100, DDC_BUS_TYPE_PCI, 1, "DD-42924I5" },
   { 0x4DDC0020, DDC_BUS_TYPE_PCI, 1, "DD-42916C1" },
   /* { 0x42912430, DDC_BUS_TYPE_PMCIA, 1, "DD-42912M3" }, */
   /* { 0x42924430, DDC_BUS_TYPE_PMCIA, 1, "DD-42924M3" }, */
};

/* Syncro Cards */
static const CARD_INFO_TYPE aCardInfoSyncro[MAX_DEV_SYNCHRO] =
{
   { 0x4DDC0D10, DDC_BUS_TYPE_PCI, 1, "SB-36XXXTX" },
   { 0x4DDC0300, DDC_BUS_TYPE_PCI, 1, "SB-3620xIx" },
   { 0x4DDC0500, DDC_BUS_TYPE_PCI, 1, "SB-3621xIx" },
   { 0x4DDC0600, DDC_BUS_TYPE_PCI, 1, "SB-3622xIx" }
};

/* Power card specific information */
static const CARD_INFO_TYPE aCardInfoPower[MAX_DEV_POWER];

/* array of pointers to each card info structure */
static const CARD_INFO_TYPE *apCardInfo[MAX_NUM_FAMILY] =
{
    aCardInfo1553,
    aCardInfo429,
    aCardInfoSyncro,
    aCardInfoPower
};

#endif /* LINUX */



/* ========================================================================== */
/*                            FUNCTION PROTOTYPES                             */
/* ========================================================================== */
static U32BIT GetDeviceSelection
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo,
    U32BIT u32FlashOperationType
);

static void PrintDeviceList
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo,
    U32BIT u32FlashOperationType
);

static BOOLEAN ddccmValidateIpAddress(char *pszIpAddress);

U8BIT Get32BitVal(U32BIT *val);
static void ddccmReadRemoteIpAddressesFromConfigFile();

#ifdef DDCCM_DIAG
void ddccmDiagMenu
(
    U8BIT u8Family
);
#endif /* DDCCM_DIAG */

/*******************************************************************************
 * Name:    ddccmPressAKey
 *
 * Description:
 *      This function allows application to pause to allow screen contents
 *      to be read.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
void ddccmPressAKey
(
    void
)
{
	printf(IDS_PRESS_A_KEY);
	getchar();
}

/*******************************************************************************
 * Name:    PrintEepWarning
 *
 * Description:
 *      This function prints the EEPROM warning message.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void PrintEepWarning()
{
    printf("(#) One or more of your installed devices (BU-67x10X) will require\n");
    printf("    an additional update to the EEPROM. Please contact DDC Tech Support\n");
    printf("    in order to update to the latest EEPROM version.\n");
    printf("    US Toll Free Technical Support: 1-800-DDC-5757 Ext. 7771\n");
    printf("    Outside US Technical Support:   1-631-567-5600 Ext. 7771\n");
}

#ifndef LINUX

/*******************************************************************************
 * Name:    ddccmGet1553DevInfo
 *
 * Description:
 *      This function returns device information to 1553 library.
 *
 * In   DevNum              logical device number
 * In   u16NumLogicalDevs   number of devices found
 * In   psLogicalDeviceTable device information including type, channel and name
 * Out  none
 *
 ******************************************************************************/
void ddccmGet1553DevInfo(S16BIT DevNum, U16BIT *u16NumLogicalDevs, DDCCM_DEVICE_TABLE *psLogicalDeviceTable)
{
    *u16NumLogicalDevs = ddc1553NumLogicalDevs;
    *psLogicalDeviceTable = ddccm1553LogicalDeviceTable[DevNum];
}

/*******************************************************************************
 * Name:    ddccmGet429DevInfo
 *
 * Description:
 *      This function returns device information to 429 library.
 *
 * In   DevNum              logical device number
 * In   u16NumLogicalDevs   number of devices found
 * In   psLogicalDeviceTable device information including PCI bus, PCI device and device name
 * Out  none
 *
 ******************************************************************************/
void ddccmGet429DevInfo(S16BIT DevNum, U16BIT *u16NumLogicalDevs, DDCCM_DEVICE_TABLE *psLogicalDeviceTable)
{
    *u16NumLogicalDevs = ddc429NumLogicalDevs;
    *psLogicalDeviceTable = ddccm429LogicalDeviceTable[DevNum];
}

#endif /* not defined LINUX */

/*******************************************************************************
 * Name:    ddccmDisplayError
 *
 * Description:
 *      This function displays severe error messages and exits the program.
 *
 * In   status      error value
 * In   nErrno      errno value
 * In   nExit       TRUE=exit, FALSE=continue execution
 * Out  none
 *
 * Returns: the family selection of the user
 ******************************************************************************/
static void ddccmDisplayError(int status, int nErrno, int nExit)
{
    unsigned int i;

    /* search for error value in the error message table */
    for (i=0; i<(sizeof(aErrMsg)/sizeof(aErrMsg[0])); i++)
    {
        /* did we find the error value? */
        if (status == aErrMsg[status].u8ErrorValue)
        {
            switch (aErrMsg[status].u8MessageLevel)
            {
                case MESSAGE_LEVEL_ERROR:
                {
                    printf("\n    Error: %s  (errno: %d)\n\n", aErrMsg[status].szError, nErrno);
                    break;
                }

                case MESSAGE_LEVEL_WARNING:
                {
                    printf("\n    Warning: %s\n\n", aErrMsg[status].szError);
                    break;
                }

                case MESSAGE_LEVEL_INFORMATION:
                default:
                {
                    printf("\n    %s\n\n", aErrMsg[status].szError);
                    break;
                }
            }

            break;
        }
    }

    if (nExit)
#ifdef LINUX
        exit(nErrno);
#else
        /* DDC TODO - what is the exit function to call??? */
        ;
#endif /* LINUX */
}


/*******************************************************************************
 * Name:    ddccmGetFamily
 *
 * Description:
 *      This function prompts the user for the product family they wish
 *      to work with. It will not return until a valid entry is entered.
 *
 * In   none
 * Out  none
 *
 * Returns: the value of the device family which the user has selected
 ******************************************************************************/
static U8BIT ddccmGetFamily(void)
{
    char szStringIn[80] = { '\0' };
    int iDataIn = 0;
    int i;

    printf("\n===========================================================\n");
    printf("  Data Device Corporation Card Manager   ddccm   v%d.%d.%d\n", DDCCM_VER, DDCCM_REV, DDCCM_REL);
    printf("===========================================================\n\n");

    for (i=0; i<MAX_NUM_FAMILY; i++)
    {
        /* start indexing at one */
        printf("    %d    %s\n", i+1, szFamilyNames[i]);
    }

#ifndef LINUX
        printf("    %d    %s\n", i+1, "Command Line Help");
#endif /* not defined LINUX */

    printf("\n");

    do
    {
        printf("Enter desired product family (-1 to quit): ");
        scanf("%s", szStringIn);
        iDataIn = atoi(szStringIn);

        /* did the user want to exit? */
        if (iDataIn == -1)
        {
            iDataIn = DDCCM_EXIT;
            bDdccmExit = TRUE;
        }

    } while (((iDataIn < 1) || (iDataIn > (i+1))) && (iDataIn != DDCCM_EXIT));

#ifndef LINUX
    if (iDataIn == (i+1))
    {
        bPrintCmdLineHelp = TRUE;
    }
#endif /* not defined LINUX */

    /* adjust entered value to account for 1 based index */
    return (U8BIT)(iDataIn - 1);
}

/*******************************************************************************
 * Name:    ddccmFindDevices
 *
 * Description:
 *      This function looks for installed PCI or USB devices for the current
 *      family. Updates global card list with found cards. Note that the driver
 *      for a particular device must be loaded for the device to be found.
 *
 *      Linux Device File Formats:
 *
 *      PCI     /dev/xxxxxxyz
 *                     |   ||
 *                     |   |+----- 1553 - 1 to 8 (channel)
 *                     |   |
 *                     |   +------ 1553 - a to h (device)
 *                     |           429  - a to h (device)
 *                     |           Disc - 1 to 8 (device)
 *                     |
 *                     +---------- Device Name
 *
 *      USB     /dev/xxxxxxyz
 *                     |   ||
 *                     |   |+----- 429  - 0 to 7 (device)
 *                     |   |       Disc - 0 to 7 (device)
 *                     |   |
 *                     |   +------ 1553 - 0 to 15 (channel pair per device 0,1  2,3  4,5 ...)
 *                     |           429  - a
 *                     |           Disc - d
 *                     |
 *                     +---------- device name
 *
 * In   u8Family         which family of products (1553, 429, etc...)
 * In   pDeviceFile     pointer to the driver file template
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmFindDevices(U8BIT u8Family, char *pDeviceFile)
{
#ifndef LINUX
    DDCCM_DEVICE_TABLE  *pddccmLogicalDeviceTable;
    char bFileNameType = 0;
#endif /* not defined LINUX */

    DEV_INFO sDevInfo;
    HWVERSIONINFO hwVersionInfo;
    FLASH_INFO sFlashInfo;
    ENHANCED_CAPABILITY_INFO sEnhancedCardCapabilities;
    U16BIT *pddcNumLogicalDevs;
    U32BIT u32DataArchiveNumber;
    U8BIT u8ChanCount = 0;
    DDC_BUS_TYPE btBusType;
    U32BIT u32RecFwVer = 0;
    U32BIT u32RecEepVer = 0;
    int hDev = -1;
    U8BIT u8FormatSpecifier = 0;
    U8BIT u8DiscreteFormatSpecifier = 0;
    U16BIT u16EntryIndex;
    BOOLEAN bFirmwareMismatch;
    BOOLEAN bEepMismatch;
    char szDevFileDiscrete[80] = "";
    char szDevFile[80] = "";
    U8BIT u8CardRef = 0;
    U8BIT u8CardRefMin = 0;
    U8BIT u8CardRefMax = 0;
    U8BIT u8CardRefIncrement;
    U8BIT u8DiscRef = 0;
    U8BIT u8ChanCnt = 0;

#ifdef INTEGRITY
    Error E;
#endif /* INTEGRITY */

    bFirmwareMismatch = FALSE;
    bEepMismatch = FALSE;

    switch (u8Family)
    {
        /*------------------------------------------*/
        case FAMILY_MIL_STD_1553:
        /*------------------------------------------*/
        {
            /* is this a USB driver? */
            if (strstr(pDeviceFile, "usb") != NULL)
            {
                /* set the start and end items we will be indexing through */
                u8CardRefMin = USB_1553_START_CHANNEL;
                u8CardRefMax = USB_1553_END_CHANNEL;
                u8CardRefIncrement = USB_1553_CHANNEL_INCREMENT;
                u8DiscRef = USB_DISC_START_DEVICE;

                u8FormatSpecifier = FORMAT_USB_1553;
                u8DiscreteFormatSpecifier = FORMAT_USB_DISC;

                btBusType = DDC_BUS_TYPE_USB;
            }
            /* Note: TotalAce called "acexasic" but is PCI */
            else
            {
                /* set the start and end items we will be indexing through */
                u8CardRefMin = PCI_1553_START_DEVICE;
                u8CardRefMax = PCI_1553_END_DEVICE;
                u8CardRefIncrement = PCI_1553_CHANNEL_INCREMENT;
                u8DiscRef = PCI_DISC_START_DEVICE;

                u8FormatSpecifier = FORMAT_PCI_1553;
                u8DiscreteFormatSpecifier = FORMAT_PCI_DISC;

                btBusType = DDC_BUS_TYPE_PCI;
            }

#ifndef LINUX
           pddccmLogicalDeviceTable = ddccm1553LogicalDeviceTable;
#endif /* not defined LINUX */

           pddcNumLogicalDevs = &ddc1553NumLogicalDevs;

            break;
        }

        /*------------------------------------------*/
        case FAMILY_ARINC_429:
        /*------------------------------------------*/
        {
            /* is this a USB driver? */
            if (strstr(pDeviceFile, "usb") != NULL)
            {
                /* set the start and end items we will be indexing through */
                u8CardRefMin = USB_429_START_CHANNEL;
                u8CardRefMax = USB_429_END_CHANNEL;
                u8CardRefIncrement = USB_429_CHANNEL_INCREMENT;
                u8DiscRef = USB_DISC_START_DEVICE;

                u8FormatSpecifier = FORMAT_USB_429;
                u8DiscreteFormatSpecifier = FORMAT_USB_DISC;

                btBusType = DDC_BUS_TYPE_USB;
            }
            else
            {
                /* set the start and end items we will be indexing through */
                u8CardRefMin = PCI_429_START_DEVICE;
                u8CardRefMax = PCI_429_END_DEVICE;
                u8CardRefIncrement = PCI_429_CHANNEL_INCREMENT;
                u8DiscRef = PCI_DISC_START_DEVICE;

                u8FormatSpecifier = FORMAT_PCI_429;
                u8DiscreteFormatSpecifier = FORMAT_PCI_DISC;

                btBusType = DDC_BUS_TYPE_PCI;
            }

#ifndef LINUX
           pddccmLogicalDeviceTable = ddccm429LogicalDeviceTable;
#endif /* not defined LINUX */

           pddcNumLogicalDevs = &ddc429NumLogicalDevs;

            break;
        }

        default:
        {
            /* unknown family */
            return;
        }
    }/* switch (u8Family) */

    /* for all of this card's channels... */
    for (u8CardRef = u8CardRefMin; u8CardRef < u8CardRefMax; u8CardRef+=u8CardRefIncrement)
    {
        /* update the device files */
        switch (u8Family)
        {
            /*------------------------------------------*/
            case FAMILY_MIL_STD_1553:
            /*------------------------------------------*/
            {

#ifdef LINUX
                /* is this a USB driver? */
                if (btBusType == DDC_BUS_TYPE_USB)
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8CardRef+3);
                }
                else if (btBusType == DDC_BUS_TYPE_PCI)
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, 1);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8DiscRef);
                }
                else
                {
                    /* unknown device type */
                    return;
                }
#endif /* LINUX */

#ifdef VX_WORKS
                bFileNameType = DDC_FILE_NAME_TYPE_MIL_STD_1553;

                if ((btBusType == DDC_BUS_TYPE_USB) || (btBusType == DDC_BUS_TYPE_PCI))
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, DDC_FILE_NAME_TYPE_MIL_STD_1553, 0);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8DiscRef, DDC_FILE_NAME_TYPE_IO, 0);
                }
                else
                {
                    /* unknown device type */
                    return;
                }
#endif /* VXWORKS */

#ifdef INTEGRITY
                bFileNameType = DDC_FILE_NAME_TYPE_MIL_STD_1553;

                if (btBusType == DDC_BUS_TYPE_PCI)
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, DDC_FILE_NAME_TYPE_MIL_STD_1553, 0);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8DiscRef, DDC_FILE_NAME_TYPE_IO, 0);
                }
                else
                {
                    /* unknown device type */
                    return;
                }
#endif /* INTEGRITY */
                break;
            }

            /*------------------------------------------*/
            case FAMILY_ARINC_429:
            /*------------------------------------------*/
            {
                /* is this a USB driver? */
#ifdef LINUX
                if (btBusType == DDC_BUS_TYPE_USB)
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8CardRef+1);
                }
                else if (btBusType == DDC_BUS_TYPE_PCI)
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8DiscRef);
                }
                else
                {
                    /* unknown device type */
                    return;
                }
#endif /* LINUX */

#ifdef VX_WORKS
                bFileNameType = DDC_FILE_NAME_TYPE_ARINC_429;

                if ((btBusType == DDC_BUS_TYPE_USB) || (btBusType == DDC_BUS_TYPE_PCI))
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, DDC_FILE_NAME_TYPE_ARINC_429, 0);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8DiscRef, DDC_FILE_NAME_TYPE_IO, 0);
                }
                else
                {
                    /* unknown device type */
                    return;
                }
#endif /* VX_WORKS */

#ifdef INTEGRITY
                bFileNameType = DDC_FILE_NAME_TYPE_ARINC_429;

                if (btBusType == DDC_BUS_TYPE_PCI)
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, DDC_FILE_NAME_TYPE_ARINC_429, 0);
                    sprintf(szDevFileDiscrete, szDeviceSprintfFormat[u8DiscreteFormatSpecifier], pDeviceFile, u8DiscRef, DDC_FILE_NAME_TYPE_IO, 0);
                }
                else
                {
                    /* unknown device type */
                    return;
                }
#endif /* INTEGRITY */
                break;
            }
        }

        /* attempt to open the device. if it fails to open, */
        /* it's probably a non-existant device.             */
#ifdef LINUX
        hDev = open(szDevFile, O_RDWR);
        if (errno == abs(DDC_UDL_ERROR__DEVICE_ALREADY_OPEN))
        {
            ddccmDisplayError(ERROR_DEVICE_ALREADY_OPEN, errno, TRUE);
        }
#endif /* LINUX */

#ifdef VX_WORKS
        hDev = open(szDevFile, O_RDWR, 0);

#if 0
        printf("open %s ", szDevFile);

        if (hDev == -1)
        {
            printf("failed\n");
            /*break;*/
        }
        else
            printf("success\n");
#endif

        /* DDC TODO - need to check to already open... */
#endif /* VX_WORKS */

#ifdef INTEGRITY

        E = RequestResource((Object*)&hDev, szDevFile, "!systempassword");

        if (E != Success)
            break;

#endif /* INTEGRITY */


        if (hDev != -1)
        {
            /*------------------------------------------*/
            /* get the card info                        */
            /*------------------------------------------*/
            ddccmGetDevInfo(hDev, &sDevInfo);

            /*------------------------------------------*/
            /* get the hardware version info            */
            /*------------------------------------------*/
            ddccmGetHwVersionInfo(hDev, &hwVersionInfo, u8Family);

            u32DataArchiveNumber = hwVersionInfo.dwHdlNumber;

            /*------------------------------------------*/
            /* get the flash info                       */
            /*------------------------------------------*/
            ddccmGetFlashInfo(hDev, &sFlashInfo);

            /*------------------------------------------*/
            /* get the channel count                    */
            /*------------------------------------------*/
            /* get channel count by reading the device capabilities */
            ddccmGetEnhancedCapabilities(hDev, &sEnhancedCardCapabilities);

            switch (u8Family)
            {
                case FAMILY_MIL_STD_1553:
                {
                    if (sEnhancedCardCapabilities.channelCount1553 > 0)
                    {
                        u8ChanCount = sEnhancedCardCapabilities.channelCount1553;
                    }
                    else
                    {
                        /* not a 1553 device */
                        u8ChanCount = 0;
                    }

                    break;
                }

                case FAMILY_ARINC_429:
                {
                    /* check to see if this is a valid 429 device */
                    if ((sEnhancedCardCapabilities.channelCount429Rx > 0) ||
                        (sEnhancedCardCapabilities.channelCount429Tx > 0) ||
                        (sEnhancedCardCapabilities.channelCount429Prog > 0) ||
                        (sEnhancedCardCapabilities.channelCount717Rx > 0) ||
                        (sEnhancedCardCapabilities.channelCount717Tx > 0) ||
                        (sEnhancedCardCapabilities.channelCount717Prog > 0) ||
                        (sEnhancedCardCapabilities.channelCountCanBus > 0) ||
                        (sEnhancedCardCapabilities.channelCountRS232 > 0) ||
                        (sEnhancedCardCapabilities.channelCountRS485 > 0) ||
                        (sEnhancedCardCapabilities.channelCountUart > 0)
                        )
                    {
                        /* 429 is device based, so we only need to loop 1 time */
                        u8ChanCount = 1;
                    }
                    else
                    {
                        /* not a 429 device */
                        u8ChanCount = 0;
                    }

                    break;
                }

                default:
                {
                    /* device not supported */
                    u8ChanCount = 0;
                    break;
                }
            }

            /* close the device */
            close(hDev);
            hDev = -1;

            u32RecFwVer = 0;
            u32RecEepVer = 0;
            bFirmwareMismatch = FALSE;
            bEepMismatch = FALSE;
            if ((bLookupTableAvailable) && (sFlashInfo.SectorCount > 0) && (u8ChanCount))
            {
                for (u16EntryIndex=0; u16EntryIndex < lookupTable.u16NumberOfLookupTableEntries; u16EntryIndex++)
                {
                    /* do the data archive numbers match? */
                    if (u32DataArchiveNumber == (U32BIT)lookupTable.entry[u16EntryIndex].nDataArchiveNumber)
                    {
                        /* Some older drivers may not support reading of the data archive number    */
                        /* and will return 00000. Since there are mutiple entries for 00000, we     */
                        /* need to match the device ID's as well. The non-zero values should also   */
                        /* match.                                                                   */
                        if (sDevInfo.dwDeviceID == (U32BIT)lookupTable.entry[u16EntryIndex].nDeviceID)
                        {
                            /* save the recommended firmware version */
                            u32RecFwVer = lookupTable.entry[u16EntryIndex].nRecFwVer;

                            /* check for a firmware mismatch */
                            if (u32RecFwVer > 0)
                            {
                                if ((hwVersionInfo.dwHdlVersion != (U32BIT)(lookupTable.entry[u16EntryIndex].nRecFwVer / 100)) ||
                                    (hwVersionInfo.dwFwVersion  != (U32BIT)(lookupTable.entry[u16EntryIndex].nRecFwVer % 100)))
                                {
                                    bFirmwareMismatch = TRUE;
                                }
                            }

                            /* save the recommended EEPROM version */
                            u32RecEepVer = lookupTable.entry[u16EntryIndex].nRecEepVer;

                            /* now check for a PLX EEPROM mismatch */
                            if (u32RecEepVer != 0)
                            {
                                U32BIT u32DevFwVer = 0;

                                /* convert device FW version from string to the same format as that of the flash.ddc file */
                                u32DevFwVer = hwVersionInfo.dwHdlVersion * 100;
                                u32DevFwVer += hwVersionInfo.dwFwVersion;

                                if ((U32BIT)(lookupTable.entry[u16EntryIndex].nRecEepVer) > u32DevFwVer)
                                {
                                    bEepMismatch = TRUE;
                                }
                            }

                            break;
                        }
                    }
                }
            }

            /*------------------------------------------*/
            /* fill in aDevices                         */
            /*------------------------------------------*/
            for (u8ChanCnt=0; u8ChanCnt < u8ChanCount; u8ChanCnt++)
            {
                /* update the device file template */
#ifdef LINUX
                if (btBusType == DDC_BUS_TYPE_USB)
                {
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, (u8CardRef + u8ChanCnt));
                }
                else if (btBusType == DDC_BUS_TYPE_PCI)
                {
                    /* channels are 1 based */
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, u8ChanCnt+1);
                }
#endif /* LINUX */

#ifdef VX_WORKS
				if ((btBusType == DDC_BUS_TYPE_USB) || (btBusType == DDC_BUS_TYPE_PCI))
                {
                    /* channels are 1 based */
					sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, bFileNameType, u8ChanCnt);
                }
#endif /* VX_WORKS */

#ifdef INTEGRITY
                if (btBusType == DDC_BUS_TYPE_PCI)
                {
                    /* /deviceName/x/Tyz */
                    sprintf(szDevFile, szDeviceSprintfFormat[u8FormatSpecifier], pDeviceFile, u8CardRef, bFileNameType, u8ChanCnt);
                }
#endif /* INTEGRITY */

                aDevices[u8NumberOfDevices].btBusType = btBusType;
                aDevices[u8NumberOfDevices].u32DevID = ((0x0000FFFF & sDevInfo.dwVendorID) << 16) | (0x0000FFFF & sDevInfo.dwDeviceID);
                aDevices[u8NumberOfDevices].u32Bus = sDevInfo.dwBusHub;
                aDevices[u8NumberOfDevices].u32Slot = sDevInfo.dwSlotPort;
                aDevices[u8NumberOfDevices].u32Chan = u8ChanCnt+1;
                aDevices[u8NumberOfDevices].u32FamilyNumber = hwVersionInfo.dwFamilyNumber;
                aDevices[u8NumberOfDevices].u32RemoteDevice = 0;
                aDevices[u8NumberOfDevices].u32RemoteLDN = 0;
                aDevices[u8NumberOfDevices].u32LogDevNum = NOT_SET;
                aDevices[u8NumberOfDevices].u32FirmwareVer = hwVersionInfo.dwHdlVersion;
                aDevices[u8NumberOfDevices].u32InternalFirmwareVer= hwVersionInfo.dwFwVersion;
                aDevices[u8NumberOfDevices].u32Capabilities = hwVersionInfo.dwCapabilities;
                aDevices[u8NumberOfDevices].u32DataArchiveNumber = u32DataArchiveNumber;
                aDevices[u8NumberOfDevices].sFlashInfo = sFlashInfo;
                aDevices[u8NumberOfDevices].u32RecFwVer = u32RecFwVer;
                aDevices[u8NumberOfDevices].u32RecEepVer = u32RecEepVer;
                aDevices[u8NumberOfDevices].bFirmwareMismatch = bFirmwareMismatch;
                aDevices[u8NumberOfDevices].bEepMismatch = bEepMismatch;

                /* decode capabilities register */
                aDevices[u8NumberOfDevices].sEnhancedCardCapabilities = sEnhancedCardCapabilities;

                strcpy(aDevices[u8NumberOfDevices].szDevName, (char *)hwVersionInfo.szModelName);
                strcpy(aDevices[u8NumberOfDevices].szDriver, szDevFile);
                strcpy(aDevices[u8NumberOfDevices].szDriverVersion, (char *)hwVersionInfo.szDriverVersion);
                strcpy(aDevices[u8NumberOfDevices].szDiscreteDriver, szDevFileDiscrete);

#ifndef LINUX
                /*
                 * since this OS does not have a file system or registry to store card manager settings,
                 * we need to keep track of the devices in a table in memory
                 */
                strcpy(pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceName, szDevFile);
                strcpy(pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceNameIo, szDevFileDiscrete);

                if (strstr(pDeviceFile, "acexpci") != NULL)
                {
                    pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceType = DDC_PHY_DEV_ACEXPCI;
                }
                else if (strstr(pDeviceFile, "acexusb") != NULL)
                {
                    pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceType = DDC_PHY_DEV_ACEXUSB;
                }
                else if (strstr(pDeviceFile, "e2mapci") != NULL)
                {
                    pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceType = DDC_PHY_DEV_E2MAPCI;
                }
                else if (strstr(pDeviceFile, "emapci") != NULL)
                {
                    pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceType = DDC_PHY_DEV_EMAPCI;
                }
                else
                {
                    pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceType = DDC_PHY_DEV_NONE;
                }

                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].familyNumber = hwVersionInfo.dwFamilyNumber;

                strcpy(pddccmLogicalDeviceTable[*pddcNumLogicalDevs].szModelName, (char *)hwVersionInfo.szModelName);

                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].boardModel = aDevices[u8NumberOfDevices].sEnhancedCardCapabilities.modelNumber;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].pciBus = aDevices[u8NumberOfDevices].u32Bus;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].pciDevice = aDevices[u8NumberOfDevices].u32Slot;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceId = aDevices[u8NumberOfDevices].u32DevID;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].fwVerMaj = aDevices[u8NumberOfDevices].u32FirmwareVer;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].fwVerMin = aDevices[u8NumberOfDevices].u32InternalFirmwareVer;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].u32RecFwVer = aDevices[u8NumberOfDevices].u32RecFwVer;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].bFirmwareMismatch = aDevices[u8NumberOfDevices].bFirmwareMismatch;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].bEepMismatch = aDevices[u8NumberOfDevices].bEepMismatch;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].bSectorCount = aDevices[u8NumberOfDevices].sFlashInfo.SectorCount;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].channel = u8ChanCnt;
                pddccmLogicalDeviceTable[*pddcNumLogicalDevs].u32RemoteDevice = aDevices[u8NumberOfDevices].u32RemoteDevice;

#endif /* not defined LINUX */

                if (bAutoEnumDevices)
                {
                    /* set the logical device # for the user */
                    switch (u8Family)
                    {
                        case FAMILY_MIL_STD_1553:
                        {
                            aDevices[u8NumberOfDevices].u32LogDevNum = *pddcNumLogicalDevs;
                            break;
                        }

                        case FAMILY_ARINC_429:
                        {
                            aDevices[u8NumberOfDevices].u32LogDevNum = *pddcNumLogicalDevs + 1;
                            break;
                        }
                    }
                }

                u8NumberOfDevices++;

                *pddcNumLogicalDevs = u8NumberOfDevices;

                if (u8NumberOfDevices > MAX_NUM_DEV)
                {
                    ddccmDisplayError(ERROR_TOO_MANY_DEVICES, 0, FALSE);
                }
            }
        } /* if (hDev != -1) */

        u8DiscRef++;
    }
}

#ifdef LINUX
/*******************************************************************************
 * Name:    ddccmFindLegacyDevices
 *
 * Description:
 *      This function looks for legacy devices for the current family.
 *      Updates global card list with found cards.
 *
 * In   u8Family - which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
void ddccmFindLegacyDevices(U8BIT u8Family)
{
    FILE            *hProcBusPciDevices;
    U32BIT u32DevID = 0x12345678;
    U16BIT u16BusSlot;
    U8BIT u8DevCnt;
    U8BIT u8ChanCnt;
    char szDiscard[512] = "";

    /* attempt to open the kernels PCI bus database */
    if( !(hProcBusPciDevices = fopen("/proc/bus/pci/devices", "r")) )
    {
        if( errno == ENOENT )
        {
            return;
        }
        else
        {
            ddccmDisplayError(ERROR_FOPEN_OF_PROC_BUS_PCI_DEVICES_FAILED, errno, FALSE);
        }
    }

    /* While 'scanf()' succeeds on '/proc/bus/pci/devices'... */
    for(;;)
    {
        /* read in the Bus/Slot info and Device ID,  'break' at EOF */
        if( 1 != fscanf(hProcBusPciDevices, "%hx", &u16BusSlot) )
        {
            break;
        }

        if( 1 != fscanf(hProcBusPciDevices, "%x", &u32DevID) )
        {
            ddccmDisplayError(ERROR_READ_OF_PROC_BUS_PCI_DEVICES_FAILED, errno, FALSE);
        }

        /* See if the Device ID matches one of ours for the family we are working with */
        for (u8DevCnt = 0; u8DevCnt < aFamInfo[u8Family].u8MaxDevTypes; u8DevCnt++)
        {
            /* If we have a known device entry of the proper bus type, create the   */
            /* card list entries with default values assigned                       */
            if( (apCardInfo[u8Family][u8DevCnt].btBusType == DDC_BUS_TYPE_PCI) &&
                (apCardInfo[u8Family][u8DevCnt].u32DevID == u32DevID) )
            {
                for(u8ChanCnt = 0; u8ChanCnt < apCardInfo[u8Family][u8DevCnt].u8NumChans; u8ChanCnt++)
                {
                    aDevices[u8NumberOfDevices].btBusType = DDC_BUS_TYPE_PCI;
                    aDevices[u8NumberOfDevices].u32DevID = u32DevID;
                    aDevices[u8NumberOfDevices].u32Bus = (u16BusSlot & 0xff00) >> 8;
                    aDevices[u8NumberOfDevices].u32Slot = (u16BusSlot & 0x00f8) >> 3;
                    aDevices[u8NumberOfDevices].u32Chan = u8ChanCnt+1;
                    aDevices[u8NumberOfDevices].u32FamilyNumber = 0;
                    aDevices[u8NumberOfDevices].u32IpAddr = 0;
                    aDevices[u8NumberOfDevices].u32RemoteLDN = 0;
                    aDevices[u8NumberOfDevices].u32LogDevNum = NOT_SET;
                    aDevices[u8NumberOfDevices].bLegacyDriver = TRUE;

                    strcpy(aDevices[u8NumberOfDevices].szDevName, apCardInfo[u8Family][u8DevCnt].szDevName);

                    u8NumberOfDevices++;

                    if( u8NumberOfDevices > MAX_NUM_DEV )
                    {
                        ddccmDisplayError(ERROR_TOO_MANY_DEVICES, 0, FALSE);
                    }
                }
            }
        }

        /* Discard the rest of the line. */
        if( NULL == fgets(szDiscard, 512, hProcBusPciDevices) )
        {
            ddccmDisplayError(ERROR_FGETS_ON_DISCARD_DATA_OF_PROC_BUS_PCI_DEVICES_FAILED, errno, FALSE);
        }
    }

    /* Close the PCI status file. */
    if (fclose(hProcBusPciDevices))
    {
        ddccmDisplayError(ERROR_FCLOSE_OF_PROC_BUS_PCI_DEVICES_FAILED , errno, FALSE);
    }
}
#endif /* LINUX */

/*******************************************************************************
 * Name:    ddccmFindPcmciaDevices
 *
 * Description:
 *      This function looks for installed PCMCIA devices for the current family.
 *      Updates global card list with found cards.
 *
 * In   u8Family - which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
#ifdef LINUX
static void ddccmFindPcmciaDevices(U8BIT u8Family)
{
    char szDiscard[512] = "";
    int iReadCount;
    U16BIT u16BusSlot;
    U8BIT u8DevCnt;
    U8BIT u8ChanCnt;
    FILE *hStab;

    /* the device ID for these cards is string based    */
    /* attempt to open the card services status file    */
    if (!(hStab = fopen("/var/lib/pcmcia/stab", "r")))
    {
        if (errno == ENOENT)
        {
            return;
        }
        else
        {
            ddccmDisplayError(ERROR_FOPEN_OF_VAR_LIB_PCMCIA_STAB_FAILED, errno, FALSE);
        }
    }

    /* loop while 'scanf()' succeeds on 'stab' file */
    while (1)
    {
        /* keep reading until we come across the string "Socket"        */
        /* use the '[]' 'fscanf()' scheme to find the desired string    */
        do
        {
            /* a return of '0' indicates we hit EOF */
            if ((iReadCount = fscanf(hStab, "%s", (char*)&szDiscard)) == EOF)
            {
                break;
            }

        } while ( strcmp( szDiscard, "Socket") );

        /* we have hit EOF... break out of here */
        if (iReadCount == -1)
        {
            break;
        }

        /* read in the socket number, ':' and the devID string */
        iReadCount = fscanf(hStab, "%x", (unsigned int *)&u16BusSlot);
        iReadCount += fscanf(hStab, "%s", (char*)&szDiscard);
        iReadCount += fscanf(hStab, "%s", (char*)&szDiscard);

        if (iReadCount != 3)
        {
            ddccmDisplayError(ERROR_READ_OF_VAR_LIB_PCMCIA_STAB_FILE_FAILED, 0, FALSE);
        }

        /* truncate the device ID string down to the basic model number */
        szDiscard[10] = '\0';

        /* see if the Device ID matches one of ours for the family we are working with */
        for (u8DevCnt = 0; u8DevCnt < aFamInfo[u8Family].u8MaxDevTypes; u8DevCnt++)
        {
            /* if we have a known device entry of the proper bus type, create the   */
            /* card list entries with default values assigned                       */
            if ((apCardInfo[u8Family][u8DevCnt].btBusType == DDC_BUS_TYPE_PCMCIA) &&
                (!strcmp( apCardInfo[u8Family][u8DevCnt].szDevName, szDiscard)))
            {
                for (u8ChanCnt = 0; u8ChanCnt < apCardInfo[u8Family][u8DevCnt].u8NumChans; u8ChanCnt++)
                {
                    aDevices[u8NumberOfDevices].btBusType = DDC_BUS_TYPE_PCMCIA;
                    aDevices[u8NumberOfDevices].u32DevID = apCardInfo[u8Family][u8DevCnt].u32DevID;
                    aDevices[u8NumberOfDevices].u32Bus = 0;
                    aDevices[u8NumberOfDevices].u32Slot = u16BusSlot;
                    aDevices[u8NumberOfDevices].u32Chan = u8ChanCnt+1; /* assuming channel count increases as expected.  how does physical card channel numbering relate to the order in which we enumerate here? */
                    aDevices[u8NumberOfDevices].u32FamilyNumber = 0;
                    aDevices[u8NumberOfDevices].u32IpAddr = 0;
                    aDevices[u8NumberOfDevices].u32RemoteLDN = 0;
                    aDevices[u8NumberOfDevices].u32LogDevNum = NOT_SET;
                    strcpy(aDevices[u8NumberOfDevices].szDevName, apCardInfo[u8Family][u8DevCnt].szDevName);

                    u8NumberOfDevices++;

                    if (u8NumberOfDevices > MAX_NUM_DEV)
                    {
                        ddccmDisplayError(ERROR_TOO_MANY_DEVICES, 0, FALSE);
                    }
                }
            }
        }
    }

    /* close the PCMCIA status file */
    if (fclose(hStab))
    {
        ddccmDisplayError(ERROR_FCLOSE_OF_VAR_LIB_PCMCIA_STAB_FAILED, errno, FALSE);
    }
}
#endif /* LINUX */

/* ========================================================================== */
/* ========================================================================== */
/* REMOTE DEVICES                                                             */
/* ========================================================================== */
/* ========================================================================== */

#include "ddcsockmsg.h"

#define DISPLAY_OPTION__DO_NOT_PRINT_ERROR      0x00000001

/*******************************************************************************
 * Name:    DisplayFindDeviceErrorsWithString
 *
 * Description:
 *      This function will display a message box that contains the passed in
 *      error value along with a user message.
 *
 * In   nLineNum    source code line #
 * In   pszMessage  user message
 * In   s16Error    error value to display
 * In   u32Options  display options
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
void DisplayFindDeviceErrorsWithString(int nLineNum, char *pszMessage, S16BIT s16Error, U32BIT u32Options)
{
    /* if not enabled, do nothing */
    if (bDisplayfindDevicesErrors == FALSE)
        return;

    /* display message */
    printf("[%-4d] %s   ", nLineNum, pszMessage);

    if ((u32Options & DISPLAY_OPTION__DO_NOT_PRINT_ERROR) == 0)
    {
        printf("   [Error %d]", s16Error);
    }
}

/*******************************************************************************
 * Name:    DDCSockConnectToServer
 *
 * Description:
 *      This function connects to the server and returns the socket.
 *
 * In   pServerAddr         Server IP Address string
 * Out  pClientConnSock     socket needed to communicate with server
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static S16BIT DDCSockConnectToServer
(
    char * pServerAddr,
    DDC_SOCKET * pClientConnSock
)
{
    int iResult;
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    DDC_SOCKET ConnectSocket = INVALID_SOCKET;

    DDC_ZERO_MEMORY( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    /* Resolve the server address and port */
    iResult = getaddrinfo(pServerAddr, DEFAULT_TCP_PORT, &hints, &result);
    if ( iResult != 0 )
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        DDC_WSACleanup();
        return iResult;
    }

    /* Attempt to connect to an address until one succeeds */
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
    {
        /* Create a SOCKET for connecting to server */
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            printf("socket failed with error: %d\n", DDC_WSAGetLastError());
            DDC_WSACleanup();
            break;
        }

        /* Connect to server */
#ifdef VX_WORKS
{
        /* since the default timeout for a TCP socket is 75 seconds (see TCP_CONN_TIMEOUT in the Kernel Configuration), */
        /* we need to use a different connect call to not wait this amount of time  */

        struct timeval time;

        /* timeout for connect */
        time.tv_sec = 5;
        time.tv_usec = 0;

        iResult = connectWithTimeout(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen, &time);
}
#else
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
#endif /* VX_WORKS */

        if (iResult != DDC_SOCK_SUCCESS)
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "connect error %d\n", DDC_WSAGetLastError());
            DDC_closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("\n(Error) Unable to connect to remote device %s\n\n", pServerAddr);
        DDC_WSACleanup();
        return DDC_SOCK_INVALID;
    }

    *pClientConnSock = ConnectSocket;
    return DDC_SOCK_SUCCESS;
}

/*-----------------------------------------------------------------------------
*  Name:    ddccmDDCSockDisconnect
*
*   Description
*       This function shuts down the connection to the server and closes the socket.
*
*   IN:  pClientConnSock  - the socket needed to shutdown
*   IN:  bThreadUp        - point to the flag to check
*   IN:  bShutdown        - TRUE if wants to shutdown the socket first
*   OUT: return           - = 0 success
*                           < 0 failed
-----------------------------------------------------------------------------*/
static S16BIT ddccmDDCSockDisconnect(DDC_SOCKET *ClientConnSock, BOOLEAN *bThreadUp, BOOLEAN bShutdown)
{
    int nSockTimeoutCount;
    S16BIT status = DDC_SOCK_SUCCESS;

    /* shutdown the connection for _dvrClientMsgDispatcher */
    if (bShutdown)
    {
        /* shutdown the connection*/
        if (shutdown(*ClientConnSock, SD_BOTH))
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddccmDDCSockDisconnect: socket shutdown failed with error %d\n", DDC_WSAGetLastError());
            status = DDC_SOCK_ERROR;
        }

        /* wait until bClientMsgDispatcherUp is down */
        if (bThreadUp)
        {
            nSockTimeoutCount = SOCK_CLOSE_TIMEOUT_COUNT;
            while (nSockTimeoutCount)
            {
                if ( *bThreadUp == FALSE)
                {
                    break;
                }

                nSockTimeoutCount--;
                DDC_SOCK_SLEEP_MS(SOCK_CLOSE_WAIT_MS);
            }

            if (nSockTimeoutCount)
            {
                DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_INFO, "ddccmDDCSockDisconnect: socket is down with nSockTimeoutCount = %d\n", nSockTimeoutCount);
            }
            else
            {
                DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddccmDDCSockDisconnect: socket shutdown timed out!\n");
                status = DDC_SOCK_ERROR;
            }
        }
    }

    /* cleanup*/
    if (DDC_closesocket(*ClientConnSock))
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddccmDDCSockDisconnect: DDC_closesocket failed with error %d\n", DDC_WSAGetLastError());
        status = DDC_SOCK_ERROR;
    }

    if (DDC_WSACleanup())
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddccmDDCSockDisconnect: DDC_WSACleanup failed with error %d\n", DDC_WSAGetLastError());
        status = DDC_SOCK_ERROR;
    }

    /* mark the connection as disconnected */
    *ClientConnSock = INVALID_SOCKET;

    return status;
}

/*******************************************************************************
 * Name:    _ddccmFindRemoteDevice
 *
 * Description:
 *      This function looks for remote devices for the current family.
 *      Updates global card list with found cards.
 *
 * In   pszIpAddress    NULL terminated IP Address string
 * In   u8Family - which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void _ddccmFindRemoteDevice(char *pszIpAddress, U8BIT u8Family)
{
#ifndef LINUX
    DDCCM_DEVICE_TABLE  *pddccmLogicalDeviceTable;
#endif /* not defined LINUX */

    int i;
    char szStr[256] = "";
    U8BIT u8NumberOfDev;
    struct in_addr addr;

    U8BIT u8MaxDevices = 0;
    U8BIT *pu8NumberOfDev = &u8NumberOfDev;
    S16BIT s16Result = 0;
    S16BIT s16ReturnRslt = 0;
    DDC_SOCKET ConnectSocket = INVALID_SOCKET;
    U16BIT *pddcNumLogicalDevs;

    REMOTE_DEV_INFO *pRemoteDevices = NULL;
    PDDC_SOCK_MSG_STRUCT pSendReqMsg = NULL;
    PDDC_SOCK_MSG_STRUCT pRecvRespMsg = NULL;

    switch (u8Family)
    {
        case FAMILY_MIL_STD_1553:
        {
            u8MaxDevices = MAX_1553_DEVICES;
            u8Family = REMOTE_DEVICE_FAMILY_1553;
            pddcNumLogicalDevs = &ddc1553NumLogicalDevs;
            break;
        }

        case FAMILY_ARINC_429:
        {
            u8MaxDevices = MAX_429_DEVICES;
            u8Family = REMOTE_DEVICE_FAMILY_429;
            pddcNumLogicalDevs = &ddc429NumLogicalDevs;
            break;
        }

        default:
        {
            /* family type not supported */
            goto FindRemoteDeviceExit;
        }
    }

    pRemoteDevices = malloc(u8MaxDevices*sizeof(REMOTE_DEV_INFO));
    if (pRemoteDevices == NULL)
    {
        sprintf(szStr, "malloc");
        goto FindRemoteDeviceExit;
    }

    *pu8NumberOfDev = 0;

    /******** connect to server, if success, get ConnectSocket ********/
    s16Result = DDCSockConnectToServer(pszIpAddress, &ConnectSocket);
    if (s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "DDCSockConnectToServer");
        goto FindRemoteDeviceExit;
    }

    /****** conszStruct a sending message ******/
    s16Result = DDCSockCreateMessageStruct(DDC_SOCK_MSG_REQ, DDC_SOCK_MSG_CMD_ENUM_DEV, DEFAULT_BUFLEN, &pSendReqMsg);
    if (s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "DDCSockCreateMessageStruct");
        goto FindRemoteDeviceExit;
    }

    s16Result = DDCSockAddFieldToMessageStruct(pSendReqMsg, DDC_SOCK_MSG_FIELD_TYPE_U8, sizeof(U8BIT), &u8Family);
    if (s16Result)
    {
        sprintf(szStr, "DDCSockAddFieldToMessageStruct");
        goto FindRemoteDeviceExit;
    }

    /********** Send the Messsage *********/
    s16Result = DDCSockMsgSend(ConnectSocket, pSendReqMsg);
    if(s16Result != DDC_SOCK_SUCCESS)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "Error Sending Message!\n");
    }

    /********* Receiving response ************/
    s16Result = DDCSockCreateMessageStruct(DDC_SOCK_MSG_RESPONSE, DDC_SOCK_MSG_CMD_ENUM_DEV, DEFAULT_BUFLEN, &pRecvRespMsg);
    if (s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "DDCSockCreateMessageStruct");
        goto FindRemoteDeviceExit;
    }

    s16Result = DDCSockMsgRecv(ConnectSocket, DDC_SOCK_MSG_RESPONSE, DDC_SOCK_MSG_CMD_ENUM_DEV, 10, pRecvRespMsg);
    if (s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "DDCSockRecvMsg");
        goto FindRemoteDeviceExit;
    }

    /********* disconnect to server ********/
    s16Result = ddccmDDCSockDisconnect(&ConnectSocket, NULL, TRUE);
    if (s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "ddccmDDCSockDisconnect");
        goto FindRemoteDeviceExit;
    }

    /******** process the response message *************/
    if (pRecvRespMsg->ddcSockMsgHeader.ddcSockMsgFieldNum < 3)
    {
        sprintf(szStr, "[DDC_SOCK_MSG_ERROR] pRecvRespMsg->ddcSockMsgHeader.ddcSockMsgFieldNum: %d", pRecvRespMsg->ddcSockMsgHeader.ddcSockMsgFieldNum);
        goto FindRemoteDeviceExit;
    }

    /* The first field should be the result */
    s16Result = DDCSockGetFieldDataFromMessageStruct(pRecvRespMsg, 0, DDC_SOCK_MSG_FIELD_TYPE_S16, sizeof(S16BIT), &s16ReturnRslt);
    if (s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "DDCSockGetFieldDataFromMessageStruct");
        DisplayFindDeviceErrorsWithString(__LINE__, szStr, s16Result, 0);
        goto FindRemoteDeviceExit;
    }

    /* The 2nd field should be the number of the devices */
    s16Result = DDCSockGetFieldDataFromMessageStruct(pRecvRespMsg, 1, DDC_SOCK_MSG_FIELD_TYPE_U8, sizeof(U8BIT), pu8NumberOfDev);
    if (s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "DDCSockGetFieldDataFromMessageStruct");
        DisplayFindDeviceErrorsWithString(__LINE__, szStr, s16Result, 0);
        goto FindRemoteDeviceExit;
    }

    /* The 3rd field should be the device information array */
    s16Result = DDCSockGetFieldDataFromMessageStruct(pRecvRespMsg, 2, DDC_SOCK_MSG_FIELD_TYPE_BUF, (sizeof(REMOTE_DEV_INFO)*(*pu8NumberOfDev)), pRemoteDevices);
    if(s16Result != DDC_SOCK_SUCCESS)
    {
        sprintf(szStr, "DDCSockGetFieldDataFromMessageStruct");
        DisplayFindDeviceErrorsWithString(__LINE__, szStr, s16Result, 0);
        goto FindRemoteDeviceExit;
    }

    for (i = 0; i < *pu8NumberOfDev; i++)
    {
        inet_aton(pszIpAddress, &addr);

        aDevices[u8NumberOfDevices].btBusType = DDC_BUS_TYPE_REMOTE;
        aDevices[u8NumberOfDevices].u32DevID = pRemoteDevices[i].u32DevID;
        aDevices[u8NumberOfDevices].u32Bus = 0;
        aDevices[u8NumberOfDevices].u32Slot = 0;
        aDevices[u8NumberOfDevices].u32Chan = pRemoteDevices[i].u32Chan;
        aDevices[u8NumberOfDevices].u32FamilyNumber = pRemoteDevices[i].u32FamilyNum;
        aDevices[u8NumberOfDevices].u32IpAddr = (U32BIT)addr.s_addr;
        aDevices[u8NumberOfDevices].u32RemoteLDN = pRemoteDevices[i].u32LogDevNum;
        aDevices[u8NumberOfDevices].u32RemoteDevice = 1;
        aDevices[u8NumberOfDevices].u32LogDevNum = NOT_SET;
        aDevices[u8NumberOfDevices].u32FirmwareVer = 0;
        aDevices[u8NumberOfDevices].u32InternalFirmwareVer = 0;
        aDevices[u8NumberOfDevices].u32Capabilities = 0;
        aDevices[u8NumberOfDevices].u32DataArchiveNumber = 0;
        aDevices[u8NumberOfDevices].sFlashInfo.StartAddress = 0;
        aDevices[u8NumberOfDevices].sFlashInfo.SectorCount = 0;
        aDevices[u8NumberOfDevices].sFlashInfo.MinSectorSize = 0;
        aDevices[u8NumberOfDevices].u32RecFwVer = 0;
        aDevices[u8NumberOfDevices].bFirmwareMismatch = FALSE;
        aDevices[u8NumberOfDevices].bEepMismatch = FALSE;

        /* decode capabilities register */
        aDevices[u8NumberOfDevices].sEnhancedCardCapabilities.modelNumber = (U8BIT)pRemoteDevices[i].u32ModelNum;

        strcpy(aDevices[u8NumberOfDevices].szDevName, (char *)pRemoteDevices[i].szDevName);
        strcpy(aDevices[u8NumberOfDevices].szDriver, "N/A");
        strcpy(aDevices[u8NumberOfDevices].szDriverVersion, "N/A");
        strcpy(aDevices[u8NumberOfDevices].szDiscreteDriver, "N/A");
        strcpy(aDevices[u8NumberOfDevices].szIpAddr, pszIpAddress);

        if (bAutoEnumDevices)
        {
            /* set the logical device # for the user */
            switch (u8Family)
            {
                case FAMILY_MIL_STD_1553:
                {
                    aDevices[u8NumberOfDevices].u32LogDevNum = *pddcNumLogicalDevs;
#ifndef LINUX
                    pddccmLogicalDeviceTable = ddccm1553LogicalDeviceTable;
#endif /* not defined LINUX */
                    break;
                }
                case FAMILY_ARINC_429:
                {
                    aDevices[u8NumberOfDevices].u32LogDevNum = *pddcNumLogicalDevs + 1;

#ifndef LINUX
                    pddccmLogicalDeviceTable = ddccm429LogicalDeviceTable;
#endif /* not defined LINUX */
                    break;
                }
            }
        }

#ifndef LINUX
        /*
         * since this OS does not have a file system or registry to store card manager settings,
         * we need to keep track of the devices in a table in memory
         */

        /* the following are not needed for remote devices */
        strcpy(pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceName, "");
        strcpy(pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceNameIo, "");
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceType = DDC_PHY_DEV_NONE;
        /* end of not needed items */

        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].familyNumber = aDevices[u8NumberOfDevices].u32FamilyNumber;

        strcpy(pddccmLogicalDeviceTable[*pddcNumLogicalDevs].szModelName, (char *)aDevices[u8NumberOfDevices].szDevName);

        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].boardModel = aDevices[u8NumberOfDevices].sEnhancedCardCapabilities.modelNumber;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].pciBus = aDevices[u8NumberOfDevices].u32Bus;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].pciDevice = aDevices[u8NumberOfDevices].u32Slot;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].deviceId = aDevices[u8NumberOfDevices].u32DevID;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].fwVerMaj = aDevices[u8NumberOfDevices].u32FirmwareVer;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].fwVerMin = aDevices[u8NumberOfDevices].u32InternalFirmwareVer;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].u32RecFwVer = aDevices[u8NumberOfDevices].u32RecFwVer;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].bFirmwareMismatch = aDevices[u8NumberOfDevices].bFirmwareMismatch;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].bEepMismatch = aDevices[u8NumberOfDevices].bEepMismatch;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].bSectorCount = aDevices[u8NumberOfDevices].sFlashInfo.SectorCount;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].channel = aDevices[u8NumberOfDevices].u32Chan;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].u32RemoteDevice = aDevices[u8NumberOfDevices].u32RemoteDevice;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].u32IpAddr = aDevices[u8NumberOfDevices].u32IpAddr;
        pddccmLogicalDeviceTable[*pddcNumLogicalDevs].u32RemoteLDN = aDevices[u8NumberOfDevices].u32RemoteLDN;
#endif /* not defined LINUX */

        u8NumberOfDevices++;

        *pddcNumLogicalDevs = u8NumberOfDevices;

        if (u8NumberOfDevices > MAX_NUM_DEV)
        {
            ddccmDisplayError(ERROR_TOO_MANY_DEVICES, 0, FALSE);
        }
    } /* for (i = 0; i < *pu8NumberOfDev; i++) */

FindRemoteDeviceExit:

    DDCSockDestroyMessageStruct(&pSendReqMsg);
    DDCSockDestroyMessageStruct(&pRecvRespMsg);

    if (pRemoteDevices)
    {
        free(pRemoteDevices);
        pRemoteDevices = NULL;
    }
}

/*******************************************************************************
 * Name:    ddccmFindRemoteDevices
 *
 * Description:
 *      This function looks for remote devices for the current family.
 *      Updates global card list with found cards.
 *
 * In   u8Family - which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmFindRemoteDevices(U8BIT u8Family)
{
    int i;

    ddccmReadRemoteIpAddressesFromConfigFile();

    for (i=0; i<u8IpAddressCount ; i++)
    {
        _ddccmFindRemoteDevice(aIpAddressList[i], u8Family);
    }
}

/*******************************************************************************
 * Name:    ddccmParseConfigFile
 *
 * Description:
 *      This function looks for config file entries for the current family.
 *      Updates global card list with logical device numbers for
 *      found cards, and with entries without associated hardware.
 *
 * In   u8Family - which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
#ifdef LINUX
static void ddccmParseConfigFile(U8BIT u8Family)
{
    FILE *hCfgFile;
    DDC_BUS_TYPE btBusType;
    U32BIT u32BusType;
    U32BIT u32DevID;
    U32BIT u32Bus;
    U32BIT u32Slot;
    U32BIT u32Chan;
    U32BIT u32FamilyNumber;
    U32BIT u32IpAddr;
    U32BIT u32RemoteDev;
    U32BIT u32RemoteLDN;
    U32BIT u32LogDevNum;
    unsigned char u8DevCnt;
    char tempBuffer[256];
    char szCfgFileName[MAX_CFG_FILE_NAM_LEN] = "";

    /* This section parses the configuration file for any device entries    */
    /* Open the appropriate configuration file, if it exists                */

    /* create the file name */
    strcpy(szCfgFileName, aFamInfo[u8Family].szCfgFileName);
    strcat(szCfgFileName, CFG_FILE_EXTENSION);

    if (!(hCfgFile = fopen(szCfgFileName, "r")))
    {
        if (errno == ENOENT)
        {
            return;
        }
        else
        {
            ddccmDisplayError(ERROR_FOPEN_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
        }
    }

    /* while 'scanf()' succeeds on '/usr/lib/ddc/ddcxxxx.cfg' */
    while (1)
    {
        /* read in the logical device number, 'break' at EOF */
        if (fscanf(hCfgFile, "%d", &u32LogDevNum) != 1)
            break;

        if ((fscanf(hCfgFile, "%x", &u32BusType)     != 1) ||
            (fscanf(hCfgFile, "%x", &u32DevID)       != 1) ||
            (fscanf(hCfgFile, "%x", &u32Bus)         != 1) ||
            (fscanf(hCfgFile, "%x", &u32Slot)        != 1) ||
            (fscanf(hCfgFile, "%x", &u32Chan)        != 1) ||
            (fscanf(hCfgFile, "%x", &u32FamilyNumber)!= 1) ||
            (fscanf(hCfgFile, "%x", &u32RemoteDev)   != 1) ||
            (fscanf(hCfgFile, "%x", &u32IpAddr)      != 1) ||
            (fscanf(hCfgFile, "%x", &u32RemoteLDN)    != 1))
        {
            ddccmDisplayError(ERROR_READ_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, 0, FALSE);
        }

        btBusType = u32BusType;

        /* read the rest of the line if needed */
        fgets(tempBuffer, sizeof(tempBuffer), hCfgFile);

        /* zero out variable*/
        /*bDevFound = 0; - not used - */

        /* we are looking at a Plug & Play device entry, let's see  */
        /* if we have system and config file entries that match.    */
        /* '&' in any other non Plug & Play bus types here          */
        if (btBusType != DDC_BUS_TYPE_ISA)
        {
            /* see if the Device ID matches one of ours */
            /* for the family we are working with       */
            for (u8DevCnt = 0 ; u8DevCnt < u8NumberOfDevices ; u8DevCnt++ )
            {
                /* If we have a matching entry of the current card  */
                /* list element in the config file, complete the    */
                /* element with the proper information, adjust the  */
                /* total card count then flag success and break.    */
                if (u32RemoteDev == 0)
                {
                    /* local devices */
                    if ((aDevices[u8DevCnt].btBusType == btBusType) &&
                        (aDevices[u8DevCnt].u32DevID == u32DevID) &&
                        (aDevices[u8DevCnt].u32Bus == u32Bus) &&
                        (aDevices[u8DevCnt].u32Slot == u32Slot) &&
                        (aDevices[u8DevCnt].u32Chan == u32Chan))
                    {
                        aDevices[u8DevCnt].u32LogDevNum = u32LogDevNum;
                        u8DevCnt = 0;
                        /*bDevFound = 1; - not used - */
                        break;
                    }
                }
                else
                {
                    /* remote devices */
                    if ((aDevices[u8DevCnt].btBusType == btBusType) &&
                        (aDevices[u8DevCnt].u32DevID == u32DevID) &&
                        (aDevices[u8DevCnt].u32IpAddr == u32IpAddr) &&
                        (aDevices[u8DevCnt].u32RemoteLDN == u32RemoteLDN))
                    {
                        aDevices[u8DevCnt].u32LogDevNum = u32LogDevNum;
                        u8DevCnt = 0;
                        /*bDevFound = 1; - not used - */
                        break;
                    }

                }

            }
        }
    }

    /* close the control panel file */
    if (fclose(hCfgFile))
    {
        ddccmDisplayError(ERROR_FCLOSE_OF_USR_LIB_DDC_DDCXXXX_CFG_FAILED, errno, FALSE);
    }
}
#endif /* LINUX */

/*******************************************************************************
 * Name:    ddccmFindAllDevices
 *
 * Description:
 *      This function calls sub-routines that search for installed cards and
 *      configuration entries.  Add to this routine as needed for new device
 *      bus types.  Call 'ddccmParseConfigFile()' last.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmFindAllDevices(U8BIT u8Family)
{
    unsigned int i;
#ifdef LINUX
    FILE        *fpDriverListFile;
    char szDevFile[DDCCM_MAX_LINE_BUFFER_LEN] = "";
    char szDriverListFilename[32] = IDS_DRIVER_LIST_FILENAME;
#endif /* LINUX */

    /* zero out the device structure */
    memset(aDevices, 0, sizeof(aDevices));

    for (i=0; i<MAX_NUM_DEV; i++)
    {
        aDevices[i].u32LogDevNum = NOT_SET;
    }

    u8NumberOfDevices = 0;

#ifdef LINUX
    /* open the driver file list */
    fpDriverListFile = fopen(szDriverListFilename, "r");

    if (fpDriverListFile == NULL)
    {
        /* error opening file (not found?) */
        ddccmDisplayError(ERROR_READ_OF_USR_LIB_DDC_DRIVERS_DDC_FILE_FAILED, 0, TRUE);
        return;
    }

    /* read a line at a time until we reach the end of the file */
    while (fgets(szDevFile, sizeof(szDevFile), fpDriverListFile) != NULL)
    {
        /* strip trailing newline */
        for (i = 0; i < strlen(szDevFile); i++)
        {
            if ((szDevFile[i] == '\n') || (szDevFile[i] == '\r'))
            {
                szDevFile[i] = '\0';
            }
        }

        ddccmFindDevices(u8Family, szDevFile);
    }

    fclose(fpDriverListFile);
#endif /* LINUX */

#ifndef LINUX
    i = 0;

    /* loop through the driver file list */
    while (szDevFile[u8Family][i][0] != '\0')
    {
        ddccmFindDevices(u8Family, szDevFile[u8Family][i]);
        i++;
    }
#endif /* not defined LINUX */

    ddccmFindRemoteDevices(u8Family);

#ifdef LINUX
    /* find legacy devices */
    ddccmFindPcmciaDevices(u8Family);
    ddccmFindLegacyDevices(u8Family);

    /* If bAutoEnumDevices is true do not read in existing config file. */
    if (!bAutoEnumDevices)
    {
        ddccmParseConfigFile(u8Family);
    }
#endif /* LINUX */
}

/*******************************************************************************
 * Name:    ddccmDisplayDevicesMilStd1553
 *
 * Description:
 *      This function displays all MIL-STD-1553 devices listed in the program
 *      card list.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmDisplayDevicesMilStd1553(void)
{
    U8BIT u8DevCnt;
    BOOLEAN bFoundFirmwareMismatch = FALSE;
    BOOLEAN bFoundEepMismatch = FALSE;

    printf("----------------------------------------------------------------------------\n");
    printf(" Item  Dev   Device Name      Model   Ch   Location          Curr.   Rec.   \n");
    printf("       Num                                                   FW Rev. FW Rev.\n");
    printf("----------------------------------------------------------------------------\n");

    for (u8DevCnt=0; u8DevCnt<u8NumberOfDevices; u8DevCnt++)
    {
        /* check for a EEP mismatch and we are not in backup flash and the recommended version is valid */
        if ((aDevices[u8DevCnt].bEepMismatch) && (aDevices[u8DevCnt].bBackup == FALSE) && (aDevices[u8DevCnt].u32RecEepVer > 0))
        {
            bFoundEepMismatch = TRUE;
            printf("(#) ");
        }
        /* check for a firmware mismatch and we are not in backup flash and the recommended version is valid */
        else if ((aDevices[u8DevCnt].bFirmwareMismatch) && (aDevices[u8DevCnt].bBackup == FALSE) && (aDevices[u8DevCnt].u32RecFwVer > 0))
        {
            bFoundFirmwareMismatch = TRUE;
            printf("(!) ");
        }
        else if ((bSkipLookupTable == TRUE) && (aDevices[u8DevCnt].sFlashInfo.SectorCount > 0))
        {
            /* firmware check skipped (VxWorks only) */
            printf("(S) ");
        }
        else if ((bLookupTableAvailable == FALSE) && (aDevices[u8DevCnt].sFlashInfo.SectorCount > 0))
        {
            /* the device is flashable, but the loopup table is missing */
            printf("(E) ");
        }
        else
        {
            /* print filler */
            printf("    ");
        }

        if (aDevices[u8DevCnt].u32RemoteDevice == 0)
        {
            /* local devices */
            if (aDevices[u8DevCnt].u32LogDevNum == NOT_SET)
            {
                printf("%-2d    -  %-17s  %-2d     %-4d%02d:%02d:00          ",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aDevices[u8DevCnt].u32Chan,
                    aDevices[u8DevCnt].u32Bus,
                    aDevices[u8DevCnt].u32Slot);
            }
            else
            {
                printf("%-2d    %-3d%-17s  %-2d     %-4d%02d:%02d:00          ",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].u32LogDevNum,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aDevices[u8DevCnt].u32Chan,
                    aDevices[u8DevCnt].u32Bus,
                    aDevices[u8DevCnt].u32Slot);
            }
        }
        else
        {
            /* remote devices */
            char aRemoteAddr[20];
            struct in_addr IpAddr;

            IpAddr.s_addr = aDevices[u8DevCnt].u32IpAddr;
            strcpy(aRemoteAddr, inet_ntoa(IpAddr));

            if (aDevices[u8DevCnt].u32LogDevNum == NOT_SET)
            {
                printf("%-2d    -  %-17s  %-2d     %-4d%-16s",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aDevices[u8DevCnt].u32Chan,
                    aRemoteAddr);
            }
            else
            {
                printf("%-2d    %-3d%-17s  %-2d     %-4d%-16s",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].u32LogDevNum,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aDevices[u8DevCnt].u32Chan,
                    aRemoteAddr);
            }
        }

        /* can this device be flashed? */
        if (aDevices[u8DevCnt].sFlashInfo.SectorCount == 0)
        {
            printf("   N/A     N/A\n");
        }
        else
        {
            printf("%2d.%-2d   ", aDevices[u8DevCnt].u32FirmwareVer, aDevices[u8DevCnt].u32InternalFirmwareVer);

            if (aDevices[u8DevCnt].u32RecFwVer == 0)
            {
                if (bLookupTableAvailable)
                    printf(" N/A\n");
                else
                    printf(" ***\n");
            }
            else if (bLookupTableAvailable)
            {
                printf("%2d.%-2d\n", aDevices[u8DevCnt].u32RecFwVer / 100, aDevices[u8DevCnt].u32RecFwVer % 100);
            }
            else
            {
                printf("***\n");
            }
        }
    }

    printf("\n");

    if (u8DevCnt)
    {
        if (bFoundFirmwareMismatch)
        {
            printf("(!) This device does not match the recommended firmware revision\n");
        }

        if (bFoundEepMismatch)
        {
            PrintEepWarning();
        }

        if (bSkipLookupTable == TRUE)
        {
            printf("(S) Skipped firmware verification\n");
        }
        else if (bLookupTableAvailable == FALSE)
        {
            printf("(E) Error reading file \"flash.ddc\"\n");
        }
    }
    else
    {
        printf("  -- No 1553 Devices Found --\n");
    }
}

/*******************************************************************************
 * Name:    ddccmDisplayDevicesARINC429
 *
 * Description:
 *      This function displays all ARINC 429 devices listed in the program
 *      card list.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmDisplayDevicesARINC429(void)
{
    U8BIT u8DevCnt;
    BOOLEAN bFoundFirmwareMismatch = FALSE;
    BOOLEAN bFoundEepMismatch = FALSE;

    printf("--------------------------------------------------------------------------------\n");
    printf(" Item  Dev   Device Name      Model   Location          Curr.   Rec.\n");
    printf("       Num                                              FW Rev. FW Rev.\n");
    printf("--------------------------------------------------------------------------------\n");

    for (u8DevCnt=0; u8DevCnt<u8NumberOfDevices; u8DevCnt++)
    {
        /* check for a EEP mismatch and we are not in backup flash and the recommended version is valid */
        if ((aDevices[u8DevCnt].bEepMismatch) && (aDevices[u8DevCnt].bBackup == FALSE) && (aDevices[u8DevCnt].u32RecEepVer > 0))
        {
            bFoundEepMismatch = TRUE;
            printf("(#) ");
        }
        /* check for a firmware mismatch and we are not in backup flash and the recommended version is valid */
        else if ((aDevices[u8DevCnt].bFirmwareMismatch) && (aDevices[u8DevCnt].bBackup == FALSE) && (aDevices[u8DevCnt].u32RecFwVer > 0))
        {
            bFoundFirmwareMismatch = TRUE;
            printf("(!) ");
        }
        else if ((bSkipLookupTable == TRUE) && (aDevices[u8DevCnt].sFlashInfo.SectorCount > 0))
        {
            /* firmware check skipped (VxWorks only) */
            printf("(S) ");
        }
        else if ((bLookupTableAvailable == FALSE) && (aDevices[u8DevCnt].sFlashInfo.SectorCount > 0))
        {
            /* the device is flashable, but the loopup table is missing */
            printf("(E) ");
        }
        else
        {
            /* print filler */
            printf("    ");
        }

        if (aDevices[u8DevCnt].u32RemoteDevice == 0)
        {
            /* local devices */
            if (aDevices[u8DevCnt].u32LogDevNum == NOT_SET)
            {
                printf("%-2d    -  %-17s  %-2d    %02d:%02d:00          ",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aDevices[u8DevCnt].u32Bus,
                    aDevices[u8DevCnt].u32Slot);
            }
            else
            {
                printf("%-2d    %-3d%-17s  %-2d    %02d:%02d:00          ",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].u32LogDevNum,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aDevices[u8DevCnt].u32Bus,
                    aDevices[u8DevCnt].u32Slot);
            }
        }
        else
        {
            /* remote devices */
            char aRemoteAddr[20];
            struct in_addr IpAddr;

            IpAddr.s_addr = aDevices[u8DevCnt].u32IpAddr;
            strcpy(aRemoteAddr, inet_ntoa(IpAddr));

            if (aDevices[u8DevCnt].u32LogDevNum == NOT_SET)
            {
                printf("%-2d    -  %-17s  %-2d    %-16s",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aRemoteAddr);
            }
            else
            {
                printf("%-2d    %-3d%-17s  %-2d    %-16s",
                    u8DevCnt+1,
                    aDevices[u8DevCnt].u32LogDevNum,
                    aDevices[u8DevCnt].szDevName,
                    aDevices[u8DevCnt].sEnhancedCardCapabilities.modelNumber,
                    aRemoteAddr);
            }
        }

        /* can this device be flashed? */
        if (aDevices[u8DevCnt].sFlashInfo.SectorCount == 0)
        {
            printf("   N/A     N/A\n");
        }
        else
        {
            printf("%2d.%-2d   ", aDevices[u8DevCnt].u32FirmwareVer, aDevices[u8DevCnt].u32InternalFirmwareVer);

            if (aDevices[u8DevCnt].u32RecFwVer == 0)
            {
                if (bLookupTableAvailable)
                    printf(" N/A\n");
                else
                    printf(" ***\n");
            }
            else if (bLookupTableAvailable)
            {
                printf("%2d.%-2d\n", aDevices[u8DevCnt].u32RecFwVer / 100, aDevices[u8DevCnt].u32RecFwVer % 100);
            }
            else
            {
                printf("***\n");
            }
        }
    }

    printf("\n");

    if (u8DevCnt)
    {
        if (bFoundEepMismatch)
        {
            PrintEepWarning();
        }

        if (bFoundFirmwareMismatch)
        {
            printf("(!) This device does not match the recommended firmware revision\n");
        }

        if (bSkipLookupTable == TRUE)
        {
            printf("(S) Skipped firmware verification\n");
        }
        else if (bLookupTableAvailable == FALSE)
        {
            printf("(E) Error reading file \"flash.ddc\"\n");
        }
    }
    else
    {
        printf("  -- No 429 Devices Found --\n");
    }
}

/*******************************************************************************
 * Name:    ddccmDisplayDevices
 *
 * Description:
 *      This function displays all devices listed in the program card list.
 *
 * In   u8Family         which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmDisplayDevices(U8BIT u8Family)
{
    switch (u8Family)
    {
        case FAMILY_MIL_STD_1553:
        {
            ddccmDisplayDevicesMilStd1553();
            break;
        }

        case FAMILY_ARINC_429:
        {
            ddccmDisplayDevicesARINC429();
            break;
        }

        default:
        {
            /* not supported */
            return;
        }
    }
}

/*******************************************************************************
 * Name:    Get32BitVal
 *
 * Description:
 *      Read a character string from stdin and convert it to a 32-bit number.
 *      a match is found, the index is updated for the device entry.
 *
 * In   val - pointer to storage for a U32BIT value
 * Out  updates val
 *
 * Returns: 1 - number is valid, 0 - invalid
 ******************************************************************************/
U8BIT Get32BitVal
(
    U32BIT *val
)
{
   char *end, buff[13];
   U8BIT retVal = 0;

   /* Read a string from stdin with buffer length checking */
   fgets(buff, sizeof(buff), stdin);

   /* Convert the string to an unsigned 32-bit number  */
   /* and store it in the user-provided storage space. */
   *val = strtoul(buff, &end, 10);

   /* Determine if it was a valid value that was converted.    */
   /* Valid = non-space at start of read-in buffer &&          */
   /*         end doesn't point to the same address as buff && */
   /*         end isn't a newline or a null                    */
   retVal = (U8BIT)(((!isspace((U8BIT)*buff)) &&
                    (end != buff) &&
                    (*end == '\n')) ||
                    (*end == '\0'));

   /* clear the standard input buffer */
   fflush(stdin);

   return (retVal);
}

/*******************************************************************************
 * Name:    PrintDeviceList
 *
 * Description:
 *      This function will print out all devices found to the display.
 *
 * In   pDeviceInfo             pointer to the head of the device info list
 * In   u32FlashOperationType   type of flash operation
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void PrintDeviceList
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo,
    U32BIT u32FlashOperationType
)
{
	U32BIT u32DeviceCapability = 0;
    U16BIT i;
    BOOLEAN bFoundUnsupportedDevice = FALSE;
    BOOLEAN bFoundTxInhibitBcDisableDevice = FALSE;
    BOOLEAN bFoundFirmwareMismatch = FALSE;
    BOOLEAN bFoundEepMismatch = FALSE;

    printf("----------------------------------------------------------------\n");

    if (u32FlashOperationType == DDCCM_FLASH_TYPE__FIRMWARE)
    {
        printf(" Device  Name            Model  FW ID   Current  Recommended\n");
        printf("                                        FW Rev.  FW Rev.\n");
    }
    else if (u32FlashOperationType == DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE)
    {
        printf(" Device  Name            Model  FW ID\n");
    }
    printf("----------------------------------------------------------------\n");

    for (i=0; i<pDeviceInfo->u16NumberOfDevices; i++)
    {
        if (pDeviceInfo->entry[i].bIsFlashable)
        {
            if (pDeviceInfo->entry[i].bValidatedDevice)
            {
                if (u32FlashOperationType == DDCCM_FLASH_TYPE__FIRMWARE)
                {
                    /* check for a EEP mismatch and we are not in backup flash and the recommended version is valid */
                    if ((aDevices[i].bEepMismatch) && (aDevices[i].bBackup == FALSE) && (aDevices[i].u32RecEepVer > 0))
                    {
                        bFoundEepMismatch = TRUE;
                        printf("(#)");
                    }
                    /* check for a firmware mismatch and we are not in backup flash and the recommended version is valid */
                    else if ((pDeviceInfo->entry[i].bFirmwareMismatch) && (pDeviceInfo->entry[i].bBackup == FALSE) && (pDeviceInfo->entry[i].u32RecFwVer > 0))
                    {
                        /* the firmware in the device does not match the version found in the lookup table */
                        printf("(!)"); /* display index # starting at 1 */

                        bFoundFirmwareMismatch = TRUE;
                    }
                    else
                    {
                        /* print filler */
                        printf("   ");
                    }

                    /* this device was found in the lookup table */
                    if ((aDevices[i].bEepMismatch) && (aDevices[i].bBackup == FALSE) && (aDevices[i].u32RecEepVer > 0))
                    {
                        printf(" * "); /* print invalid selection */
                    }
                    else
                    {
                        printf(" %-2d", i + 1); /* display index # starting at 1 */
                    }
                }
                else if (u32FlashOperationType == DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE)
                {
                    /* print filler */
                    printf("   ");

                    /* Verify each board to see if it has Tx Inhibit/BC Disable capability supported */
                    if (TxInhibitBcDisable_GetCapability(&pDeviceInfo->entry[i], &u32DeviceCapability) != FLASH_SUCCESS)
                    {
                        printf(" * "); /* print invalid selection */
                        bFoundTxInhibitBcDisableDevice = TRUE;
                        pDeviceInfo->entry[i].bIsTxInhibitBcDisableCapable = FALSE;
                    }
                    else
                    {
                        if ((u32DeviceCapability & TX_INH_BC_DISABLE_MASK) != TX_INH_BC_DISABLE_MASK)
                        {
                            bFoundTxInhibitBcDisableDevice = TRUE;
                            printf(" * "); /* print invalid selection */
                            pDeviceInfo->entry[i].bIsTxInhibitBcDisableCapable = FALSE;
                        }
                        else
                        {
                            printf(" %-2d", i + 1); /* display index # starting at 1 */
                            pDeviceInfo->entry[i].bIsTxInhibitBcDisableCapable = TRUE;
                        }
                    }
                }
            }
            else
            {
                /* unknown device */
                bFoundUnsupportedDevice = TRUE;
            }

            if (u32FlashOperationType == DDCCM_FLASH_TYPE__FIRMWARE)
            {
                printf("%s   %-16s  %-2d   %05d   %2d.%-2d",
                    (bFoundUnsupportedDevice == TRUE) ? "  (*) " : "",
                    pDeviceInfo->entry[i].sHwVersionInfo.szModelName,
                    pDeviceInfo->entry[i].sEnhancedCardCapabilities.modelNumber,
                    pDeviceInfo->entry[i].u32DataArchiveNumber,
                    (int)pDeviceInfo->entry[i].sHwVersionInfo.dwHdlVersion,
                    (int)pDeviceInfo->entry[i].sHwVersionInfo.dwFwVersion);

                /* is this device in backup, unsupported, or the recommended version is 0? */
                if ((pDeviceInfo->entry[i].bBackup == TRUE) || bFoundUnsupportedDevice || (pDeviceInfo->entry[i].u32RecFwVer == 0))
                {
                    if (bLookupTableAvailable)
                        /* the recommended firmware version is N/A */
                        printf("     N/A   ");
                    else
                        printf("     ***   ");

                    printf("%s", (pDeviceInfo->entry[i].bBackup == TRUE) ? "(Backup)" : "");
                }
                else
                {
                    printf("    %2d.%-2d   %s",
                        (pDeviceInfo->entry[i].u32RecFwVer / 100),
                        (pDeviceInfo->entry[i].u32RecFwVer % 100),
                        (pDeviceInfo->entry[i].bBackup == TRUE) ? "(Backup)" : "");
                }
            }
            else if (u32FlashOperationType == DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE)
            {

                printf("%s   %-16s  %-2d   %05d  %s",
                    (bFoundUnsupportedDevice == TRUE) ? "  (*) " : "",
                    pDeviceInfo->entry[i].sHwVersionInfo.szModelName,
                    pDeviceInfo->entry[i].sEnhancedCardCapabilities.modelNumber,
                    pDeviceInfo->entry[i].u32DataArchiveNumber,
                    (pDeviceInfo->entry[i].bBackup == TRUE) ? "(Backup)" : "");
            }
        }
        else
        {
            /* not a flashable device, skip it */
        }

        printf("\n");
    }

    printf("----------------------------------------------------------------\n");

    printf("\n");

    if (bFoundEepMismatch)
    {
        PrintEepWarning();
    }

    if (bFoundFirmwareMismatch)
    {
        printf("  (!) This device does not match the recommended firmware revision\n");
    }

    if (bFoundUnsupportedDevice)
    {
        printf("  (*) Unsupported device, please contact DDC\n");
    }

    if (bFoundTxInhibitBcDisableDevice)
    {
        printf("  (*) Device does not support TX Inhibit / BC Disable\n");
    }

    printf("\n");
}

/*******************************************************************************
 * Name:    GetDeviceSelection
 *
 * Description:
 *      This function will ask the user to select a device. The selection returned
 *      will always be a valid value.
 *
 * In   pDeviceInfo             pointer to the head of the device info list
 * In   u32FlashOperationType   type of flash operation
 * Out  none
 *
 * Returns: valid user selection
 ******************************************************************************/
static U32BIT GetDeviceSelection
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo,
    U32BIT u32FlashOperationType
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
        PrintDeviceList(pDeviceInfo, u32FlashOperationType);

        printf("\nEnter the number of the Device # to flash (0 to exit): ");

        u8Valid = Get32BitVal(&u32Value);

        /* make sure the value is valid and the selection is in the rage of device numbers */
        if ((u8Valid) && (u32Value <= pDeviceInfo->u16NumberOfDevices))
        {
            /* does the user want to exit? */
            if (u32Value == SELECTION_EXIT)
                break;

            /* make sure the selection is a valid entry (not a placeholder) */
            /* index is 0 based, but entries are 1 based */

            if (u32FlashOperationType == DDCCM_FLASH_TYPE__FIRMWARE)
            {
                /* make sure that the device is validated and there is no EEPROM mismatch */
                if ((pDeviceInfo->entry[u32Value-1].bValidatedDevice == TRUE) &&
                    (pDeviceInfo->entry[u32Value-1].bEepMismatch == FALSE))
                {
                    break;
                }
            }
            else if (u32FlashOperationType == DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE)
            {
                /* make sure that the device supports Tx Inhibit/BC Disable */
                if (pDeviceInfo->entry[u32Value-1].bIsTxInhibitBcDisableCapable == TRUE)
                {
                    break;
                }
            }
        }

        printf("\n\n  *** Invalid Entry, please try again ***\n\n");
    }

    return u32Value;
}

/*******************************************************************************
 * Name:    ddccmDisplayDeviceInformation
 *
 * Description:
 *      This function displays information about a particular device.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmDisplayDeviceInformation(U8BIT u8Family)
{
    int iDataIn = 0;
    unsigned char bDeviceIndex;
    char szStringIn[80] = "";

    if (!u8NumberOfDevices)
    {
        printf("\n    No items to set.\n");
        return;
    }

    /* display the current card list */
    ddccmDisplayDevices(u8Family);
    printf("\n");

    /* get item number to display */
    do
    {
        printf("Enter item to display (-1 to abort): ");
        scanf("%s", szStringIn);
        iDataIn = atoi(szStringIn);

        if (iDataIn == -1)
            return;

    } while ((iDataIn < 1) || (iDataIn > u8NumberOfDevices));

    /* adjust 1 / 0 indexing into card list here */
    bDeviceIndex = iDataIn - 1;

    if (aDevices[bDeviceIndex].u32LogDevNum == NOT_SET)
    {
        printf("\n    Logical Device #....Not Set");
    }
    else
    {
        printf("\n    Logical Device #... %d", aDevices[bDeviceIndex].u32LogDevNum);
    }

    printf("\n    Device Type........ %s", szBusType[aDevices[bDeviceIndex].btBusType]);
    /* the following information is only available for newer drivers */
    if (!aDevices[bDeviceIndex].bLegacyDriver)
    {
        printf("\n    Driver............. %s", aDevices[bDeviceIndex].szDriver);
        printf("\n    Driver Version..... %s", aDevices[bDeviceIndex].szDriverVersion);
    }

    if (u8Family == FAMILY_MIL_STD_1553)
        printf("\n    Channel............ %d", (int)aDevices[bDeviceIndex].u32Chan);

    printf("\n");

    /* the following information is only available for newer drivers */
    if (!aDevices[bDeviceIndex].bLegacyDriver)
    {
        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount1553 > 0)
            printf("\n    1553 Channels................ %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount1553);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount429Rx > 0)
            printf("\n    429 Rx Channels.............. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount429Rx);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount429Tx > 0)
            printf("\n    429 Tx Channels.............. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount429Tx);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount429Prog > 0)
            printf("\n    Programmable 429 Channels.... %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount429Prog);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount717Rx > 0)
            printf("\n    717 Rx Channels.............. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount717Rx);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount717Tx > 0)
            printf("\n    717 Tx Channels.............. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount717Tx);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount717Prog > 0)
            printf("\n    Programmable 717 Channels.... %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCount717Prog);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountDIO > 0)
            printf("\n    Digital I/O.................. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountDIO);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountAIO > 0)
            printf("\n    Avionic I/O.................. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountAIO);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountRS232 > 0)
            printf("\n    RS-232....................... %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountRS232);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountRS485 > 0)
            printf("\n    RS-422/485................... %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountRS485);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountUart > 0)
            printf("\n    Programmable Serial Channels. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountUart);

        if (aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountCanBus > 0)
            printf("\n    CAN Channel.................. %d", (int)aDevices[bDeviceIndex].sEnhancedCardCapabilities.channelCountCanBus);

        /* only display firmware version if device has a an erasable flash */
        if (aDevices[bDeviceIndex].sFlashInfo.SectorCount > 0)
        {
            printf("\n");
            printf("\n    Firmware Version... %d.%d", (int)aDevices[bDeviceIndex].u32FirmwareVer, (int)aDevices[bDeviceIndex].u32InternalFirmwareVer);
            printf("\n    Firmware ID........ %05d", (int)aDevices[bDeviceIndex].u32DataArchiveNumber);
        }

        if (aDevices[bDeviceIndex].u32Capabilities)
        {
            /* print device capabilities */
            printf("\n\n    Capabilities:\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_MF)
                printf("      * Multi-Function\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_REPLAY)
                printf("      * Replay\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_MTI)
                printf("      * MTI\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_MTIE_MEC)
                printf("      * MEC (Message Error Capture)\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_MTIR_AES)
                printf("      * AES (Advanced Error Sampling)\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_VAR_VOLT_OUT)
                printf("      * Variable Voltage Output\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_PROG_VAR_SPEED_429)
                printf("      * 429 Variable Speed\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_VAR_VOLT_OUT_429)
                printf("      * 429 Variable Voltage Output\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_COUPLING)
                printf("      * Coupling\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_TERMINATION)
                printf("      * Termination\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_EXTERNAL_CLOCK_IN)
                printf("      * External Clock In\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_EXTERNAL_CLOCK_OUT)
                printf("      * External Clock Out\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_IRIG_IN_ANALOG)
                printf("      * IRIG In (Analog)\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_IRIG_IN_DIGITAL)
                printf("      * IRIG In (Digital)\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_IRIG_OUT_DIGITAL)
                printf("      * IRIG Out (Digital)\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_IRIG_RANGE)
                printf("      * IRIG Range\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_TX_INHIBIT_BC_DISABLE)
                printf("      * TX Inhibit / BC Disable\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_CAN)
                printf("      * CAN\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_PPS)
                printf("      * Pulse-Per-Second\n");

            if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_RT_AUTO_BOOT)
                printf("      * RT Auto Boot\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_PROG_ARINC_429)
                printf("      * Programmable ARINC 429\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_PROG_ARINC_717)
                printf("      * Programmable ARINC 717\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_PROG_SERIAL)
                printf("      * Programmable Serial\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_MSI)
                printf("      * MSI\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_TX_INHIBIT)
                printf("      * External TX Inhibit\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_BC_DISABLE)
                printf("      * External BC Inhibit\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_BC_EXT_TRIGGER)
                printf("      * BC External Trigger\n");

           if (aDevices[bDeviceIndex].u32Capabilities & HWVER_CAPABILITY_MRT_DISABLE)
                printf("      * Multi-RT Disable\n");

        }
    }

    printf("\n");
}


/*******************************************************************************
 * Name:    ddccmSetLogicalDeviceNumber
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
#ifdef LINUX
static void ddccmSetLogicalDeviceNumber(U8BIT u8Family)
{
    unsigned char bDeviceIndex;
    unsigned char bDeviceIndex2;
    char szStringIn[80] = "";
    int iDataIn = 0;

    if (!u8NumberOfDevices)
    {
        printf("\n    No items to set.\n");
        return;
    }

    /* display the current card list */
    ddccmDisplayDevices(u8Family);
    printf("\n");

    /* get item number to modify */
    do
    {
        printf("Enter item to modify (-1 to abort): ");
        scanf("%s", szStringIn);
        iDataIn = atoi(szStringIn);

        if (iDataIn == -1)
            return;

    } while ((iDataIn < 1) || (iDataIn > u8NumberOfDevices));

    /* adjust 1 / 0 indexing into card list here */
    bDeviceIndex = iDataIn - 1;

    /* get logical device number to set item to */
    do
    {
        printf("Enter new logical device number [%d - %d, or -1 to disable]> ",
            aFamInfo[u8Family].u8LogNumBase,
            aFamInfo[u8Family].u8LogNumMax);

        scanf("%s", szStringIn);
        iDataIn = atoi(szStringIn);

        if (iDataIn == -1)
        {
            aDevices[bDeviceIndex].u32LogDevNum = NOT_SET;
            ddccmDisplayDevices(u8Family);
            return;
        }

    } while ( (iDataIn < aFamInfo[u8Family].u8LogNumBase) || (iDataIn > aFamInfo[u8Family].u8LogNumMax) );

    /* Check to see if any other device entries are already */
    /* set to this value.  We can't have more than one      */
    /* device of the same logical device number.            */
    for (bDeviceIndex2=0; bDeviceIndex2 < u8NumberOfDevices ; bDeviceIndex2++)
    {
        if (bDeviceIndex != bDeviceIndex2)
        {
            if (aDevices[bDeviceIndex2].u32LogDevNum == iDataIn)
            {
                printf("\n    Disable the other item with this logical device number first.\n");
                return;
            }
        }
    }

    /* assign the logical device number to the device entry */
    aDevices[bDeviceIndex].u32LogDevNum = iDataIn;

    /* if we succeeded, display the card list */
    ddccmDisplayDevices(u8Family);

    return;
}

/*******************************************************************************
 * Name:    ddccmWriteConfigFile
 *
 * Description:
 *      This function writes the current program card list to the current
 *      product family config file.
 *
 * In   u8Family - which family of products (1553, 429, etc...)
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmWriteConfigFile(U8BIT u8Family)
{
    FILE *hCfgFile;
    unsigned char bDeviceIndex, cbCfgEnt = 0;
    char szCfgFileName[MAX_CFG_FILE_NAM_LEN] = "";

    /* create the file name */
    strcpy(szCfgFileName, aFamInfo[u8Family].szCfgFileName);
    strcat(szCfgFileName, CFG_FILE_EXTENSION);

    /* delete the current config. file so the new one   */
    /* will have the proper ownership and permissions   */
    if (remove(szCfgFileName))
    {
        if (errno != ENOENT)
        {
            ddccmDisplayError(ERROR_REMOVE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
        }
    }

    /* determine if any card list entries are to be written to disk */
    /* if not, don't bother creating the configuration file.        */
    for (bDeviceIndex = 0 ; bDeviceIndex < u8NumberOfDevices ; bDeviceIndex++ )
    {
        if (aDevices[bDeviceIndex].u32LogDevNum != NOT_SET)
            cbCfgEnt++;
    }

    if (!cbCfgEnt)
        return;

    /* create and open a new configuration file */
    if (!(hCfgFile = fopen(szCfgFileName, "w+")))
    {
        ddccmDisplayError(ERROR_FOPEN_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
    }

    for ( bDeviceIndex = 0; bDeviceIndex < u8NumberOfDevices; bDeviceIndex++)
    {
        if (aDevices[bDeviceIndex].u32LogDevNum != NOT_SET)
        {
            fprintf(hCfgFile, "%d\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%08x\t%x\n",
                aDevices[bDeviceIndex].u32LogDevNum,
                (unsigned int)aDevices[bDeviceIndex].btBusType,
                (unsigned int)aDevices[bDeviceIndex].u32DevID,
                (unsigned int)aDevices[bDeviceIndex].u32Bus,
                (unsigned int)aDevices[bDeviceIndex].u32Slot,
                (unsigned int)aDevices[bDeviceIndex].u32Chan,
                (unsigned int)aDevices[bDeviceIndex].u32FamilyNumber,
                (unsigned int)aDevices[bDeviceIndex].u32RemoteDevice,
                (unsigned int)aDevices[bDeviceIndex].u32IpAddr,
                (unsigned int)aDevices[bDeviceIndex].u32RemoteLDN);
        }
    }

    /* close the open files */
    if (fclose(hCfgFile))
    {
        ddccmDisplayError(ERROR_FCLOSE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
    }

    /* set the file permissions such that anyone can alter it */
    if (chmod(szCfgFileName, S_IRUSR | S_IWUSR |
        S_IRGRP | S_IWGRP |
        S_IROTH | S_IWOTH   ))
    {
        ddccmDisplayError(ERROR_CHMOD666_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
    }

    /*------------------------------------------*/
    /* create and open a new configuration file */
    /*------------------------------------------*/

    strcpy(szCfgFileName, aFamInfo[u8Family].szCfgFileName);
    strcat(szCfgFileName, EXTENDED_FILE_TYPE);
    strcat(szCfgFileName, CFG_FILE_EXTENSION);

    /* delete the current config. file so the new one   */
    /* will have the proper ownership and permissions   */
    if (remove(szCfgFileName))
    {
        if (errno != ENOENT)
        {
            ddccmDisplayError(ERROR_REMOVE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
        }
    }

    if (!(hCfgFile = fopen(szCfgFileName, "w+")))
    {
        ddccmDisplayError(ERROR_FOPEN_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
    }

    for ( bDeviceIndex = 0; bDeviceIndex < u8NumberOfDevices; bDeviceIndex++)
    {
        if (aDevices[bDeviceIndex].u32LogDevNum != NOT_SET)
        {
            /* write the extended information only for new devices */
            if ((u8Family == FAMILY_MIL_STD_1553) ||
                (u8Family == FAMILY_ARINC_429))
            {
                if (aDevices[bDeviceIndex].bLegacyDriver == FALSE)
                {
                    fprintf(hCfgFile, "%d\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%08x\t%x\t%s\t%s\n",
                        aDevices[bDeviceIndex].u32LogDevNum,
                        (unsigned int)aDevices[bDeviceIndex].btBusType,
                        (unsigned int)aDevices[bDeviceIndex].u32DevID,
                        (unsigned int)aDevices[bDeviceIndex].u32Bus,
                        (unsigned int)aDevices[bDeviceIndex].u32Slot,
                        (unsigned int)aDevices[bDeviceIndex].u32Chan,
                        (unsigned int)aDevices[bDeviceIndex].u32FamilyNumber,
                        (unsigned int)aDevices[bDeviceIndex].u32RemoteDevice,
                        (unsigned int)aDevices[bDeviceIndex].u32IpAddr,
                        (unsigned int)aDevices[bDeviceIndex].u32RemoteLDN,
                        aDevices[bDeviceIndex].szDriver,
                        aDevices[bDeviceIndex].szDiscreteDriver);
                }
            }
        }
    }

    /* close the open files */
    if (fclose(hCfgFile))
    {
        ddccmDisplayError(ERROR_FCLOSE_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
    }

    /* set the file permissions such that anyone can alter it */
    if (chmod(szCfgFileName, S_IRUSR | S_IWUSR |
        S_IRGRP | S_IWGRP |
        S_IROTH | S_IWOTH   ))
    {
        ddccmDisplayError(ERROR_CHMOD666_OF_USR_LIB_DDC_DDCXXXX_CFG_FILE_FAILED, errno, FALSE);
    }
}
#endif /* LINUX */

/*******************************************************************************
 * Name:    ddccmFlashDevice
 *
 * Description:
 *      This function will display a list of devices that can be flashed,
 *      to the user. Once the user has made a valid selection, the flash
 *      utility functions will be called.
 *
 * In   u32FlashOperationType   type of flash operation
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmFlashDevice(U32BIT u32FlashOperationType)
{
    /* list of information for all devices */
    DEVICE_INFORMATION_TYPE deviceInfo;

    U32BIT u32Selection;
    S32BIT nError;
    U16BIT i;
    BOOLEAN bFoundFlashableDevice = FALSE;

    /*------------------------------------------------------*/
    /* find all known devices                               */
    /*------------------------------------------------------*/
    nError = ddccmOsFindDevices(&deviceInfo, TRUE);
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
    /* match devices found with devices in the lookup table */
    /*------------------------------------------------------*/
    MatchDevices(&deviceInfo, &lookupTable);

    /* was there at least one flashable device found? */
    for (i=0; i<deviceInfo.u16NumberOfDevices; i++)
    {
        if (deviceInfo.entry[i].bIsFlashable)
        {
            bFoundFlashableDevice = TRUE;
            break;
        }
    }

    if (bFoundFlashableDevice == FALSE)
    {
        printf("\n\n(Error) *** No valid devices found! ***\n");
        ddccmPressAKey();
        return;
    }

    /*------------------------------------------------------*/
    /* get the selection from the user                      */
    /*------------------------------------------------------*/
    u32Selection = GetDeviceSelection(&deviceInfo, u32FlashOperationType);

    switch (u32Selection)
    {
        case SELECTION_EXIT:
        {
            ddccmOsFreeDevices(&deviceInfo);
            return;
        }

        default:
        {
            nError = FlashDevice(&deviceInfo.entry[u32Selection-1], &lookupTable, u32FlashOperationType); /* display index # starts at 1, so we need to adjust for it */

            if (nError == FLASH_SUCCESS)
            {
                printf("\n\n   Device Flashing Complete!\n\n   You MUST power down your system and restart for changes to take effect.");
            }

            ddccmPressAKey();
            break;
        }
    }

    ddccmOsFreeDevices(&deviceInfo);
}

#ifdef VX_WORKS
/*******************************************************************************
 * Name:    ddccmAddIpAddress
 *
 * Description:
 *      This function will add an IP address to the aIpAddressList.
 *
 *      Updates global variable: aIpAddressList
 *
 * In   szStringIn      IP Address string "xxx.xxx.xxx.xxx"
 * Out  none
 *
 * Returns:
 *      0   IP address added to table
 *     -1   IP address table full
 *     -2   invalid IP address
 *     -3   duplicate IP address
 ******************************************************************************/
int ddccmAddIpAddress
(
    char *szStringIn
)
{
    BOOLEAN bValidIpAddress = FALSE;
    int i;

    if (u8IpAddressCount == MAX_IP_ADDRESSES)
    {
        /* ERROR: table full, can not add any more addresses */
        return -1;
    }

    bValidIpAddress = ddccmValidateIpAddress(szStringIn);

    if (bValidIpAddress == FALSE)
    {
        /* ERROR: invalid IP address */
        return -2;
    }

    /* check to see of the IP address is already in the table */
    for (i=0; i<u8IpAddressCount; i++)
    {
        if (!strcmp(aIpAddressList[i], szStringIn))
        {
            /* ERROR: duplicate IP address */
            return -3;
        }
    }

    /* add the entry */
    strcpy(aIpAddressList[u8IpAddressCount], szStringIn);
    u8IpAddressCount++;

    return 0;
}
#endif /* VX_WORKS */

/*******************************************************************************
 * Name:    ddccmReadRemoteIpAddressesFromConfigFile
 *
 * Description:
 *      This function will try to open the IP address config file. This file
 *      contains a list of IP addresses used to find remote devices. If the
 *      file does not exist, the function just returns.
 *
 *      Updates global variable: aIpAddressList
 *
 *      Config File Format: list of IP Address strings
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmReadRemoteIpAddressesFromConfigFile()
{
#ifdef LINUX
    FILE    *hCfgFile;
    unsigned int i;

    hCfgFile = fopen(szIpAddressCfgFileName, "r");

    if (hCfgFile == NULL)
    {
        return;
    }
    u8IpAddressCount = 0;

    /* parse out entries */
    while (fgets(aIpAddressList[u8IpAddressCount], sizeof(aIpAddressList[u8IpAddressCount]), hCfgFile) != NULL)
    {
        /* strip trailing newline */
        for (i = 0; i < strlen(aIpAddressList[u8IpAddressCount]); i++)
        {
            if ((aIpAddressList[u8IpAddressCount][i] == '\n') || (aIpAddressList[u8IpAddressCount][i] == '\r'))
            {
                aIpAddressList[u8IpAddressCount][i] = '\0';
            }
        }

        u8IpAddressCount++;
        if (u8IpAddressCount == MAX_IP_ADDRESSES)
        {
            break;
        }
    }
    fclose(hCfgFile);
#endif /* LINUX */
}

/*******************************************************************************
 * Name:    ddccmWriteRemoteIpAddressesToConfigFile
 *
 * Description:
 *      This function will write the IP address list to a config file.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmWriteRemoteIpAddressesToConfigFile()
{
    FILE    *hCfgFile;
    int i;

    hCfgFile = fopen(szIpAddressCfgFileName, "w");

    if (hCfgFile != NULL)
    {
        /* write out the entries */
        for (i = 0; i < u8IpAddressCount; i++)
        {
            fprintf(hCfgFile, "%s\n", aIpAddressList[i]);
        }
    }
    else
    {
        ddccmDisplayError(ERROR_FCLOSE_OF_IP_ADDRESS_CFG_FILE_FAILED, errno, FALSE);
    }

    /* set the file permissions such that anyone can alter it */
    if (chmod(szIpAddressCfgFileName, S_IRUSR | S_IWUSR |
        S_IRGRP | S_IWGRP |
        S_IROTH | S_IWOTH   ))
    {
        ddccmDisplayError(ERROR_CHMOD666_OF_IP_ADDRESS_CFG_FILE_FAILED, errno, FALSE);
    }

    fclose(hCfgFile);
}


/*******************************************************************************
 * Name:    ddccmModifyIpAddressInList
 *
 * Description:
 *      This function will modify an entry in the IP addresses list.
 *
 * In   pszIpAddress    NULL terminated IP Address string
 * Out  none
 *
 * Returns: TRUE=valid IP Address, FALSE=invalid IP Address
 ******************************************************************************/
static BOOLEAN ddccmValidateIpAddress(char *pszIpAddress)
{
    struct sockaddr_in sa;
    int nResult;

    /* check for a valid address by seeing if the inet_pton function can */
    /* convert the address */
    nResult = inet_pton(AF_INET, pszIpAddress, &(sa.sin_addr));

    /* check for an error */
    if (nResult != 1)
    {
        return FALSE;
    }

    return TRUE;
}

/*******************************************************************************
 * Name:    ddccmMAddIpAddressToList
 *
 * Description:
 *      This function will add an entry to the IP addresses list.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmMAddIpAddressToList()
{
    char szStringIn[80] = "";
    BOOLEAN bValidIpAddress = FALSE;

    printf("Enter the new IP Address: ");

    scanf("%s", szStringIn);

    bValidIpAddress = ddccmValidateIpAddress(szStringIn);

    if (bValidIpAddress)
    {
        /* add the entry */
        strcpy(aIpAddressList[u8IpAddressCount], szStringIn);
        u8IpAddressCount++;
    }
    else
    {
        printf("(Error) Invalid IP Address entered\n");
    }
}

/*******************************************************************************
 * Name:    ddccmModifyIpAddressInList
 *
 * Description:
 *      This function will modify an entry in the IP addresses list.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmModifyIpAddressInList()
{
    int iDataIn;
    char szStringIn[80] = "";
    BOOLEAN bValidIpAddress = FALSE;

    do
    {
        printf("Enter the selection of the address to modify (-1 to abort): ");
        scanf("%s", szStringIn);
        iDataIn = atoi(szStringIn);

        if (iDataIn == -1)
            return;

    } while ((iDataIn < 0) || (iDataIn > u8IpAddressCount));

    printf("Enter the new IP Address: ");

    scanf("%s", szStringIn);

    bValidIpAddress = ddccmValidateIpAddress(szStringIn);

    if (bValidIpAddress)
    {
        /* update the entry */
        strcpy(aIpAddressList[iDataIn], szStringIn);
    }
    else
    {
        printf("(Error) Invalid IP Address entered\n");
    }
}

/*******************************************************************************
 * Name:    ddccmDeleteIpAddressFromList
 *
 * Description:
 *      This function will remove an entry from the IP addresses list.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmDeleteIpAddressFromList()
{
    int iDataIn;
    int i;
    char szStringIn[80] = "";

    do
    {
        printf("Enter the selection of the address to delete (-1 to abort): ");
        scanf("%s", szStringIn);
        iDataIn = atoi(szStringIn);

        if (iDataIn == -1)
            return;

    } while ((iDataIn < 0) || (iDataIn > u8IpAddressCount));

    /* remove item by shifting following entries up one spot, then reduce the count by 1 */
    if (u8IpAddressCount)
    {
        u8IpAddressCount--;
        for (i = iDataIn; i < u8IpAddressCount; i++)
        {
            strcpy(aIpAddressList[i], aIpAddressList[i+1]);
        }
    }
}

/*******************************************************************************
 * Name:    ddccmDisplayRemoteHelp
 *
 * Description:
 *      This function displays user help information for Remote Access.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmDisplayRemoteHelp(void)
{
    printf("\n");
    printf("    d    (d)isplay IP address list\n");
    printf("    a    (a)dd IP address to list\n");
    printf("    m    (m)odify IP address in the list\n");
    printf("    r    (r)emove IP address from the list\n");
    printf("    c    (c)lear IP address list\n");
    printf("    q    (q)uit and return to previous menu\n");
    printf("\n");
}

/*******************************************************************************
 * Name:    ddccmManageRemoteIpAddresses
 *
 * Description:
 *      This function will manage a list of remote IP addresses.
 *      This includes:
 *          - Displaying the list
 *          - Adding an address to the list
 *          - Modifying an address in the list
 *          - Removing an address from the list
 *          - Clearing the list
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmManageRemoteIpAddresses()
{
    int i;
    char szStringIn[80] = "";

    ddccmReadRemoteIpAddressesFromConfigFile();

    ddccmDisplayRemoteHelp();

    do
    {
        printf("\n---------------------------\n");
        printf(" Item   IP Address\n");
        printf("---------------------------\n");

        if (u8IpAddressCount > 0)
        {
            /* print IP address list */
            for (i=0; i<u8IpAddressCount; i++)
            {
                printf("    ");
                printf("%-2d  %s\n", i, aIpAddressList[i]);
            }
        }
        else
        {
            printf("(No Entries)\n");
        }

        printf("\nEnter command ('h' for help): ");

        scanf("%s", szStringIn);
        printf("\n");

        if (!strcmp(szStringIn, "a"))
        {
            /* add IP address to list */
            ddccmMAddIpAddressToList();
        }
        else if (!strcmp(szStringIn, "m"))
        {
            ddccmModifyIpAddressInList();
        }
        else if (!strcmp(szStringIn, "r"))
        {
            ddccmDeleteIpAddressFromList();
        }
        else if (!strcmp(szStringIn, "c"))
        {
            /* clear IP address list */
            u8IpAddressCount = 0;
        }
        else if (!strcmp(szStringIn, "d"))
        {
            continue;
        }
        else if (!strcmp(szStringIn, "q"))
        {
            break;
        }
        else if (!strcmp(szStringIn, "h"))
        {
            ddccmDisplayRemoteHelp();
        }
        else
        {
                printf("\n    Invalid command!\n");
        }

        /* always write the config file, even if there were no changes */
        /* this way we do not need to keep track of the if we need to write or not */
        ddccmWriteRemoteIpAddressesToConfigFile();

    } while (1);
}
/*******************************************************************************
 * Name:    ddccmGenerateBridgingHtmlTable
 *
 * Description:
 *      This function generate individual HTML files for each channel per device.
 *      Used for bridging port mapping.
 *
 * In   pDeviceInfo     Device Information List
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmGenerateBridgingHtmlTable
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo
)
{
#ifdef DDCCM_USE_FILE_SYSTEM

    U16BIT i;
    U8BIT u8ChannelIndex;
    U8BIT u8ChannelCount;
    U8BIT u8PortNo;
    U8BIT u8HtmlFileNo;

    static struct ifreq ifr;
    struct sockaddr_in inet_sockaddr;
    int sock;
    struct ifaddrs *addrs,*tmp;

    char szHtmlOuputFileName[MAX_CFG_FILE_NAM_LEN];
    FILE *fpLookupTableFile;

    /* remove the maximum possible # of files first */
    for (i=0; i < MAX_NUM_HTML_DEVICE_NAMES; i++)
    {
        /* 429 Device Bridging information */
        sprintf(szHtmlOuputFileName, HTML_429_DEVICE_NAME_BASE, i);
        remove(szHtmlOuputFileName);

        /* 1553 Device Bridging information */
        sprintf(szHtmlOuputFileName, HTML_1553_DEVICE_NAME_BASE, i);
        remove(szHtmlOuputFileName);

        /* Ethernet Device Bridging information */
        sprintf(szHtmlOuputFileName, HTML_ETH_DEVICE_NAME_BASE, i);
        remove(szHtmlOuputFileName);
    }

    /*
     * Generate 1553 Bridging information file for each device in the list
     */

    /* Channels numbers are base 0*/
    u8PortNo = 0;
    u8HtmlFileNo = 0;
    for (i = 0; i < pDeviceInfo->u16NumberOfDevices; i++)
    {
        /* Must have at least one channel */
        for (u8ChannelIndex = 0; u8ChannelIndex < pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount1553; u8ChannelIndex++)
        {
            sprintf(szHtmlOuputFileName, HTML_1553_DEVICE_NAME_BASE, u8HtmlFileNo);

            /* open a file for writing */
            fpLookupTableFile = fopen(szHtmlOuputFileName, "w+");

            if (fpLookupTableFile)
            {
                /* Write to html file */
                fprintf(fpLookupTableFile, "<td width=\"200\" align=\"center\">");
                fprintf(fpLookupTableFile, "%d", u8PortNo++);
                fprintf(fpLookupTableFile, "</td>\n");

                fprintf(fpLookupTableFile, "<td width=\"300\" align=\"center\">");
                fprintf(fpLookupTableFile, "%s", pDeviceInfo->entry[i].sHwVersionInfo.szModelName);
                fprintf(fpLookupTableFile, "</td>\n");

                fclose(fpLookupTableFile);
                u8HtmlFileNo++;
            }
        }
    }

    /*
     * Generate 429 Bridging information file for each device in the list
     */

    /* Channels numbers are base 0*/
    u8PortNo = 0;
    u8HtmlFileNo = 0;
    for (i = 0; i < pDeviceInfo->u16NumberOfDevices; i++)
    {
        /* Programmable channel check */
        if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Prog > 0)
        {
            u8ChannelCount = pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Prog;
        }
        /* For fixed channel, assign the larger of 2 values, Tx or Rx count */
        else if ( (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Tx > 0) ||
                  (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Rx > 0) )
        {
            if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Rx >
                pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Tx)
            {
                u8ChannelCount = pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Rx;
            }
            else
            {
                u8ChannelCount = pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Tx;
            }
        }
        else
        {
            u8ChannelCount = 0;
        }

        /* Must have at least one channel */
        for (u8ChannelIndex = 0; u8ChannelIndex < u8ChannelCount; u8ChannelIndex++)
        {
            sprintf(szHtmlOuputFileName, HTML_429_DEVICE_NAME_BASE, u8HtmlFileNo);

            /* open a file for writing */
            fpLookupTableFile = fopen(szHtmlOuputFileName, "w+");

            if (fpLookupTableFile)
            {
                /* Write to html file */
                fprintf(fpLookupTableFile, "<td width=\"200\" align=\"center\">");
                fprintf(fpLookupTableFile, "%d", u8PortNo++);
                fprintf(fpLookupTableFile, "</td>\n");

                fprintf(fpLookupTableFile, "<td width=\"300\" align=\"center\">");
                fprintf(fpLookupTableFile, "%s", pDeviceInfo->entry[i].sHwVersionInfo.szModelName);
                fprintf(fpLookupTableFile, "</td>\n");

                fclose(fpLookupTableFile);
                u8HtmlFileNo++;
            }
        }
    }

    /*
     * Generate Ethernet Bridging information
     */

    /* Scan for available valid Ethernet devices */
    getifaddrs(&addrs);
    tmp = addrs;
    u8PortNo = 0;

    while (tmp)
    {
        if (strcmp (tmp->ifa_name, "lo"))
        {
            if (tmp->ifa_addr->sa_family==AF_INET)
            {
                /* Clear out possible stale errno to 0 */
                errno = 0;

                sock = socket(AF_INET, SOCK_STREAM, 0);
                if(sock < 0)
                {
                    freeifaddrs(addrs);

                    return;
                }

                memset(&ifr,0,sizeof(ifr));
                strcpy( ifr.ifr_name, tmp->ifa_name);

                if(ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
                {
                    freeifaddrs(addrs);

                    return;
                }

                if(!(ifr.ifr_flags & IFF_UP))
                {
                    /* if the ethernet port is not active, bring it up */
                    freeifaddrs(addrs);
                }

                /* Clear out possible stale errno to 0 */
                errno = 0;

                if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
                {
                    freeifaddrs(addrs);

                    return;
                }
                memcpy(&inet_sockaddr, &(ifr.ifr_addr), sizeof(inet_sockaddr));

                sprintf(szHtmlOuputFileName, HTML_ETH_DEVICE_NAME_BASE, u8PortNo);

                /* open a file for writing */
                fpLookupTableFile = fopen(szHtmlOuputFileName, "w+");

                if (fpLookupTableFile)
                {
                    /* Write to html file */
                    fprintf(fpLookupTableFile, "<td width=\"100\" align=\"center\">");
                    fprintf(fpLookupTableFile, "%d", u8PortNo++);
                    fprintf(fpLookupTableFile, "</td>\n");

                    fprintf(fpLookupTableFile, "<td width=\"200\" align=\"center\">");
                    fprintf(fpLookupTableFile, "%s", inet_ntoa(inet_sockaddr.sin_addr));
                    fprintf(fpLookupTableFile, "</td>\n");

                    fprintf(fpLookupTableFile, "<td width=\"200\" align=\"center\">");
                    fprintf(fpLookupTableFile, "%s", tmp->ifa_name);
                    fprintf(fpLookupTableFile, "</td>\n");

                    fclose(fpLookupTableFile);
                }

                close(sock);
            } /* if (strcmp (tmp->ifa_name, "lo")) */
        } /* if (strcmp (tmp->ifa_name, "lo")) */

        tmp = tmp->ifa_next;
    }
#endif /* DDCCM_USE_FILE_SYSTEM */
}

/*******************************************************************************
 * Name:    ddccmGenerateDeviceHtmlTable
 *
 * Description:
 *      This function generates device information and writes it to
 *      an HTML file.  Each device has its own HTML file/
 *
 * In   pDeviceInfo     Device Information List
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmGenerateDeviceHtmlTable
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo
)
{
#ifdef DDCCM_USE_FILE_SYSTEM

    S32BIT nError;
    U16BIT i;

    char szHtmlOuputFileName[MAX_CFG_FILE_NAM_LEN];

    /* remove all files first */
    for (i=0; i < MAX_NUM_DEV; i++)
    {
        /* Device information */
        sprintf(szHtmlOuputFileName, HTML_DEVICE_NAME_BASE, i);
        remove(szHtmlOuputFileName);
    }

    nError = ParseLookupTable(&lookupTable);

    if (nError)
    {
        /* could not parse lookup table, so we can not identify the recommended firmware revision */
        bLookupTableAvailable = FALSE;
    }
    else
    {
        bLookupTableAvailable = TRUE;
    }

    /* Generate Device information file for each device in the list */
    for (i=0; i < pDeviceInfo->u16NumberOfDevices; i++)
    {
        FILE *fpLookupTableFile;

        sprintf(szHtmlOuputFileName, HTML_DEVICE_NAME_BASE, i);

        /* open a file for writing */
        fpLookupTableFile = fopen(szHtmlOuputFileName, "w+");
        //fpLookupTableFile = stdout;

        if (fpLookupTableFile)
        {
            fprintf(fpLookupTableFile, "<!-- DEVICE_FILE %s -->\n", pDeviceInfo->entry[i].szDriver);
            fprintf(fpLookupTableFile, "<!-- DEVICE_FLASHABLE %d -->\n", (pDeviceInfo->entry[i].u32SectorCount > 0) ? 1 : 0);
            fprintf(fpLookupTableFile, "\n");

            fprintf(fpLookupTableFile, "<style>\n");
            fprintf(fpLookupTableFile, "  table,th,td\n");
            fprintf(fpLookupTableFile, "  {\n");
            fprintf(fpLookupTableFile, "      border:1px solid #C0C0C0;\n");
            fprintf(fpLookupTableFile, "      border-collapse:collapse;\n");
            fprintf(fpLookupTableFile, "      padding:2px;\n");
            fprintf(fpLookupTableFile, "  }\n");
            fprintf(fpLookupTableFile, "</style>\n");
            fprintf(fpLookupTableFile, "\n");


            fprintf(fpLookupTableFile, "<table>\n");

            fprintf(fpLookupTableFile, "  <tr>\n");
            fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Device Name </td>\n");
            fprintf(fpLookupTableFile, "    <td> %s </td>\n", pDeviceInfo->entry[i].sHwVersionInfo.szModelName);
            fprintf(fpLookupTableFile, "  </tr>\n");

            fprintf(fpLookupTableFile, "  <tr>\n");
            fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Driver Version </td>\n");
            fprintf(fpLookupTableFile, "    <td> %s </td>\n", pDeviceInfo->entry[i].sHwVersionInfo.szDriverVersion);
            fprintf(fpLookupTableFile, "  </tr>\n");

#if 0
            fprintf(fpLookupTableFile, "  <tr>\n");
            fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Device File </td>\n");
            fprintf(fpLookupTableFile, "    <td> %s </td>\n", pDeviceInfo->entry[i].szDriver);
            fprintf(fpLookupTableFile, "  </tr>\n");
#endif

            /* only show this info if we are not in backup */
            if (pDeviceInfo->entry[i].bBackup == FALSE)
            {
                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount1553)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> 1553 Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount1553);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Rx)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> 429 RX Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Rx);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Tx)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> 429 TX Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Tx);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Prog)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Programmable 429 Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount429Prog);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount717Rx)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> 717 RX Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount717Rx);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount717Tx)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> 717 TX Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount717Tx);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount717Prog)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Programmable 717 Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCount717Prog);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountDIO)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Digital I/O </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountDIO);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountAIO)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Avionic I/O </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountAIO);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountRS232)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> RX-232 </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountRS232);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountRS485)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> RS-422/485 </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountRS485);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountUart)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Programmable Serial Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountUart);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                if (pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountCanBus)
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                    fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> CAN Channels </td>\n");
                    fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sEnhancedCardCapabilities.channelCountCanBus);
                    fprintf(fpLookupTableFile, "  </tr>\n");
                }

                fprintf(fpLookupTableFile, "  <tr>\n");
                fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Capabilities </td>\n");

                /* print device capabilities */
                fprintf(fpLookupTableFile, "    <td>\n");
                fprintf(fpLookupTableFile, "      <ul>");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_MF)
                    fprintf(fpLookupTableFile, "        <li>Multi-Function</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_REPLAY)
                    fprintf(fpLookupTableFile, "        <li>Replay</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_MTI)
                    fprintf(fpLookupTableFile, "        <li>MTI</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_MTIE_MEC)
                    fprintf(fpLookupTableFile, "        <li>MEC (Message Error Capture)</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_MTIR_AES)
                    fprintf(fpLookupTableFile, "        <li>AES (Advanced Error Sampling)</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_VAR_VOLT_OUT)
                    fprintf(fpLookupTableFile, "        <li>Variable Voltage Output</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_PROG_VAR_SPEED_429)
                    fprintf(fpLookupTableFile, "        <li>429 Variable Speed</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_VAR_VOLT_OUT_429)
                    fprintf(fpLookupTableFile, "        <li>429 Variable Voltage Output</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_COUPLING)
                    fprintf(fpLookupTableFile, "        <li>Coupling</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_TERMINATION)
                    fprintf(fpLookupTableFile, "        <li>Termination</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_EXTERNAL_CLOCK_IN)
                    fprintf(fpLookupTableFile, "        <li>External Clock In</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_EXTERNAL_CLOCK_OUT)
                    fprintf(fpLookupTableFile, "        <li>External Clock Out</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_IRIG_IN_ANALOG)
                    fprintf(fpLookupTableFile, "        <li>IRIG In (Analog)</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_IRIG_IN_DIGITAL)
                    fprintf(fpLookupTableFile, "        <li>IRIG In (Digital)</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_IRIG_OUT_DIGITAL)
                    fprintf(fpLookupTableFile, "        <li>IRIG Out (Digital)</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_IRIG_RANGE)
                    fprintf(fpLookupTableFile, "        <li>IRIG Range</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_TX_INHIBIT_BC_DISABLE)
                    fprintf(fpLookupTableFile, "        <li>TX Inhibit / BC Disable</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_CAN)
                    fprintf(fpLookupTableFile, "        <li>CAN</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_PPS)
                    fprintf(fpLookupTableFile, "        <li>Pulse-Per-Second</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_RT_AUTO_BOOT)
                    fprintf(fpLookupTableFile, "        <li>RT Auto Boot</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_PROG_ARINC_429)
                    fprintf(fpLookupTableFile, "        <li>Programmable ARINC 429</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_PROG_ARINC_717)
                     fprintf(fpLookupTableFile, "        <li>Programmable ARINC 717</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_PROG_SERIAL)
                     fprintf(fpLookupTableFile, "        <li>Programmable Serial</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_MSI)
                     fprintf(fpLookupTableFile, "        <li>MSI</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_TX_INHIBIT)
                     fprintf(fpLookupTableFile, "        <li>External TX Inhibit</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_BC_DISABLE)
                     fprintf(fpLookupTableFile, "        <li>External BC Inhibit</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_BC_EXT_TRIGGER)
                     fprintf(fpLookupTableFile, "        <li>BC External Trigger</li>\n");

                if (pDeviceInfo->entry[i].sHwVersionInfo.dwCapabilities & HWVER_CAPABILITY_MRT_DISABLE)
                     fprintf(fpLookupTableFile, "        <li>Multi-RT Disable</li>\n");

                fprintf(fpLookupTableFile, "      </ul>\n");
                fprintf(fpLookupTableFile, "    </td>\n");
                fprintf(fpLookupTableFile, "  </tr>\n");
            }

            if (pDeviceInfo->entry[i].u32SectorCount > 0)
            {
                fprintf(fpLookupTableFile, "  <tr>\n");
                fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Firmware ID</td>\n");
                fprintf(fpLookupTableFile, "    <td> %d </td>\n", pDeviceInfo->entry[i].sHwVersionInfo.dwHdlNumber);
                fprintf(fpLookupTableFile, "  </tr>\n");

                if (pDeviceInfo->entry[i].bBackup)
                {
                    /* yellow background row color */
                    fprintf(fpLookupTableFile, "  <tr style=\"background-color:#FFFF99;\">\n");
                }
                else
                {
                    fprintf(fpLookupTableFile, "  <tr>\n");
                }

                fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Firmware Version </td>\n");

                fprintf(fpLookupTableFile, "    <td> %d.%d ", pDeviceInfo->entry[i].sHwVersionInfo.dwHdlVersion, pDeviceInfo->entry[i].sHwVersionInfo.dwFwVersion);

                if (pDeviceInfo->entry[i].bBackup)
                {
                    fprintf(fpLookupTableFile, " (Backup)");
                }

                fprintf(fpLookupTableFile, " </td>\n");

                fprintf(fpLookupTableFile, "  </tr>\n");

                if (bLookupTableAvailable)
                {
                    U16BIT u16EntryIndex;

                    for (u16EntryIndex=0; u16EntryIndex < lookupTable.u16NumberOfLookupTableEntries; u16EntryIndex++)
                    {
                        /* do the data archive numbers match? */
                        if (pDeviceInfo->entry[i].sHwVersionInfo.dwHdlNumber == (U32BIT)lookupTable.entry[u16EntryIndex].nDataArchiveNumber)
                        {
                            /* Some older drivers may not support reading of the data archive number    */
                            /* and will return 00000. Since there are mutiple entries for 00000, we     */
                            /* need to match the device ID's as well. The non-zero values should also   */
                            /* match.                                                                   */
                            if (pDeviceInfo->entry[i].sDevInfo.dwDeviceID == (U32BIT)lookupTable.entry[u16EntryIndex].nDeviceID)
                            {
                                /* save the recommended firmware version */
                                pDeviceInfo->entry[i].u32RecFwVer = lookupTable.entry[u16EntryIndex].nRecFwVer;
                            }
                        }
                    }
                }

                if (pDeviceInfo->entry[i].u32RecFwVer > 0)
                {
                    if (( pDeviceInfo->entry[i].sHwVersionInfo.dwHdlVersion != (U32BIT)(pDeviceInfo->entry[i].u32RecFwVer / 100)) ||
                        ( pDeviceInfo->entry[i].sHwVersionInfo.dwFwVersion  != (U32BIT)(pDeviceInfo->entry[i].u32RecFwVer % 100)))
                    {
                        /* Firmware Mismatch */
                        fprintf(fpLookupTableFile, "  <tr style=\"background-color:#FFFF99;\">\n");
                    }
                    else
                    {
                        fprintf(fpLookupTableFile, "  <tr>\n");
                    }
                }

                fprintf(fpLookupTableFile, "    <td style=\"width:150px\"> Recommended Firmware Version </td>\n");
                fprintf(fpLookupTableFile, "    <td> %d.%d </td>\n", pDeviceInfo->entry[i].u32RecFwVer/100, pDeviceInfo->entry[i].u32RecFwVer%100);
                fprintf(fpLookupTableFile, "  </tr>\n");
            }

            fprintf(fpLookupTableFile, "</table>\n");
            fprintf(fpLookupTableFile, "<p>\n");

            fclose(fpLookupTableFile);
        }
    }
#endif /* DDCCM_USE_FILE_SYSTEM */
}

/*******************************************************************************
 * Name:    ddccmGenerateHtmlTables
 *
 * Description:
 *      This function generate HTML files for each device in the
 *      list.
 *
 * In   pDeviceInfo     Device Information List
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmGenerateHtmlTables
(
    DEVICE_INFORMATION_TYPE *pDeviceInfo
)
{
#ifdef DDCCM_USE_FILE_SYSTEM
    ddccmGenerateDeviceHtmlTable(pDeviceInfo);

    ddccmGenerateBridgingHtmlTable(pDeviceInfo);
#endif /* DDCCM_USE_FILE_SYSTEM */
}

#ifndef LINUX
/*******************************************************************************
 * Name:    ddccmPrintCmdLineHelp
 *
 * Description:
 *      This function prints out the command line help.
 *
 * In
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmPrintCmdLineHelp()
{

    printf("\n");
    printf("Command Line Usage:\n");
    printf("\n");
    printf("ddccm (<Family Init>, <Show Table>)\n");
    printf("\n");
    printf("    <Family Init>\n");
    printf("     0       Interactive card manager, <Show Table> ignored\n");
    printf("     1       Configure MIL-STD-1553 devices only, non-interactive\n");
    printf("     2       Configure ARINC 429 devices only, non-interactive\n");
    printf("    -1       Configure all family types, non-interactive\n");
    printf("\n");
    printf("    <Show Table> - this parameter is ignored if <Family Init> is 0\n");
    printf("    0        Do not print card manager table(s) to console\n");
    printf("    1        Print card manager table to console\n");
    printf("\n");
}
#endif /* not defined LINUX */

/*******************************************************************************
 * Name:    ddccmDisplayHelp
 *
 * Description:
 *      This function displays user help and list of cards currently supported
 *      for the current family.
 *
 * In   none
 * Out  none
 *
 * Returns: none
 ******************************************************************************/
static void ddccmDisplayHelp(void)
{
    printf("\n    h    display (h)elp screen");
    printf("\n    d    display all (d)evices");
    printf("\n    i    display (i)nformation about a device");

#ifdef LINUX
    printf("\n    s    (s)et logical device number of a device");
#endif /* LINUX */

    printf("\n    f    update the (f)irmware of a device");
    printf("\n    t    update the (t)x Inhibit / BC Disable Config of a device");
    printf("\n    r    manage (R)emote IP Addresses");
    printf("\n    u    save config file and re-scan and (u)pdate device list");

#ifdef LINUX
    printf("\n    x    abort (config file NOT saved)");
    printf("\n    q    (q)uit and save config file\n");
#endif /* LINUX */

#ifndef LINUX
    printf("\n    q    (q)uit\n");
#endif /* not defined LINUX */

#ifdef DDCCM_DIAG
    printf("\n    g    dia(g)nostic menu\n");
#endif /* DDCCM_DIAG */

    printf("\n");
}


#define DDCCM_INIT_FAMILY__ALL_COMBINED         -2
#define DDCCM_INIT_FAMILY__ALL_INDIVIDUAL       -1
#define DDCCM_INIT_FAMILY__DEFAULT              0
#define DDCCM_INIT_FAMILY__1553                 1
#define DDCCM_INIT_FAMILY__429                  2

#define DDCCM_TABLE_OUTPUT__CONSOLE_DONT_SHOW   0x00
#define DDCCM_TABLE_OUTPUT__CONSOLE_SHOW        0x01
#define DDCCM_TABLE_OUTPUT__HTML_OUTPUT         0x02 /* INTERNAL USE ONLY */

/*******************************************************************************
 * Name:    ddccm/main
 *
 * Description:
 *      This function is the main entry point for the DDC Card Manager application.
 *
 * In   Param 1 - Init Family
 *      Values:
 *         -2       All Configurations Combined and Individual (1553 & 429)
 *         -1       All Configurations Individual (1553 & 429)
 *          1       1553 Only
 *          2       429 Only
 *    default       Interactive
 *
 *      Param 2 - Table Output (Bit Values - can be combined)
 *      Values:
 *          0       Do not show table
 *          1       Show Table (always show in interactive mode)
 *          2       HTML output and create output files (combined mode only)
 *
 * Out  none
 *
 * Returns:
 *      < 0 - Error Code denoting error.
 *      0   - No Logical number devices assigned.
 *      > 0 - Total number of 1553 and 429 logical devices assigned.
 *
 ******************************************************************************/
#if (defined VX_WORKS) || (defined INTEGRITY)
int ddccm(int initFamily, unsigned char bTableOutput)
#else
int main(int argc, char *argv[])
#endif /* not defined LINUX */
{
    /* product family we are working with */
    U8BIT u8Family;
    BOOL bGenerateCombinedList = FALSE;
    char szStringIn[80] = "";
    S32BIT nError;

#ifdef LINUX
    int initFamily = DDCCM_INIT_FAMILY__DEFAULT;
    unsigned char bTableOutput = DDCCM_TABLE_OUTPUT__CONSOLE_SHOW;
    char bSaveCfgFile = TRUE;
    uid_t           UID;


    UID = getuid();

    if (UID != 0)
    {
        ddccmDisplayError(ERROR_NOT_ROOT_USER, UID, FALSE);
    }

    /* check for 2 command line arguments */
    if (argc == 3)
    {
        initFamily = atoi(argv[1]);
        bTableOutput = atoi(argv[2]);
    }
    else
    {
        /* interactive, we do not need to auto-enum now */
        bAutoEnumDevices = FALSE;
        bTableOutput = DDCCM_TABLE_OUTPUT__CONSOLE_SHOW;
    }

#endif /* LINUX */

    bDdccmExit = FALSE;
#ifndef LINUX
    bPrintCmdLineHelp = FALSE;
#endif /* not defined LINUX */
    bLookupTableAvailable = FALSE;
    bSkipLookupTable = FALSE;

    switch (initFamily)
    {
        case DDCCM_INIT_FAMILY__1553:
        {
            u8Family = FAMILY_MIL_STD_1553;
            bSkipLookupTable = TRUE;
            break;
        }

        case DDCCM_INIT_FAMILY__429:
        {
            u8Family = FAMILY_ARINC_429;
            bSkipLookupTable = TRUE;
            break;
        }

        case DDCCM_INIT_FAMILY__ALL_INDIVIDUAL:
        {
            /* init all */
            u8Family = MAX_NUM_FAMILY;
            bSkipLookupTable = TRUE;
            break;
        }

        case DDCCM_INIT_FAMILY__ALL_COMBINED:
        {
            /* init all */
            bGenerateCombinedList = TRUE;
            u8Family = MAX_NUM_FAMILY;
            bSkipLookupTable = TRUE;
            break;
        }

        case DDCCM_INIT_FAMILY__DEFAULT:
        default:
        {
            /* ask the user which product family they wish to configure */
            u8Family = ddccmGetFamily();
            bSkipLookupTable = FALSE;
            bTableOutput = DDCCM_TABLE_OUTPUT__CONSOLE_SHOW;
            break;
        }
    }

    if (bDdccmExit)
    {
#ifdef LINUX
            ddccmDisplayError(ERROR_EXITING, 0, TRUE);
#endif /* LINUX */
    }

#ifndef LINUX
    else if (bPrintCmdLineHelp)
    {
            ddccmPrintCmdLineHelp();
    }
#endif /* not defined LINUX */
    else
    {
        /*------------------------------------------------------*/
        /* parse the lookup table                               */
        /*------------------------------------------------------*/
        if (bSkipLookupTable == FALSE)
        {
            nError = ParseLookupTable(&lookupTable);

            if (nError)
            {
                /* could not parse lookup table, so we can not identify the recommended firmware revision */
                bLookupTableAvailable = FALSE;
            }
            else
            {
                bLookupTableAvailable = TRUE;
            }
        }

        ddc1553NumLogicalDevs = 0;
        ddc429NumLogicalDevs = 0;

        /* generate the device list */
        switch (u8Family)
        {
            case FAMILY_MIL_STD_1553:
            case FAMILY_ARINC_429:
            {
                ddccmFindAllDevices(u8Family);

                if (bTableOutput & DDCCM_TABLE_OUTPUT__CONSOLE_SHOW)
                {
                    /* show the device list first */
                    ddccmDisplayDevices(u8Family);
                }
#ifdef LINUX
                if (initFamily != 0)
                {
                    /* always  write the config file for auto-enum */
                    ddccmWriteConfigFile(u8Family);
                }
#endif /* LINUX */

                break;
            }

            default:
            {
                int i;

                /* init all types */
                for (i=0; i<MAX_NUM_FAMILY; i++)
                {
                    ddccmFindAllDevices(i);

                    if (bTableOutput & DDCCM_TABLE_OUTPUT__CONSOLE_SHOW)
                    {
                        /* show the device list first */
                        ddccmDisplayDevices(i);

                        /* only print spacing if we are not on the last item and there are devices */
                        if ((i<MAX_NUM_FAMILY) && (u8NumberOfDevices))
                        {
                            printf("\n\n");
                        }
                    }
#ifdef LINUX
                    if (initFamily != 0)
                    {
                        /* always  write the config file for auto-enum */
                        ddccmWriteConfigFile(i);
                    }
#endif /* LINUX */

                }

                if (bGenerateCombinedList == TRUE)
                {
                    DEVICE_INFORMATION_TYPE deviceInfo;

                    /*------------------------------------------------------*/
                    /* find all known devices                               */
                    /*------------------------------------------------------*/

                    nError = ddccmOsFindDevices(&deviceInfo, FALSE);

                    if (nError)
                    {
                        return 0;
                    }

                    /* were there any devices found? */
                    if (deviceInfo.u16NumberOfDevices == 0)
                    {
                        return 0;
                    }

                    /* generate HTML output now? */
                    if (bTableOutput & DDCCM_TABLE_OUTPUT__HTML_OUTPUT)
                    {
                        ddccmGenerateHtmlTables(&deviceInfo);
                    }
                }

                break;
            }
        }

        if (initFamily != 0)
        {
            /* auto init selected... we are done */
            return (ddc1553NumLogicalDevs+ddc429NumLogicalDevs);
        }


        /* enter the command loop */
        for (;;)
        {
            printf("\nEnter command ('h' for help): ");
            scanf("%s", szStringIn);

            if (!strcmp(szStringIn, "h"))
            {
                ddccmDisplayHelp();
            }
            else if (!strcmp(szStringIn, "d"))
            {
                ddccmDisplayDevices(u8Family);
            }
            else if (!strcmp(szStringIn, "f"))
            {
                ddccmFlashDevice(DDCCM_FLASH_TYPE__FIRMWARE);
            }
            else if (!strcmp(szStringIn, "t"))
            {
                ddccmFlashDevice(DDCCM_FLASH_TYPE__TX_INHIBIT_BC_DISABLE);
            }
            else if (!strcmp(szStringIn, "i"))
            {
                ddccmDisplayDeviceInformation(u8Family);
            }
#ifdef LINUX
            else if (!strcmp(szStringIn, "s"))
            {
                ddccmSetLogicalDeviceNumber(u8Family);
            }
            else if (!strcmp(szStringIn, "x"))
            {
                bSaveCfgFile = FALSE;
                break;
            }
#endif /* LINUX */
            else if (!strcmp(szStringIn, "r"))
            {
                ddccmManageRemoteIpAddresses();

                /* redisplay list */
                ddccmDisplayDevices(u8Family);
            }
            else if (!strcmp(szStringIn, "u"))
            {
#ifdef LINUX
                /* write the current family config file */
                ddccmWriteConfigFile(u8Family);
#endif /* LINUX */

                /* generate the device list */
                ddccmFindAllDevices(u8Family);

                /* show the device list first */
                ddccmDisplayDevices(u8Family);
            }
            else if (!strcmp(szStringIn, "q"))
            {
                break;
            }
#ifdef DDCCM_DIAG
            else if (!strcmp(szStringIn, "g"))
            {
                ddccmDiagMenu(u8Family);

#ifdef LINUX
                /* do not write the config file */
                bSaveCfgFile = FALSE;
#endif /* LINUX */

                break;
            }
#endif /* DDCCM_DIAG */
            else
            {
                printf("\n    Invalid command!\n");
            }
        }

#ifdef LINUX
        /* only write the config file if the user exited via 'q' */
        if (bSaveCfgFile)
        {
            /* write the current family config file */
            ddccmWriteConfigFile(u8Family);
        }
#endif /* LINUX */
    }

#ifndef LINUX
    return (ddc1553NumLogicalDevs + ddc429NumLogicalDevs);
#else
    return 0;
#endif /* not defined LINUX */
}
