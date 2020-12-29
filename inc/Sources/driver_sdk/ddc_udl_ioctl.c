/*******************************************************************************
 * FILE: ddc_udl_ioctl.c
 *
 * DESCRIPTION:
 *
 *  This file provides the driver side of the USER <=> KERNEL interface.
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
#include "include/ddc_types.h"
#include "include/ddc_ioctl.h"
#include "include/ddc_device_ids.h"
#include "include/ddc_error_list.h"
#include "driver_sdk/ddc_udl_um_private.h"
#include "bus/ddc_udl_bus_private.h"
#include "bus/pci/ddc_udl_bus_pci_private.h"

#include "driver_sdk/ddc_udl_sdk.h"
#include "driver_sdk/ddc_udl_bd_private.h"
#include "driver_sdk/ddc_udl_flash_private.h"
#include "driver_sdk/ddc_udl_interrupt_private.h"
#include "driver_sdk/ddc_udl_driver_private.h"
#include "driver_sdk/debug/ddc_udl_debug_private.h"

#include "core/1553/ddc_udl_1553_private.h"
#include "core/1553/ddc_udl_1553_bc_private.h"
#include "core/1553/ddc_udl_1553_rt_private.h"
#include "core/1553/ddc_udl_1553_mt_private.h"
#include "core/1553/ddc_udl_1553_error_inj_private.h"
#include "core/1553/ddc_udl_1553_trigger_private.h"
#include "core/irig/ddc_udl_irigb_private.h"
#include "core/aio/ddc_udl_aio.h"
#include "core/aio/ddc_udl_aio_private.h"
#include "core/dio/ddc_udl_dio_private.h"
#include "core/arinc429/ddc_udl_arinc429_private.h"
#include "core/arinc717/ddc_udl_arinc717_private.h"
#include "core/can/ddc_udl_can.h"
#include "os/interface/ddc_udl_os_util_private.h"

/******************************************************************************
 *****************************************************************************/
S16BIT ddcUdlIoctl
(
    struct _DDC_UDL_DEVICE_CONTEXT *pDeviceContext,
    DDC_IOCTL_PARAMS *pIoctlParams,
    U32BIT u32IoctlCmd,
    U8BIT u8CardNumber,
    U8BIT u8ChannelNumber, 
    U8BIT u8IsCompatCall
)
{
    U32BIT i;
    U32BIT u32IntMask;

    void   *pvoidData = NULL;
    char   *pUserBuf = NULL;
    char   *pBuf = NULL;
    U32BIT u32Data = 0;
    U16BIT u16Data = 0;
    U32BIT u32DataLen = 0;
    U32BIT OutputBufferLength = 0;
    size_t bytesReturned = 0;
    U32BIT u32BytesReturned = 0;
    S16BIT status = DDC_UDL_ERROR__SUCCESS;

    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_CntrlRoutines,
        "IOCTL Group: %d  Type: %d\n",
        DDC_IOCTL__GROUP_GET_VAL(u32IoctlCmd),
        DDC_IOCTL__TYPE_GET_VAL(u32IoctlCmd));

    switch (u32IoctlCmd)
    {
        /* ========================================================================== */
        /* Device Group                                                               */
        /* ========================================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_GET_INFO:
        /* ---------------------------------------------------- */
        {
            /* determine the type of info requested */
            switch (pIoctlParams->Param1)
            {
                /* REGISTERS */
                case DDC_GET_INFO__REG_BASE_ADDR:
                {
                    switch (pIoctlParams->Param2)
                    {
                        case DDC_GET_INFO_ADDR__BD_GLOBAL:
                        {
                            status = DDC_COPY_TO_USER((void *)pIoctlParams->sIoctlInfo.pOutBuffer, (void *)(pDeviceContext->pBoardInfo[pIoctlParams->Channel]->pu32RegBA), sizeof(U32BIT));
                            
                            if (status)
                            {
                                status = DDC_UDL_ERROR__COPY2USER_FAIL;
                            }
                            break;
                        }
                        
                        default:
                        {
                            status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
                            break;
                        }
                    }

                    break;
                }

                /* MEMORY */
                
                
                /* ERROR */
                default:
                {
                    status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
                    break;
                }
            }

            break;
        }
        
        /* ---------------------------------------------------- */
        case IOCTL_GET_DEV_INFO:
        /* ---------------------------------------------------- */
        {
            DEV_INFO *pDevInfo;
            U16BIT u16Location;
            U16BIT u16SubLocation;
            U32BIT *pRdData = NULL;

            bytesReturned = sizeof(DEV_INFO);
            pRdData = DDC_KERNEL_MALLOC(bytesReturned);
            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            pDevInfo = (DEV_INFO *)pRdData;
            memset(pDevInfo, 0, sizeof(*pDevInfo));


            pDevInfo->dwVendorID = DDC_VENDOR_ID;
            pDevInfo->dwDeviceID = pDeviceContext->u16DeviceID;

            ddcUdlBusGetLocationInfo(pDeviceContext, &u16Location, &u16SubLocation);
            pDevInfo->dwBusHub = u16Location;
            pDevInfo->dwSlotPort = u16SubLocation;

            if (pDeviceContext->u8Num1553Channels > 0)
            {
                pDevInfo->dwChanMemBA = pDeviceContext->pChannel1553[pIoctlParams->Channel]->u32UserMemBA;
                pDevInfo->dwChanMemLen = pDeviceContext->pChannel1553[pIoctlParams->Channel]->u32UserMemSizeBytes;
                pDevInfo->u32Capability = pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32Capability;
            }
            else
            {
                pDevInfo->dwChanMemBA = 0x00000000;
                pDevInfo->dwChanMemLen = 0x00000000;
                pDevInfo->u32Capability = 0x00000000;
            }

            memcpy(&(pDevInfo->sHwVersionInfo), &(pDeviceContext->sHwVersionInfo), sizeof(pDevInfo->sHwVersionInfo));

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) pRdData, sizeof(DEV_INFO));

            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_ENHANCED_CAPABILITIES:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            pRdData = DDC_KERNEL_MALLOC(sizeof(ENHANCED_CAPABILITY_INFO));

            if (!pRdData)
            {
                break;
            }

            ddcUdlBdReadEnhancedCapabilities(pDeviceContext, (PENHANCED_CAPABILITY_INFO)pRdData);

            bytesReturned = sizeof(ENHANCED_CAPABILITY_INFO);
            status = DDC_COPY_TO_USER((void *)pIoctlParams->sIoctlInfo.pOutBuffer, (void *)pRdData, bytesReturned);

            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_HW_VERSION_INFO:
        /* ---------------------------------------------------- */
        {
            status = DDC_COPY_TO_USER((void *) pIoctlParams->sIoctlInfo.pOutBuffer, (void *) &(pDeviceContext->sHwVersionInfo), sizeof(HWVERSIONINFO));

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_IO_COUNT:
        /* ---------------------------------------------------- */
        {
            CHANCOUNT_p pChanCount;

            pChanCount = DDC_KERNEL_MALLOC(sizeof(CHANCOUNT_t));

            if (!pChanCount)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            pChanCount->bDiscrete = pDeviceContext->u8NumDiscreteIO;
            pChanCount->bAvionic = pDeviceContext->u8NumAvionicIO;
            pChanCount->b1553 = pDeviceContext->u8Num1553Channels;
            pChanCount->bTx = pDeviceContext->u8NumDed429Tx;
            pChanCount->bRx = pDeviceContext->u8NumDed429Rx;
            pChanCount->RS232 = pDeviceContext->u8NumRS232;
            pChanCount->RS485 = pDeviceContext->u8NumRS485;
            pChanCount->bBoardModel = (U8BIT)pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32ModelNum;
            pChanCount->bGroup = pChanCount->bTx;
            pChanCount->bDioTt = pDeviceContext->u8NumDioTt;

            bytesReturned = sizeof(CHANCOUNT_t);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pChanCount, bytesReturned);

            DDC_KERNEL_FREE(pChanCount);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_DEV_FEATURE_INFO:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            bytesReturned = sizeof(DEV_FEATURE_STATUS_INFO);

            pRdData = DDC_KERNEL_MALLOC(bytesReturned);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            bdGetFeatureInfo(pDeviceContext, pIoctlParams, (DEV_FEATURE_STATUS_INFO *) pRdData);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_BOARD_STATUS:
        /* ---------------------------------------------------- */
        {
            /* check to see if this device supports this feature - add features as necessary */
            if (pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdThermalMon.u32Options & HAS_THERMAL_DETECTION_HARDWARE)
            {
                ddcUdlGetBoardStatus(pDeviceContext, pIoctlParams);
            }
            else
            {
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_DEVICE_INIT:
        /* ---------------------------------------------------- */
        {
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_DEVICE_CONFIGURE:
        /* ---------------------------------------------------- */
        {
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_DEVICE_CONFIGURE CALLED -");

            /*-------------------------------------------------------------------
                pIoctlParams->Param1 passes in one of the structures:
               (ACEX_MRT_CONFIG, ACEX_RT_ACCESS, ACEX_RT_CONFIG, MT_MTI_CONFIG,
               ACEX_BC_INIT_INFO, ACEX_BC_QUEUE_INFO, ACEX_BC_HBUF_INSTALL_INFO,
               ACEX_BC_HBUF_UNINSTALL_INFO)
               pIoctlParams->Param2 is sizeof the structure.
               All of these structures have common ACEX_CONFIG_ID in them, which
               is used to determine which structure pIoctlParams->Param1 pass in.
               ------------------------------------------------------------------*/
            pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param2);

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->Param1), pIoctlParams->Param2);

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                " case 0x%x\n", ((ACEX_CONFIG_ID *)pvoidData)->u16Type);


            /* retrieve type of configuration requested */

            switch (((ACEX_CONFIG_ID *)pvoidData)->u16Type)
            {
                /* ---------------------------------------------------- */
                case ACEX_CHAN_INIT:
                /* ---------------------------------------------------- */
                {
                    gen1553ChannelInit(pDeviceContext, (U8BIT)((ACEX_CONFIG_ID *)pvoidData)->u16Channel);
                    break;
                }

                /* ==================================================== */
                /*                   RT CONFIGURATION                   */
                /* ==================================================== */

                /* ---------------------------------------------------- */
                case ACEX_MRT_CONFIG:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    i = ((ACEX_CONFIG_ID *)pvoidData)->u16Channel;
                    status = mrtOpen(pDeviceContext, (ACEX_MRT_CONFIG_TYPE *)pvoidData);

                    bytesReturned = sizeof(ACEX_MRT_HW_INFO);
                    pRdData = DDC_KERNEL_MALLOC(bytesReturned);

                    if (!pRdData)
                    {
                        status = DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                        break;
                    }

                    ((ACEX_MRT_HW_INFO *)pRdData)->u32ConfigurationBA = *(pDeviceContext->pChannel1553[i]->sRT.pu32RegBA);
                    ((ACEX_MRT_HW_INFO *)pRdData)->u32GblDStkPtrBA = *(pDeviceContext->pChannel1553[i]->sRT.pu32RegBA) + REG_MRT_GBL_DATA_STK_PTR;
                    ((ACEX_MRT_HW_INFO *)pRdData)->u16RtSource = (pDeviceContext->pChannel1553[i]->sRT.u16RtSource);
                    ((ACEX_MRT_HW_INFO *)pRdData)->bMode = (pDeviceContext->pChannel1553[i]->sRT.bMode);
                    ((ACEX_MRT_HW_INFO *)pRdData)->u32MemBA = *(pDeviceContext->pChannel1553[i]->pu32MemBA);

                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "ACEX_MRT_CONFIG copy to user fail\n");

                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    DDC_KERNEL_FREE(pRdData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_MRT_FREE:
                /* ---------------------------------------------------- */
                {
                    status = mrtClose(pDeviceContext, (U8BIT)((ACEX_CONFIG_ID *)pvoidData)->u16Channel);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_MRT_CONFIG_UPDATE: /* update MRT Global Configuration Register */
                /* ---------------------------------------------------- */
                {
                    pDeviceContext->pChannel1553[((ACEX_CONFIG_ID *)pvoidData)->u16Channel]->sRT.sRtxCfgReg.u32RTGConfig = ((ACEX_RT_ACCESS *)pvoidData)->u32Data;

                 DDC_REG_WRITE(pDeviceContext,
                        (*(pDeviceContext->pChannel1553[((ACEX_CONFIG_ID *)pvoidData)->u16Channel]->sRT.pu32RegBA)) + REG_MRT_GCONFIG,
                        &(pDeviceContext->pChannel1553[((ACEX_CONFIG_ID *)pvoidData)->u16Channel]->sRT.sRtxCfgReg.u32RTGConfig));

                 break; }

                /* ---------------------------------------------------- */
                case ACEX_MRT_HBUF_CTRL:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    OutputBufferLength = pIoctlParams->Param3;
                    pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

                    if (!pRdData)
                    {
                        break;
                    }

                    status = mrtHbufCtrl(pDeviceContext, ((ACEX_RT_ACCESS *)pvoidData), OutputBufferLength, &bytesReturned, (U32BIT *)pRdData);

                    if (status)
                    {
                        DDC_KERNEL_FREE(pRdData);
                        break;
                    }

                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                    DDC_KERNEL_FREE(pRdData);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    if (u8IsCompatCall)
                    {
                        u32BytesReturned = bytesReturned;
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &u32BytesReturned, sizeof(U32BIT));
                    }
                    else
                    {
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &bytesReturned, sizeof(size_t));
                    }

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_MRT_CMDSTK_STATS:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    bytesReturned = sizeof(ACEX_1553_MRT_STAT_TYPE);
                    pRdData = DDC_KERNEL_MALLOC(bytesReturned);

                    if (!pRdData)
                    {
                        break;
                    }

                    memcpy((((ACEX_1553_MRT_STAT_TYPE *)pRdData)),
                        &(pDeviceContext->pChannel1553[((ACEX_CONFIG_ID *)pvoidData)->u16Channel]->sRT.stats),
                        sizeof(ACEX_1553_MRT_STAT_TYPE));

                    if (((ACEX_RT_ACCESS *)pvoidData)->u32Data == ACEX_MRT_RTCMD_HIGH_PCT_RESET) /* reset high % */
                    {
                        i = pDeviceContext->pChannel1553[((ACEX_CONFIG_ID *)pvoidData)->u16Channel]->sRT.stats.u32LastNumCmdStkEntries;
                        pDeviceContext->pChannel1553[((ACEX_CONFIG_ID *)pvoidData)->u16Channel]->sRT.stats.u32HighNumCmdStkEntries = i;
                    }

                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                    DDC_KERNEL_FREE(pRdData);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_OPEN:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    pRdData = (U32BIT *)rtOpen(pDeviceContext, (ACEX_RT_CONFIG *)pvoidData);

                    bytesReturned = sizeof(ACEX_RT_HW_INFO);
                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_CONFIG_SET:
                /* ---------------------------------------------------- */
                {
                    status = rtConfigSet(pDeviceContext, (ACEX_RT_CONFIG *)pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_CONFIG_CLR:
                /* ---------------------------------------------------- */
                {
                    status = rtConfigClr(pDeviceContext, (ACEX_RT_CONFIG *)pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_BITWD_RD:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    pRdData = DDC_KERNEL_MALLOC(pIoctlParams->Param3);

                    if (!pRdData)
                    {
                        break;
                    }

                    status = rtInternalBITWdRd(pDeviceContext, ((ACEX_RT_ACCESS *)pvoidData), (U16BIT *)pRdData);

                    bytesReturned = pIoctlParams->Param3;
                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                    DDC_KERNEL_FREE(pRdData);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_START:
                /* ---------------------------------------------------- */
                {
                    rtStart(pDeviceContext, ((ACEX_RT_ACCESS *)pvoidData));
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_START_RESET_CMDSTK:
                /* ---------------------------------------------------- */
                {
                    mrtRtCmdStkSync(pDeviceContext, (U8BIT)((ACEX_CONFIG_ID *)pvoidData)->u16Channel);
                    rtStart(pDeviceContext, ((ACEX_RT_ACCESS *)pvoidData));
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_STOP:
                /* ---------------------------------------------------- */
                {
                    rtStop(pDeviceContext, ((ACEX_RT_ACCESS *)pvoidData));
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_CLOSE:
                /* ---------------------------------------------------- */
                {
                    rtClose(pDeviceContext, (ACEX_RT_CONFIG *)pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_LATCH_CTRL:
                /* ---------------------------------------------------- */
                {
                    rtLatchCtrl(pDeviceContext, (ACEX_RT_ACCESS *)pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_SOURCE_CTRL:
                /* ---------------------------------------------------- */
                {
                    status = rtSourceCtrl(pDeviceContext, (ACEX_RT_ACCESS *)pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_MRT_CMDSTK_SET_TO_LATEST:
                /* ---------------------------------------------------- */
                {
                    status = mrtSetCmdStkPtrToLatest(pDeviceContext, (U8BIT)((ACEX_CONFIG_ID *)pvoidData)->u16Channel);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_STREAM_CTRL:
                /* ---------------------------------------------------- */
                {
                    mrtStreamCtrl(pDeviceContext, ((ACEX_RT_ACCESS *)pvoidData));
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_MRT_GET_BRDCST_ADDRS:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    pRdData = (U32BIT *)mrtOpenBrdcst(pDeviceContext, (ACEX_CONFIG_ID *)pvoidData);

                    bytesReturned = sizeof(ACEX_RT_HW_INFO);
                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_RT_DATA_ARRAY_CTRL:
                /* ---------------------------------------------------- */
                {
                    mrtDataArrayCtrl(pDeviceContext, ((ACEX_RT_DATA_ARRAY *)pvoidData));
                    break;
                }


                /* ==================================================== */
                /*                  MT CONFIGURATION                    */
                /* ==================================================== */

                /* ---------------------------------------------------- */
                case ACEX_MT_INIT:
                /* ---------------------------------------------------- */
                {
                    status = mtInitialize(pDeviceContext, (MT_MTI_CONFIG *)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "mtInitialize ERROR: status=%d\n", status);
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_MT_CONFIG:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    pRdData = DDC_KERNEL_MALLOC(sizeof(MT_MTI_HW_INFO));

                    if (!pRdData)
                    {
                        break;
                    }

                    status = mtOpen(pDeviceContext, (MT_MTI_CONFIG *)pvoidData, (MT_MTI_HW_INFO *)pRdData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "mtOpen ERROR: status=%d\n", status);

                        DDC_KERNEL_FREE(pRdData);
                        break;
                    }

                    bytesReturned = sizeof(MT_MTI_HW_INFO);
                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                    DDC_KERNEL_FREE(pRdData);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_MT_FREE:
                /* ---------------------------------------------------- */
                {
                    status = mtClose(pDeviceContext, ((ACEX_CONFIG_ID *)pvoidData)->u16Channel);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "mtClose ERROR: status=%d\n", status);

                        break;
                    }

                    break;
                }


                /* ==================================================== */
                /*                  BC CONFIGURATION                    */
                /* ==================================================== */

                /* ---------------------------------------------------- */
                case ACEX_BC_INIT:
                /* ---------------------------------------------------- */
                {
                    /* BC initialize */
                    status = bcOpen(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_CONFIG:
                /* ---------------------------------------------------- */
                {
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_FREE:
                /* ---------------------------------------------------- */
                {
                    /* BC free */
                    status = bcClose(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_HBUF_INSTALL:
                /* ---------------------------------------------------- */
                {
                    /* BC HBuf install */
                    status = bcHbufEnable(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_HBUF_UNINSTALL:
                /* ---------------------------------------------------- */
                {
                    /* BC HBuf uninstall */
                    status = bcHbufDisable(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_GPQ_INIT:
                /* ---------------------------------------------------- */
                {
                    /* BC General Purpose Queue init */
                    status = bcGpqInit(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_LPQ_INIT:
                /* ---------------------------------------------------- */
                {
                    /* BC Low Priority Queue init */
                    status = bcLpqInit(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_HPQ_INIT:
                /* ---------------------------------------------------- */
                {
                    /* BC High Priority Queue init */
                    status = bcHpqInit(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_REPLAY_INIT:
                /* ---------------------------------------------------- */
                {
                    /* initialize replay interrupt status queue index */
                    DDC_ISR_LOCK_TAKE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond,
                                      pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

                    /* Flag replay interrupts as enabled */
                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.bReplayIsrEnabled = TRUE;

                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQHead = 0;
                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQTail = 0;
                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u16IrqEventCond = 0;

                    DDC_ISR_LOCK_GIVE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond,
                                      pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

                    /* Replay initialize */
                    status = bcReplayInit(pDeviceContext, pvoidData);
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_BC_MUX_INIT:
                /* ---------------------------------------------------- */
                {
                    /* Channel Mux init */
                    status = bcMuxInit(pDeviceContext, pvoidData);
                    break;
                }


                /* ==================================================== */
                /*                  ARINC CONFIGURATION                 */
                /* ==================================================== */

                /* ---------------------------------------------------- */
                case ACEX_ARINC_429_PROGRAMABLE:
                /* ---------------------------------------------------- */
                {
                    status = ARINC429ProgrammableConfig(pDeviceContext, (PARINC_429_PROGRMMABLE_CONFIG)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "aring429ProgrammableConfig ERROR: status=%d\n", status);
                    }
                    else
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "aring429ProgrammableConfig Passed\n");
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_ARINC_717_CONFIG:
                /* ---------------------------------------------------- */
                {
                    status = arinc717ProgrammableOpen(pDeviceContext, (PARINC_717_PROGRMMABLE_CONFIG)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "arinc717ProgrammableOpen failed \n");
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_ARINC_717_FREE:
                /* ---------------------------------------------------- */
                {
                    status = arinc717ProgrammableClose(pDeviceContext, (PARINC_717_PROGRMMABLE_CONFIG)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "arinc717ProgrammableClose failed \n");
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_ARINC_717_STATE:
                /* ---------------------------------------------------- */
                {
                    status = arinc717ProgrammableSetState(pDeviceContext, (PARINC_717_PROGRMMABLE_CONFIG)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "arinc717ProgrammableSetState failed \n");
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_ARINC_717_INTERRUPTS:
                /* ---------------------------------------------------- */
                {
                    status = arinc717Interrupts(pDeviceContext, (PARINC_717_PROGRMMABLE_CONFIG)pvoidData, (U32BIT *)pIoctlParams->sIoctlInfo.pOutBuffer, FALSE);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "arinc717ProgrammableIntSet failed \n");
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_CAN_BUS_CONFIG:
                /* ---------------------------------------------------- */
                {
                    status = canBusOpen(pDeviceContext, (CAN_BUS_CONFIG *)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "DdcSfpUsbEvtIoDeviceControl: canBusOpen failed \n");
                    }

                    bytesReturned = 0;
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_CAN_BUS_FREE:
                /* ---------------------------------------------------- */
                {
                    status = canBusClose(pDeviceContext, (CAN_BUS_CONFIG *)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "DdcSfpUsbEvtIoDeviceControl: canBusClose failed \n");
                    }

                    bytesReturned = 0;
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_CAN_BUS_STATE:
                /* ---------------------------------------------------- */
                {
                    status = canBusSetState(pDeviceContext, (CAN_BUS_CONFIG *)pvoidData);

                    if (status)
                    {
                        DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                            "DdcSfpUsbEvtIoDeviceControl: canBusSetState failed \n");
                    }

                    bytesReturned = 0;
                    break;
                }

                /* ---------------------------------------------------- */
                case ACEX_CAN_FIRWARE_VERSION:
                /* ---------------------------------------------------- */
                {
                    OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;

                    status = DDC_BLK_MEM_READ_IOCTL( pDeviceContext,
                        pDeviceContext->sCanBus.u32FirmwareVersionAddress,
                        &pDeviceContext->sCanBus.u32FirmwareVersion,
                        OutputBufferLength,
                        DDC_USB_ACC_TYPE_WAIT);

                    if (status != DDC_UDL_ERROR__SUCCESS)
                    {
                        break;
                    }

                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        "DdcSfpUsbEvtIoDeviceControl: canBusSetState fw VERSION %x u32FirmwareVersionAddress %x status %x \n",
                        pDeviceContext->sCanBus.u32FirmwareVersion,
                        pDeviceContext->sCanBus.u32FirmwareVersionAddress,
                        status);

                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer),
                        (char *)&pDeviceContext->sCanBus.u32FirmwareVersion, OutputBufferLength);

                    break;
                }

                /* ==================================================== */
                /*              INVALID DEVICE REQUEST                  */
                /* ==================================================== */

                /* ------------------------------------------------------------ */
                default:
                /* ------------------------------------------------------------ */
                {
                    status = DDC_UDL_ERROR__INVALID_DEVICE_REQUEST;

                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        "Invalid IOCTL_DDCSFP_1553_CONFIGURE Request: %d\n", ((ACEX_CONFIG_ID *)pvoidData)->u16Type);

                    break;
                }
            }

            DDC_KERNEL_FREE(pvoidData);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BOARD_COMPONENT_RESET:
        /* ---------------------------------------------------- */
        {
            ddcUdlBdReset(pDeviceContext, REG_BD_RESET_MODULE, pIoctlParams->Param1);
            break;
        }

        /* ==================================================== */
        /*                   REG  REG  REG  REG                 */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_REG_READ32:
        case IOCTL_REG_READ:
        /* ---------------------------------------------------- */
        {
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_REG_READ\n");

            status = DDC_REG_READ(pDeviceContext, pIoctlParams->Param1, &u32Data);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_REG_READ ERROR: status=%d\n", status);

                return status;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_REG_WRITE32:
        case IOCTL_REG_WRITE:
        /* ---------------------------------------------------- */
        {
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_REG_WRITE\n");

            status = DDC_REG_WRITE(pDeviceContext, pIoctlParams->Param1, (U32BIT *) &(pIoctlParams->Param2));
            break;
        }


        /* ---------------------------------------------------- */
        case IOCTL_REG_READ_BLK:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            OutputBufferLength = pIoctlParams->Param3;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            /* convert relative to absolute address for every case except for default */
            switch (pIoctlParams->Param2)
            {
                /* ---------------------------------------------------- */
                case REG_TYPE_BD_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_BC_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sBC.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sBC.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_MRT_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sRT.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sRT.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_MT_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sMT.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sMT.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_1553_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_EI_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sErrorInj.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sErrorInj.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_REPLAY_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sReplay.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sReplay.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_TRG_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sTrigger.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sTrigger.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                default:
                /* ---------------------------------------------------- */
                {
                    /* absolute address passed */
                    i = pIoctlParams->Param1;
                    break;
                }
            }

            if (status)
            {
                break;
            }

            status = DDC_BLK_REG_READ(pDeviceContext, i, pRdData, (OutputBufferLength / 4));

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_REG_READ_BLK Addr: 0x%08x, Val=0x%08x length: 0x%08x \n", i, *pRdData, OutputBufferLength);


            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_REG_READ_BLK ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                return status;
            }

            bytesReturned = OutputBufferLength;
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_REG_WRITE_BLK:
        /* ---------------------------------------------------- */
        {
            pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param3); /* pIoctlParams->Param3 is number of bytes to write */

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param3);

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            /* convert relative to absolute address for every case except for default */
            switch (pIoctlParams->Param2)     /* pIoctlParams->Param2 is the type of register */ /* //todo - make var */
            {
                /* ---------------------------------------------------- */
                case REG_TYPE_BD_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_BC_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sBC.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sBC.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_MRT_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sRT.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sRT.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_MT_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sMT.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sMT.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_1553_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_EI_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sErrorInj.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sErrorInj.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_REPLAY_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sReplay.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sReplay.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                case REG_TYPE_TRG_OFFSET:
                /* ---------------------------------------------------- */
                {
                    if (!pDeviceContext->pChannel1553[pIoctlParams->Channel]->sTrigger.pu32RegBA)
                    {
                        status = DDC_UDL_ERROR__NULL_PTR;
                        i = 0;
                        break;
                    }

                    i = (*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sTrigger.pu32RegBA) + pIoctlParams->Param1);
                    break;
                }

                /* ---------------------------------------------------- */
                default:
                /* ---------------------------------------------------- */
                {
                    /* absolute address passed */
                    i = pIoctlParams->Param1;
                    break;
                }
            }

            if (status)
            {
                break;
            }

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_REG_WRITE_BLK addr 0x%08x Val 0x%08x length: 0x%08lx\n", i, *(U32BIT *)pvoidData, pIoctlParams->Param3);

            status = DDC_BLK_REG_WRITE(pDeviceContext, i, pvoidData, (pIoctlParams->Param3 / 4));
            DDC_KERNEL_FREE(pvoidData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_REG_WRITE_BLK ERROR: status=%d\n", status);
            }

            break;
        }


        /* ==================================================== */
        /*                   MEM  MEM  MEM  MEM                 */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_MEM_READ:
        /* ---------------------------------------------------- */
        {
            status = DDC_MEM_READ(pDeviceContext, pIoctlParams->Param1, &u32Data, ACEX_32_BIT_ACCESS);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_MEM_READ ERROR: status=%d\n", status);

                return status;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MEM_WRITE:
        /* ---------------------------------------------------- */
        {
            status = DDC_MEM_WRITE(pDeviceContext, pIoctlParams->Param1, (U32BIT *) &(pIoctlParams->Param2), ACEX_32_BIT_ACCESS);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MEM_READ_BLK:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            OutputBufferLength = pIoctlParams->Param3; /* byte */
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            if (pIoctlParams->Param2 == IOCTL_16_BIT_ACCESS)
            {
                /* 16-bit read */

                status = DDC_16BIT_BLK_MEM_READ(pDeviceContext,
                    pIoctlParams->Param1, /* in 16-bit */
                    (U16BIT *)pRdData,
                    (OutputBufferLength / 2)); /* bytes to 16-bit */

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                    "IOCTL_MEM_READ_BLK 16 BIT: Addr=0x%08x Data=0x%04x Cnt=0x%08x\n",
                    (unsigned int)pIoctlParams->Param1, (U16BIT)(*pRdData), OutputBufferLength / 2);
            }
            else
            {
                /* 32-bit read */

                status = DDC_BLK_MEM_READ(pDeviceContext,
                    pIoctlParams->Param1, /* in 32-bit */
                    pRdData,
                    (OutputBufferLength / 4),
                    pIoctlParams->Param2); /* bytes to 32-bit */

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                    "IOCTL_MEM_READ_BLK 32 BIT: Addr=0x%08x Data=0x%08x Cnt=0x%08x \n",
                    (unsigned int)pIoctlParams->Param1, (unsigned int)(*pRdData), OutputBufferLength / 4);
            }

            bytesReturned = OutputBufferLength;
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
/* //todo */
            DDC_KERNEL_FREE(pRdData);
            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MEM_WRITE_BLK:
        /* ---------------------------------------------------- */
        {
            pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param3);

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param3);

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            if (pIoctlParams->Param2 == IOCTL_16_BIT_ACCESS)
            {
                /* 16-bit write */

                status = DDC_16BIT_BLK_MEM_WRITE(pDeviceContext,
                    pIoctlParams->Param1, /* in 16-bit */
                    (U16BIT *)pvoidData,
                    (pIoctlParams->Param3 / 2)); /* bytes to 16-bit */

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                    "IOCTL_MEM_WRITE_BLK 16 BIT: Addr=0x%08x Data=0x%04x Cnt=0x%08lx\n",
                    (unsigned int)pIoctlParams->Param1, *(U16BIT *)pvoidData, pIoctlParams->Param3 / 2);
            }
            else
            {
                /* 32-bit write */

                status = DDC_BLK_MEM_WRITE(pDeviceContext,
                    pIoctlParams->Param1, /* in 32-bit */
                    pvoidData,
                    (pIoctlParams->Param3 / 4),
                    pIoctlParams->Param2); /* bytes to 32-bit */

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                    "IOCTL_MEM_WRITE_BLK 32 BIT: Addr=0x%08x Data=0x%08x Cnt=0x%08lx\n",
                    (unsigned int)pIoctlParams->Param1, *(unsigned int *)pvoidData, pIoctlParams->Param3 / 4);
            }

            DDC_KERNEL_FREE(pvoidData);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BLK_READ:
            /* cmd:   BLK_REG_READ
              desc:  return dump of 32-bit registers starting at register offset identified in Param1.
              notes: it is assumed buffer length is on 32bit boundary..if not data may be truncated */
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            OutputBufferLength = pIoctlParams->Param3;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_BLK_WRITE ERROR: DDC_UDL_ERROR__INSUFFICIENT_MEMORY\n");

                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            if (pIoctlParams->Param2 == DDC_REGISTER_ACCESS)
            {
                status = DDC_BLK_REG_READ(pDeviceContext, pIoctlParams->Param1, pRdData, (OutputBufferLength / 4));
            }
            else    /* assume memory access */
            {
                status = DDC_BLK_MEM_READ(pDeviceContext, pIoctlParams->Param1, pRdData, (OutputBufferLength / 4), ACEX_32_BIT_ACCESS);
            }

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_BLK_READ ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                return status;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, OutputBufferLength);

            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BLK_WRITE:
        /* ---------------------------------------------------- */
        /* cmd:   BLK_WRITE                   desc:            */
        {
            pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param3);

            if (!pvoidData)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_BLK_WRITE ERROR: DDC_UDL_ERROR__INSUFFICIENT_MEMORY\n");

                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param3);

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            if (pIoctlParams->Param2 == DDC_REGISTER_ACCESS)
            {
                status = DDC_BLK_REG_WRITE(pDeviceContext, pIoctlParams->Param1, pvoidData, (pIoctlParams->Param3 / 4));
            }
            else
            {
                status = DDC_BLK_MEM_WRITE(pDeviceContext, pIoctlParams->Param1, pvoidData, (pIoctlParams->Param3 / 4), ACEX_32_BIT_ACCESS);
            }

            DDC_KERNEL_FREE(pvoidData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_BLK_WRITE ERROR: status=%d\n", status);
            }

            break;
        }


        /* ---------------------------------------------------- */
        case IOCTL_CLEAR_MEMORY:
        /* ---------------------------------------------------- */
        {
            status = ddcUdlBdClearMemory(pDeviceContext, pIoctlParams->Param1, pIoctlParams->Param2);
            break;
        }


        /* ==================================================== */
        /*           FLASH   FLASH   FLASH   FLASH   FLASH      */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_FLASH_MEM_INFO:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            bytesReturned = sizeof(FLASH_INFO);
            pRdData = DDC_KERNEL_MALLOC(bytesReturned);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            /* return Start Address, Sector Count & Min Sector Size */
            ((FLASH_INFO *)pRdData)->StartAddress = pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32FlashStartAddr;
            ((FLASH_INFO *)pRdData)->SectorCount = pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32FlashNumClusters;
            ((FLASH_INFO *)pRdData)->MinSectorSize = pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32FlashClusterSize;

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_FLASH_MEM_WRITE_PROTECTED:
        /* ---------------------------------------------------- */
        {
            u32Data = flashMemWriteProtected(pDeviceContext);

            bytesReturned = sizeof(U32BIT);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, bytesReturned);

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_FLASH_MEM_BLOCK_ERASE:
        /* ---------------------------------------------------- */
        {
            status = flashMemBlkErase(pDeviceContext, pIoctlParams);

            if (status)
            {
                return status;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_FLASH_MEM_READ:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* allocate pRdData large enough to contain data*/
            pRdData = DDC_KERNEL_MALLOC(pIoctlParams->Param2);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = flashMemRead(pDeviceContext, pIoctlParams, pRdData);

            if (status)
            {
                DDC_KERNEL_FREE(pRdData);
                return DDC_UDL_ERROR__READ;
            }

            bytesReturned = pIoctlParams->Param2;

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_FLASH_MEM_WRITE:
        /* ---------------------------------------------------- */
        {
            /* allocate pvoidData large enough to contain data*/
            pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param2);

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER(pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param2);

            if (status)
            {
                /* free malloc'd buffer*/
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            status = flashMemWrite(pDeviceContext, pIoctlParams, pvoidData);

            /* free malloc'd buffer*/
            DDC_KERNEL_FREE(pvoidData);

            if (status)
            {
                return status;
            }

            break;
        }

        /* ==================================================== */
        /*               IRIG  IRIG  IRIG  IRIG  IRIG           */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_IRIG_GET_TX:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            if (pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32Capability & BD_CAPABILITIES_TX_IRIG_B)
            {
                pRdData = DDC_KERNEL_MALLOC(sizeof(IRIG_TX));

                if (!pRdData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                irigbGetIRIGTx(pDeviceContext, (IRIG_TX *)pRdData);

                bytesReturned = sizeof(IRIG_TX);
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                DDC_KERNEL_FREE(pRdData);

                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;
                }
            }
            else
            {
                bytesReturned = 0;
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_IRIG_SET_TX:
        /* ---------------------------------------------------- */
        {
            if (pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32Capability & BD_CAPABILITIES_TX_IRIG_B)
            {
                pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param2);

                if (!pvoidData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->Param1), pIoctlParams->Param2);

                if (status)
                {
                    DDC_KERNEL_FREE(pvoidData);
                    return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
                }

                irigbSetIRIGTx(pDeviceContext, (IRIG_TX *)pvoidData);
                DDC_KERNEL_FREE(pvoidData);
                bytesReturned = 0;
                status = DDC_UDL_ERROR__SUCCESS;
            }
            else
            {
                bytesReturned = 0;
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_IRIG_SET_ID:
        /* ---------------------------------------------------- */
        {
            pDeviceContext->u16IrigPacketChannelId[pIoctlParams->Channel] = (U16BIT)pIoctlParams->Param1;
            bytesReturned = 0;
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_IRIG_SET_INT_STATE:
        /* ---------------------------------------------------- */
        {
            status = irigInterruptSet(pDeviceContext, pIoctlParams);
            break;
        }


        /* ==================================================== */
        /*                       TIME TAG                       */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_SET_TT_RESOLUTION:
        /* ---------------------------------------------------- */
        {
            status = gen1553SetTimeTagResolution(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_TT_ROLLOVER_POINT:
        /* ---------------------------------------------------- */
        {
            status = gen1553SetTimeTagRolloverPoint(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_TT_INTERRUPT:
        /* ---------------------------------------------------- */
        {
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_SET_TT_INTERRUPT\n");

            u32IntMask = 1; /* Enable Timttag Rollover Interrupt */

            if (pIoctlParams->Param1 == 1)
            {
                status = gen1553InterruptSet(pDeviceContext, (U8BIT)pIoctlParams->Channel, u32IntMask);
            }
            else
            {
                status = gen1553InterruptClear(pDeviceContext, (U8BIT)pIoctlParams->Channel, u32IntMask);
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_EXT_TT_CNT_CTRL:
        /* ---------------------------------------------------- */
        {
            status = gen1553ExtTTCntCtrl(pDeviceContext, pIoctlParams);
            break;
        }


        /* ---------------------------------------------------- */
        case IOCTL_DIO_TT_CFG:
        /* ---------------------------------------------------- */
        {
            status = ddcUdlDioTtCfg(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_DIO_TT_CTL:
        /* ---------------------------------------------------- */
        {
            status = ddcUdlDioTtCtl(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_DIO_TT_READ:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* //todo */
            status = ddcUdlDioTtRead(pDeviceContext, pRdData, OutputBufferLength);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_DIO_TT_BLOCK_ON_IRQ:
        /* ---------------------------------------------------- */
        {
            /* See if interrupts are enabled */
            if (pDeviceContext->sDioTt.bIsrEnabled == FALSE)
            {
                /* Return error indicating interrupts are not enabled */
                status = DDC_UDL_ERROR__ISR_NOT_ENABLED;

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_DIO_TT_BLOCK_ON_IRQ EXIT 1 <==== \n");

                break;
            }

            DDC_ISR_LOCK_TAKE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);

            /* decrement the event count */
            if (pDeviceContext->sDioTt.u32IntQLen > 0)
            {
                pDeviceContext->sDioTt.u32IntQLen--;
            }

            /* wait if there is no IRQ status in the queue */
            if (pDeviceContext->sDioTt.u32IntQLen == 0)
            {
                DDC_ISR_LOCK_GIVE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);

                DDC_WAIT_INTERRUPTABLE(
                    pDeviceContext->sDioTt.waitqueueIrqEvent,
                    pDeviceContext->sDioTt.u32IntQLen);

                DDC_ISR_LOCK_TAKE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);
            }

            if (pDeviceContext->sDioTt.u32IntQLen)
            {
                DDC_ISR_LOCK_GIVE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);

                status = DDC_COPY_TO_USER(
                    (char*)(pIoctlParams->sIoctlInfo.pOutBuffer),
                    &pDeviceContext->sDioTt.sIntStatus[pDeviceContext->sDioTt.u32IntQTail],
                    sizeof(pDeviceContext->sDioTt.sIntStatus[0])); /* just use the size of the first item */

                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;
                }

                /* clear out the status */
                pDeviceContext->sDioTt.sIntStatus[pDeviceContext->sDioTt.u32IntQTail] = 0x00000000;

                DDC_ISR_LOCK_TAKE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);

                pDeviceContext->sDioTt.u32IntQTail++;

                if (pDeviceContext->sDioTt.u32IntQTail >= (DIOTT_IRQ_STATUS_QUEUE_SIZE - 1))
                {
                    pDeviceContext->sDioTt.u32IntQTail = 0;
                }
            }

            DDC_ISR_LOCK_GIVE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_DIO_TT_RELEASE_IRQ:
        /* ---------------------------------------------------- */
        {   /* Cancel pending DIO TT interrupt request */

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_DIO_TT_RELEASE_IRQ: Wake up DIO TT Ch %d IRQ\n", u8ChannelNumber);

            /* Flag DIO TT interrupt processing as disabled */
            pDeviceContext->sDioTt.bIsrEnabled = FALSE;

            /* Wake up the blocked replay thread so it can terminate */
            DDC_ISR_LOCK_TAKE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);
            pDeviceContext->sDioTt.u32IntQLen = 1;

            /* initialize interrupt status queue index */
            pDeviceContext->sDioTt.u32IntQHead = 0;
            pDeviceContext->sDioTt.u32IntQTail = 0;

            DDC_ISR_LOCK_GIVE(pDeviceContext->sDioTt.semIrqEventCond, pDeviceContext->sDioTt.semIrqEventCondFlag);

            DDC_WAKE_UP_INTERRUPTIBLE(&pDeviceContext->sDioTt.waitqueueIrqEvent);

            break;
        }


        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_WAIT:
        /* ---------------------------------------------------- */
        {
            ddcUdlOsWaitMs((S32BIT)pIoctlParams->Param1);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_DDC_DEBUG:
        /* ---------------------------------------------------- */
        {
            /*
                Param1  Module
                Param2  Mask
                Param3  Operation: 0=clear  1=set
            */

            if (pIoctlParams->Param3 == 1)
            {
                ddcUdlDebugSetTraceLevel(pIoctlParams->Param1, pIoctlParams->Param2);
            }
            else
            {
                ddcUdlDebugClearTraceLevel(pIoctlParams->Param1, pIoctlParams->Param2);
            }
            break;
        }


        /* ========================================================================== */
        /* I/O & Serial Group                                                         */
        /* ========================================================================== */

        /* ==================================================== */
        /*                AIO  AIO  AIO  AIO  AIO               */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_GET_AIO_OUTPUT:
        /* ---------------------------------------------------- */
        {
            u32Data = GetAioOutput(pDeviceContext, pIoctlParams);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_AIO_DIRECTION:
        /* ---------------------------------------------------- */
        {
            u32Data = GetAioDirection(pDeviceContext, pIoctlParams);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_AIO_INPUT:
        /* ---------------------------------------------------- */
        {
            u32Data = GetAioInput(pDeviceContext, pIoctlParams);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_AIO_ALL:
        /* ---------------------------------------------------- */
        {
            u32Data = GetAioAll(pDeviceContext);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_AIO_OUTPUT:
        /* ---------------------------------------------------- */
        {
            SetAioOutput(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_AIO_DIRECTION:
        /* ---------------------------------------------------- */
        {
            SetAioDirection(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_AIO_ALL:
        /* ---------------------------------------------------- */
        {
            SetAioAll(pDeviceContext, pIoctlParams->Param1);
            break;
        }


        /* ==================================================== */
        /*                DIO  DIO  DIO  DIO  DIO               */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_GET_DIO_OUTPUT:
        /* ---------------------------------------------------- */
        {
            u32Data = GetDioOutput(pDeviceContext, pIoctlParams->Param1);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            bytesReturned = sizeof(U32BIT);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_DIO_DIRECTION:
        /* ---------------------------------------------------- */
        {
            u32Data = GetDioDirection(pDeviceContext, pIoctlParams->Param1);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            bytesReturned = sizeof(U32BIT);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_DIO_INPUT:
        /* ---------------------------------------------------- */
        {
            u32Data = GetDioInput(pDeviceContext, pIoctlParams->Param1);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            bytesReturned = sizeof(U32BIT);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_GET_DIO_ALL:
        /* ---------------------------------------------------- */
        {
            u32Data = GetDioAll(pDeviceContext);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, sizeof(U32BIT));

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            bytesReturned = sizeof(U32BIT);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_DIO_OUTPUT:
        /* ---------------------------------------------------- */
        {
            SetDioOutput(pDeviceContext,
                pIoctlParams->Param1, /* discrete */
                pIoctlParams->Param2); /* level */

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_DIO_DIRECTION:
        /* ---------------------------------------------------- */
        {
            SetDioDirection(pDeviceContext,
                pIoctlParams->Param1, /* discrete */
                pIoctlParams->Param2); /* level */

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_SET_DIO_ALL:
        /* ---------------------------------------------------- */
        {
            SetDioAll(pDeviceContext, pIoctlParams->Param1);
            break;
        }

        /* ==================================================== */
        /*                CAN  CAN  CAN  CAN  CAN               */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_CAN_BUS_TX_DATA:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* Load CAN Bus transmit data */

            /* allocate pvoidData large enough to contain data*/
            pRdData = DDC_KERNEL_MALLOC(pIoctlParams->sIoctlInfo.u32OutBufferSize);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER(pRdData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->sIoctlInfo.u32OutBufferSize);

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_CAN_BUS_TX_DATA status=0x%08x\n", status);

            if (status)
            {
                /* free malloc'd buffer */
                DDC_KERNEL_FREE(pRdData);
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            status = canBusTransmitData(pDeviceContext, pIoctlParams, pRdData);

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_CAN_BUS_TX_DATA status2 =0x%08x\n", status);

            /* free malloc'd buffer */
            DDC_KERNEL_FREE(pRdData);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_CAN_BUS_RX_DATA:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* Retrieve CAN Bus data */

            /*          OutputBufferLength = pIoctlParams->Param1 * CAN_BUS_DWORDS_PER_MSG * 4; */
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;  /* byte */

            /* allocate pRdData large enough to contain data*/
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            /* status = canBusReadData(pDeviceContext, pIoctlParams, pRdData,(size_t *)&OutputBufferLength); */
            status = canBusReadData(pDeviceContext, pIoctlParams, pRdData, &bytesReturned);
            if (status != DDC_UDL_ERROR__SUCCESS)
            {
                DDC_KERNEL_FREE(pRdData);
                return status;
            }

            /* If no data returned, must ensure 1st 2 DWORDS are copied.
             * 1st DWORD is message control & 2nd is # of messages */
            if (bytesReturned == 0)
            {
                bytesReturned = 8;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

            DDC_KERNEL_FREE(pRdData);
            break;
        }

        /* ========================================================================== */
        /* 1553 Group                                                                 */
        /* ========================================================================== */

        /* ==================================================== */
        /*              1553 IRQ  1553 IRQ  1553 IRQ            */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_1553_CONFIGURE_INTERRUPT_CONDITIONS:
        /* ---------------------------------------------------- */
        {
            DDC_IOCTL_PARAMS *pIoConfigureOutput;
            
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize; /* in bytes */

            /* allocate pRdData large enough to contain data*/
            pIoConfigureOutput = (DDC_IOCTL_PARAMS *)DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pIoConfigureOutput)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }
            
            status = ddcUdlBdConfigureIoInterruptConditions(
                pDeviceContext,
                pIoctlParams->Channel,  /* 1553 Channel */
                pIoctlParams->Param1,   /* Command */
                pIoctlParams->Param2,   /* Rising Edge Enable/Disable */
                pIoctlParams->Param3,   /* Falling Edge Enable/Disable */
                pIoctlParams->Param4,   /* Reserved */
                pIoConfigureOutput
            );

            if (pIoctlParams->Param1 == ACEX_AIO_INTERRUPT_CONFIGURE__GET)
            {
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pIoConfigureOutput, OutputBufferLength);
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_ENABLE_IRQ:
        /* ---------------------------------------------------- */
        {
            /* initialize interrupt status queue index */
            DDC_ISR_LOCK_TAKE(pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCondFlag);
            pDeviceContext->pChannel1553[u8ChannelNumber]->u32IrqStatusQHead = 0;
            pDeviceContext->pChannel1553[u8ChannelNumber]->u32IrqStatusQTail = 0;
            pDeviceContext->pChannel1553[u8ChannelNumber]->u16IrqEventCond = 0;
            pDeviceContext->pChannel1553[u8ChannelNumber]->u16BlockOnIrqReadyEventCond = 0;
            DDC_ISR_LOCK_GIVE(pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCondFlag);

            status = irqEnableInterrupt(pDeviceContext, u8ChannelNumber, u8CardNumber);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_DISABLE_IRQ:
        /* ---------------------------------------------------- */
        {
            /* Cancel pending 1553 interrupt request */
            status = irqDisableInterrupt(pDeviceContext, u8ChannelNumber, u8CardNumber);

            /* initialize interrupt status queue index */
            DDC_ISR_LOCK_TAKE(pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCondFlag);
            pDeviceContext->pChannel1553[u8ChannelNumber]->u32IrqStatusQHead = 0;
            pDeviceContext->pChannel1553[u8ChannelNumber]->u32IrqStatusQTail = 0;
            DDC_ISR_LOCK_GIVE(pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->semIrqEventCondFlag);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_BLOCK_ON_IRQ:
        /* ---------------------------------------------------- */
        {
            struct _ACEX_1553_CHANNEL_TYPE *p1553Ch = pDeviceContext->pChannel1553[u8ChannelNumber];

            /* See if interrupts are enabled */
            DDC_ISR_LOCK_TAKE(pDeviceContext->semDeviceOpenEventCond, pDeviceContext->semDeviceOpenEventCondFlag);

            if (p1553Ch->bIsr1553Enabled == FALSE)
            {
                DDC_ISR_LOCK_GIVE(pDeviceContext->semDeviceOpenEventCond, pDeviceContext->semDeviceOpenEventCondFlag);

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_1553_BLOCK_ON_IRQ failed: interrupts not enabled\n");

                /* Return error indicating interrupts are not enabled */
                status = DDC_UDL_ERROR__ISR_NOT_ENABLED;
                break;
            }

            DDC_ISR_LOCK_GIVE(pDeviceContext->semDeviceOpenEventCond, pDeviceContext->semDeviceOpenEventCondFlag);

            /* Sleep and wait for the local ISR or ISR clean-up to wake us up and proceed */
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "Wait for IRQ on channel %d\n", u8ChannelNumber);

            /* decrement the event count */
            DDC_ISR_LOCK_TAKE(p1553Ch->semIrqEventCond, p1553Ch->semIrqEventCondFlag);

            if (p1553Ch->u16IrqEventCond > 0)
            {
                p1553Ch->u16IrqEventCond--;
            }

            if (p1553Ch->u16BlockOnIrqReadyEventCond == 0)
            {
                /* let the library know that the IOCTL_1553_BLOCK_ON_IRQ call is about to block */
                p1553Ch->u16BlockOnIrqReadyEventCond++;
                DDC_ISR_LOCK_GIVE(p1553Ch->semIrqEventCond, p1553Ch->semIrqEventCondFlag);

                DDC_WAKE_UP_INTERRUPTIBLE(&p1553Ch->waitqueueBlockOnIrqReadyEvent);

                DDC_ISR_LOCK_TAKE(p1553Ch->semIrqEventCond, p1553Ch->semIrqEventCondFlag);
            }

            /* wait if there is no IRQ status in the queue */
            if (p1553Ch->u16IrqEventCond == 0)
            {
                DDC_ISR_LOCK_GIVE(p1553Ch->semIrqEventCond, p1553Ch->semIrqEventCondFlag);

                DDC_WAIT_INTERRUPTABLE(p1553Ch->waitqueueIrqEvent, p1553Ch->u16IrqEventCond);
            }
            else
            {
                DDC_ISR_LOCK_GIVE(p1553Ch->semIrqEventCond, p1553Ch->semIrqEventCondFlag);
            }

            status = DDC_COPY_TO_USER((char*)(pIoctlParams->sIoctlInfo.pOutBuffer),
                &p1553Ch->pu32IrqStatusQ[p1553Ch->u32IrqStatusQTail],
                sizeof(p1553Ch->pu32IrqStatusQ[0]));

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            /* point to the next entry of the IRQ status queue */
            DDC_ISR_LOCK_TAKE(p1553Ch->semIrqEventCond, p1553Ch->semIrqEventCondFlag);
            if (p1553Ch->u16IrqEventCond)
            {
                p1553Ch->u32IrqStatusQTail++;

                if (p1553Ch->u32IrqStatusQTail == (IRQ_STATUS_QUEUE_SIZE - 1))
                {
                    p1553Ch->u32IrqStatusQTail = 0;
                }
            }
            DDC_ISR_LOCK_GIVE(p1553Ch->semIrqEventCond, p1553Ch->semIrqEventCondFlag);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_BLOCK_ON_IRQ_READY:
        /* ---------------------------------------------------- */
        {
            int nResult = 0;

            nResult = DDC_WAIT_INTERRUPTABLE_TIMEOUT_MS(
                pDeviceContext->pChannel1553[u8ChannelNumber]->waitqueueBlockOnIrqReadyEvent,
                pDeviceContext->pChannel1553[u8ChannelNumber]->u16BlockOnIrqReadyEventCond,
                500);

            if (nResult == 0)
            {
                /* timeout error */
                status = DDC_UDL_ERROR__TIMEOUT;
                break;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_SET_MODULE_INTERRUPT_STATE:
        /* ---------------------------------------------------- */
        {
            if (pIoctlParams->Param1)
            {
                gen1553InterruptSet(pDeviceContext, (U8BIT)pIoctlParams->Channel, pIoctlParams->Param2);
            }
            else
            {
                gen1553InterruptClear(pDeviceContext, (U8BIT)pIoctlParams->Channel, pIoctlParams->Param2);
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_SET_IRQ_CONDITIONS:
        /* ---------------------------------------------------- */
        {
            gen1553SetIRQ(pDeviceContext, pIoctlParams->Param1, (U16BIT)pIoctlParams->Param2, (U8BIT)pIoctlParams->Channel);
            break;
        }


        /* ==================================================== */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_1553_SET_RAM_PARITY_CHECKING:
        /* ---------------------------------------------------- */
        {
            status = gen1553SetRamParityChecking(pDeviceContext, pIoctlParams);
            break;
        }


        /* ---------------------------------------------------- */
        case IOCTL_1553_IMP_READ:
        /* ---------------------------------------------------- */
        {
            /* general IMPROVEMENTS Read Control */

            switch (pIoctlParams->Param1)    /* IMPROVEMENT COMMANDS */
            {
                /* ---------------------------------------------------- */
                case IOCTL_IMP_MRT_CMD:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    /* Param2: (((U32BIT)RtAddrNumCmds<<16) | (U32BIT)RtSAddr) */

                    OutputBufferLength = pIoctlParams->Param3;

                    pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

                    if (!pRdData)
                    {
                        break;
                    }

                    status = mrtImpReadCmd(pDeviceContext,
                        (U16BIT)pIoctlParams->Channel,
                        (pIoctlParams->Param2 >> 16) & 0x0000ffff,
                        OutputBufferLength,
                        &bytesReturned,
                        pRdData);

                    if (status)
                    {
                        DDC_KERNEL_FREE(pRdData);
                        break;
                    }

                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                        "bytesReturned: %d\n", (int)bytesReturned);


                    if (bytesReturned > 0)
                    {
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                    }

                    DDC_KERNEL_FREE(pRdData);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    if (u8IsCompatCall)
                    {
                        u32BytesReturned = bytesReturned;
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &u32BytesReturned, sizeof(U32BIT));
                    }
                    else
                    {
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &bytesReturned, sizeof(size_t));
                    }

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case IOCTL_IMP_MRT_DATA:
                /* ---------------------------------------------------- */
                {
                    U32BIT *pRdData = NULL;

                    /* Param2: (((U32BIT)RtAddrNumCmds<<16) | (U32BIT)RtSAddr) */

                    OutputBufferLength = pIoctlParams->Param3;

                    pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

                    if (!pRdData)
                    {
                        break;
                    }

                    status = mrtImpReadData(pDeviceContext,
                        (U16BIT)pIoctlParams->Channel,
                        (U16BIT)((pIoctlParams->Param2 >> 16) && 0x0000ffff),
                        (U16BIT)(pIoctlParams->Param2 && 0x0000ffff),
                        OutputBufferLength,
                        &bytesReturned,
                        pRdData);

                    if (status)
                    {
                        DDC_KERNEL_FREE(pRdData);
                        break;
                    }

                    if (bytesReturned > 0)
                    {
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                    }

                    DDC_KERNEL_FREE(pRdData);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    if (u8IsCompatCall)
                    {
                        u32BytesReturned = bytesReturned;
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &u32BytesReturned, sizeof(U32BIT));
                    }
                    else
                    {
                        status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &bytesReturned, sizeof(size_t));
                    }

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    break;
                }

                /* ---------------------------------------------------- */
                case IOCTL_IMP_BC_CMD:
                /* ---------------------------------------------------- */
                {
                    break;
                }

                /* ---------------------------------------------------- */
                case IOCTL_IMP_BC_DATA:
                /* ---------------------------------------------------- */
                {
                    break;
                }

                /* ---------------------------------------------------- */
                default:
                /* ---------------------------------------------------- */
                {
                    break;
                }

            }    /* switch on pIoctlParams->Param1 */

            break;

        } /* IOCTL_IMP_READ */

        /* ---------------------------------------------------- */
        case IOCTL_1553_SET_RESP_TIME_OUT:
        /* ---------------------------------------------------- */
        {
            /* Param1 - Module, Param2 - TimeOut, Param3 - RT Addr */
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL SET RESP TIME OUT CALLED: Channe %ld, Mode 0x%lX, %ld - 0.5us multiples\n",
                pIoctlParams->Channel, pIoctlParams->Param1, pIoctlParams->Param2);

            /* Determine Device's mode of opertion */
            if ((pIoctlParams->Param1 & ACEX_RESP_MTI))
            {
                status = mtSetRespTimeOut(pDeviceContext, pIoctlParams);
            }

            if ((status == DDC_UDL_ERROR__SUCCESS) && (pIoctlParams->Param1 & ACEX_RESP_MRT))
            {
                status = rtSetNoRespTimeOut(pDeviceContext, (U16BIT)pIoctlParams->Channel, (U16BIT)pIoctlParams->Param3, pIoctlParams->Param2);
            }

            if ((status == DDC_UDL_ERROR__SUCCESS) && (pIoctlParams->Param1 & ACEX_RESP_BC))
            {
                status = bcSetRespTimeOut(pDeviceContext, pIoctlParams);
            }

            break;
        }

        /* ==================================================== */
        /*             1553 COUPLING  1553 COUPLING             */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_1553_GET_COUPLING:
        /* ---------------------------------------------------- */
        {
            /* check to see if this device supports this feature */
            if (pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32Capability & BD_CAPABILITIES_BUS_COUPLING)
            {
                U32BIT *pRdData = NULL;

                pRdData = DDC_KERNEL_MALLOC(sizeof(ACEX_COUPLING));

                if (!pRdData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                gen1553GetCoupling(pDeviceContext, pIoctlParams, (ACEX_COUPLING *)pRdData);

                bytesReturned = sizeof(ACEX_COUPLING);
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

                DDC_KERNEL_FREE(pRdData);

                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;
                }
            }
            else
            {
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_SET_COUPLING:
        /* ---------------------------------------------------- */
        {
            /* check to see if this device supports this feature */
            if (pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32Capability & BD_CAPABILITIES_BUS_COUPLING)
            {
                gen1553SetCoupling(pDeviceContext, pIoctlParams);
            }
            else
            {
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ==================================================== */
        /*             1553 AMPLITUDE  1553 AMPLITUDE           */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_1553_GET_AMPLITUDE:
        /* ---------------------------------------------------- */
        {
            /* check to see if this device supports this feature */
            if (pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32Capability & BD_CAPABILITIES_VARIABLE_XCVR)
            {
                gen1553GetAmplitude(pDeviceContext, pIoctlParams, &u32Data);

                bytesReturned = sizeof(U32BIT);
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)&u32Data, bytesReturned);
                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;
                }
            }
            else
            {
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_SET_AMPLITUDE:
        /* ---------------------------------------------------- */
        {
            /* check to see if this device supports this feature */
            if (pDeviceContext->pBoardInfo[ACEX_BD_INSTANCE_0]->sBdReg.u32Capability & BD_CAPABILITIES_VARIABLE_XCVR)
            {
                gen1553SetAmplitude(pDeviceContext, pIoctlParams);
            }
            else
            {
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ==================================================== */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_1553_CHECK_MF_CAPABLE:
        /* ---------------------------------------------------- */
        {
            /* return information if the channel support MF concurrency */
            status = gen1553CheckMfCapable(pDeviceContext, pIoctlParams, &bytesReturned, &u32Data);

            bytesReturned = sizeof(U32BIT);
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)&u32Data, bytesReturned);
            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }
            break;
        }


        /* ==================================================== */
        /*               1553 TRIGGER  1553 TRIGGER             */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_1553_TRG_RESET:
        /* ---------------------------------------------------- */
        {
            /* reset trigger */
            status = trigger1553Reset(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_1553_TRG_IRQ_STATUS:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* check the data array status */
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            /* read trigger IRQ status */
            status = trigger1553StatusRead(pDeviceContext, pIoctlParams, &bytesReturned, pRdData);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ========================================================================== */
        /* 1553 BC Group                                                              */
        /* ========================================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_BC_GET_ASYNC_COUNT:
        /* ---------------------------------------------------- */
        {
            /* get async msg count*/
            status = bcGetLpAsyncMsgCount(pDeviceContext, pIoctlParams, &bytesReturned, &u32Data);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcGetLpAsyncMsgCount ERROR: status=%d\n", status);

                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32Data, bytesReturned);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GET_DATA_BUFFER:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* get msg data*/
            OutputBufferLength = pIoctlParams->Param4;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = bcGetData(pDeviceContext, pIoctlParams, &bytesReturned, (U16BIT*)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcGetData ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GET_FRAME_TO_HBUF:
        /* ---------------------------------------------------- */
        {
            /* get data from IMP to HBuf, triggered by user */
            bcGetFrameToHBuf(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GET_HBUF_MESSAGE:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* Read BC HBuf messages.
               Do not return error code! */
            OutputBufferLength = pIoctlParams->Param3 * sizeof(U16BIT);
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                break;
            }

            status = bcGetHBufMsg(pDeviceContext, pIoctlParams, &bytesReturned, (U16BIT*)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcGetHBufMsg ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                status = DDC_UDL_ERROR__SUCCESS;
                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);
            if (u8IsCompatCall)
            {
                u32BytesReturned = bytesReturned;
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &u32BytesReturned, sizeof(U32BIT));
            }
            else
            {
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param4), (char *) &bytesReturned, sizeof(size_t));
            }
            status = DDC_UDL_ERROR__SUCCESS;

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GET_METRICS:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* read BC GPQ/HBuf metric information */
            OutputBufferLength = pIoctlParams->Param2;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = bcGetMetric(pDeviceContext, pIoctlParams, &bytesReturned, (U16BIT*)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcGetMetric ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GET_MESSAGE:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* Get BC message with given message ID */
            /* Do not return error code!*/
            OutputBufferLength = pIoctlParams->Param4;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__SUCCESS /*DDC_UDL_ERROR__INSUFFICIENT_MEMORY*/; /* Windows require not return error code */
            }

            status = bcGetMsg(pDeviceContext, pIoctlParams, &bytesReturned, (U16BIT*)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcGetMsg ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                return DDC_UDL_ERROR__SUCCESS /*status*/;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__SUCCESS /*DDC_UDL_ERROR__COPY2USER_FAIL*/;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GPQ_CLEAR:
        /* ---------------------------------------------------- */
        {
            /* clear GPQ information*/
            status = bcGpqClear(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GPQ_COUNT:
        /* ---------------------------------------------------- */
        {
            /* get GPQ count*/
            status = bcGpqGetCount(pDeviceContext, pIoctlParams, &bytesReturned, &u16Data);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcGpqGetCount ERROR: status=%d\n", status);

                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u16Data, bytesReturned);

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                " ACEX_MRT_CONFIG\n");

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GPQ_READ:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* read GPQ entry */
            OutputBufferLength = pIoctlParams->Param2; /* sizeof(GPQ_ENTRY) */
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = bcGpqRead(pDeviceContext, pIoctlParams, &bytesReturned, (U16BIT*)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcGetData ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_POST_ASYNC_MESSAGE:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "bcPostAsyncMsg\n");

            pRdData = DDC_KERNEL_MALLOC(sizeof(ACEX_BC_ASYNC_STATUS));

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = bcPostAsyncMsg(pDeviceContext, pIoctlParams, &bytesReturned, pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcPostAsyncMsg ERROR: status=%d\n", status);

                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, sizeof(ACEX_BC_ASYNC_STATUS));
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_SET_MSG_BUFFER:
        /* ---------------------------------------------------- */
        {
            /* set BC message buffer */
            if ((pIoctlParams->Param2 != 0) && ((U32BIT *)pIoctlParams->sIoctlInfo.pOutBuffer != NULL))
            {
                pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param2 * sizeof(U32BIT)); /* pIoctlParams->Param2 is number of dword */

                if (!pvoidData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param2 * sizeof(U32BIT));

                if (status)
                {
                    DDC_KERNEL_FREE(pvoidData);
                    return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
                }

                status = bcSetMsgBuffer(pDeviceContext, pIoctlParams, pvoidData);
                DDC_KERNEL_FREE(pvoidData);
            }
            else
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                    "IOCTL_BC_SET_MSG_BUFFER No Data\n");
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_SET_DATA_BUFFER:
        /* ---------------------------------------------------- */
        {
            /* set BC data buffer*/
            if ((pIoctlParams->Param2 != 0) && ((U16BIT *)pIoctlParams->sIoctlInfo.pOutBuffer != NULL))
            {
                pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param2 * sizeof(U16BIT)); /* pIoctlParams->Param2 is number of word */

                if (!pvoidData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param2 * sizeof(U16BIT));

                if (status)
                {
                    DDC_KERNEL_FREE(pvoidData);
                    return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
                }

                status = bcSeDataBuffer(pDeviceContext, pIoctlParams, pvoidData);

                DDC_KERNEL_FREE(pvoidData);
            }
            else
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                    "IOCTL_BC_SET_DATA_BUFFER No Data\n");
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_STATE:
        /* ---------------------------------------------------- */
        {
            /* set state */
            status = bcSetState(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_DATASTR_INIT:
        /* ---------------------------------------------------- */
        {
            /* start data stream */
            status = bcDataStreamInit(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_DATASTR_STATUS:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* check if the data stream is completed */
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize; /* sizeof(ACEX_BC_DATASTR_STATUS) */
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = bcDataStreamCheckCompletion(pDeviceContext, pIoctlParams, &bytesReturned, pRdData);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_DATA_ARRAY_INIT:
        /* ---------------------------------------------------- */
        {
            /* start data array */
            status = bcDataArrayInit(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_DATA_ARRAY_STATUS:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* check the data array status */
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize; /* sizeof(ACEX_BC_DATA_ARRAY_STATUS) */
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = bcDataArrayCheckStatus(pDeviceContext, pIoctlParams, &bytesReturned, pRdData);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_GET_EXT_TRIGGER_AVAIL:
        /* ---------------------------------------------------- */
        {
            /* return external trigger availability information*/
            status = bcExtTriggerAvailInfo(pDeviceContext, &bytesReturned, &u32Data);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcExtTriggerAvailInfo ERROR: status=%d\n", status);

                break;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)&u32Data, bytesReturned);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

           break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_ASYNC_QUEUE_INFO:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize; /* sizeof(ACEX_BC_ASYNC_QUEUE_INFO) */
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = bcGetAsyncQueueInfo(pDeviceContext, pIoctlParams, &bytesReturned, pRdData);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_CMD_STACK_READ:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* Get BC message stack with given message address*/
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__SUCCESS /*DDC_UDL_ERROR__INSUFFICIENT_MEMORY*/; /* Windows require not return error code */
            }

            status = bcCmdStackRead(pDeviceContext, pIoctlParams, &bytesReturned, (U16BIT*)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcCmdStackRead ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                return DDC_UDL_ERROR__SUCCESS /*status*/;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__SUCCESS /*DDC_UDL_ERROR__COPY2USER_FAIL*/;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_DATA_STACK_READ:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* Get BC message data with given message data block address*/
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__SUCCESS /*DDC_UDL_ERROR__INSUFFICIENT_MEMORY*/; /* Windows require not return error code */
            }

            status = bcDataStackRead(pDeviceContext, pIoctlParams, &bytesReturned, (U16BIT*)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "bcDataStackRead ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                return DDC_UDL_ERROR__SUCCESS /*status*/;
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__SUCCESS /*DDC_UDL_ERROR__COPY2USER_FAIL*/;
            }

            break;
        }


        /* ==================================================== */
        /*            BC REPLAY  BC REPLAY  BC REPLAY           */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_BC_REPLAY_STATUS:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* check the data array status */
            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            /* check the data array status */
            status = bcReplayStatus(pDeviceContext, pIoctlParams, &bytesReturned, pRdData);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_REPLAY_BLOCK_ON_IRQ:
        /* ---------------------------------------------------- */
        {
            /* See if replay interrupt is enabled */
            DDC_ISR_LOCK_TAKE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);
            if (!pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.bReplayIsrEnabled)
            {
                DDC_ISR_LOCK_GIVE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_BLOCK_ON_IRQ failed: replay interrupts not enabled\n");

                /* Return error indicating interrupts are not enabled */
                status = DDC_UDL_ERROR__ISR_NOT_ENABLED;
                break;
            }

            DDC_ISR_LOCK_GIVE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

            /* Sleep and wait for the local ISR or ISR clean-up to wake us up and proceed. */
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1, "Waits for Replay IRQ at channel %d\n", u8ChannelNumber);
            DDC_WAIT_INTERRUPTABLE(
                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.waitqueueIrqEvent,
                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u16IrqEventCond);


            DDC_ISR_LOCK_TAKE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

            if (pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u16IrqEventCond)
            {
                status = DDC_COPY_TO_USER((char*)(pIoctlParams->sIoctlInfo.pOutBuffer),
                    &pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.pu32IrqStatusQ[pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQTail],
                    sizeof(U32BIT));

                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;

                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        "Replay: copy data error\n");
                }

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                    "Wake up Replay IRQ at channel %d, IrqStatus = %08X\n", u8ChannelNumber,
                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.pu32IrqStatusQ[pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQTail]);

                pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u16IrqEventCond--;

                /* point to the next entry of the IRQ status queue */
                pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQTail++;

                if (pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQTail == (REPLAY_IRQ_STATUS_QUEUE_SIZE - 1))
                {
                    pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQTail = 0;
                }

            }
            else
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                        "IOCTL_BLOCK_ON_IRQ: no data\n");
                u32Data = 0;
                status = DDC_COPY_TO_USER((char*)(pIoctlParams->sIoctlInfo.pOutBuffer),
                    &u32Data,
                    sizeof(U32BIT));
            }
            DDC_ISR_LOCK_GIVE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_REPLAY_RELEASE_IRQ:
        /* ---------------------------------------------------- */
        {
            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_BC_REPLAY_RELEASE_IRQt: Wake up replay ch %d irq\n", u8ChannelNumber);

            /* Wake up the blocked replay thread so it can terminate */
            DDC_ISR_LOCK_TAKE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

            /* Flag replay interrupt processing as disabled */
            pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.bReplayIsrEnabled = FALSE;

            pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u16IrqEventCond = 1;

            /* initialize replay interrupt status queue index */
            pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQHead = 0;
            pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.u32IrqStatusQTail = 0;

            DDC_ISR_LOCK_GIVE(pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCond, pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.semIrqEventCondFlag);

            DDC_WAKE_UP_INTERRUPTIBLE(&pDeviceContext->pChannel1553[u8ChannelNumber]->sReplay.waitqueueIrqEvent);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_REPLAY_DMA:
        /* ---------------------------------------------------- */
        {
            /*BC_REPLAY_DATA_IOCTL_TYPE *pBcReplayDataIoctl = (BC_REPLAY_DATA_IOCTL_TYPE *)pIoctlParams;*/

            pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param3);

            if (!pvoidData)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_BC_REPLAY_DMA: DDC_KERNEL_MALLOC failed\n");

                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param3);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_BC_REPLAY_DMA: DDC_COPY_FROM_USER failed\n");

                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            /* start replay dma */
            status = bcReplayDma(pDeviceContext, pIoctlParams, (U32BIT *)pvoidData);

            DDC_KERNEL_FREE(pvoidData);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_REPLAY_RAW:
        /* ---------------------------------------------------- */
        {
            /* start replay dma */
            status = bcReplayRaw(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_BC_REPLAY_WRITE:
        /* ---------------------------------------------------- */
        {
            /*BC_REPLAY_DATA_IOCTL_TYPE *pBcReplayDataIoctl = (BC_REPLAY_DATA_IOCTL_TYPE *)pIoctlParams;*/

            pvoidData = DDC_KERNEL_MALLOC(pIoctlParams->Param3);

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->Param3);

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            /* write replay data */
            status = bcReplayWrite(pDeviceContext, pIoctlParams, (U32BIT *)pvoidData);

            DDC_KERNEL_FREE(pvoidData);
            break;
        }


        /* ========================================================================== */
        /* DDC_IOCTL_GROUP_1553_MT                                                    */
        /* ========================================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_MT_SET_STATE:
        /* ---------------------------------------------------- */
        {
            status = mtSetMtiState(pDeviceContext, pIoctlParams);
            bytesReturned = 0;
            break;
        }


        /* ---------------------------------------------------- */
        case IOCTL_MT_SET_STROBE_REG:
        /* ---------------------------------------------------- */
        {
            status = mtSetStrobeReg(pDeviceContext, pIoctlParams);
            bytesReturned = 0;
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MT_INTERUPT_CONFIG:
        /* ---------------------------------------------------- */
        {
            pvoidData = DDC_KERNEL_MALLOC(sizeof(MT_MTI_CONFIG));

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->Param1), sizeof(MT_MTI_CONFIG));

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            status = mtMtiInterruptConfig(pDeviceContext, (MT_MTI_CONFIG *)pvoidData);
            DDC_KERNEL_FREE(pvoidData);

            if (status)
            {
            /* //todo - correct return val? */
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MT_GET_METRICS_CONFIG:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            pRdData = DDC_KERNEL_MALLOC(sizeof(MT_MTI_METRICS));

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = mtMtiGetMetrics(pDeviceContext, pIoctlParams, (MT_MTI_METRICS *)pRdData);
            bytesReturned = sizeof(MT_MTI_METRICS);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }
        }

        /* ---------------------------------------------------- */
        case IOCTL_MT_FREE_MEM_COUNT:
        /* ---------------------------------------------------- */
        {
            DDC_REG_WRITE(pDeviceContext,
                ((*(pDeviceContext->pChannel1553[pIoctlParams->Channel]->sMT.pu32RegBA)) + REG_MT_MTI_FREE_MEM_COUNT_RW),
                (U32BIT *) &pIoctlParams->Param1);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MTI_GET_CH10_STATUS:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            pRdData = DDC_KERNEL_MALLOC(sizeof(MT_MTI_INFO));

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = mtGetMtiCh10Status(pDeviceContext, pIoctlParams, (MT_MTI_INFO *)pRdData);

            if (status)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "mtGetMtiCh10Status ERROR: status=%d\n", status);

                DDC_KERNEL_FREE(pRdData);
                return status;
            }

            bytesReturned = sizeof(MT_MTI_INFO);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MTI_GET_CH10_DATA_PKT:
        /* ---------------------------------------------------- */
        {
            ACEX_MTI_CH10_DATA_PKT *pPkt;
            struct _ACEX_1553_MT_TYPE *pMT;
            U32BIT *pRdData = NULL;

            pMT = &(pDeviceContext->pChannel1553[u8ChannelNumber]->sMT);

            status = mtGetMtiCh10DataPkt(pDeviceContext, pIoctlParams);

            if (status)
            {
                if (status < DDC_UDL_ERROR__SUCCESS)
                {
                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        "mtGetMtiCh10DataPkt ERROR: status=%d\n", status);
                }

                return status;
            }

            DDC_ISR_LOCK_TAKE(pMT->semMtiDataPkt, pMT->semMtiDataPktFlag);
            pRdData = (U32BIT *)(pMT->pMtiDataListEntry[pMT->u32MtiDataPktTail]->pDataPkt);
            DDC_ISR_LOCK_GIVE(pMT->semMtiDataPkt, pMT->semMtiDataPktFlag);

            pPkt = (ACEX_MTI_CH10_DATA_PKT *)pRdData;
            bytesReturned = pPkt->u32PktLength;

            /* copy the packet header first */
            pUserBuf = (char *)(pIoctlParams->sIoctlInfo.pOutBuffer);
            pBuf = (char *)pRdData;
            u32DataLen = (MTI_CH10_PKT_HEADER_SIZE + MTI_CH_SPEC_DATA_HDR_SIZE);
            status = DDC_COPY_TO_USER(pUserBuf, pBuf, u32DataLen);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }
            else
            {
                /* copy the packet data */
                pUserBuf += u32DataLen;

                DDC_ISR_LOCK_TAKE(pMT->semMtiDataPkt, pMT->semMtiDataPktFlag);
                pBuf = (char *)GET_ACEX_MTI_CH10_PKT_MSG_DATA_POINTER(((char *)pMT->pMtiDataListEntry[pMT->u32MtiDataPktTail]->pDataPkt));

                /* data is shifted by copyOffset bytes */
                pBuf += (pMT->pMtiDataListEntry[pMT->u32MtiDataPktTail]->copyOffset);
                DDC_ISR_LOCK_GIVE(pMT->semMtiDataPkt, pMT->semMtiDataPktFlag);

                u32DataLen = bytesReturned - u32DataLen;
                status = DDC_COPY_TO_USER(pUserBuf, pBuf, u32DataLen);
                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;
                }
            }

            mtFreeDataPktListTail(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_MTI_GET_CH10_TIME_PKT:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            status = mtGetMtiCh10TimePkt(pDeviceContext, pIoctlParams);

            if (status)
            {
                if (status < DDC_UDL_ERROR__SUCCESS)
                {
                   DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        "mtGetMtiCh10TimePkt ERROR: status=%d\n", status);
                }

                return status;
            }

            pRdData = (U32BIT *) &pDeviceContext->pChannel1553[u8ChannelNumber]->sMT.pMtiTimeListEntry->sMtiTimePkt;
            bytesReturned = sizeof(MTI_CH10_TIME_PKT);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ========================================================================== */
        /* 1553 RT Group                                                              */
        /* ========================================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_RT_DATA_SEND_STREAM:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            pvoidData = DDC_KERNEL_MALLOC(sizeof(MRT_STREAM_TYPE));

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->Param1), sizeof(MRT_STREAM_TYPE));

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            pRdData = DDC_KERNEL_MALLOC(pIoctlParams->sIoctlInfo.u32OutBufferSize);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pRdData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->sIoctlInfo.u32OutBufferSize);

            if (status)
            {
                DDC_KERNEL_FREE(pRdData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            rtSendStream(pDeviceContext, (MRT_STREAM_TYPE *)pvoidData, (void *)pRdData, (U32BIT *) &bytesReturned);
            DDC_KERNEL_FREE(pvoidData);
            DDC_KERNEL_FREE(pRdData);

            if (u8IsCompatCall)
            {
                u32BytesReturned = bytesReturned;
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param3), (char *) &u32BytesReturned, sizeof(U32BIT));
            }
            else
            {
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param3), (char *) &bytesReturned, sizeof(size_t));
            }

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_RT_DATA_RECEIVE_STREAM:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            pvoidData = DDC_KERNEL_MALLOC(sizeof(MRT_STREAM_TYPE));

            if (!pvoidData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->Param1), sizeof(MRT_STREAM_TYPE));

            if (status)
            {
                DDC_KERNEL_FREE(pvoidData);
                return DDC_UDL_ERROR__COPYFROMUSER_FAIL;
            }

            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            (((MRT_STREAM_TYPE *)pvoidData)->pRequest) = (void *)pRdData;
            rtReceiveStream(pDeviceContext, (MRT_STREAM_TYPE *)pvoidData, (U32BIT *) &bytesReturned);
            DDC_KERNEL_FREE(pvoidData);

            if (u8IsCompatCall)
            {
                u32BytesReturned = bytesReturned;
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param3), (char *) &u32BytesReturned, sizeof(U32BIT));
            }
            else
            {
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->Param3), (char *) &bytesReturned, sizeof(size_t));
            }

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }
            else if (bytesReturned > 0)
            {
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;
                }
            }

            DDC_KERNEL_FREE(pRdData);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_RT_READ_MODE_CODE_DATA:
        /* ---------------------------------------------------- */
        {
            u16Data = mrtReadModeCodeData(pDeviceContext, u8ChannelNumber, pIoctlParams->Param1, pIoctlParams->Param2);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), &u16Data, sizeof(U16BIT));

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_RT_WRITE_MODE_CODE_DATA:
        /* ---------------------------------------------------- */
        {
            mrtWriteModeCodeData(pDeviceContext, u8ChannelNumber, pIoctlParams->Param1, pIoctlParams->Param2, pIoctlParams->Param3);
            break;
        }

        /* ========================================================================== */
        /* DDC_IOCTL_GROUP_ARINC                                                      */
        /* ========================================================================== */


        /* ==================================================== */
        /*           ARINC 429  ARINC 429  ARINC 429            */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_ENABLE_IRQ:
        /* ---------------------------------------------------- */
        {
            /* initialize 429 interrupt status queue index */
            DDC_ISR_LOCK_TAKE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);
            pDeviceContext->u32ArincIntQHead = 0;
            pDeviceContext->u32ArincIntQTail = 0;
            pDeviceContext->u32ArincIntQLen = 0;
            pDeviceContext->u16ArincBlockOnIrqReadyEventCond = 0;
            DDC_ISR_LOCK_GIVE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);

            status = irqEnableInterrupt429(pDeviceContext, u8CardNumber);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_DISABLE_IRQ:
        /* ---------------------------------------------------- */
        {
            ArincDisableIrq(pDeviceContext, u8CardNumber);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_BLOCK_ON_IRQ:
        /* ---------------------------------------------------- */
        {
            /* See if interrupts are enabled */
            if (!pDeviceContext->bArincSerialIsrEnabled)
            {
                /* Return error indicating interrupts are not enabled */
                status = DDC_UDL_ERROR__ISR_NOT_ENABLED;

                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "IOCTL_ARINC_BLOCK_ON_IRQ EXIT 1 <==== \n");

                break;
            }

            DDC_ISR_LOCK_TAKE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);

            /* decrement the event count */
            if (pDeviceContext->u32ArincIntQLen > 0)
            {
                pDeviceContext->u32ArincIntQLen--;
            }

            if (pDeviceContext->u16ArincBlockOnIrqReadyEventCond == 0)
            {
                /* let the library know that the IOCTL_ARINC_BLOCK_ON_IRQ call is about to block */
                pDeviceContext->u16ArincBlockOnIrqReadyEventCond++;
                DDC_WAKE_UP_INTERRUPTIBLE(&pDeviceContext->waitqueueArincBlockOnIrqReadyEvent);
            }

            /* wait if there is no IRQ status in the queue */
            if (pDeviceContext->u32ArincIntQLen == 0)
            {
                DDC_ISR_LOCK_GIVE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);

                DDC_WAIT_INTERRUPTABLE(
                    pDeviceContext->waitqueueArincBlockOnIrqEvent,
                    pDeviceContext->u32ArincIntQLen);

                DDC_ISR_LOCK_TAKE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);
            }

            if (pDeviceContext->u32ArincIntQLen)
            {
                DDC_ISR_LOCK_GIVE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);

                status = DDC_COPY_TO_USER((char*)(pIoctlParams->sIoctlInfo.pOutBuffer), &pDeviceContext->sArincIntStatus[pDeviceContext->u32ArincIntQTail], sizeof(ARINC_INTERRUPT));

                if (status)
                {
                    status = DDC_UDL_ERROR__COPY2USER_FAIL;
                }

                /* clear out the status */
                pDeviceContext->sArincIntStatus[pDeviceContext->u32ArincIntQTail].u32MasterStatus = 0x00000000;

                DDC_ISR_LOCK_TAKE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);

                pDeviceContext->u32ArincIntQTail++;

                if (pDeviceContext->u32ArincIntQTail >= (ARINC_IRQ_STATUS_QUEUE_SIZE - 1))
                {
                    pDeviceContext->u32ArincIntQTail = 0;
                }
            }

            DDC_ISR_LOCK_GIVE(pDeviceContext->semArincIrqEventCond, pDeviceContext->semArincEventCondFlag);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_BLOCK_ON_IRQ_READY:
        /* ---------------------------------------------------- */
        {
            int nResult = 0;

            nResult = DDC_WAIT_INTERRUPTABLE_TIMEOUT_MS(
                pDeviceContext->waitqueueArincBlockOnIrqReadyEvent,
                pDeviceContext->u16ArincBlockOnIrqReadyEventCond,
                500);

            if (nResult == 0)
            {
                /* timeout error */
                status = DDC_UDL_ERROR__TIMEOUT;
                break;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_RELEASE_IRQ:
        /* ---------------------------------------------------- */
        {
            /* //todo */
            break;
        }


        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_LOADTXONE:
        /* ---------------------------------------------------- */
        {
            ARINC429LoadTxQueueOne(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_LOADTXMORE:
        /* ---------------------------------------------------- */
        {
            ARINC429LoadTxQueueMore(pDeviceContext,
                pIoctlParams,
                (U32BIT *)pIoctlParams->sIoctlInfo.pOutBuffer,
                pIoctlParams->sIoctlInfo.u32OutBufferSize);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_GET_TT_CONFIG:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            bytesReturned = sizeof(DDC_IOCTL_PARAMS);

            pRdData = DDC_KERNEL_MALLOC(bytesReturned);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            ARINC429GetTimeStampConfig(pDeviceContext, (DDC_IOCTL_PARAMS *)pRdData);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_SET_TT_CONFIG:
        /* ---------------------------------------------------- */
        {
            ARINC429SetTimeStampConfig(pDeviceContext, pIoctlParams);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_GET_PGRM_LOOPBACK:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            bytesReturned = sizeof(U32BIT);

            pRdData = DDC_KERNEL_MALLOC(bytesReturned);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            *pRdData = ARINC429GetLoopback(pDeviceContext, (S16BIT)pIoctlParams->Param1);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_SET_PGRM_LOOPBACK:
        /* ---------------------------------------------------- */
        {
            ARINC429SetLoopback(pDeviceContext, (S16BIT)pIoctlParams->Param1, (S16BIT)pIoctlParams->Param2);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_REG_READ:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* only allow legacy devices to call this function */
            if ((pDeviceContext->u16DriverType == ACEX_DD429_PCIE_DRIVER) ||
                (pDeviceContext->u16DriverType == ACEX_DD429_PCI_DRIVER))
            {
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            bytesReturned = sizeof(U32BIT);

            pRdData = DDC_KERNEL_MALLOC(bytesReturned);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            *pRdData = ARINC429RegRead(pDeviceContext, pIoctlParams);

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_REG_WRITE:
        /* ---------------------------------------------------- */
        {
            /* only allow legacy devices to call this function */
            if ((pDeviceContext->u16DriverType == ACEX_DD429_PCIE_DRIVER) ||
                (pDeviceContext->u16DriverType == ACEX_DD429_PCI_DRIVER))
            {
                status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;
            }

            ARINC429RegWrite(pDeviceContext, pIoctlParams);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_MEM_READ:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            bytesReturned = sizeof(U32BIT);

            pRdData = DDC_KERNEL_MALLOC((size_t)bytesReturned);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }
            *pRdData = ARINC429MemRead(pDeviceContext, pIoctlParams);

#if DDC_PPC

            /* 429 memory does not do byte swapping */
            *pRdData = BYTE_ORDER_L(*pRdData);
#endif /* DDC_PPC */

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);

            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                return DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_MEM_READ_MORE:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_ARINC_429_MEM_READ_MORE ENTER ====> \n");

            OutputBufferLength = pIoctlParams->Param3; /* byte */
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = DDC_BLK_MEM_READ(pDeviceContext, pIoctlParams->Param1, pRdData, (OutputBufferLength / 4), ACEX_32_BIT_ACCESS);

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_ARINC_429_MEM_READ_MORE 32 BIT: Addr=0x%08x Data=0x%08x Cnt=0x%08x \n",
                (unsigned int)pIoctlParams->Param1, (unsigned int)(*pRdData), OutputBufferLength / 4);

            bytesReturned = OutputBufferLength;
            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            if (status)
            {
                status = DDC_UDL_ERROR__COPY2USER_FAIL;
            }

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_Level1,
                "IOCTL_ARINC_429_MEM_READ_MORE EXIT <==== \n");

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_MEM_WRITE:
        /* ---------------------------------------------------- */
        {
#if DDC_PPC

            /* 429 memory does not do byte swapping */
            pIoctlParams->Param2 = BYTE_ORDER_L(pIoctlParams->Param2);
#endif /* DDC_PPC */

            ARINC429MemWrite(pDeviceContext, pIoctlParams);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_SET_OLD_LOOPBACK:
        /* ---------------------------------------------------- */
        {
            ARINC429SetLegacyLoopback(pDeviceContext);
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_GET_OLD_LOOPBACK:
        /* ---------------------------------------------------- */
        {
            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_RX_HBUF_CTRL:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            if (pIoctlParams->sIoctlInfo.u32OutBufferSize > 0)
            {
                pRdData = DDC_KERNEL_MALLOC(pIoctlParams->sIoctlInfo.u32OutBufferSize);

                if (!pRdData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }
            }

            status = ARINC429HostBufferControl(pDeviceContext,
                pIoctlParams,
                &bytesReturned,
                (U32BIT*)pRdData);

            if (status)
            {
                DDC_KERNEL_FREE(pRdData);
                return DDC_UDL_ERROR__READ;
            }

            if (pIoctlParams->sIoctlInfo.u32OutBufferSize > 0)
            {
                status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                DDC_KERNEL_FREE(pRdData);

                if (status)
                {
                    return DDC_UDL_ERROR__COPY2USER_FAIL;
                }
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_429_GET_QUEUE_STATUS:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;
            pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

            if (!pRdData)
            {
                return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
            }

            status = ARINC429GetQueueStatus(pDeviceContext,
                pIoctlParams->Channel,
                &bytesReturned,
                (U32BIT*)pRdData);

            if (status != DDC_UDL_ERROR__SUCCESS)
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    "DdcSfpUsbEvtIoDeviceControl: ARINC429GetQueueStatus failed \n");
            }

            status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
            DDC_KERNEL_FREE(pRdData);

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_DD429X_COMMAND:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;

            if (OutputBufferLength)
            {
                pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

                if (!pRdData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                status = ARINC429ProcessCommand(pDeviceContext, pIoctlParams, pRdData, OutputBufferLength, &bytesReturned);

                if (bytesReturned > OutputBufferLength)
                {
                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        " - IOCTL_DD429X_COMMAND - Buffer Too Small - "
                        " OutputBufferLength: %d Bytes   Needed: %d Bytes\n",
                        (int)OutputBufferLength, (int)sizeof(U32BIT));

                    bytesReturned = 0;
                    status = DDC_UDL_ERROR__BUFFER_SIZE;

                    break;
                }

                if (bytesReturned)
                {
                    status = DDC_COPY_TO_USER((void *)pIoctlParams->sIoctlInfo.pOutBuffer, (void *)pRdData, bytesReturned);
                }

                DDC_KERNEL_FREE(pRdData);
            }
            else
            {
                DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                    " - IOCTL_DD429X_COMMAND - ERROR: pIoctlParams->sIoctlInfo.u32OutBufferSize is zero");
            }

            break;
        }


        /* ==================================================== */
        /*           ARINC 717  ARINC 717  ARINC 717            */
        /* ==================================================== */

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_717_LOAD_TX:
        /* ---------------------------------------------------- */
        {
            /* Load transmit data */

            if (pIoctlParams == NULL)
            {
                break;
            }

            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;

            if (OutputBufferLength)
            {
                pvoidData = DDC_KERNEL_MALLOC(OutputBufferLength);

                if (!pvoidData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                status = DDC_COPY_FROM_USER((char *)pvoidData, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), OutputBufferLength);

                if (status)
                {
                    return DDC_UDL_ERROR__COPY2USER_FAIL;
                }

                ARINC717LoadTxQueueData(pDeviceContext, pIoctlParams, (U32BIT *)pvoidData);

                DDC_KERNEL_FREE(pvoidData);
            }

            break;
        }

        /* ---------------------------------------------------- */
        case IOCTL_ARINC_717_GET_RX:
        /* ---------------------------------------------------- */
        {
            U32BIT *pRdData = NULL;

            /* Retrieve 717 data */

            if (pIoctlParams == NULL)
            {
                break;
            }

            OutputBufferLength = pIoctlParams->sIoctlInfo.u32OutBufferSize;

            if (OutputBufferLength)
            {
                pRdData = DDC_KERNEL_MALLOC(OutputBufferLength);

                if (!pRdData)
                {
                    return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                }

                status = ARINC717ReadData(pDeviceContext, pIoctlParams, pRdData, OutputBufferLength, &bytesReturned);

                if (bytesReturned > OutputBufferLength)
                {
                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        " - IOCTL_ARINC_717_GET_RX - Buffer Too Small - "
                        " OutputBufferLength: %d Bytes   Needed: %d Bytes\n",
                        (int)OutputBufferLength, (int)sizeof(U32BIT));

                    bytesReturned = 0;
                    status = DDC_UDL_ERROR__BUFFER_SIZE;
                    break;
                }

                if (bytesReturned)
                {
                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *)pRdData, bytesReturned);
                }
                DDC_KERNEL_FREE(pRdData);
            }
            break;
        }

        /* ------------------------ */
        /* PLX                      */
        /* ------------------------ */
        case IOCTL_PLX_READ:
        {
            U32BIT *pRdData = NULL;

            switch (pIoctlParams->Param1)
            {
                case PLX_READ_REG:
                {
                    /* Param2 = register */
                    U32BIT u32RegVal = 0;

                    DDC_PLX_READ(pDeviceContext, pIoctlParams->Param2, &u32RegVal);

                    status = DDC_COPY_TO_USER((char *)(pIoctlParams->sIoctlInfo.pOutBuffer), (char *) &u32RegVal, sizeof(u32RegVal));

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    bytesReturned = sizeof(U32BIT);

                    break;
                }

                case PLX_READ_EEPROM:
                {
                    size_t i;
                    U32BIT *pu32Data;
                    U32BIT u32Index = 0;

                    pu32Data = (U32BIT *) pRdData;

                    /* Param2 = address */
                    /* pRdData = location to put read data */
                    /* OutputBufferLength = # bytes to read */

                    for (i=0; i < OutputBufferLength; i+=4)
                    {
                        ddcUdlBusPciPlx9000_EepromReadByOffset(pDeviceContext, (U16BIT)(i), &pu32Data[u32Index]);
                        u32Index++;
                    }

                    bytesReturned = OutputBufferLength;
                    break;
                }

                default:
                {
                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        "Invalid IOCTL_PLX_READ Param1 Option (%d)", (int)pIoctlParams->Param1);

                    break;
                }
            }

            break;
        }

        case IOCTL_PLX_WRITE:
        {
            /* <UDL23> */
            switch (pIoctlParams->Param1)
            {
                case PLX_WRITE_REG:
                {
                    /* Param2 = register */
                    /* Param3 = value */
                    U32BIT u32RegVal = pIoctlParams->Param3;

                    DDC_PLX_WRITE(pDeviceContext, pIoctlParams->Param2, u32RegVal);
                    break;
                }

                case PLX_WRITE_EEPROM:
                {
                    size_t i;
                    U32BIT *pu32Data = NULL;
                    U32BIT u32Index = 0;

                    pu32Data = DDC_KERNEL_MALLOC(pIoctlParams->sIoctlInfo.u32OutBufferSize);

                    if (!pu32Data)
                    {
                        return DDC_UDL_ERROR__INSUFFICIENT_MEMORY;
                    }

                    status = DDC_COPY_FROM_USER(pu32Data, (char *)(pIoctlParams->sIoctlInfo.pOutBuffer), pIoctlParams->sIoctlInfo.u32OutBufferSize);

                    if (status)
                    {
                        status = DDC_UDL_ERROR__COPY2USER_FAIL;
                    }

                    /* Param2 = address */
                    /* pRdData = data to write */
                    /* OutputBufferLength = # bytes to write */

                    for (i=0; i < OutputBufferLength; i+=4)
                    {
                        ddcUdlBusPciPlx9000_EepromWriteByOffset(pDeviceContext, (U16BIT)(i), pu32Data[u32Index]);
                        u32Index++;
                    }

                    break;
                }

                default:
                {
                    DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                        "Invalid IOCTL_PLX_WRITE Param1 Option (%d)", (int)pIoctlParams->Param1);

                    break;
                }
            }

            break;
        }

        /* ==================================================================== */
        /* ==================================================================== */
        /*                        INVALID IOCTL VALUE                           */
        /* ==================================================================== */
        /* ==================================================================== */

        /* ---------------------------------------------------- */
        default:
        /* ---------------------------------------------------- */
        {
            status = DDC_UDL_ERROR__IOCTL_NOT_IMPLEMENTED;

            DDC_DBG_PRINT(DDC_DBG_MODULE_IOCTL, DDC_DBG_IOCTL_Ioctl_failures,
                "ERROR: Invalid IOCTL Request - Group: %d  Type: %d\n",
                DDC_IOCTL__GROUP_GET_VAL(u32IoctlCmd),
                DDC_IOCTL__TYPE_GET_VAL(u32IoctlCmd));

            break;
        }
    }

    if (pIoctlParams->sIoctlInfo.pu32BytesReturned != 0)
    {
        U32BIT u32Status;

        U32BIT u32BytesReturned = (U32BIT)bytesReturned;

        u32Status = DDC_COPY_TO_USER((void *)pIoctlParams->sIoctlInfo.pu32BytesReturned, (void *)&u32BytesReturned, sizeof(u32BytesReturned));

        if (u32Status)
        {
            status = DDC_UDL_ERROR__COPY2USER_FAIL;
        }
    }
    return status;
}

