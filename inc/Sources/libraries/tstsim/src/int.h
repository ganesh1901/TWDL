/* ILC Data Device Corp.
 *
 *        MIL-STD-1553 Simulator/Tester
 *        'C' Software Library
 *
 *        BUS-69068 rel 1.0
 *
 *        Copyright (c) 1997 by ILC Data Device Corp.
 *        All Rights Reserved.
 *
 * INT.H (HARDWARE INTERFACE MODULE)
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for INT.H (HEADER FILE)
 *
 *   Release   Date           Description
 *
 *	1.0		01-DEC-1997		initial release
 *	1.00.01	19-JUN-1998		increased size of IDEA_INFO.driver_ver from 6 to 10
 *			09-SEP-1998		added hClass field to card structure device_t
 *			04-SEP-2001		Updated Linux support
 *			27-MAR-2002		Added routine to return RTL core revision.
 *
 */

#ifndef INT_H
#define INT_H

#ifdef LINUX
#include <pthread.h>
#endif

/* INTERFACE TYPE DEFINITIONS AND STRUCTURES -----------------------------------*/

/* structure defines the card_state */
typedef struct
{
	S16BIT bc_emulated; /* determines if card specifies a bc or not */
	S16BIT bcrt;		/* determines bcrt state: INIT_STATE, HALTED, RUNNING*/
	S16BIT mon;			/* determines mon state: INIT_STATE, HALTED, RUNNING*/
	S16BIT card;		/* determines card's state: INIT_STATE, HALT_STATE, 
						 * RUN_BCRT_STATE, RUN_MONITOR_STATE, RUN_BCRT_MONITOR_STATE
						 */
} CARD_STATE, *CARD_STATE_p;

/* structure which defines configuration of card*/
typedef struct
{
	U16BIT paragraph;
	char *bcrt_code;
	char *mon_code;
	char *imrlib;
	char *idea_sym;
	U16BIT int_req;
	char std_1553;
	U16BIT card_type;
} DRV_CONFIG,*DRV_CONFIG_p; /* End of configuration structure */

/* structure which which gives information about the card */
typedef struct
{
	U16BIT paragraph;
	S16BIT std_1553;
	S16BIT int_req;
	S16BIT card_type;
	S16BIT mon_type;
	S16BIT bcrt_type;
	S16BIT self_test;
	U32BIT part_number;
	char driver_ver[10];
}IDEA_INFO,*IDEA_INFO_p;


/**** structure used to keep track of the context ****/
typedef struct
{
#ifdef LINUX
	pthread_t	thDispatchInt;
	void*		thResultInt;
	pthread_t	thDispatchDma;
	void*		thResultDma;
#endif
	DDC_HANDLE hDriver;		/* handle to the VxD						*/
	U32BIT Devnode;		/* the device node for the device			*/
	U32BIT AttrBase;    /* pointer to attribute memory base addr    */
	U32BIT MemBase;     /* pointer to memory base address			*/
#ifdef DOS_EMULATION
	U32BIT MemBase2;   /* pointer to second memory base address */
#endif
#ifdef VX_WORKS
	U32BIT IOBase;		/* offset to register(i/o map)				*/
#else
	U16BIT IOBase;		/* offset to register(i/o map)				*/
#endif
	U16BIT Irq;			/* level of irq								*/
	U32BIT MemoryLength;  /* size of ace memory(words)				*/
	U8BIT  Socket;        /* pcmcia slot installation				*/
	U8BIT  CurPage;       /* current page of memory                 */
	U8BIT  PageMode;      /* type of paging desired                 */
	U16BIT IrqMonCount;   /* Host count for monitor interrupts      */
	CARD_STATE_p CardState; /* ptr. structure holds card state info.*/
	IDEA_INFO_p IdeaInfo;   /* ptr. structure holds card info.      */
	U16BIT TestFlag;		/* flag for testing interrupts			*/
	U32BIT hClass;			/* handle to a Class					*/

	/* Pointers to interrupt routines */
	S16BIT  (__stdcall *oldBCRTHandler)(S16BIT type, S16BIT param);/* ptr BC/RT isr	*/
	S16BIT  (__stdcall *oldMONHandler)(void);				/* ptr to MON isr	*/
	Error_t (*newBCRTHandler)(LPVOID pCrd, S16BIT type, S16BIT param);/* ptr BC/RT isr	*/
	Error_t (*newBCRTHandlerEx)(LPVOID pCrd, S16BIT Type, S16BIT SubType, S16BIT Param); /* takes extended message IDs */
	Error_t (*newMONHandler)(LPVOID pCrd);				/* ptr to MON isr	*/

	DDC_HANDLE hBlock;
	FILE *pReplayFile;		/* handle to opened Reconstructor file */

	DDC_HANDLE hBlockEvent;
	DDC_HANDLE hExitEvent;
	DDC_HANDLE hThread[2];
	U32BIT dwThreadID[2];
	U32BIT bIrqInstalled;
	U16BIT lastReadMonStkPtr;
	U8BIT  IrigEnabled;


	DDC_HANDLE hThreadDma[2];
	U32BIT dwThreadIDDma[2];
	U8BIT *pHostDmaBuffer;
	U32BIT dwHostDmaBufferSplit;
	U32BIT dwHostDmaBufferSize;
	U32BIT dwHostDmaBufferHead;
	U32BIT dwHostDmaBufferTail;
	DDC_HANDLE hDriverDma;
	DDC_HANDLE hDmaBlockEvent;
	DDC_HANDLE hDmaExitEvent;

#ifdef VX_WORKS
	SEM_ID syncSem;
	SEM_ID syncSemDma;
	int IsrTID;
	int DmaTID;
	U16BIT wModelID;
	U16BIT wChannel;
	U16BIT wDmaStatus;
#endif

#if (defined( _WIN32 ) || defined( LINUX ) || defined(VX_WORKS) )
	U16BIT GlueLogicVersion;
	U8BIT DoubleReadDualRam;
	U8BIT CorrectIrigTime;
	U8BIT Add1Day;
	U8BIT VMECloseThreads;
	U16BIT LogicDevNumber;
#endif
#ifdef _WIN32
	CRITICAL_SECTION crit_sec;
#endif
}Device_t,*Device_p;   /* End of Device Structure */

/* INTERFACE FUNCTION PROTOTYPES ------------------------------------------*/

Error_t __DECL start_init_idea(void); /* dummy routines */
Error_t __DECL end_init_idea(void);

/* returns the current state of the card */
Error_t __DECL card_state(CARD_STATE *state);
Error_t __DECL ddcCardState(Device_p pCrd, CARD_STATE *state);

/* returns information about the card, including part number */
Error_t __DECL get_idea_info(IDEA_INFO *info);
IDEA_INFO __DECL ddcGetIdeaInfo(Device_p pCrd);

/* halts the BC,RTs and Monitor */
Error_t __DECL halt_idea(void);
Error_t __DECL ddcHaltIdea(Device_p pCrd);

/* the function is no longer supported */
Error_t __DECL idea_address(S16BIT **bcrt,S16BIT **monitor);

/* returns version number for RTL and driver along with part number */
Error_t __DECL idea_version(char *card_ver,char *driver_ver,char *serial_no);
Error_t __DECL ddcIdeaVersion(Device_p pCrd,char *rtl_ver,char *driver_ver,char *part_no);
Error_t __DECL idea_versionBuilt(U32BIT *built_version);

/* returns version number for RTL core */
Error_t __DECL ddcIdeaCoreVersion(Device_p pCrd,char *rtl_core_ver);

/* returns version number for FPGA Glue Logic */
Error_t __DECL ddcGetGlueLogicVersion(Device_p pCrd,U16BIT *gl_ver);

/* Loads an already created setup file into memory, returns card with correct state */
Error_t __DECL load_setup_to_card(char *file_name,S16BIT card_num,S16BIT part_of_card);
Error_t __DECL ddcLoadSetupToCard(Device_p pCrd,char *file_name,S16BIT part_of_card);

/* initializes the card with a config structure */
Error_t __DECL reset_card(DRV_CONFIG *config);
Error_t __DECL ddcResetCard(Device_p *pCrd,DRV_CONFIG *config, U16BIT LogNum);
Error_t __DECL _ddcResetCardFlex(Device_p *ppCrd,DRV_CONFIG *config, U16BIT LogNum,
 							char *i0File,
							char *embeddedVersion, S16BIT vhdlVersion);
Error_t __DECL ddcResetCardSelfBoot(Device_p *ppCrd,DRV_CONFIG *config, U16BIT LogNum);

/* reinitialize BC/RT with default values in shared mem */
Error_t __DECL reset_card_bcrt(DRV_CONFIG *config);
Error_t __DECL ddcResetCardBCRT(Device_p *pCrd,DRV_CONFIG *config, U16BIT LogNum);

/* reinitialize Monitor with default values in shared mem */
Error_t __DECL reset_card_monitor(DRV_CONFIG *config);
Error_t __DECL ddcResetCardMonitor(Device_p *pCrd,DRV_CONFIG *config, U16BIT LogNum);

/* compatibility "idea" resets (they call "card" resets) */
Error_t __DECL reset_idea(char *bcrt_file,char *mon_file,char *imr_lib_file);
Error_t __DECL reset_idea_bcrt(char *bcrt_file,char *mon_file,char *imrlib_file);
Error_t __DECL reset_idea_monitor(char *bcrt_file,char *mon_file,char *imrlib_file);

/* This function supported on PCI cards */
Error_t __DECL set_amp(S16BIT val);
Error_t __DECL ddcSetAmp(Device_p pCrd, S16BIT val);
Error_t __DECL ddcReadAmp(Device_p pCrd, S16BIT *val);
Error_t __DECL ddcSetCoupling(Device_p pCrd, U16BIT Coupling, U16BIT Termination);
Error_t __DECL ddcReadCoupling(Device_p pCrd, S16BIT *val);

/* This function set the response timeout value */
Error_t __DECL set_resp_timeout(S16BIT val);
Error_t __DECL ddcSetRespTimeout(Device_p pCrd,S16BIT val);

/* This function allows the user to change the card that the functions access */
Error_t __DECL set_card_number(S16BIT card_number);

/* shuts down the card  */
Error_t __DECL shut_down_idea(void);
Error_t __DECL ddcShutDownIdea(Device_p *pCrd);

Error_t __DECL restore_driver_variables(char *fname);
Error_t __DECL ddcRestore_driver_variables(Device_p pCrd,char *fname);
Error_t __DECL store_driver_variables(char *fname);
Error_t __DECL ddcStore_driver_variables(Device_p pCrd,char *fname);
Error_t _CreateBinaryPaths(char *EmbeddedPath, char *VHDLPath,
						   char *EmbeddedFile, char *VHDLFile);
Error_t _InitSharedRam(Device_p pCrd);
Error_t _setDevActive(U16BIT LogNum, char YesNo);

/*
 * END
 * INT.H (HARDWARE INTERFACE MODULE)
 */
#endif /* INT_H */

