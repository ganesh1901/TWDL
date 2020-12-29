/*******************************************************************************
 * FILE: ddcsockmsg.h
 *
 * DESCRIPTION:
 *
 *      DDC socket interface for remote devices.
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

#ifndef _DDCSOCKMSG_H_
#define _DDCSOCKMSG_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */


#define DEFAULT_BUFLEN          0x20020 /* default bytes for send and receive buffers */
#define DEFAULT_TIMEOUT_COUNT   6       /* 1 minute timeout when combined with DEFAULT_TIMEOUT_SEC*/
#define DEFAULT_TIMEOUT_SEC     10
#define DEFAULT_TCP_PORT        "27015"
#define DEFAULT_TCP_PORT_NUM    27015
#define DEFAULT_UDP_PORT        "27016"
#define DEFAULT_UDP_PORT_NUM    27016

#define DDC_SOCK_SUCCESS        0
#define DDC_SOCK_CLOSED         -1001
#define DDC_SOCK_TIMEOUT        -1002
#define DDC_SOCK_ERROR          -1003
#define DDC_SOCK_MSG_ALLOC_FAIL -1004
#define DDC_SOCK_MSG_ERROR      -1005
#define DDC_SOCK_MSG_NO_SPACE   -1006
#define DDC_SOCK_MSG_SEND_FAIL  -1007
#define DDC_SOCK_MSG_RECV_FAIL  -1008
#define DDC_SOCK_FILE_OPEN_FAIL -1009
#define DDC_SOCK_INVALID        -1010


#define DDC_SOCK_DBG            0

#define DDC_SOCK_DBG_FUNC       0x0001
#define DDC_SOCK_DBG_INFO       0x0002
#define DDC_SOCK_DBG_ERROR      0x0004

#define WINDOWS_OS_ID           0
#define LINUX_OS_ID             1
#define VXWORKS_OS_ID           2
#define INTEGRITY_OS_ID         5

#define MAX_429_DEVICES         32
#define MAX_1553_DEVICES        32
#define MAX_SOCK_CONNECT        50

#define SOCK_CLOSE_TIMEOUT_COUNT    250
#define SOCK_CLOSE_WAIT_MS          50
extern U32BIT u32RemoteOsType429[MAX_429_DEVICES];
extern U32BIT u32RemoteOsType1553[MAX_1553_DEVICES];

extern U16BIT gDdcSockDbgPriority;

#if DDC_SOCK_DBG
#define DDC_SOCK_DBG_PRINT(x, ...) \
    if(x & gDdcSockDbgPriority) \
    {\
        printf(__VA_ARGS__);\
    }
#else
#define DDC_SOCK_DBG_PRINT(x, ...)
#endif


typedef enum _DDC_SOCK_MSG_TYPE
{
    DDC_SOCK_MSG_REQ = 0,
    DDC_SOCK_MSG_RESPONSE,
    DDC_SOCK_MSG_IRQ,
    DDC_SOCK_MSG_INVALID

} DDC_SOCK_MSG_TYPE;

/*
 * The following values are the command links between Server and Client
 *
 *  They must not change!!!!!
 *
 * Next available number is 497.
 *
 */
typedef enum _DDC_SOCK_MSG_CMD
{
    /****************************************
              Card Manager Socket CMD
    *****************************************/
    DDC_SOCK_MSG_CMD_ENUM_DEV                                   = 0,
    /****************************************
              1553 API Socket Commands
    *****************************************/

    /* config.h */
    DDC_SOCK_MSG_CMD_INITIALIZE                                 = 1,
    DDC_SOCK_MSG_CMD_FREE                                       = 2,
    DDC_SOCK_MSG_CMD_SET_ADDR_MODE                              = 3,    /* not used */
    DDC_SOCK_MSG_CMD_SET_CLK_FREQ                               = 4,
    DDC_SOCK_MSG_CMD_SET_TIME_TAG_VALUE                         = 5,
    DDC_SOCK_MSG_CMD_SET_TIME_TAG_VALUE_EX                      = 6,
    DDC_SOCK_MSG_CMD_GET_TIME_TAG_VALUE                         = 7,
    DDC_SOCK_MSG_CMD_GET_TIME_TAG_VALUE_EX                      = 8,
    DDC_SOCK_MSG_CMD_RESET_TIME_TAG                             = 9,
    DDC_SOCK_MSG_CMD_SET_RESP_TIMEOUT                           = 10,
    DDC_SOCK_MSG_CMD_SET_IRQ_CONFIG                             = 11,
    DDC_SOCK_MSG_CMD_SET_TIME_TAG_RES                           = 12,
    DDC_SOCK_MSG_CMD_SET_IRQ_CONDITIONS                         = 13,
    DDC_SOCK_MSG_CMD_SET_DECODER_CONFIG                         = 14,   /* not used */
    DDC_SOCK_MSG_CMD_SET_RAM_PARITY_CHECKING                    = 15,
    DDC_SOCK_MSG_CMD_REG_READ                                   = 16,
    DDC_SOCK_MSG_CMD_REG_WRITE                                  = 17,
    DDC_SOCK_MSG_CMD_MEM_READ                                   = 18,
    DDC_SOCK_MSG_CMD_MEM_WRITE                                  = 19,
    DDC_SOCK_MSG_CMD_GET_MEM_REG_INFO                           = 20,   /* not used*/
    DDC_SOCK_MSG_CMD_ISQ_CLEAR                                  = 21,
    DDC_SOCK_MSG_CMD_ISQ_READ                                   = 22,
    DDC_SOCK_MSG_CMD_ISQ_ENABLE                                 = 23,
    DDC_SOCK_MSG_CMD_SET_METRICS                                = 24,
    DDC_SOCK_MSG_CMD_GET_LIB_VER                                = 25,   /* not used*/
    DDC_SOCK_MSG_CMD_GET_CORE_VER                               = 26,   /* not used*/
    DDC_SOCK_MSG_CMD_GET_SW_VER_INFO                            = 27,   /* not used*/
    DDC_SOCK_MSG_CMD_GET_HW_VER_INFO                            = 28,
    DDC_SOCK_MSG_CMD_INT_80_ENABLE                              = 29,   /* not used*/
    DDC_SOCK_MSG_CMD_SET_ASYNC_ISR                              = 30,   /* not used*/
    DDC_SOCK_MSG_CMD_GET_CARD_TYPE                              = 31,
    DDC_SOCK_MSG_CMD_REG_READ32                                 = 32,
    DDC_SOCK_MSG_CMD_REG_WRITE32                                = 33,
    DDC_SOCK_MSG_CMD_MEM_READ32                                 = 34,
    DDC_SOCK_MSG_CMD_MEM_WRITE32                                = 35,
    DDC_SOCK_MSG_CMD_IO_INITIALIZE                              = 36,
    DDC_SOCK_MSG_CMD_IO_FREE                                    = 37,
    DDC_SOCK_MSG_CMD_GET_CHANNEL_CNT                            = 38,
    DDC_SOCK_MSG_CMD_GET_IRIG_TX                                = 39,
    DDC_SOCK_MSG_CMD_SET_IRIG_TX                                = 40,
    DDC_SOCK_MSG_CMD_GET_COUPLING                               = 41,
    DDC_SOCK_MSG_CMD_SET_COUPLING                               = 42,
    DDC_SOCK_MSG_CMD_GET_AMPLITUDE                              = 43,
    DDC_SOCK_MSG_CMD_SET_AMPLITUDE                              = 44,
    DDC_SOCK_MSG_CMD_COMPONENT_REG_READ                         = 45,
    DDC_SOCK_MSG_CMD_EI_TX_SHUTDOWN_ENABLE                      = 46,
    DDC_SOCK_MSG_CMD_EI_TX_SHUTDOWN_DISABLE                     = 47,
    DDC_SOCK_MSG_CMD_SET_HUB_ADDRESS                            = 48,   /* not used */
    DDC_SOCK_MSG_CMD_ACEX_REG_READ                              = 49,
    DDC_SOCK_MSG_CMD_ACEX_REG_WRITE                             = 50,
    /* config.h.  Debug Access  */
    DDC_SOCK_MSG_CMD_DEBUG_GET_DEVICE                           = 486,
    DDC_SOCK_MSG_CMD_IOCTL_REG_READ_BLK                         = 487,
    DDC_SOCK_MSG_CMD_IOCTL_REG_WRITE_BLK                        = 488,
    DDC_SOCK_MSG_CMD_IOCTL_MEM_READ_BLK                         = 489,
    DDC_SOCK_MSG_CMD_IOCTL_MEM_WRITE_BLK                        = 480,
    DDC_SOCK_MSG_CMD_IOCTL_FLASH_MEM_INFO                       = 491,
    DDC_SOCK_MSG_CMD_IOCTL_FLASH_MEM_READ                       = 492,
    DDC_SOCK_MSG_CMD_DEBUG_GET_IO_COUNT                         = 493,

    /* bc.h */
    DDC_SOCK_MSG_CMD_BC_CONFIGURE                               = 51,
    DDC_SOCK_MSG_CMD_BC_CONFIGURE_REPLAY                        = 52,
    DDC_SOCK_MSG_CMD_BC_DATA_BLK_CREATE                         = 53,
    DDC_SOCK_MSG_CMD_BC_DATA_BLK_DELETE                         = 54,
    DDC_SOCK_MSG_CMD_BC_DATA_BLK_WRITE                          = 55,
    DDC_SOCK_MSG_CMD_BC_DATA_BLK_READ                           = 56,
    DDC_SOCK_MSG_CMD_BC_DATA_BLK_READ32                         = 57,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE_BC2RT                        = 58,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE_RT2BC                        = 59,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE_RT2RT                        = 60,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE_MODE                         = 61,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE_BCST                         = 62,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE_BCST_RT2RT                   = 63,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE_BCST_MODE                    = 64,
    DDC_SOCK_MSG_CMD_BC_MSG_CREATE                              = 65,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY_BC2RT                        = 66,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY_RT2BC                        = 67,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY_RT2RT                        = 68,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY_MODE                         = 69,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY_BCST                         = 70,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY_BCST_RT2RT                   = 71,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY_BCST_MODE                    = 72,
    DDC_SOCK_MSG_CMD_BC_MSG_MODIFY                              = 73,
    DDC_SOCK_MSG_CMD_BC_MSG_DELETE                              = 74,
    DDC_SOCK_MSG_CMD_BC_OPCODE_CREATE                           = 75,
    DDC_SOCK_MSG_CMD_BC_OPCODE_DELETE                           = 76,
    DDC_SOCK_MSG_CMD_BC_FRAME_CREATE                            = 77,
    DDC_SOCK_MSG_CMD_BC_EX_FRAME_CREATE                         = 78,
    DDC_SOCK_MSG_CMD_BC_FRAME_DELETE                            = 79,
    DDC_SOCK_MSG_CMD_BC_START                                   = 80,
    DDC_SOCK_MSG_CMD_BC_START_EX                                = 81,   /* not used */
    DDC_SOCK_MSG_CMD_BC_START_REPLAY                            = 82,
    DDC_SOCK_MSG_CMD_BC_START_REPLAY_RAW                        = 83,
    DDC_SOCK_MSG_CMD_BC_STOP                                    = 84,
    DDC_SOCK_MSG_CMD_BC_MSG_GAP_TIMER_ENABLE                    = 85,
    DDC_SOCK_MSG_CMD_BC_SET_GPF_STATE                           = 86,
    DDC_SOCK_MSG_CMD_BC_GET_CONDITION_CODE                      = 87,
    DDC_SOCK_MSG_CMD_BC_SET_WATCH_DOG_TIMER                     = 88,
    DDC_SOCK_MSG_CMD_BC_GET_MSG_FROM_ID_RAW                     = 89,
    DDC_SOCK_MSG_CMD_BC_GET_MSG_FROM_ID_RAW_EX                  = 90,
    DDC_SOCK_MSG_CMD_BC_GET_MSG_HDR_FROM_ID_RAW                 = 91,
    DDC_SOCK_MSG_CMD_BC_GET_MSG_HDR_FROM_ID_RAW_EX              = 92,
    DDC_SOCK_MSG_CMD_BC_GET_MSG_FROM_ID_DECODED                 = 93,
    DDC_SOCK_MSG_CMD_BC_GET_MSG_FROM_ID_DECODED_EX              = 94,
    DDC_SOCK_MSG_CMD_BC_DECODE_RAW_MSG                          = 95,   /* not used */
    DDC_SOCK_MSG_CMD_BC_INSTALL_HBUF                            = 96,
    DDC_SOCK_MSG_CMD_BC_UNINSTALL_HBUF                          = 97,
    DDC_SOCK_MSG_CMD_BC_GET_HBUF_MSG_CNT                        = 98,
    DDC_SOCK_MSG_CMD_BC_GET_HBUF_MSGS_RAW                       = 99,
    DDC_SOCK_MSG_CMD_BC_GET_HBUF_MSGS_RAW_EX                    = 100,
    DDC_SOCK_MSG_CMD_BC_GET_HBUF_MSG_DECODED                    = 101,
    DDC_SOCK_MSG_CMD_BC_GET_HBUF_MSG_DECODED_EX                 = 102,
    DDC_SOCK_MSG_CMD_BC_FRM_TO_HBUF                             = 103,
    DDC_SOCK_MSG_CMD_BC_FRM_TO_HBUF_32                          = 104,
    DDC_SOCK_MSG_CMD_BC_GPQ_GET_CNT                             = 105,
    DDC_SOCK_MSG_CMD_BC_GPQ_READ                                = 106,
    DDC_SOCK_MSG_CMD_BC_SET_MSG_RETRY                           = 107,
    DDC_SOCK_MSG_CMD_BC_CREATE_IMAGE_FILES                      = 108,
    DDC_SOCK_MSG_CMD_BC_GET_HBUF_METRIC                         = 109,
    DDC_SOCK_MSG_CMD_BC_GET_GPQ_METRIC                          = 110,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_CREATE_BC2RT                  = 111,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_CREATE_RT2BC                  = 112,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_CREATE_RT2RT                  = 113,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_CREATE_MODE                   = 114,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_CREATE_BCST                   = 115,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_CREATE_BCST_RT2RT             = 116,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_CREATE_BCST_MODE              = 117,
    DDC_SOCK_MSG_CMD_BC_SEND_ASYNC_MSG_LP                       = 118,
    DDC_SOCK_MSG_CMD_BC_SEND_ASYNC_MSG_HP                       = 119,
    DDC_SOCK_MSG_CMD_BC_EMPTY_ASYNC_LIST                        = 120,
    DDC_SOCK_MSG_CMD_BC_RESET_ASYNC_PTR                         = 121,
    DDC_SOCK_MSG_CMD_BC_GET_ASYNC_CNT                           = 122,
    DDC_SOCK_MSG_CMD_BC_DISABLE_MSG                             = 123,
    DDC_SOCK_MSG_CMD_BC_INITIALIZE                              = 124,
    DDC_SOCK_MSG_CMD_BC_FREE                                    = 125,
    DDC_SOCK_MSG_CMD_BC_TIME_SLOT_CREATE                        = 126,
    DDC_SOCK_MSG_CMD_BC_TIME_SLOT_ITEM_CREATE                   = 127,
    DDC_SOCK_MSG_CMD_BC_GET_CURR_TIME_SLOT                      = 128,
    DDC_SOCK_MSG_CMD_BC_DATA_STREAM_CREATE_BCRT                 = 129,
    DDC_SOCK_MSG_CMD_BC_DATA_STREAM_CREATE_RTRT                 = 130,
    DDC_SOCK_MSG_CMD_BC_DATA_STREAM_DELETE                      = 131,
    DDC_SOCK_MSG_CMD_BC_DATA_STREAM_SEND                        = 132,
    DDC_SOCK_MSG_CMD_BC_DATA_STREAM_RECEIVE                     = 133,
    DDC_SOCK_MSG_CMD_BC_DATA_ARRAY_CREATE_BC2RT                 = 134,
    DDC_SOCK_MSG_CMD_BC_DATA_ARRAY_DELETE                       = 135,
    DDC_SOCK_MSG_CMD_BC_DATA_ARRAY_SEND                         = 136,
    DDC_SOCK_MSG_CMD_BC_GET_STATUS                              = 137,
    DDC_SOCK_MSG_CMD_BC_GET_STATUS_REPLAY                       = 138,
    DDC_SOCK_MSG_CMD_BC_EXT_TRIGGER_ENABLE                      = 139,
    DDC_SOCK_MSG_CMD_BC_EXT_TRIGGER_DISABLE                     = 140,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_SEND_LP                       = 141,
    DDC_SOCK_MSG_CMD_BC_ASYNC_MSG_SEND_HP                       = 142,
    DDC_SOCK_MSG_CMD_BC_ASYNC_QUEUE_INFO_LP                     = 143,
    DDC_SOCK_MSG_CMD_BC_ASYNC_QUEUE_INFO_HP                     = 144,
    DDC_SOCK_MSG_CMD_BC_MEM_WRD_CREATE                          = 145,
    DDC_SOCK_MSG_CMD_BC_MEM_WRD_DELETE                          = 146,
    DDC_SOCK_MSG_CMD_BC_MEM_WRD_READ                            = 147,
    DDC_SOCK_MSG_CMD_BC_MEM_WRD_WRITE                           = 148,
    DDC_SOCK_MSG_CMD_BC_MEM_OBJ_CREATE                          = 149,
    DDC_SOCK_MSG_CMD_BC_MEM_OBJ_DELETE                          = 150,
    DDC_SOCK_MSG_CMD_BC_SET_MSG_ERROR                           = 151,
    DDC_SOCK_MSG_CMD_BC_MSG_ERROR_ENABLE                        = 152,
    DDC_SOCK_MSG_CMD_BC_MSG_ERROR_DISABLE                       = 153,
    DDC_SOCK_MSG_CMD_BC_SET_RESP_TIMEOUT                        = 154,
    DDC_SOCK_MSG_CMD_BC_OP_CODE_READ                            = 155,
    DDC_SOCK_MSG_CMD_BC_OP_CODE_WRITE                           = 156,
    DDC_SOCK_MSG_CMD_BC_DBC_ENABLE                              = 157,
    DDC_SOCK_MSG_CMD_BC_DBC_DISABLE                             = 158,
    DDC_SOCK_MSG_CMD_BC_IMR_TIRG_SELECT                         = 159,
    DDC_SOCK_MSG_CMD_BC_PAUSE                                   = 160,
    DDC_SOCK_MSG_CMD_BC_CONTINUE                                = 161,

    /* rt.h */
    DDC_SOCK_MSG_CMD_RT_CONFIG                                  = 162,
    DDC_SOCK_MSG_CMD_RT_SET_ADDRESS                             = 163,
    DDC_SOCK_MSG_CMD_RT_GET_ADDRESS                             = 164,
    DDC_SOCK_MSG_CMD_RT_DATA_BLK_CREATE                         = 165,
    DDC_SOCK_MSG_CMD_RT_DATA_BLK_DELETE                         = 166,
    DDC_SOCK_MSG_CMD_RT_DATA_BLK_WRITE                          = 167,
    DDC_SOCK_MSG_CMD_RT_DATA_BLK_READ                           = 168,
    DDC_SOCK_MSG_CMD_RT_DATA_BLK_CIRC_BUF_INFO                  = 169,
    DDC_SOCK_MSG_CMD_RT_MODE_CODE_IRQ_ENABLE                    = 170,
    DDC_SOCK_MSG_CMD_RT_MODE_CODE_IRQ_DISABLE                   = 171,
    DDC_SOCK_MSG_CMD_RT_MODE_CODE_IRQ_STATUS                    = 172,
    DDC_SOCK_MSG_CMD_RT_MODE_CODE_READ_DATA                     = 173,
    DDC_SOCK_MSG_CMD_RT_MODE_CODE_WRITE_DATA                    = 174,
    DDC_SOCK_MSG_CMD_RT_MSG_LEGALITY_ENABLE                     = 175,
    DDC_SOCK_MSG_CMD_RT_MSG_LEGALITY_DISABLE                    = 176,
    DDC_SOCK_MSG_CMD_RT_MSG_LEGALITY_STATUS                     = 177,
    DDC_SOCK_MSG_CMD_RT_BUSY_BITS_TBL_SET                       = 178,
    DDC_SOCK_MSG_CMD_RT_BUSY_BITS_TBL_CLR                       = 179,
    DDC_SOCK_MSG_CMD_RT_BUSY_BITS_TBL_STATUS                    = 180,
    DDC_SOCK_MSG_CMD_RT_STATUS_BITS_SET                         = 181,
    DDC_SOCK_MSG_CMD_RT_STATUS_BITS_CLR                         = 182,
    DDC_SOCK_MSG_CMD_RT_STATUS_BITS_STATUS                      = 183,
    DDC_SOCK_MSG_CMD_RT_DATA_BLK_MAP_TO_SA                      = 184,
    DDC_SOCK_MSG_CMD_RT_DATA_BLK_UNMAP_FROM_SA                  = 185,
    DDC_SOCK_MSG_CMD_RT_START                                   = 186,
    DDC_SOCK_MSG_CMD_RT_STOP                                    = 187,
    DDC_SOCK_MSG_CMD_RT_BIT_WRD_READ                            = 188,
    DDC_SOCK_MSG_CMD_RT_BIT_WRD_WRITE                           = 189,
    DDC_SOCK_MSG_CMD_RT_GET_STK_MSGS_RAW                        = 190,
    DDC_SOCK_MSG_CMD_RT_GET_STK_MSG_DECODED                     = 191,
    DDC_SOCK_MSG_CMD_RT_INSTALL_HBUF                            = 192,
    DDC_SOCK_MSG_CMD_RT_UNINSTALL_HBUF                          = 193,
    DDC_SOCK_MSG_CMD_RT_STK_TO_HBUF                             = 194,  /* not used */
    DDC_SOCK_MSG_CMD_RT_STK_TO_HBUF32                           = 195,
    DDC_SOCK_MSG_CMD_RT_GET_HBUF_MSGS_RAW                       = 196,
    DDC_SOCK_MSG_CMD_RT_GET_HBUF_MSG_CNT                        = 197,
    DDC_SOCK_MSG_CMD_RT_GET_HBUF_MSG_DECODED                    = 198,
    DDC_SOCK_MSG_CMD_RT_CREATE_IMAGE_FILES                      = 199,  /* not used*/
    DDC_SOCK_MSG_CMD_RT_GET_HBUF_METRIC                         = 200,
    DDC_SOCK_MSG_CMD_RT_GET_STK_METRIC                          = 201,
    DDC_SOCK_MSG_CMD_RT_SET_ADDR_SOURCE                         = 202,
    DDC_SOCK_MSG_CMD_RT_GET_ADDR_SOURCE                         = 203,
    DDC_SOCK_MSG_CMD_RT_RELATCH_ADDR                            = 204,
    DDC_SOCK_MSG_CMD_RT_ACQUIRE_ADDR                            = 205,  /* not used */
    DDC_SOCK_MSG_CMD_RT_DECODE_RAW_MSG                          = 206,
    DDC_SOCK_MSG_CMD_RT_BIT_WRD_CONFIG                          = 207,

    /* mrt.h */
    DDC_SOCK_MSG_CMD_MRT_CONFIGURE                              = 208,
    DDC_SOCK_MSG_CMD_MRT_START                                  = 209,
    DDC_SOCK_MSG_CMD_MRT_STOP                                   = 210,
    DDC_SOCK_MSG_CMD_MRT_ENABLE_RT                              = 211,
    DDC_SOCK_MSG_CMD_MRT_DISABLE_RT                             = 212,
    DDC_SOCK_MSG_CMD_MRT_DATA_BLK_MAP_TO_RT_SA                  = 213,
    DDC_SOCK_MSG_CMD_MRT_DATA_BLK_UNMAP_FROM_RT_SA              = 214,
    DDC_SOCK_MSG_CMD_MRT_CONFIG_RT_BIT_WRD                      = 215,
    DDC_SOCK_MSG_CMD_MRT_READ_RT_BIT_WRD                        = 216,
    DDC_SOCK_MSG_CMD_MRT_WRITE_RT_BIT_WRD                       = 217,
    DDC_SOCK_MSG_CMD_MRT_SET_RT_BUSY_BITS_TBL                   = 218,
    DDC_SOCK_MSG_CMD_MRT_CLR_RT_BUSY_BITS_TBL                   = 219,
    DDC_SOCK_MSG_CMD_MRT_GET_RT_BUSY_BITS_TBL_STATUS            = 220,
    DDC_SOCK_MSG_CMD_MRT_GET_RT_STATUS_BITS                     = 221,
    DDC_SOCK_MSG_CMD_MRT_SET_RT_STATUS_BITS                     = 222,
    DDC_SOCK_MSG_CMD_MRT_CLR_RT_STATUS_BITS                     = 223,
    DDC_SOCK_MSG_CMD_MRT_GET_RT_MSG_LEGALITY_STATUS             = 224,
    DDC_SOCK_MSG_CMD_MRT_ENABLE_RT_MSG_LEGALITY                 = 225,
    DDC_SOCK_MSG_CMD_MRT_DISABLE_RT_MSG_LEGALITY                = 226,
    DDC_SOCK_MSG_CMD_MRT_ENABLE_RT_MODE_CODE_IRQ                = 227,
    DDC_SOCK_MSG_CMD_MRT_DISABLE_RT_MODE_CODE_IRQ               = 228,
    DDC_SOCK_MSG_CMD_MRT_GET_RT_MODE_CODE_IRQ_STATUS            = 229,
    DDC_SOCK_MSG_CMD_MRT_WRITE_RT_MODE_CODE_DATA                = 230,
    DDC_SOCK_MSG_CMD_MRT_READ_RT_MODE_CODE_DATA                 = 231,
    DDC_SOCK_MSG_CMD_MRT_DATA_STREAM_CREATE                     = 232,
    DDC_SOCK_MSG_CMD_MRT_DATA_STREAM_DELETE                     = 233,
    DDC_SOCK_MSG_CMD_MRT_DATA_STREAM_SEND                       = 234,
    DDC_SOCK_MSG_CMD_MRT_DATA_STREAM_RECEIVE                    = 235,
    DDC_SOCK_MSG_CMD_MRT_DATA_ARRAY_CREATE                      = 236,
    DDC_SOCK_MSG_CMD_MRT_DATA_ARRAY_DELETE                      = 237,
    DDC_SOCK_MSG_CMD_MRT_DATA_ARRAY_SEND                        = 238,
    DDC_SOCK_MSG_CMD_MRT_SET_MSG_ERROR                          = 239,
    DDC_SOCK_MSG_CMD_MRT_MSG_ERROR_ENABLE                       = 240,
    DDC_SOCK_MSG_CMD_MRT_MSG_ERROR_DISABLE                      = 241,
    DDC_SOCK_MSG_CMD_MRT_SET_RESP_TIME                          = 242,
    DDC_SOCK_MSG_CMD_MRT_RESP_TIME_ENABLE                       = 243,
    DDC_SOCK_MSG_CMD_MRT_RESP_TIME_DISABLE                      = 244,
    DDC_SOCK_MSG_CMD_MRT_SET_RESP_TIME_OUT                      = 245,
    DDC_SOCK_MSG_CMD_MRT_DBC_ENABLE                             = 246,
    DDC_SOCK_MSG_CMD_MRT_DBC_DISABLE                            = 247,
    DDC_SOCK_MSG_CMD_MRT_IMR_MAP_TO_RTSA                        = 248,
    DDC_SOCK_MSG_CMD_MRT_IMR_TRIG_SELECT                        = 249,

    /* mt.h */
    DDC_SOCK_MSG_CMD_MT_CONFIGURE                               = 250,
    DDC_SOCK_MSG_CMD_MT_INSTALL_HBUF                            = 251,
    DDC_SOCK_MSG_CMD_MT_UNINSTALL_HBUF                          = 252,
    DDC_SOCK_MSG_CMD_MT_SET_HBUF_TRIGGER                        = 253,
    DDC_SOCK_MSG_CMD_MT_CLR_HBUF_TRIGGER                        = 254,
    DDC_SOCK_MSG_CMD_MT_SWAP_STKS                               = 255,
    DDC_SOCK_MSG_CMD_MT_GET_HBUF_MSG_CNT                        = 256,
    DDC_SOCK_MSG_CMD_MT_GET_HBUF_MSG_RAW                        = 257,
    DDC_SOCK_MSG_CMD_MT_GET_HBUF_MSG_DECODED                    = 258,
    DDC_SOCK_MSG_CMD_MT_STK_TO_HBUF                             = 259,
    DDC_SOCK_MSG_CMD_MT_STK_TO_HBUF32                           = 260,
    DDC_SOCK_MSG_CMD_MT_GET_INFO                                = 261,
    DDC_SOCK_MSG_CMD_MT_START                                   = 262,
    DDC_SOCK_MSG_CMD_MT_STOP                                    = 263,
    DDC_SOCK_MSG_CMD_MT_PAUSE                                   = 264,
    DDC_SOCK_MSG_CMD_MT_CONTINUE                                = 265,
    DDC_SOCK_MSG_CMD_MT_ENABLE_RT_FILTER                        = 266,
    DDC_SOCK_MSG_CMD_MT_DISABLE_RT_FILTER                       = 267,
    DDC_SOCK_MSG_CMD_MT_GET_RT_FILTER                           = 268,
    DDC_SOCK_MSG_CMD_MT_DECODE_RAW_MSG                          = 269,  /* not used*/
    DDC_SOCK_MSG_CMD_MT_GET_STK_MSGS_RAW                        = 270,
    DDC_SOCK_MSG_CMD_MT_GET_STK_MSG_DECODED                     = 271,
    DDC_SOCK_MSG_CMD_MT_CREATE_IMAGE_FILES                      = 272,  /* not used*/
    DDC_SOCK_MSG_CMD_MT_GET_HBUF_METRIC                         = 273,
    DDC_SOCK_MSG_CMD_MT_GET_STK_METRIC                          = 274,

    /* mti.h */
    DDC_SOCK_MSG_CMD_MTI_CONFIG                                 = 275,
    DDC_SOCK_MSG_CMD_MTI_SET_EXTERNAL_CLK                       = 276,
    DDC_SOCK_MSG_CMD_MTI_START                                  = 277,
    DDC_SOCK_MSG_CMD_MTI_STOP                                   = 278,
    DDC_SOCK_MSG_CMD_MTI_INITIATE_HOST_IRQ                      = 279,
    DDC_SOCK_MSG_CMD_MTI_GET_CH10_DATA_PKT                      = 280,
    DDC_SOCK_MSG_CMD_MTI_FREE_CH10_DATA_PKT                     = 281,  /* not used */
    DDC_SOCK_MSG_CMD_MTI_GET_CH10_TIME_PKT                      = 282,
    DDC_SOCK_MSG_CMD_MTI_FREE_CH10_TIME_PKT                     = 283,  /* not used */
    DDC_SOCK_MSG_CMD_MTI_PAUSE                                  = 284,
    DDC_SOCK_MSG_CMD_MTI_CONTINUE                               = 285,
    DDC_SOCK_MSG_CMD_MTI_CH10_TIME_FMT                          = 286,
    DDC_SOCK_MSG_CMD_MTI_CH10_TIME_PKT_ENABLE                   = 287,
    DDC_SOCK_MSG_CMD_MTI_GET_METRICS                            = 288,
    DDC_SOCK_MSG_CMD_MTI_DECODE_RAW_MSG                         = 289,  /* not used */
    DDC_SOCK_MSG_CMD_MTI_SET_CH10_TIME_RANGE                    = 290,
    DDC_SOCK_MSG_CMD_MTI_GET_CH10_TIME_RANGE                    = 291,
    DDC_SOCK_MSG_CMD_MTI_SET_CH10_TIME_PKT_ID                   = 292,
    DDC_SOCK_MSG_CMD_MTI_SET_RESP_TIMEOUT                       = 293,

    /* rmmt.h */
    DDC_SOCK_MSG_CMD_RTMT_CONFIGURE                             = 294,
    DDC_SOCK_MSG_CMD_RTMT_START                                 = 295,
    DDC_SOCK_MSG_CMD_RTMT_STOP                                  = 296,
    DDC_SOCK_MSG_CMD_RTMT_STK_TO_HBUF                           = 297,
    DDC_SOCK_MSG_CMD_RTMT_STK_TO_HBUF32                         = 298,
    DDC_SOCK_MSG_CMD_RTMT_INSTALL_HUBF                          = 299,
    DDC_SOCK_MSG_CMD_RTMT_UNINSTALL_HUBF                        = 300,
    DDC_SOCK_MSG_CMD_RTMT_GET_HBUF_MSG_CNT                      = 301,
    DDC_SOCK_MSG_CMD_RTMT_GET_HBUF_MSG_RAW                      = 302,
    DDC_SOCK_MSG_CMD_RTMT_GET_HBUF_MSG_DECODED                  = 303,
    DDC_SOCK_MSG_CMD_RTMT_GET_HBUF_METRIC                       = 304,

    /* rmmti.h */
    DDC_SOCK_MSG_CMD_RTMTI_CONFIGURE                            = 305,
    DDC_SOCK_MSG_CMD_RTMTI_START                                = 306,
    DDC_SOCK_MSG_CMD_RTMTI_STOP                                 = 307,

    /* aio.h */
    DDC_SOCK_MSG_CMD_GET_AIO_OUT                                = 308,
    DDC_SOCK_MSG_CMD_SET_AIO_DIR                                = 309,
    DDC_SOCK_MSG_CMD_GET_AIO_DIR                                = 310,
    DDC_SOCK_MSG_CMD_GET_AIO_IN                                 = 311,
    DDC_SOCK_MSG_CMD_SET_AIO_OUT                                = 312,
    DDC_SOCK_MSG_CMD_GET_AIO_ALL                                = 313,
    DDC_SOCK_MSG_CMD_SET_AIO_ALL                                = 314,

    /* dio.h */
    DDC_SOCK_MSG_CMD_GET_DISC_OUT                               = 315,
    DDC_SOCK_MSG_CMD_SET_DISC_DIR                               = 316,
    DDC_SOCK_MSG_CMD_GET_DISC_DIR                               = 317,
    DDC_SOCK_MSG_CMD_GET_DISC_IN                                = 318,
    DDC_SOCK_MSG_CMD_SET_DISC_OUT                               = 319,
    DDC_SOCK_MSG_CMD_GET_DISC_ALL                               = 320,
    DDC_SOCK_MSG_CMD_SET_DISC_ALL                               = 321,
    DDC_SOCK_MSG_CMD_SET_DISC_CONFIG                            = 322,
    DDC_SOCK_MSG_CMD_CLR_DISC_CONFIG                            = 323,

    /* test.h */
    DDC_SOCK_MSG_CMD_TEST_REGISTERS                             = 324,
    DDC_SOCK_MSG_CMD_TEST_MEMORY                                = 325,
    DDC_SOCK_MSG_CMD_TEST_IRQS                                  = 326,
    DDC_SOCK_MSG_CMD_TEST_LOOP_BACK                             = 327,
    DDC_SOCK_MSG_CMD_TEST_PROTOCOL                              = 328,
    DDC_SOCK_MSG_CMD_TEST_VECTORS                               = 329,  /* not used*/
    DDC_SOCK_MSG_CMD_TEST_CONFIGURE                             = 330,  /* not used*/
    DDC_SOCK_MSG_CMD_TEST_VECTORS_STATICS                       = 331,  /* not used*/
    DDC_SOCK_MSG_CMD_TEST_LOOPBACK_INT                          = 332,  /* not used*/
    DDC_SOCK_MSG_CMD_TRG_RESET                                  = 333,

    /* trg.h */
    DDC_SOCK_MSG_CMD_TRG_ENABLE                                 = 334,
    DDC_SOCK_MSG_CMD_TRG_DISABLE                                = 335,
    DDC_SOCK_MSG_CMD_TRG_CONFIG                                 = 336,
    DDC_SOCK_MSG_CMD_TRG_EVENT_SELECT                           = 337,
    DDC_SOCK_MSG_CMD_TRG_EVENT_ENABLE                           = 338,
    DDC_SOCK_MSG_CMD_TRG_EVENT_DISABLE                          = 339,
    DDC_SOCK_MSG_CMD_TRG_GET_TIME_TAG                           = 340,
    DDC_SOCK_MSG_CMD_TRG_GET_STATUE                             = 341,

    /****************************************
              429 API Socket Commands
    *****************************************/
    /* CardInit.h */
    DDC_SOCK_MSG_CMD_429_INIT_CARD                              = 342,
    DDC_SOCK_MSG_CMD_429_FREE_CARD                              = 343,
    DDC_SOCK_MSG_CMD_429_INSTALL_HANDLER                        = 344,
    DDC_SOCK_MSG_CMD_429_UNINSTALL_HANDLER                      = 345,
    DDC_SOCK_MSG_CMD_429_SET_INT_CONDITION                      = 346,
    DDC_SOCK_MSG_CMD_429_GET_INT_STATUS                         = 347,
    DDC_SOCK_MSG_CMD_429_GET_CARD_TYPE                          = 348,
    DDC_SOCK_MSG_CMD_429_GET_HW_VERSION_INFO                    = 349,
    DDC_SOCK_MSG_CMD_429_GET_CHANNEL_COUNT                      = 350,

    /* Control.h */
    DDC_SOCK_MSG_CMD_429_RESET_GROUP                            = 351,
    DDC_SOCK_MSG_CMD_429_RESET_RX_CHANNEL                       = 352,
    DDC_SOCK_MSG_CMD_429_RESET_TX_CHANNEL                       = 353,
    DDC_SOCK_MSG_CMD_429_GET_BIT_FORMAT                         = 354,
    DDC_SOCK_MSG_CMD_429_SET_BIT_FORMAT                         = 355,
    DDC_SOCK_MSG_CMD_429_GET_LOOP_BACK                          = 356,
    DDC_SOCK_MSG_CMD_429_SET_LOOP_BACK                          = 357,
    DDC_SOCK_MSG_CMD_429_GET_CHANNEL_LOOP_BACK                  = 358,
    DDC_SOCK_MSG_CMD_429_SET_CHANNEL_LOOP_BACK                  = 359,
    DDC_SOCK_MSG_CMD_429_GET_LOOP_BACK_MAPPING                  = 360,
    DDC_SOCK_MSG_CMD_429_SET_LOOP_BACK_MAPPING                  = 361,
    DDC_SOCK_MSG_CMD_429_SET_DISC_OUT                           = 362,
    DDC_SOCK_MSG_CMD_429_GET_DISC_OUT                           = 363,
    DDC_SOCK_MSG_CMD_429_SET_DISC_DIR                           = 364,
    DDC_SOCK_MSG_CMD_429_GET_DISC_DIR                           = 365,
    DDC_SOCK_MSG_CMD_429_GET_DISC_IN                            = 366,
    DDC_SOCK_MSG_CMD_429_GET_DISC_ALL                           = 367,
    DDC_SOCK_MSG_CMD_429_SET_DISC_ALL                           = 368,
    DDC_SOCK_MSG_CMD_429_SET_AVION_OUT                          = 369,
    DDC_SOCK_MSG_CMD_429_GET_AVION_OUT                          = 370,
    DDC_SOCK_MSG_CMD_429_SET_AVION_OUT_ENABLE                   = 371,
    DDC_SOCK_MSG_CMD_429_GET_AVOIN_OUT_ENABLE                   = 372,
    DDC_SOCK_MSG_CMD_429_GET_AVION_IN                           = 373,
    DDC_SOCK_MSG_CMD_429_GET_AVION_ALL                          = 374,
    DDC_SOCK_MSG_CMD_429_SET_AVION_ALL                          = 375,
    DDC_SOCK_MSG_CMD_429_IO_INITIALIZE                          = 376,
    DDC_SOCK_MSG_CMD_429_IO_FREE                                = 377,
    DDC_SOCK_MSG_CMD_429_RESET_429_PROGRAMMABLE                 = 378,

    /* transmit.h */
    DDC_SOCK_MSG_CMD_429_ENABLE_TX                              = 379,
    DDC_SOCK_MSG_CMD_429_ENABLE_PASSIVE_TX                      = 380,
    DDC_SOCK_MSG_CMD_429_GET_TX_PARITY                          = 381,
    DDC_SOCK_MSG_CMD_429_SET_TX_PARITY                          = 382,
    DDC_SOCK_MSG_CMD_429_GET_TX_SPEED                           = 383,
    DDC_SOCK_MSG_CMD_429_SET_TX_SPEED                           = 384,
    DDC_SOCK_MSG_CMD_429_GET_TX_STATUS                          = 385,
    DDC_SOCK_MSG_CMD_429_GET_TX_QUEUE_STATUS                    = 386,
    DDC_SOCK_MSG_CMD_429_LOAD_TX_QUEUE_ONE                      = 387,
    DDC_SOCK_MSG_CMD_429_LOAD_TX_QUEUE_MORE                     = 388,
    DDC_SOCK_MSG_CMD_429_ADD_REPEATED                           = 389,
    DDC_SOCK_MSG_CMD_429_CLEAR_REPEATED                         = 390,
    DDC_SOCK_MSG_CMD_429_DEL_REPEATED                           = 391,
    DDC_SOCK_MSG_CMD_429_GET_NUM_OF_REPEATED                    = 392,
    DDC_SOCK_MSG_CMD_429_GET_REPEATED                           = 393,
    DDC_SOCK_MSG_CMD_429_GET_ALL_REPEATED                       = 394,
    DDC_SOCK_MSG_CMD_429_MODIFY_REPEATED_DATA                   = 395,

    /* receive.h */
    DDC_SOCK_MSG_CMD_429_ENABLE_RX                              = 396,
    DDC_SOCK_MSG_CMD_429_ENABLE_PASSIVE_RX                      = 397,
    DDC_SOCK_MSG_CMD_429_GET_RX_MODE                            = 398,
    DDC_SOCK_MSG_CMD_429_GET_RX_CHANNEL_MODE                    = 399,
    DDC_SOCK_MSG_CMD_429_SET_RX_MODE                            = 400,
    DDC_SOCK_MSG_CMD_429_SET_RX_CHANNEL_MODE                    = 401,
    DDC_SOCK_MSG_CMD_429_GET_RX_PARITY                          = 402,
    DDC_SOCK_MSG_CMD_429_GET_RX_CHANNEL_PARITY                  = 403,
    DDC_SOCK_MSG_CMD_429_SET_RX_PARITY                          = 404,
    DDC_SOCK_MSG_CMD_429_SET_RX_CHANNEL_PARITY                  = 405,
    DDC_SOCK_MSG_CMD_429_GET_RX_SPEED                           = 406,
    DDC_SOCK_MSG_CMD_429_GET_RX_CHANNEL_SPEED                   = 407,
    DDC_SOCK_MSG_CMD_429_SET_RX_SPEED                           = 408,
    DDC_SOCK_MSG_CMD_429_SET_RX_CHANNEL_SPEED                   = 409,
    DDC_SOCK_MSG_CMD_429_GET_RX_STATUS                          = 410,
    DDC_SOCK_MSG_CMD_429_ENABLE_TIME_STAMP                      = 411,
    DDC_SOCK_MSG_CMD_429_CONFIG_TIME_STAMP                      = 412,
    DDC_SOCK_MSG_CMD_429_GET_TIME_STAMP_CONFIG                  = 413,
    DDC_SOCK_MSG_CMD_429_GET_TIME_STAMP_STATUS                  = 414,
    DDC_SOCK_MSG_CMD_429_RESET_TIME_STAMP                       = 415,
    DDC_SOCK_MSG_CMD_429_GET_TIME_STAMP                         = 416,
    DDC_SOCK_MSG_CMD_429_CLEAR_MAILBOX                          = 417,
    DDC_SOCK_MSG_CMD_429_GET_MAILBOX                            = 418,
    DDC_SOCK_MSG_CMD_429_GET_MAILBOX_STATUS                     = 419,
    DDC_SOCK_MSG_CMD_429_READ_MAILBOX_IRIG                      = 420,
    DDC_SOCK_MSG_CMD_429_CLEAR_RX_QUEUE                         = 421,
    DDC_SOCK_MSG_CMD_429_GET_RX_QUEUE_STATUS                    = 422,
    DDC_SOCK_MSG_CMD_429_READ_RX_QUEUE_IRIG_ONE                 = 423,
    DDC_SOCK_MSG_CMD_429_READ_RX_QUEUE_IRIG_MORE                = 424,
    DDC_SOCK_MSG_CMD_429_ENABLE_FILTER                          = 425,
    DDC_SOCK_MSG_CMD_429_CONFIG_FILTER                          = 426,
    DDC_SOCK_MSG_CMD_429_ADD_FILTER                             = 427,
    DDC_SOCK_MSG_CMD_429_CLEAR_FILTER                           = 428,
    DDC_SOCK_MSG_CMD_429_DEL_FILTER                             = 429,
    DDC_SOCK_MSG_CMD_429_GET_FILTER                             = 430,
    DDC_SOCK_MSG_CMD_429_GET_ALL_FILTER                         = 431,
    DDC_SOCK_MSG_CMD_429_GET_FILTER_STATUS                      = 432,
    DDC_SOCK_MSG_CMD_429_GET_NUM_OF_FILTER                      = 433,
    DDC_SOCK_MSG_CMD_429_INSTALL_FIFO_RX_HOST_BUFFER            = 434,
    DDC_SOCK_MSG_CMD_429_UNINSTALL_FIFO_RX_HOST_BUFFER          = 435,
    DDC_SOCK_MSG_CMD_429_ENABLE_RX_HOST_BUFFER                  = 436,
    DDC_SOCK_MSG_CMD_429_DISABLE_RX_HOST_BUFFER                 = 437,
    DDC_SOCK_MSG_CMD_429_READ_RX_HOST_BUFFER                    = 438,
    DDC_SOCK_MSG_CMD_429_GET_HOST_BUF_METRICS                   = 439,

    /* serial.h */
    DDC_SOCK_MSG_CMD_429_WRITE_UART_CONFIG                      = 440,
    DDC_SOCK_MSG_CMD_429_READ_UART_CONFIG                       = 441,
    DDC_SOCK_MSG_CMD_429_ENABLE_UART                            = 442,
    DDC_SOCK_MSG_CMD_429_DISABLE_UART                           = 443,
    DDC_SOCK_MSG_CMD_429_WRITE_UART                             = 444,
    DDC_SOCK_MSG_CMD_429_READ_UART                              = 445,

    /* arinc717.h */
    DDC_SOCK_MSG_CMD_429_ACEX_ARINC_717_PROG_CONFIG             = 446,
    DDC_SOCK_MSG_CMD_429_ACEX_ARINC_717_PROG_STATE              = 447,
    DDC_SOCK_MSG_CMD_429_ACEX_ARINC_717_PROG_LOAD_TX_DATA       = 448,
    DDC_SOCK_MSG_CMD_429_ACEX_ARINC_717_INTERRUPTS              = 449,
    DDC_SOCK_MSG_CMD_429_ACEX_ARINC_717_PROG_RX_DATA            = 450,

    /* can.h */
    DDC_SOCK_MSG_CMD_429_ACEX_CAN_BUS_CONFIG                    = 451,
    DDC_SOCK_MSG_CMD_429_ACEX_CAN_BUS_STATE                     = 452,
    DDC_SOCK_MSG_CMD_429_ACEX_CAN_BUS_TX_DATA                   = 453,
    DDC_SOCK_MSG_CMD_429_ACEX_CAN_BUS_RX_DATA                   = 454,
    DDC_SOCK_MSG_CMD_429_ACEX_CAN_BUS_FIRMWARE_VERSION          = 455,

    /* tester.h */
    DDC_SOCK_MSG_CMD_429_DD429X_ADD_REPEATED                    = 456,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_REPEATED                    = 457,
    DDC_SOCK_MSG_CMD_429_DD429X_MODIFY_REPEATED_DATA            = 458,
    DDC_SOCK_MSG_CMD_429_DD429X_LOAD_TX_QUEUE_ONE               = 459,
    DDC_SOCK_MSG_CMD_429_DD429X_LOAD_TX_QUEUE_MORE              = 460,
    DDC_SOCK_MSG_CMD_429_DD429X_SET_VARIABLE_SPEED              = 461,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_VARIABLE_SPEED              = 462,
    DDC_SOCK_MSG_CMD_429_DD429X_SEND_TX_FRAME_ASYNC             = 463,
    DDC_SOCK_MSG_CMD_429_DD429X_SET_TX_FRAME_RESOLUTION         = 464,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_TX_FRAME_RESOLUTION         = 465,
    DDC_SOCK_MSG_CMD_429_DD429X_SET_TX_FRAME_CONTROL            = 466,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_TX_FRAME_INFO               = 467,
    DDC_SOCK_MSG_CMD_429_DD429X_ADD_TX_FRAME                    = 468,
    DDC_SOCK_MSG_CMD_429_DD429X_MODIFY_TX_FRAME_DATA            = 469,
    DDC_SOCK_MSG_CMD_429_DD429X_SET_TX_MAJOR_FRAME_REPEAT_COUNT = 470,
    DDC_SOCK_MSG_CMD_429_DD429X_SET_AMPLITUDE                   = 471,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_AMPLITUDE                   = 472,
    DDC_SOCK_MSG_CMD_429_DD429X_VOLTAGE_MONITOR_ENABLE          = 473,
    DDC_SOCK_MSG_CMD_429_DD429X_VOLTAGE_MONITOR_START           = 474,
    DDC_SOCK_MSG_CMD_429_DD429X_VOLTAGE_MONITOR_GET_STATUS      = 475,
    DDC_SOCK_MSG_CMD_429_DD429X_VOLTAGE_MONITOR_GET_DATA        = 476,

    /* hardware.h */
    DDC_SOCK_MSG_CMD_429_MEM_READ_32                            = 477,
    DDC_SOCK_MSG_CMD_429_MEM_WRITE_32                           = 478,
    DDC_SOCK_MSG_CMD_429_REG_READ_32                            = 479,
    DDC_SOCK_MSG_CMD_429_REG_WRITE_32                           = 480,

    /* errors.h */
    DDC_SOCK_MSG_CMD_429_GET_ERROR_MSG                          = 481,
    DDC_SOCK_MSG_CMD_429_PRINT_ERROR_MSG                        = 482,

    /* device429r.h.  Unique to Remote Access only */
    DDC_SOCK_MSG_CMD_429_RE_ENABLE_INTERRUPTS                   = 483,
    DDC_SOCK_MSG_CMD_ANY                                        = 484,

    /* serial.h */
    DDC_SOCK_MSG_CMD_429_WRITE_UART_CONFIG_EXT                  = 485,
    DDC_SOCK_MSG_CMD_429_READ_UART_CONFIG_EXT                   = 486,

    /* tester.h */
    DDC_SOCK_MSG_CMD_429_DD429X_ADD_REPEATED_ITEM               = 487,
    DDC_SOCK_MSG_CMD_429_DD429X_MODIFY_REPEATED_DATA_ITEM       = 488,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_REPEATED_ITEM               = 489,
    DDC_SOCK_MSG_CMD_429_DD429X_DEL_REPEATED_ITEM               = 490,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_ALL_REPEATED_ITEM           = 491,

    DDC_SOCK_MSG_CMD_429_DD429X_CONFIG_REPEATER                 = 492,
    DDC_SOCK_MSG_CMD_429_DD429X_SET_REPEATER_MODE               = 493,
    DDC_SOCK_MSG_CMD_429_DD429X_GET_REPEATER_MODE               = 494,

    DDC_SOCK_MSG_CMD_429_DD429X_GET_IRIG_TX                     = 495,
    DDC_SOCK_MSG_CMD_429_DD429X_SET_IRIG_TX                     = 496,

    /****************************************
              DDC Socket CMD END
    *****************************************/
    DDC_SOCK_MSG_CMD_INVALID                                    = 497

} DDC_SOCK_MSG_CMD;

typedef enum _DDC_SOCK_MSG_FIELD_TYPE
{
    DDC_SOCK_MSG_FIELD_TYPE_U8 = 0,
    DDC_SOCK_MSG_FIELD_TYPE_U16,
    DDC_SOCK_MSG_FIELD_TYPE_U32,
    DDC_SOCK_MSG_FIELD_TYPE_U64,
    DDC_SOCK_MSG_FIELD_TYPE_S8,
    DDC_SOCK_MSG_FIELD_TYPE_S16,
    DDC_SOCK_MSG_FIELD_TYPE_S32,
    DDC_SOCK_MSG_FIELD_TYPE_S64,
    DDC_SOCK_MSG_FIELD_TYPE_BOOL,
    DDC_SOCK_MSG_FIELD_TYPE_BUF,
    DDC_SOCK_MSG_FIELD_TYPE_INVALID

} DDC_SOCK_MSG_FIELD_TYPE;


typedef struct _DDC_SOCK_DATA_TYPE
{
    U32BIT u32Type;
    U32BIT u32Size;
    void   *pBuf;

} DDC_SOCK_DATA_TYPE, *PDDC_SOCK_DATA_TYPE;

#pragma pack(1)
typedef struct _DDC_SOCK_MSG_HEADER
{
    U32BIT ddcSockMsgCurrentLen;
    U32BIT ddcSockMsgMaxLen;
    U32BIT ddcSockMsgType;
    U32BIT ddcSockMsgCmd;
    U32BIT ddcSockMsgFieldNum;

} DDC_SOCK_MSG_HEADER, * PDDC_SOCK_MSG_HEADER;

#define DDC_SOCK_MSG_FIELD_NUM 32
typedef struct _DDC_SOCK_MSG_STRUCT
{
    DDC_SOCK_MSG_HEADER ddcSockMsgHeader;
    U8BIT * pddcSockMsgFields[DDC_SOCK_MSG_FIELD_NUM];

} DDC_SOCK_MSG_STRUCT, * PDDC_SOCK_MSG_STRUCT;

#define GET_DDC_SOCK_MSG_FIELDS_DATA_POINTER(ptr)   (U8BIT **)(((U8BIT *)ptr) + (U8BIT)(sizeof(DDC_SOCK_MSG_HEADER)))

typedef struct _DDC_SOCK_MSG_FIELD_HEADER
{
    U32BIT ddcSockMsgFieldLen;
    U32BIT ddcSockMsgFieldType;
} DDC_SOCK_MSG_FIELD_HEADER, * PDDC_SOCK_MSG_FIELD_HEADER;
#pragma pack()

/* ========================================================================== */
/* DDC_SOCKET */
/* ========================================================================== */
#ifdef WIN32
#include <Winerror.h>

#define DDC_SOCK_MSG_WORD       WORD
#define DDC_SOCK_SLEEP_MS(dwMs) Sleep((dwMs))

#define DDC_SOCKET              SOCKET
#define DDC_WSAGetLastError     WSAGetLastError
#define DDC_WSAStartup          WSAStartup
#define DDC_WSACleanup          WSACleanup
#define DDC_closesocket         closesocket
#define DDC_MAKEWORD            MAKEWORD
#define DDC_ZERO_MEMORY         ZeroMemory
#define DDC_WSADATA             WSADATA
#define DDC_LPWSADATA           LPWSADATA

#define DDC_WSAENOTSOCK         WSAENOTSOCK
#define DDC_WSAEINTR            WSAEINTR
#endif /* WIN32 */


#ifdef LINUX
#define DDC_SOCK_SLEEP_MS(dwMs) usleep((dwMs) * 1000)
#endif /* LINUX */

#ifdef VX_WORKS
#include <vxWorks.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sysLib.h>  /* sysClkRateGet() */
#include <taskLib.h> /* taskDelay() */

#define DDC_SOCK_SLEEP_MS(dwMs) { \
    int ticks = 0; \
    if (dwMs) \
    { \
        ticks = (dwMs * sysClkRateGet() / 1000) + 1; \
    } \
    taskDelay(ticks); \
}


#endif /* VX_WORKS */

#ifndef WIN32 /* not WIN32 - common to all OS's other than Windows */
#define DDC_SOCK_MSG_WORD       U16BIT
#define DDC_SOCKET              int

#define DDC_ZERO_MEMORY(s,n)    memset((s),0,(n));
#define DDC_MAKEWORD(x,y)       ((x<<8)|y)

#define INVALID_SOCKET          DDC_SOCK_INVALID
#define WAIT_TIMEOUT            ETIMEDOUT
#define WAIT_OBJECT_0           0
#define SD_BOTH                 SHUT_RDWR

#define DDC_WSAENOTSOCK         ENOTSOCK
#define DDC_WSAEINTR            EINTR

#define DDC_WSADESCRIPTION_LEN  256
#define DDC_WSASYS_STATUS_LEN   128

typedef struct _DDC_WSADATA
{
  DDC_SOCK_MSG_WORD wVersion;
  DDC_SOCK_MSG_WORD wHighVersion;
  char szDescription[DDC_WSADESCRIPTION_LEN+1];
  char szSystemStatus[DDC_WSASYS_STATUS_LEN+1];
  unsigned short iMaxSockets;
  unsigned short iMaxUdpDg;
  char *lpVendorInfo;

} DDC_WSADATA, *DDC_LPWSADATA;

extern int DDC_WSAClearLastError
(
    void
);

extern int DDC_WSAGetLastError
(
    void
);

extern int DDC_WSAStartup
(
    DDC_SOCK_MSG_WORD wVersionRequested,
    DDC_LPWSADATA lpWSAData
);

extern int DDC_WSACleanup
(
    void
);

extern int DDC_closesocket
(
    DDC_SOCKET socket
);

#endif /* not WIN32 - common to all OS's other than Windows */


/* ========================================================================== */
/*                            FUNCTION PROTOTYPES                             */
/* ========================================================================== */

extern S16BIT DDCSockCreateMessageStruct
(
    U32BIT u32MsgType,
    U32BIT u32MsgCmd,
    U32BIT u32MaxLen,
    PDDC_SOCK_MSG_STRUCT * ppMsgHeader
);

extern S16BIT DDCSockDestroyMessageStruct
(
    PDDC_SOCK_MSG_STRUCT *ppMsgHeader
);

extern S16BIT DDCSockAddFieldToMessageStruct
(
    PDDC_SOCK_MSG_STRUCT pMsgHeader,
    U32BIT u32FieldType,
    U32BIT u32FieldDataLen,
    void * pDataBuf
);

extern S16BIT DDCSockGetFieldDataFromMessageStruct
(
    PDDC_SOCK_MSG_STRUCT pMsgHeader,
    U8BIT u8FieldIndex,
    U32BIT u32ExpectFieldType,
    U32BIT u32ExpectFieldDataLen,
    void * pDataBuf
);

extern S16BIT DDCSockMsgEncode
(
    PDDC_SOCK_MSG_STRUCT pMsgHeader,
    char * pMsgBuf
);

extern S16BIT DDCSockMsgDecode
(
    U8BIT * pMsgBuf,
    PDDC_SOCK_MSG_STRUCT pDecodedMsgHeader
);

extern S16BIT DDCSockMsgSend
(
    DDC_SOCKET ConnectSocket,
    PDDC_SOCK_MSG_STRUCT pMsgHeader
);

extern S16BIT DDCSockMsgRecvRaw
(
    DDC_SOCKET ConnectSocket,
    U32BIT u32MsgType,
    U32BIT u32MsgCmd,
    S32BIT s32TimeOut,
    U8BIT * pMsgBuf
);

extern S16BIT DDCSockMsgRecv
(
    DDC_SOCKET ConnectSocket,
    U32BIT u32MsgType,
    U32BIT u32MsgCmd,
    S32BIT s32TimeOut,
    PDDC_SOCK_MSG_STRUCT pMsgHeader
);

extern void DDCSockDataSet
(
    DDC_SOCK_DATA_TYPE* pData,
    U16BIT u16Index,
    U32BIT u32FieldType,
    U32BIT u32FieldDataLen,
    void *pDataBuf
);

#ifdef __cplusplus
}
#endif

#endif /* _DDCSOCKMSG_H_ */
