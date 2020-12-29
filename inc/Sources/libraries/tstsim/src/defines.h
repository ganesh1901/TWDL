	/************************************************************************
	 *                                                                      * 
	 *     BU-65558 SIMULATOR/TESTER                                        * 
	 *                                                                      * 
	 *     FILE: defines.h                                                  * 
	 *     PURPOSE: Predefined constants                                    * 
	 *                                                                      * 
	 *     Revision history:                                                * 
	 *     created: 8/20/97  Adam Molny                                     * 
	 *              9/04/2001      Updated Linux support                    *
	 ************************************************************************/
#ifndef DEFINES_H
#define DEFINES_H
/* Inserted */
#define    INIT_STATE             0
#define    HALT_STATE             1
#define    HALTED                 2
#define    RUNNING                3
#define    RUN_BCRT_STATE         4
#define    RUN_MONITOR_STATE      5
#define    RUN_BCRT_MONITOR_STATE 6
#define    UNDEFINED_STATE        7
#define    BCRT_MON_HW        1
#define    BCRT_HW            2                /* no monitor in hardware */
#define    MARK2_HW           3
#define    VME_HW             4
#define    IDEA_MIL_HW        5
#define    MARK3_HW           6
#define    MON_SW          0x00
#define    FAST_MON_SW     0x10
#define    PARAM_MON_SW    0x20
#define    VALID_MON_SW    0x30
#define    IDEA_1553B      0x100
#define    IDEA_1553A      0x101
#define    DRIVER_1553B    0x102
#define    DRIVER_1553A    0x103
#define    RECONSTR_BC     0x104
#define    RECONSTR_RT     0x105
#define    DEMO            0xFF
#define	   IRIG_LOWWORD_TABLE_SIZE 0x2CDA
#define    GLUELOGIC_6		  6

/* Predefined Constants */
#define NUM_MESSAGES 1024
  
/* GENERAL PARAMETERS */
#define BCRT		0
#define MON			1
#define NO			0
#define YES			1
#define SINGLE		0
#define DOUBLE		1
#define NILL		0
#define BUS_A		0
#define BUS_B		1
#define RECEIVE		0
#define TRANSMIT	1
#define MODE		2
#define RT_RT		3
#define IDLE		5
#define NOP			4
#define LEGAL		1
#define ILLEGAL		0
#define DEFAULT_RESP_TIMEOUT	16		


/* BC/RT ERROR INJECTION CONSTANTS */
#define  E_NONE                  0           
#define  LENGTH_BIT              0x0300       
#define  LENGTH_BIT_STATUS      (0x0100 | 0 ) 
#define  LENGTH_BIT_DATA        (0x0200 | 0 ) 
#define  LENGTH_WORD            (0x0400 | 64) 
#define  ENCODE_STATUS_GLITCH   (0x0500 | 0 ) 
#define  ENCODE_STATUS_INVERSE  (0x0700 | 0 )
#define  ENCODE_WORD_GLITCH     (0x0900 | 0 )
#define  ENCODE_WORD_INVERSE    (0x0C00 | 0 )
#define  ALTERNATE_STATUS       (0x0F00 | 0 ) 
#define  GAP_ERROR              (0x1000 | 0 ) 
#define  NO_RESPONSE_A          (0x1200 | 0 ) 
#define  NO_RESPONSE_B          (0x1300 | 0 )
#define  NO_RESPONSE_BOTH       (0x1400 | 0 )
#define  LATE_RESPONSE          (0x1500 | 1 )
#define  ALTER_BUS_RESPONSE     (0x1600 | 128)

/* FRAME CONSTANTS */
#define BC_PAUSE 251
#define SKIP 252
#define BREAK_POINT 253
#define END_OF_MAJOR 254
#define END_OF_MINOR 255
#define FOREVER -1l	/* Ignore the VXWorks compiler warning here. */

/* DETECTED ERRORS (read message) */
#undef NO_ERROR
#define     NO_ERROR                0
#define     INVERSE_SYNC            2                   
#define     INVALID_DATA            4 
#define     GAP                     8 
#define     NO_RESPONSE            16 
#define     INVALID_STATUS         32 
#define     BIT_LIT_IN_STATUS      64 
#define     WRONG_TADR_IN_STATUS  128 

/* Intermessage routine indices */
#define NO_OPERATION            1
#define RTRY_ONCE_ALT_BUS       2
#define RTRY_REMAIN_ALT_BUS     3
#define RTRY_SAME_BUS           4
#define INT_ON_END_OF_MESSAGE   5
#define INT_FRAME_SYMBOL        6
#define SET_SERQ_BIT            7
#define RESET_SERQ_BIT          8             
#define RT_INT_AFTER_DATA       9
#define RT_INT_AFTER_MODE      10
#define RT_INT_CMD_MATCH       11
#define RT_INT_MODE_MATCH      12
#define TIME_TAG               13
#define RESERVE_14             14       /* 29.06.94 */
#define RTRY_TWICE             15
#define SET_STATUS_BIT         16
#define RESET_STATUS_BIT       17
#define SET_TRIGGER            18
#define RESET_TRIGGER          19
#define WAIT_TRIGGER           20
#define RESERVE_21             21       /* 29.06.94 */
#define DISABLE_RT             22
#define SET_BUSY               23
#define RESET_BUSY             24
#define SET_BUSY_RESET_SRQ     25       /* 29.06.94 */
#define SET_SRQ_RESET_BUSY     26       /* 29.06.94 */
#define RESERVE_27             27       /* 29.06.94 */
#define RESERVE_28             28       /* 29.06.94 */
#define RESERVE_29             29       /* 29.06.94 */
#define RESERVE_30             30       /* 29.04.94 */
#define SKIP_NEXT              31
#define SET_DISCRETE_0         32
#define SET_DISCRETE_1         33
#define SET_DISCRETE_2         34
#define SET_DISCRETE_3         35
#define RESET_DISCRETE_0       36
#define RESET_DISCRETE_1       37
#define RESET_DISCRETE_2       38
#define RESET_DISCRETE_3       39
#define RESERVE_40             40       /* 29.06.94 */
#define RESERVE_41             41       /* 29.06.94 */
#define RESERVE_42             42       /* 29.06.94 */
#define RESERVE_43	           43       /* 29.06.94 */
#define SKIP_NEXT_ONCE         44       /* 29.06.94 */
#define RESERVE_45	           45       /* 29.06.94 */
#define RESERVE_46	           46       /* 29.06.94 */
#define BLOCK_DATA_BC          47       /* 29.08.95 */
#define BLOCK_DATA_RT          48       /* 29.08.95 */
#define RESERVE_49	           49       /* 29.08.95 */
#define RESERVE_50	           50       /* 29.08.95 */
#define SKIP_NEXT_ONCE_EX      51

#define BC_IMR				0x100
/*  This comment is for internal use. Whenever, a new IMR
  is added, MAX_IMR_INDEX must be updated */

#define MAX_IMR_INDEX          50

/* MONITOR  STACK */

#define     CYCLIC_STACK            1
#define     NON_CYCLIC_STACK        0

/* MONITOR  CAPTURE */

#define   CAPTURE_IMMEDIATE          0
#define   CAPTURE_COMMAND_TEMPLATE   1
#define   CAPTURE_EXCEPTION          2
#define   CAPTURE_TRIGGER            3

#define   ANY_EXCEPTION                 0
#define   INVALID_COMMAND_EXCEPTION     1
#define   INVALID_DATA_EXCEPTION        2
#define   INVALID_STATUS_EXCEPTION      3
#define   GAP_PREC_DATA_EXCEPTION       4
#define   RESPONSE_TIME_EXCEPTION       5
#define   WRONG_RT_ADDR_EXCEPTION       6
#define   FLAGGED_STATUS_BIT_EXCEPTION  7
#define   ILLEGAL_COMMAND_EXCEPTION     8

/* BUS COUPLING */
#define COUPLING_DIRECT			0
#define COUPLING_TRANSFORMER	1
#define TERMINATION_NONE		0
#define TERMINATION_HALF		1
#define TERMINATION_FULL		2

/* For Read of Mode Code BSCT data */
#define MCDATA_BCST_SYNCHRONIZE			33		
#define MCDATA_BCST_SEL_T_SHUTDWN		36
#define MCDATA_BCST_OVR_SEL_T_SHUTDWN	37

#endif /* #ifndef COMMON_H */

