/*******************************************************************************
 * FILE: TODO
 *
 * DESCRIPTION:
 *
 *      TODO
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

#ifndef __DEVICEOP_H__
#define __DEVICEOP_H__

#include <unistd.h>
#include "include/ddc_types.h"

/* Struct Definitions*/
typedef struct _ACE_DRV_CONTEXT
{
    int              hDriverApp;           /* Handle to Linux device */
    U32BIT           u32InterruptQueHead;  /* Head of interrupt pending queue */
    U32BIT           u32InterruptQueTail;  /* Tail of interrupt pending queue */
    DDC_IRQ_STATUS_TYPE *pu32IrqStatus;    /* Array of interrupt status(s) for IQS */
    pthread_t        thDispatch;           /* Interrupt thread (Dispatch) */
    pthread_t        thWorker;             /* Interrupt thread (Worker) */
    pthread_t        thReplay;             /* Interrupt thread (Replay */
    U16BIT           u16ThParam;           /* Parameter for new threads */
    pthread_cond_t   thcQue;               /* Interrupt que condition object */
    pthread_mutex_t  thmQue;               /* Interrupt que mutex */
} ACE_DRV_CONTEXT;

typedef struct _ACE_OSCONTEXT
{
	HANDLE hDriverApp;
	HANDLE hDriverIrq;
	HANDLE hDriverMtiData;
	HANDLE hDriverMtiTime;
	HANDLE hBlockEvent;
	HANDLE hExitEvent;
	HANDLE hThread[2];
	U32BIT dwThreadID[2];
} ACE_OSCONTEXT;

typedef struct _ACE_IOINFO
{
	U16BIT bInitialized; /* Initialization status. */
	ACE_OSCONTEXT *pContext; /* OS specific information. */
	U32BIT dwCardType; /* The card being accessed */
	HANDLE hDriverLib; /* Driver handle for library */
	CHANCOUNT_t strChanCount; /* structure that holds number of IO's. */
}ACE_IOINFO, *PACE_IOINFO;


#define ACE_SLEEP_MS(dwMs) (usleep(dwMs * 1000))

typedef struct _BC_DATA_BLOCK_ADDR
{
	U32BIT	Address;
	U16BIT	OpCode;
} BC_DATA_BLOCK_ADDR, *PBC_DATA_BLOCK_ADDR;


#define PCI_MEM_WIN_LEN 0x20000 /* EMACE PCI Boards Channel RAM window size (128Kb) */
#define PCI_REG_WIN_LEN 0x4000  /* EMACE PCI Boards REG window size (16Kb) */

#define USB_MAX_CARD            8
#define USB_1553_CHANNELS       2


S16BIT _DECL _dvDriverResourceAllocation
(
	S16BIT DevNum
);

S16BIT _DECL _dvDriverResourceRelease
(
	S16BIT DevNum
);
S16BIT _dvLinuxOpenDDCDriver
(
	S16BIT DevNum
);

S16BIT _dvLinuxCloseDDCDriver
(
	S16BIT DevNum
);


S16BIT _dvLinuxInstallIrq
(
    S16BIT DevNum
);

S16BIT _dvLinuxUninstallIrq
(
    S16BIT DevNum
);

U16BIT _DECL _dvRegRead
(
	S16BIT DevNum,
	U16BIT wRegAddr,
	U16BIT wMask
);

VOID _DECL _dvRegWrite
(	S16BIT DevNum,
	U16BIT wRegAddr,
	U16BIT wValue,
	U16BIT wMask
);

U32BIT _DECL _dvRegRead32
(
	S16BIT DevNum,
	U32BIT wRegAddr,
	U32BIT wMask
);

VOID _DECL _dvRegWrite32
(	S16BIT DevNum,
	U32BIT wRegAddr,
	U32BIT wValue,
	U32BIT wMask
);

U16BIT _DECL _dvMemRead
(
	S16BIT	DevNum,
	U32BIT	dwMemAddr
);

S16BIT _DECL _dvMemWrite
(
	S16BIT DevNum,
	U32BIT dwMemAddr,
	U16BIT wValue
);

S16BIT _DECL _dvMemWrite32
(
	S16BIT DevNum,
	U32BIT dwMemAddr,
	U32BIT dwValue
);

VOID _DECL _dvCmdStackRead
(
	S16BIT	DevNum,
	U32BIT	dwMemAddr,
	U16BIT	*pBuffer,
	U16BIT	wLength
);

VOID _DECL _dvDataStackRead
(
	S16BIT	DevNum,
	U32BIT	dwMemAddr,
	U16BIT	*pBuffer,
	U16BIT	wLength,
	U16BIT	wOffset
);

VOID _DECL _dvDataStackWrite
(
	S16BIT	DevNum,
	U32BIT	dwMemAddr,
	U16BIT	*pBuffer,
	U16BIT	wLength,
	U16BIT	wOffset
);

VOID _DECL _dvCmdStackRead32
(
	S16BIT	DevNum,
	U32BIT	dwMemAddr,
	U32BIT	*pBuffer,
	U16BIT	wLength
);

VOID _DECL _dvDataStackRead32
(
	S16BIT	DevNum,
	U32BIT	dwMemAddr,
	U32BIT	*pBuffer,
	U16BIT	wLength,
	U16BIT	wOffset
);

S16BIT _DECL _dvClearMemory
(
	S16BIT DevNum,
	U32BIT dwMemAddr,
	U32BIT dwMemLength
);

VOID _DECL _dvDeviceInit
(
	S16BIT DevNum
);

VOID _DECL _dvGetTimeTagValue
(
	S16BIT	DevNum,
	U64BIT	*ullTTValue
);

VOID _DECL _dvSetTimeTagValue
(
	S16BIT	DevNum,
	U64BIT	ullTTValue
);

U16BIT _DECL _dvGetCommandStkArea
(
	S16BIT DevNum
);

U32BIT _DECL _dvGetBcDataBlockAddr
(
	S16BIT	DevNum,
	U32BIT	XqfAddress,
	U16BIT	*pOpCode
);

S16BIT _DECL _dvGetBcExtTriggerAvail
(
	S16BIT	DevNum,
	U32BIT  *pu32Avail
);

U32BIT _DECL _dvGetMtCommandStkPointer
(
	S16BIT DevNum,
	U32BIT StackLocation
);

VOID _DECL _dvGetMtCommandStkNext
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U16BIT *pBuffer,
	U16BIT Length
);

VOID _DECL _dvGetMtCommandStkNext32
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U32BIT *pBuffer,
	U16BIT Length
);

VOID _DECL _dvGetMtCommandStkLatest
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U16BIT *pBuffer,
	U16BIT Length
);

VOID _DECL _dvGetMtCommandStkData
(
	S16BIT DevNum,
	U32BIT dwAddress,
	U32BIT dwDataStkBase,
	U16BIT *pBuffer,
	U16BIT Length,
	U16BIT RolloverLength
);

VOID _DECL _dvGetMtCommandStkData32
(
	S16BIT DevNum,
	U32BIT dwAddress,
	U32BIT dwDataStkBase,
	U32BIT *pBuffer,
	U16BIT Length,
	U16BIT RolloverLength
);

U16BIT _DECL _dvGetMtCommandStkDataCount
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U32BIT dwDataStkAddr,
	U32BIT dwDataStkAddrNext,
	U16BIT wDataStkSize,
	U16BIT wTotalDataLength
);

U32BIT _DECL _dvGetRtCommandStkPointer
(
	S16BIT DevNum,
	U32BIT StackLocation
);

VOID _DECL _dvGetRtCommandStkNext
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U16BIT *pBuffer,
	U16BIT Length
);

VOID _DECL _dvGetRtCommandStkNext32
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U32BIT *pBuffer,
	U16BIT Length
);

VOID _DECL _dvGetRtCommandStkLatest
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U16BIT *pBuffer,
	U16BIT Length
);

VOID _DECL _dvSetBcMessageBlk
(
	S16BIT	DevNum,
	U32BIT	MsgAddress,
	U16BIT	*pBuffer,
	U16BIT	Length
);

VOID _DECL _dvSetBcMessageDataPtr
(
	S16BIT	DevNum,
	U32BIT	MsgAddress,
	U32BIT	DataBlkPtr
);

VOID _DECL _dvSetRtCommandStkPurge
(
	S16BIT DevNum,
	U32BIT StackLocation,
	U16BIT PurgeData
);

VOID _DECL _dvGetRtCommandStkData
(
	S16BIT DevNum,
	U32BIT dwAddress,
	U32BIT dwDataStkBase,
	U16BIT *pBuffer,
	U16BIT Length,
	U16BIT RolloverLength
);

VOID _DECL _dvGetRtCommandStkData32
(
	S16BIT DevNum,
	U32BIT dwAddress,
	U32BIT dwDataStkBase,
	U32BIT *pBuffer,
	U16BIT Length,
	U16BIT RolloverLength
);

U16BIT _DECL _dvGetRtCommandStkDataCount
(
	S16BIT DevNum,
	U32BIT dwCmdStkAddr,
	U32BIT dwDataStkAddr,
	U32BIT dwDataStkAddrNext,
	U16BIT wDataStkSize,
	U16BIT wTotalDataLength
);

U32BIT _DECL _dvGetRtDataBlkOffset
(
	S16BIT DevNum,
	U16BIT LookupTableAddr,
	U32BIT DataBlkBaseAddr
);

U32BIT _DECL _dvGetRtDataBlkPointer
(
	S16BIT DevNum,
	U16BIT LookupTableAddr
);

U16BIT _DECL _dvGetRtSubAddrCtrlWord
(
	S16BIT DevNum,
	U16BIT SubAddress
);

S16BIT _DECL _dvGetHwVersionInfo
(
	S16BIT			DevNum,
	PHWVERSIONINFO	pHwVersionInfo
);

VOID _DECL _dvSetMtCommandStkPurge
(
	S16BIT DevNum,
	U32BIT StackLocation,
	U16BIT PurgeData
);

VOID _DECL _dvSetMtCommandStkPointer
(
	S16BIT DevNum,
	U32BIT StackLocation,
	U32BIT StackPtr
);

VOID _DECL _dvSetMtDataStkPointer
(
	S16BIT DevNum,
	U32BIT StackLocation,
	U32BIT StackPtr

);

VOID _DECL _dvSetMtCommandStkSize
(
	S16BIT DevNum,
	U16BIT wCmdStkSize,
	U16BIT wMask

);

VOID _DECL _dvSetMtDataStkSize
(
	S16BIT DevNum,
	U16BIT wCmdStkSize,
	U16BIT wMask

);

VOID _DECL _dvSetRtCommandStkPurge
(
	S16BIT DevNum,
	U32BIT StackLocation,
	U16BIT PurgeData
);

VOID _DECL _dvSetRtCommandStkPointer
(
	S16BIT DevNum,
	U32BIT StackLocation,
	U32BIT StackPtr
);

VOID _DECL _dvSetRtDataBlkPointer
(
	S16BIT DevNum,
	U16BIT LookupTableAddr,
	U32BIT DataBlkPtr
);

VOID _DECL _dvSetRtSubAddrCtrlWord
(
	S16BIT DevNum,
	U16BIT SubAddress,
	U16BIT SubAddressCtrlWord
);

VOID _DECL _dvSetRtCommandStkSize
(
	S16BIT DevNum,
	U16BIT wCmdStkSize,
	U16BIT wMask

);

S16BIT _DECL _dvGetMtiCh10DataPkt
(
	S16BIT DevNum,
	PMTI_CH10_DATA_PKT *ppCh10Pkt,
	S16BIT Timeout
);

S16BIT _DECL _dvGetMtiCh10TimePkt
(
	S16BIT DevNum,
	PMTI_CH10_TIME_PKT *ppCh10Pkt,
	S16BIT Timeout
);

S16BIT _DECL _dvSetMtiConfig
(
	S16BIT		DevNum,
	PMTI_CONFIG	MtiConfig
);

VOID _DECL _dvSetMtiState
(
	S16BIT DevNum,
	U16BIT State
);

VOID _DECL _dvSetMtiDmaBufferAddr
(
	S16BIT DevNum,
	U32BIT Address
);

VOID _DECL _dvSetMtiDmaBufferSize
(
	S16BIT DevNum,
	U32BIT Size
);

S16BIT _DECL _dvSetMtiCh10DataPktFree
(
	S16BIT				DevNum,
	PMTI_CH10_DATA_PKT	*ppCh10Pkt
);

S16BIT _DECL _dvSetMtiCh10TimePktFree
(
	S16BIT				DevNum,
	PMTI_CH10_TIME_PKT	*ppCh10Pkt
);

VOID _DECL _dvSetMtiFree
(
	S16BIT DevNum
);

DISC_LEVEL _DECL _dvGetDiscOut
(
	S16BIT	DevNum,
	S16BIT	Discrete
);

S16BIT _DECL _dvGetDiscDir
(
	S16BIT	DevNum,
	S16BIT	Discrete
);

DISC_LEVEL _DECL _dvGetDiscIn
(
	S16BIT	DevNum,
	S16BIT	Discrete
);

S16BIT _DECL _dvSetDiscOut
(
	S16BIT		DevNum,
	S16BIT		Discrete,
	DISC_LEVEL	Level
);

S16BIT _DECL _dvSetDiscDir
(
	S16BIT		DevNum,
	S16BIT		Discrete,
	DISC_DIR	Direction
);

S16BIT _DECL _dvGetDiscAll
(
	S16BIT	DevNum,
	U16BIT	*Directions,
	U16BIT *Levels
);

S16BIT _DECL _dvSetDiscAll
(
	S16BIT	DevNum,
	U16BIT	Directions,
	U16BIT  Levels
);

S16BIT _DECL _dvGetChannelCount
(
	S16BIT DevNum,
	CHANCOUNT_t *pChanCount
);

S16BIT _DECL _dvSetAioDir
(
	S16BIT DevNum,
	S16BIT Channel,
	AVIONIC_DIRECTION Direction
);

S16BIT _DECL _dvSetAioOutput
(
	S16BIT DevNum,
	S16BIT Channel,
	AVIONIC_LEVEL Level
);

AVIONIC_DIRECTION _DECL _dvGetAioDir
(
	S16BIT DevNum,
	S16BIT Channel
);

AVIONIC_LEVEL _DECL _dvGetAioOutput
(
	S16BIT DevNum,
	S16BIT Channel
);

AVIONIC_LEVEL _DECL _dvGetAioInput
(
	S16BIT DevNum,
	S16BIT Channel
);

S16BIT _DECL _dvGetAioAll
(
	S16BIT DevNum,
	U16BIT *Directions,
	U16BIT *Levels
);

S16BIT _DECL _dvSetAioAll
(
	S16BIT DevNum,
	U16BIT Directions,
	U16BIT Levels
);
BOOLEAN _DECL _dvFlashMemBlockErase
(
	S16BIT DevNum,
	U32BIT Address
);

BOOLEAN _DECL _dvFlashMemRead
(
	S16BIT DevNum,
	U32BIT Address,
	U8BIT *pBuffer,
	U32BIT ByteCount
);

BOOLEAN _DECL _dvFlashMemWrite
(
	S16BIT DevNum,
	U32BIT Address,
	U8BIT *pBuffer,
	U32BIT ByteCount
);

BOOLEAN _DECL _dvFlashMemWriteProtected
(
	S16BIT DevNum
);

VOID _DECL _dvFlashMemInfo
(
	S16BIT	DevNum,
	U32BIT	*StartAddress,
	U32BIT	*SectorCount,
	U32BIT	*MinSectorSize
);

U32BIT _DECL _dvGpRegRead
(
	S16BIT	DevNum,
	U32BIT	dwRegAddr
);

VOID _DECL _dvGpRegWrite
(
	S16BIT DevNum,
	U32BIT dwRegAddr,
	U32BIT dwValue,
	U32BIT dwMask
);

S16BIT _DECL _dvIODriverResourceAllocation
(
	S16BIT DevNum
);

S16BIT _DECL _dvIODriverResourceRelease
(
	S16BIT DevNum
);

//-----------------------------------------------------------------------------
//
// OS Specific Host Buffer Thread Synchronization Mutex
//
//-----------------------------------------------------------------------------
VOID _DECL _dvInitializeCriticalSection
(
	S16BIT DevNum
);

VOID _DECL _dvDeleteCriticalSection
(
	S16BIT DevNum
);

VOID _dvEnterCriticalSection
(
	S16BIT DevNum
);

VOID _dvLeaveCriticalSection
(
	S16BIT DevNum
);

VOID _DECL _dvIrqDispatcher
(
    S16BIT *pwDevNum
);

VOID _DECL _dvIrqWorker
(
    S16BIT *pwDevNum
);

void _dvSystemIsr
(
    S16BIT wDevNum,
    U32BIT dwStatus
);

S16BIT _DECL _osFreeIO
(
	S16BIT DevNum
);

S16BIT _DECL _osInitIO
(
	S16BIT DevNum,
	S16BIT wOptions
);

S16BIT _DECL _dvGetIRIGTx
(
    S16BIT			DevNum,
    PACE_IRIG_TX	pstructIRIG
);

S16BIT _DECL _dvSetIRIGTx
(
    S16BIT			DevNum,
    ACE_IRIG_TX	structIRIG
);

S16BIT _DECL _dvMTISetCh10TimePktId
(
    S16BIT DevNum,
    U16BIT u16ChannelId
);

#endif /* __DEVICEOP_H_ */
