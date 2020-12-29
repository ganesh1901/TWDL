/*
 * Revision History for Acex_Trans.H (HEADER FILE)
 *
 *   Release   Date           Description
 */

#ifndef ACEX_TRANS_H
#define ACEX_TRANS_H

/* TL:  Include AceXtreme headers */
#include "stdemace.h"

#if (defined(LINUX)) || (defined(VX_WORKS))
#define MAX_PATH	260   /*match WinDef.h in Windows SDK 7.0 */
#endif

#ifdef LINUX
#include "linuxutil.h"
/*/ #include <unistd.h> */
#endif

/********************************************************************************************************************/
/* TL: ACEXTREME TRANSLATION MACROS AND CONSTANTS *******************************************************************/
/********************************************************************************************************************/
#define BOOL						U8BIT
#define TS_DBLK_STD(x)				x+2					/* Converts Test/Sim Data Table (Starts @ 0) to ACEx Data Block for standard messages */
#define TS_DBLK_MC(x)				x+1025				/* Converts Test/Sim Data Table (Starts @ 0) to ACEx Data Block for Mode Code Messages */
#define TS_DBLK_BCINC(x)			2048+x				/* Converts Test/Sim Data Table (Starts @ 0) to ACEx Data Block for BC Block Data Messages */

#define TS_OP_XEQ(x)				x					/* Standard Msg Execute (XEQ) Offset */
#define TS_OP_MNR_CAL(x)			1025+x				/* Minor Frame Opcode (CAL) Offset */
#define TS_OP_IMR(x)				2050+x				/* Intermessage Routine Opcode (IMR) Offset */
#define TS_OP_RTN(x)				3075+x				/* Return for Intermessage Routine Opcode (RTN) Offset */
#define TS_OP_IMR_CAL(x)			4100+x				/* Call for Intermessage Routine Opcode (CAL) Offset */
#define TS_OP_PSI1(x)				5125+x				/* Call for Intermessage Routine Opcode (CAL) Offset */
#define TS_OP_PSI2(x)				6150+x				/* Call for Intermessage Routine Opcode (CAL) Offset */
#define TS_OP_FRM_IRQ(x)			7175+x				/* Call for Frame IRQ OpCode */

#define TS_FRM_IMR(x)				1025+x				/* IMR Frame (per message) Offeset */
#define TS_FRM_MJR					32767				/* Major Frame ID (can only be one) */

#define TS_MONSTACK_FIXED_SIZE		42					/* Holds 40 words (max message w/o IRIG) plus 2 configuration words */
#define TS_MONSTACK_FIXED_SIZE_IRIG 44					/* Hold 44 words (max message w/IRIG) plus 2 config words */
#define MAX_DIO_USES				16					/* Array size for aDioSettings in EMACEINFO */
#define MAX_LOG_DEV_NUMS			32					/* Array size for aDeviceInfo Structure (Max # LDN's supported ) */
#define STATE_RUN					0					/* State Control (RUN) */
#define STATE_HALT					1					/* State Control (HALT) */
#define STATE_IDLE					2					/* State Control (IDLE) */
#define STATE_STOPPED				3					/* State Control (STOPPED) */

#define DT_CONTEXT_NONE				0					/* Data Table Context is NONE (Not linked to BC or RT Operation) */
#define DT_CONTEXT_BC				1					/* Data Table Context is BC (Used in BC Operation) */
#define DT_CONTEXT_RT				2					/* Data Table Context is RT (Used in RT Operation) */

#define ACEX_BC_HBUF_SIZE			256*1024
#define ACEX_RT_HBUF_SIZE			256*1024

#define RX_MAPPING					0					/* Mapping constant for Receive RT Data Blocks */
#define TX_MAPPING					1					/* Mapping constant for Transmit RT Data Blocks */

#define CH10PKT_TMATS_BUF_SIZE		0x10000						/* TMATS Buffer Size */
#define CH10PKT_BUF_SIZE			0x10000						/* CH10 Data Buffer Size */
#define CH10_TIME_PKT_BUF_SIZE		sizeof(MTI_CH10_TIME_PKT)	/* CH10 Time Buffer Size */
#define MTI_IRIG_TIME_CHNL_ID       0x01						/* MTI IRIG TIME Channel ID */
#define FIRST_TIMEPKT_TIMEOUT       1200						/* First Time Packet Timeout value */

#define BROADCAST_MSG_TYPE			0x8					/* Broadcast Msg Type for MTI->T/S Conversion */
#define MODE_CMD					0					/* MODE Msg Type for MTI->T/S Conversion */
#define RX_CMD						1					/* RX Msg Type for MTI->T/S Conversion */
#define TX_CMD						2					/* TX Msg Type for MTI->T/S Conversion */
#define RTRT_CMD					3					/* RTRT Msg Type for MTI->T/S Conversion */

#define FRM_SYM_SKIP				0x01
#define	FRM_SYM_BREAK				0x02
#define FRM_SYM_EOMINOR				0x04
#define FRM_SYM_EOMAJOR				0x08

/* these are definitions for detected errors in the message. These definitions */
/* should be in the TestSim header files but they are not so we define them here. */
#define TS_MSGERR_WRONG_RT_TX_INVALID_DATA        0x01
#define TS_MSGERR_TX_STATUS_SET_INVALID_DATA      0x02
#define TS_MSGERR_INVALID_DATA                    0x03
#define TS_MSGERR_INVALID_COMMAND                 0x04
#define TS_MSGERR_EXTRA_WORD                      0x0C
#define TS_MSGERR_TX_STATUS_SET_EXTRA_WORD        0x0D
#define TS_MSGERR_WRONG_RT_TX_EXTRA_WORD          0x0E
#define TS_MSGERR_TX_STATUS_SET                   0x11
#define TS_MSGERR_WRONG_RT_TX                     0x12
#define TS_MSGERR_GAP                             0x14
#define TS_MSGERR_TX_STATUS_SET_GAP               0x15
#define TS_MSGERR_WRONG_RT_TX_GAP                 0x16
#define TS_MSGERR_NO_RESP_TX                      0x24
#define TS_MSGERR_WRONG_RT_TX_INVALID_TX_STATUS   0x25
#define TS_MSGERR_TX_STATUS_SET_INVALID_TX_STATUS 0x26
#define TS_MSGERR_INVALID_TX_STATUS               0x27
#define TS_MSGERR_RX_STATUS_SET                   0x31
#define TS_MSGERR_WRONG_RT_RX                     0x32
#define TS_MSGERR_NO_RESP_RX                      0x34
#define TS_MSGERR_TX_STATUS_SET_NO_RESP_RX        0x35
#define TS_MSGERR_WRONG_RT_TX_NO_RESP_RX          0x36
#define TS_MSGERR_INVALID_STATUS_RX               0x38
#define TS_MSGERR_TX_STATUS_SET_INVALID_STATUS_RX 0x39
#define TS_MSGERR_WRONG_RT_TX_INVALID_STATUS_RX   0x3A
#define TS_MSGERR_UNKNOWN_ERROR					  0x3F		/*/ should be 0x3FF */

/* T/S Translation Internal Error Codes */
#define TSTL_ERR_EMPTY_MAJOR_FRAME -20001

/* Timng Conversions */
#define TIMING_CMD                      20 /* us */
#define TIMING_STATUS                   20 /* us */
#define TIMING_DATA                     20 /* us */

#define TIMING_BC_TO_RT                 (TIMING_CMD + TIMING_STATUS) /* RX CMD + RX STATUS */
#define TIMING_RT_TO_BC                 (TIMING_CMD + TIMING_STATUS) /* TX CMD + TX STATUS */
#define TIMING_RT_TO_RT                 (TIMING_CMD + TIMING_CMD + TIMING_STATUS + TIMING_STATUS) /* TX CMD + RX CMD + TX STATUS + RX STATUS */

#define MY_ACEX_MRT_IMR_BLK_DATA    0x02000000			/* mjr ???? */

/*-----------------------------------------------------------------------
Name:	DISPLAY_EMACE_ERR_RETURN

Description:
	This macro is used in each of the TestSim functions as a standardized
	method of optionally dipslaying a windows message box with the error
	string for the specified error value and then returning. The message
	box will only be displayed if there is an error.

In		InfoString = string to which the error string will be appended
						to be displayed.
In		nResult
Out		nResult
------------------------------------------------------------------------*/
#define DEBUG_PRINT_PRINTF 1		/* Use Printf For Acex Error Messages */
#define DEBUG_PRINT_MSGBOX 2		/* Use MsgBox for Acex Error Messages */

#if (defined(LINUX) || defined(VX_WORKS))

#define DISPLAY_EMACE_ERR_RETURN(printError, InfoString, acexResult, tsResult) \
{\
	if(printError==DEBUG_PRINT_PRINTF)\
	{\
		char myErrorString[80]; \
		aceErrorStr(acexResult, myErrorString, sizeof(myErrorString)); \
		printf("ACEX ERR: %s (%d) %s T/S Returning %d\n",myErrorString,acexResult,InfoString,tsResult); \
	}else if(printError==DEBUG_PRINT_MSGBOX)\
	{\
		char acexErrorString[80]; \
		aceErrorStr(acexResult, acexErrorString, sizeof(acexErrorString)); \
		printf("ACEX ERR: %s (%d) %s T/S Returning %d\n", acexErrorString, acexResult, InfoString, tsResult); \
	}\
	return tsResult;\
}

#else

#define DISPLAY_EMACE_ERR_RETURN(printError, InfoString, acexResult, tsResult) \
{\
	if(printError==DEBUG_PRINT_PRINTF)\
	{\
		char myErrorString[80]; \
		aceErrorStr(acexResult, myErrorString, sizeof(myErrorString)); \
		printf("ACEX ERR: %s (%d) %s T/S Returning %d\n",myErrorString,acexResult,InfoString,tsResult); \
	}else if(printError==DEBUG_PRINT_MSGBOX)\
	{\
		char errorToPrint[256]; \
		char acexErrorString[80]; \
		aceErrorStr(acexResult, acexErrorString, sizeof(acexErrorString)); \
		sprintf(errorToPrint,"ACEX ERR: %s (%d) %s T/S Returning %d", acexErrorString, acexResult, InfoString, tsResult); \
		MessageBox( NULL, (LPCTSTR)errorToPrint, "BU-69092 AceXtreme SDK Error Occurred", MB_OK | MB_ICONINFORMATION ); \
	}\
	return tsResult;\
}

#endif

/*********************************************************************************************************************/
/* TL: ACEXTREME TRANSLATION STRUCTURE DEFINITIONS *******************************************************************/
/*********************************************************************************************************************/

/* TL:  Needed for MtErrors Structure */
#include "common.h"

typedef struct _CH10MSG /*a 1553 message from a Ch10 packet */
{
	U16BIT	u16TimeTag0;
	U16BIT	u16TimeTag1;
	U16BIT	u16TimeTag2;
	U16BIT	u16TimeTag3;
	U16BIT	u16BlkStatus;
	U16BIT	u16MsgGap;
	U16BIT	u16TotalBytes;
	U16BIT	u16MsgWords[1];
} CH10MSG, *pCH10MSG;

/* TL:  MT Counter Info Structure */
typedef struct MTCOUNTERINFO
{
	U32BIT		dwTotalMessages;
	U32BIT		dwTotalInvalid;
	/* define 115 MTErrors structs to be compatible with menu although */
	/* it appears that only 32 are used for RT counters and entry 40 is */
	/* used for LED status. */
	struct MTErrors	aRtCounters[41];	/* MTErrors requires common.h */
} MTCOUNTERINFO;

/* TL:  BC Message Info Structure */
typedef struct BCMESSAGEINFO
{
	BOOLEAN		bMessageDefined;
	BOOLEAN		bMessageCreated;
	MESSAGE		sMessage;
	INJ_ERR		sInjectedError;
	U16BIT		wFrameSymbol;
} BCMESSAGEINFO;

/* TL:  DIO Assignments structure */
typedef enum _DIO_ASSIGNMENT        /* this is the index for aDioSettings. */
{
   DIO_MTI_TRIGGER = 0,
   DIO_BC_IMR_OUTPUT,           /* BC triggered digital output */
   DIO_MRT_IMR_OUTPUT,          /* MRT triggered digital output */
/*   DIO_MC_IMR_OUTPUT,           // mode Code (MRT) digital output, share with MRT for now. */
   DIO_BC_IMR_INPUT_TRIGGER,    /* input trigger for BC IMR */
   DIO_MRT_IMR_INPUT_TRIGGER,   /* input trigger for MRT IMR */
/*   DIO_MC_IMR_INPUT_TRIGGER,    // input trigger for (MRT) mode code IMR, share with MRT for now. */
   DIO_REPLAY_TRIGGER_START,
   DIO_REPLAY_TRIGGER_STOP,
   DIO_LAST_ENTRY,
} DIO_ASSIGNMENT;


/* TL:  DIO Settings */
typedef struct DIO_SETTINGS
{
    S16BIT  s16Discrete;        /* default = -1, none.  range = 0 - 7 (or max of the card). */
} DIO_SETTINGS;

/* TL:  Host Buffer */
typedef struct HOSTBUFFER
{
	U16BIT *pBase;		/* Pointer to a host buffer for msgs */
	U32BIT dwHead;		/* offset to a next message */
	U32BIT dwTail;		/* offset to next free location */
	U32BIT dwSize;		/* number of words in host buffer */
	U32BIT dwCount;		/* number of msgs in host buffer */
	U32BIT dwLost;		/* HBuf full, possible msg loss */
	U32BIT dwPctFull;	/* Current Percentage of HBuf used */
	U32BIT dwHighPct;	/* Highest Percentage of HBuf used */
	U32BIT dwCurSize;   /* Number of Words to interrupt on */
} HOSTBUFFER;


/* TL:  AceXtreme EMACEINFO Info Structure */
typedef struct EMACEINFO
{
	S16BIT		        nConcurrentMon;
	S16BIT		        nBcEmulation;
	S16BIT		        aRtEmulation[32];
	BOOL				bBCRunning;
	BOOL				bRTRunning;
	BOOL				bMONRunning;
	BOOL				bReplayRunning;
	S16BIT		        nRtLegalityDetection;
	S16BIT		        nRtBcastLegality;
	S16BIT		        aModeLegality[32];
	U16BIT		        wMtStatusMask;
	U16BIT				wBcExpStatus;			/* BC Exception Status Mask */
	U16BIT				wBccwExpMask;			/* Mask for BC Control Word to implment BC Exception Status Mask */
	U16BIT				wDataBufType;
	S16BIT		        aFrameArray[1024];
	U16BIT		        aFrameLength;
	BCMESSAGEINFO	    aMessageInfo[1024];
	S32BIT		        iMinorFrameTime;
	S16BIT		        aBasicStatusWord[32];
	U16BIT				wRTCmdWordMsk;
	U16BIT				wRTCmdWordCmp;
    S16BIT              dbca[32];				/* bus control acceptance */
    S32BIT              dbc_delay[32];			/* delay before activating BC */
	HOSTBUFFER			sMTHBuf;				 /* MT Host Buffer */

/* Interrupt Information */
	U16BIT				wIntMask;

/* Data Table Information */
	S16BIT		        aDataTableSize[1024];				/* Size of Data Table:  Range of 0 to 1023 */
	S16BIT		        aDataTables[1024][32];				/* Data Table Data Words:  Max 32 words/table */
	S16BIT		        aDataTableContext[1024];			/* Data Table Context (BC, RT, NONE) */
	S16BIT		        aRtDataBlockMap[32][2][32];			/* indexed by RT Address, T/R, Subaddress */
	S16BIT		        aRtDataBlockEndMap[32][2][32];		/* indexed by RT Address, T/R, Subaddress */
	S16BIT		        aModeCodeData[32][16];				/* indexed by RT Address, Mode Code (lower 4 bits) */
	S16BIT		        nBcTerminalAddress;
	U32BIT		        aMtFilter[32][2];					/* indexed by RT Address, T/R */
	MTCOUNTERINFO	    sMtCounters;
	Error_t(*pUsrMtHandler)(Device_p pCrd);
	S16BIT		        nResponseTimeout;
	S16BIT		        nMtBusASelect;
	S16BIT		        nMtBusBSelect;
	S8BIT		        nCaptureFlag;
	U16BIT		        wCaptureType;
	U16BIT		        wCaptureP1;
	U16BIT		        wCaptureP2;
	DDC_HANDLE		    hThreadGetMsg;				/* MTI message retrieving thread */
	MTI_CH10_DATA_PKT	*pCh10Pkt;			        /* pointer to the parsed data */
	BOOL				bCh10PktInPlace;
    MTI_CH10_TIME_PKT   *pCh10TimePkt;              /* pointer to the time data */
	S32BIT				dwIrigTime;
    PMTI_CH10_FILE_HANDLE pCh10FileHandle;
/*  double              dwBusLoad[32][2];           // holds bus load value */
/*  float               nBusLoadPercentage[32][2]; */
	U16BIT				wOperatingMode;
    CHAR                aMtiFileName[MAX_PATH];
	U32BIT		        dwMtiState;					/* state variable for MTI loop */
	S16BIT				wLastMonMsg[42];			/* Storage for the last monitor message */
	U16BIT				wLastMonMsgSize;			/* Size (in Words) of Last Mon Message */
	BOOL				bLastMonNewMsgs;			/* TRUE if new messages have been received since last call */

	U16BIT				aRTLastCmd[32];
	U16BIT				aRTLastStatus[32];
	U8BIT				u8RTimeTagLastWritten;		/* Current Time Tag Index for RT Time Tags */
	U8BIT				u8RTimeTagLastRead;			/* Current Time Tag Index for RT Time Tags */
	U32BIT				aRTTimeTags[64];			/* Time Tag Counter Array */


	CHANNEL_COUPLING    eCoupling;					/* Bus Coupling Configuration */
	BOOL				bIrigEnabled;				/* IRIG-B Input Enabled Status */
	CHANNEL_TERMINATION eTermination;				/* Bus Termination Configuration */
	U16BIT		        wAction;					/* A general flag for passing action notes */
	U32BIT		        dwBcActivation;				/* BC Button state in BusTrACEr */
	INJ_ERR		        sRtInjectedError[32];		/* Error Injection for RT */
	U32BIT				dwReplayRTDisabled;			/* RT's to "Disable" during Replay */
	ACEX_ERR_INJ		sRtAcexErrorInj[32];		/* Active RT Error Injection */
	U32BIT		        dwAmplitudeScale;			/* A 10 bit value represending voltage scale. */
    BOOL                bIsCaptureFlagFound;        /* Determine whether to save to file. */
    BOOL                bMtirMode;                  /* indicates if MTIR mode is enabled or not (MF cards only) */
    BOOL                bNoFailSafe;                /* turn off failsafe in order to generated certain errors. */
    U8BIT               nMsgTiming;                 /* indicates if BC TTNM or inter-message gap time is enabled (MF cards only) */
    U32BIT              nRTRespTime[32];            /* RT Response Time */
    U32BIT              aDataTableImr[1024];        /* contains intermessage routine numbers attached to a data table. */
    U32BIT              aModeCodeImr[32];           /* contains mode code intermessage routine (one per mode code). */
	S16BIT				aDataTableRoutine_1[1056];  /* RT Data Table and mode code table (1024+32) Routine #1 */
	S16BIT				aDataTableRoutine_2[1056];  /* RT Data Table and mode code talbe (1024+32) Routine #2 */
    DIO_SETTINGS        aDioSettings[MAX_DIO_USES];  /* contains pin number and polarity information for each DIO use case. */
	U16BIT				aBcBlkDataTable[32768];		/* a buffer for BC Block mode data transfer IMR. */
	U16BIT				aRtBlkDataTable[8192];		/* a buffer for RT Rx Block mode data transfer IMR. */
	BOOL				bRtBlkImrAssigned;
	BOOL				bPendingSkip;				/* a skip message flag. */
	BOOL				bPendingBreak;				/* a break point message flag. */
} EMACEINFO;

/* TL:  AceXtreme Main Device Info Structure */
typedef struct TRANS_DEV_INFO
{
	U8BIT			u8DebugPrint;					/* Print ACEX Debug Information */
	U32BIT		    dwCapabilities;	                /* the least significant four bits are used for family value. */
	Device_p	    pTsCrd;
    HWVERSIONINFO   strHWVersionInfo;
	EMACEINFO	    *pEmaceCrd;
} TRANS_DEV_INFO;




/*********************************************************************************************************************/
/* TL: =END= (ACEXTREME TRANSLATION STRUCTURE DEFINITIONS ************************************************************/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/* TL: ACEXTREME TRANSLATION INTERNAL FUNCTION DEFINITIONS ***********************************************************/
/*********************************************************************************************************************/
U16BIT _tlFindEventID(S16BIT s16DioNumber);
void _tlFreeCh10Buffers(Device_p pCrd);
S16BIT _DECL _tlHaltMti(Device_p pCrd);
S16BIT _tlEnableIrqConditions(S16BIT DevNum);
S16BIT _tlMTiConvertCh10Msg2TsStk(S16BIT DevNum,
								  CH10MSG *pCh10Msg,
								  S16BIT *pBufPtr,
								  U16BIT *pCount,
								  EMACEINFO *pEmaceCrd,
								  U32BIT *pTotalBytes);
S16BIT _tlUpdateCounters(Device_p pCrd, struct Header *pMyBufPtr);  /* Count Errors per RT per Bus */
/*
 * END
 * Acex_Trans.H (ACEXTREME TRANSLATION SUPPORT MODULE)
 */
#endif /* Acex_Trans_H */

