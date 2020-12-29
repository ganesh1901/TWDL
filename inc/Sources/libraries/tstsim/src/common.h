	/************************************************************************
	 *                                                                      * 
	 *     BU-65558 SIMULATOR/TESTER                                        * 
	 *                                                                      * 
	 *     FILE: Common.h                                                   * 
	 *     PURPOSE: Definition of Variable structures                       *
	 *              For internal use only. Do not include in release files  *
	 *                                                                      * 
	 *     Revision history:                                                * 
	 *     created: 7/28/97  Adam Molny                                     *
	 *                                                                      * 
	 *   Release   Date           Description                               *
	 *                                                                      *
	 *   1.0       15-MAY-1998    initial release                           *
	 *   1.0.1     6/18/98        Added version numbers                     *
	 *             9/10/1998      VHDL version 06. Added MT error structure *
	 *             9/29/1998      Added counters for total and invalid cmds *
	 *             9/04/2001      Updated Linux support                     *
	 *             3/27/2002      Added multi OS version reporting feature  *
	 *                                                                      *
 	 ************************************************************************/

#ifndef COMMON_H
#define COMMON_H

/* Software Version Numbers */
#if defined( _WIN32 )
#define RTL_VERSION			"0.0.9"
#endif

#define BUILT_VERSION		1001
#define RTL_CORE_VERSION	"0.0.9"

/*  Structure Definitions  */
struct Message {
	U16BIT dataIndx		:10;    /* data table index			*/
	U16BIT res00		:1;     /* reserved					*/
	U16BIT asyncFlag	:1;     /* not used; see insertMessage() */
	U16BIT busAB		:1;  	/* 0=bus A, 1=bus B			*/
	U16BIT cmdType		:3;		/* Tx, Rx, Mode, RT-RT, NOP	*/
	
	U16BIT IMR1			:7;		/* intermessage routine 1	*/
	U16BIT res01		:1;     /* reserved					*/
	U16BIT IMR2			:7;		/* intermessage routine 2	*/
	U16BIT res02		:1;     /* reserved					*/
	
	U16BIT error1;				/* precalculated error word	*/
	U16BIT error2;				/* precalculated error word	*/
	U16BIT cmd1;				/* first command word		*/
	U16BIT cmd2;				/* second command word		*/
	U16BIT timeToNextMsg;		/* time in microseconds		*/

	U16BIT startIndx	:10;	/* start of data table		*/
	S16BIT wordCntErr	:6;		/* used with LENGTH_WORD	*/

	U16BIT endIndx		:10;	/* end of data table		*/
	U16BIT res08		:6;     /* reserved					*/

	U16BIT currIndx		:10;	/* current data table		*/
	U16BIT res09		:6;     /* reserved					*/
	
	U16BIT RxStatus;			/* returned Rx status word	*/
	U16BIT TxStatus;			/* returned Tx status word	*/
	
	U16BIT detError		:8;		/* error detected by BC		*/
	S16BIT errLoc		:7;		/* -1 = no error			*/
	U16BIT res10		:1;		/* reserved					*/
};

struct Status {
	U16BIT status;				/* RT Status word			*/
	U16BIT error1;				/* precalculated error word	*/
	U16BIT error2;				/* precalculated error word	*/
	U16BIT errType;				/* all other cases   		*/
	
	S16BIT wordCntErr	:6;		/* used with LENGTH_WORD	*/
	S16BIT errorLoc		:7;		/* -1 = no error			*/
	U16BIT AccBusCtrl	:1;		/* Accept dynamic bus control */
	U16BIT Broadcast	:1;		/* not used; see defLegalityBcst()  */
	U16BIT TermEmuOn	:1;		/* Terminal emulation on/off  */
	
	U16BIT busCtrlDelay_lsw;	/* Dynamic bus control delay*/
	U16BIT busCtrlDelay_msw;	/* Dynamic bus control delay*/
	U16BIT lastStatus;			/* last status word			*/
	U16BIT lastCommand;			/* last command word		*/
	U16BIT default_status;		/* Backup the RT error settings */
};

struct Header {					/* occupies 8 words */
	U16BIT busAB		:1;		/* active bus (A=0, B=1) */
	U16BIT capture		:1;
	U16BIT res_01		:3;
	U16BIT illegal		:1;		/* illegal command */
	U16BIT cmdMode		:2;		/* 00-mode cmd; 01-receive; 10-transmit; 11-RTRT */
	U16BIT wordCnt		:6;		/* word count, 32 words=100000 */
	U16BIT res_02		:2;
	
	U16BIT recMode;				/* contains valid receive or mode commands */
	U16BIT xmitInvalid;			/* contains transmit or invalid commands */
	U16BIT TxStatus;			/* returned status for transmit command */
	U16BIT RxStatus;			/* returned status for receive command */
	U16BIT timeTag_msw;			/* real time clock */
	U16BIT timeTag_lsw;			/* real time clock */
	
	U16BIT res_03		:1;
	U16BIT errCode		:6;		/* message error code */
	U16BIT res_04		:2;
	U16BIT longErr		:1;		/* data word > 21 usec */
	U16BIT encodErr		:1;		/* manchester encoding error */
	U16BIT parityErr	:1;		/* parity error */
	U16BIT shortErr		:1;		/* data word too short */
	U16BIT res_05		:3;
};

struct MTErrors {
	U16BIT InvalidData;
	U16BIT InvalidStatus;
	U16BIT DelayedData;
	U16BIT ExtraWord;
	U16BIT RespTiming;
	U16BIT WrongTadr;
	U16BIT BitInStatus;
	U16BIT IllegalMode;
	U32BIT Total;
};

#define DATA_TABLE_SIZE			0x3000	/* size of data tables */
#define MT_BUFFER_SIZE			0x3200	/* size of monitor buffer --Increased to 12kW AJM 5/3/01 */
#define RAW_DATA_SIZE			0x0ff 	/* size of captured raw data circular buffer */
#define STACK_SIZE				0x3020	/* size of replay stacks 1 and 2 307x40msgs */

#endif /* #ifndef COMMON_H */

