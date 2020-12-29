#ifndef VIFUNC_H
#define VIFUNC_H

#include "visatype.h"

/*======================================================================*/
/*      Constant Definitions                                            */
/*======================================================================*/

/*+ CARD AND DRIVER STATES */
#define IDEA_INIT_STATE						0
#define IDEA_HALT_STATE						1
#define IDEA_HALTED							2
#define IDEA_RUNNING						3
#define IDEA_RUN_BCRT_STATE					4
#define IDEA_RUN_MONITOR_STATE				5
#define IDEA_RUN_BCRT_MONITOR_STATE			6
#define IDEA_UNDEFINED_STATE				7

/*+ CARD TYPES */
#define IDEA_BCRT_MON_HW					0x1
#define IDEA_BCRT_HW						0x2
#define IDEA_MARK2_HW						0x3
#define IDEA_VME_HW							0x4
#define IDEA_MARK3_HW						0x6
#define IDEA_VXI_HW							0x7

/*+ Card Modes */
#define IDEA_BC_AND_MON						0x0
#define IDEA_VXI_BC_ONLY					0x1
#define IDEA_VXI_MON_ONLY					0x2

#define IDEA_MON_SW							0x00
#define IDEA_FAST_MON_SW					0x10
#define IDEA_PARAM_MON_SW					0x20
#define IDEA_VALID_MON_SW					0x30
#define IDEA_1553B							0x100
#define IDEA_1553A							0x101
#define IDEA_DRIVER_1553B					0x102
#define IDEA_DRIVER_1553A					0x103
#define IDEA_RECONSTR_BC					0x104
#define IDEA_RECONSTR_RT					0x105
#define IDEA_DEMO							0xFF

/*+ GENERAL PARAMETERS */
#define IDEA_BCRT							0
#define IDEA_MON							1
#define IDEA_IMRLIB							2
#define IDEA_BCRT_MON						3

#define IDEA_NO								VI_FALSE
#define IDEA_YES							VI_TRUE

#define IDEA_SINGLE							VI_FALSE
#define IDEA_DOUBLE							VI_TRUE

#define IDEA_NILL							0

#define IDEA_RECEIVE						0
#define IDEA_TRANSMIT						1
#define IDEA_MODE							2
#define IDEA_RT_RT							3
#define IDEA_NOP							4

#define IDEA_LEGAL							VI_TRUE
#define IDEA_ILLEGAL						VI_FALSE

#define IDEA_BUS_A							0
#define IDEA_BUS_B							1

#define IDEA_PROTOCOL_A						'A'
#define IDEA_PROTOCOL_B						'B'

#define IDEA_SELF_TEST_PASS					0

#define IDEA_SINGLE_MODE_FRAME_LENGTH		1001
#define IDEA_DOUBLE_MODE_FRAME_LENGTH		100

#define IDEA_MESSAGE_COUNT					255
#define IDEA_RT_COUNT						31


/*+ BC/RT ERROR INJECTION CONSTANTS */
#define IDEA_E_NONE							0
#define IDEA_LENGTH_BIT						0x0300
#define IDEA_LENGTH_BIT_STATUS				( 0x0100 | 0 )
#define IDEA_LENGTH_BIT_DATA				( 0x0200 | 0 )
#define IDEA_LENGTH_WORD					( 0x0400 | 64)
#define IDEA_ENCODE_STATUS_GLITCH			( 0x0500 | 0 )
#define IDEA_ENCODE_STATUS_INVERSE			( 0x0700 | 0 )
#define IDEA_ENCODE_WORD_GLITCH				( 0x0900 | 0 )
#define IDEA_ENCODE_WORD_INVERSE			( 0x0C00 | 0 )
#define IDEA_ALTERNATE_STATUS				( 0x0F00 | 0 )
#define IDEA_GAP_ERROR						( 0x1000 | 0 )
#define IDEA_NO_RESPONSE_A					( 0x1200 | 0 )
#define IDEA_NO_RESPONSE_B					( 0x1300 | 0 )
#define IDEA_NO_RESPONSE_BOTH				( 0x1400 | 0 )
#define IDEA_LATE_RESPONSE					( 0x1500 | 1 )
#define IDEA_ALTER_BUS_RESPONSE				( 0x1600 | 128)
#define IDEA_ERR_START_RES_500_NSEC			0
#define IDEA_ERR_START_RES_50_NSEC			1

/*+ FRAME CONSTANTS */
#define IDEA_GLOB_TAB						251
#define IDEA_SKIP							252
#define IDEA_BREAK_POINT					253
#define IDEA_END_OF_MAJOR					254
#define IDEA_END_OF_MINOR					255
#define IDEA_FOREVER						(-1L)

/*+ DETECTED ERRORS   ( read message) */
#define IDEA_NO_ERROR						0
#define IDEA_INVERSE_SYNC					2
#define IDEA_INVALID_DATA					4
#define IDEA_GAP							8
#define IDEA_NO_RESPONSE					16
#define IDEA_INVALID_STATUS					32
#define IDEA_BIT_LIT_IN_STATUS				64
#define IDEA_WRONG_TADR_IN_STATUS			128

/*+ MONITOR  STACK */
#define IDEA_NON_CYCLIC_STACK				VI_FALSE
#define IDEA_CYCLIC_STACK					VI_TRUE

/*+ MONITOR  CAPTURE */
#define IDEA_CAPTURE_IMMEDIATE				0
#define IDEA_CAPTURE_COMMAND_TEMPLATE		1
#define IDEA_CAPTURE_EXCEPTION				2
#define IDEA_CAPTURE_TRIGGER				3

#define IDEA_ANY_EXCEPTION					0
#define IDEA_INVALID_COMMAND_EXCEPTION		1
#define IDEA_INVALID_DATA_EXCEPTION			2
#define IDEA_INVALID_STATUS_EXCEPTION		3
#define IDEA_GAP_PREC_DATA_EXCEPTION		4
#define IDEA_RESPONSE_TIME_EXCEPTION		5
#define IDEA_WRONG_RT_ADDR_EXCEPTION		6
#define IDEA_FLAGGED_STATUS_BIT_EXCEPTION	7
#define IDEA_ILLEGAL_COMMAND_EXCEPTION		8

/*+ ENABLE SHARED MEMORY CONSTANTS */
#define IDEA_SHMEM_DISABLE					VI_FALSE
#define IDEA_SHMEM_ENABLE					VI_TRUE

/*+ LOAD_SETUP_TO_CARD CONSTANTS */
#define IDEA_STP_NOT_RESET					0x0
#define IDEA_STP_LOAD_MON					0x1
#define IDEA_STP_LOAD_BCRT					0x2
#define IDEA_STP_LOAD_ALL					0x3

#define IDEA_FIRST_HALF						0
#define IDEA_SECOND_HALF					1

/*+ CODES FOR USE IN INTERRUPT HANDLERS */
#define IDEA_INT_BC_MSG						0x80
#define IDEA_INT_BC_ERR						0x81
#define IDEA_INT_BC_STAT					0x82
#define IDEA_INT_BC_SKIP					0xFC83
#define IDEA_INT_BC_BREAK					0xFD83
#define IDEA_INT_BC_FRM						0xFE83
#define IDEA_INT_BC_MINOR					0xFF83

#define IDEA_INT_ERR_NONE					0
#define IDEA_INT_ERR_DSYNC					0x0002
#define IDEA_INT_ERR_INVAL					0x0004
#define IDEA_INT_ERR_GAP					0x0008
#define IDEA_INT_ERR_NORES					0x0010
#define IDEA_INT_ERR_STAT					0x0020
#define IDEA_INT_ERR_TADR					0x0080

#define IDEA_INT_RT_TABLE					0
#define IDEA_INT_RT_MODE					0x01
#define IDEA_INT_RT_TABLE_ERR				0x02
#define IDEA_INT_RT_MODE_ERR				0x03

/****************************************************************************/
/*		Error Codes															*/
/****************************************************************************/
#define _IDEA_ERROR							( -1879048191L-1) 

#define _IDEA_ERROR_BASE_					( _IDEA_ERROR+0x3FFC1000L)

#define _IDEA_ERROR_DEF_TB_S_				( _IDEA_ERROR_BASE_ + 100L)
#define _IDEA_ERROR_DEF_FRAME_				( _IDEA_ERROR_BASE_ + 120L)
#define _IDEA_ERROR_RUN_BC_					( _IDEA_ERROR_BASE_ + 140L)
#define _IDEA_ERROR_DEF_MINOR_				( _IDEA_ERROR_BASE_ + 160L)
#define _IDEA_ERROR_RD_DATA_				( _IDEA_ERROR_BASE_ + 180L)
#define _IDEA_ERROR_DEF_RT_					( _IDEA_ERROR_BASE_ + 200L)
#define _IDEA_ERROR_DEF_BUFF_				( _IDEA_ERROR_BASE_ + 220L)
#define _IDEA_ERROR_GENERAL_				( _IDEA_ERROR_BASE_ + 240L)
#define _IDEA_ERROR_READ_SYM_				( _IDEA_ERROR_BASE_ + 260L)
#define _IDEA_ERROR_DEF_MC_					( _IDEA_ERROR_BASE_ + 280L)
#define _IDEA_ERROR_DEF_EMUL_				( _IDEA_ERROR_BASE_ + 300L)
#define _IDEA_ERROR_INIT_CRD_				( _IDEA_ERROR_BASE_ + 320L)
#define _IDEA_ERROR_DEF_MSG_				( _IDEA_ERROR_BASE_ + 340L)
#define _IDEA_ERROR_SET_CFG_				( _IDEA_ERROR_BASE_ + 360L)
#define _IDEA_ERROR_READ_CFG_				( _IDEA_ERROR_BASE_ + 370L)
#define _IDEA_ERROR_DRV_INST_				( _IDEA_ERROR_BASE_ + 380L)
#define _IDEA_ERROR_IDEA_VER_				( _IDEA_ERROR_BASE_ + 400L)
#define _IDEA_ERROR_SELF_TST_				( _IDEA_ERROR_BASE_ + 410L)
#define _IDEA_ERROR_DEF_BCST_				( _IDEA_ERROR_BASE_ + 430L)
#define _IDEA_ERROR_SET_AMP_				( _IDEA_ERROR_BASE_ + 440L)
#define _IDEA_ERROR_SET_RESP_				( _IDEA_ERROR_BASE_ + 450L)
#define _IDEA_ERROR_DEF_LAST_				( _IDEA_ERROR_BASE_ + 460L)
#define _IDEA_ERROR_DEF_MAP_				( _IDEA_ERROR_BASE_ + 470L)
#define _IDEA_ERROR_WR_DATA_				( _IDEA_ERROR_BASE_ + 480L)
#define _IDEA_ERROR_WR_MDATA_				( _IDEA_ERROR_BASE_ + 490L)
#define _IDEA_ERROR_RD_MDATA_				( _IDEA_ERROR_BASE_ + 500L)
#define _IDEA_ERROR_RD_LMSG_				( _IDEA_ERROR_BASE_ + 510L)
#define _IDEA_ERROR_RESET_					( _IDEA_ERROR_BASE_ + 520L)
#define _IDEA_ERROR_SET_BIT_				( _IDEA_ERROR_BASE_ + 530L)
#define _IDEA_ERROR_SELECT_					( _IDEA_ERROR_BASE_ + 540L)
#define _IDEA_ERROR_CAPTURE_				( _IDEA_ERROR_BASE_ + 550L)
#define _IDEA_ERROR_DEF_EXCP_				( _IDEA_ERROR_BASE_ + 560L)
#define _IDEA_ERROR_MON_STCK_				( _IDEA_ERROR_BASE_ + 570L)
#define _IDEA_ERROR_CRD_STATE_				( _IDEA_ERROR_BASE_ + 580L)
#define _IDEA_ERROR_DEF_TB_R_				( _IDEA_ERROR_BASE_ + 590L)
#define _IDEA_ERROR_IN_MSG_					( _IDEA_ERROR_BASE_ + 600L)
#define _IDEA_ERROR_CARD_NUM_				( _IDEA_ERROR_BASE_ + 610L)
#define _IDEA_ERROR_STR_VARS_				( _IDEA_ERROR_BASE_ + 620L)
#define _IDEA_ERROR_RES_VARS_				( _IDEA_ERROR_BASE_ + 630L)
#define _IDEA_ERROR_INIT_IDEA_				( _IDEA_ERROR_BASE_ + 640L)
#define _IDEA_ERROR_MON_PAR_				( _IDEA_ERROR_BASE_ + 650L)
#define _IDEA_ERROR_DRV_STATUS_				( _IDEA_ERROR_BASE_ + 670L)
#define _IDEA_ERROR_MON_SELECT_				( _IDEA_ERROR_BASE_ + 680L)
#define _IDEA_ERROR_IGNORE_MON_				( _IDEA_ERROR_BASE_ + 690L)
#define _IDEA_ERROR_SETUP_					( _IDEA_ERROR_BASE_ + 740L)
#define _IDEA_ERROR_NT_SYS_					( _IDEA_ERROR_BASE_ + 760L)
#define _IDEA_ERROR_NT_INTR_HANDLER_		( _IDEA_ERROR_BASE_ + 780L)

#define IDEA_ERROR_NO_SPACE_FOR_TABLE				( _IDEA_ERROR_DEF_TB_S_ + 1L)
#define IDEA_ERROR_ILLEGAL_TABLE_ID					( _IDEA_ERROR_DEF_TB_S_ + 2L)
#define IDEA_ERROR_ILLEGAL_TABLE_SIZE				( _IDEA_ERROR_DEF_TB_S_ + 3L)

#define IDEA_ERROR_FRAME_OVERFLOW					( _IDEA_ERROR_DEF_FRAME_ + 1L)
#define IDEA_ERROR_MINOR_FRAME_OVERFLOW				( _IDEA_ERROR_DEF_FRAME_ + 2L)
#define IDEA_ERROR_ILLEGAL_FRAME_LENGTH				( _IDEA_ERROR_DEF_FRAME_ + 3L)
#define IDEA_ERROR_NOT_ENOUGH_MEMORY_FOR_FRAME		( _IDEA_ERROR_DEF_FRAME_ + 4L)

#define IDEA_ERROR_ILLEGAL_FRAME_POSITION			( _IDEA_ERROR_RUN_BC_ + 1L)
#define IDEA_ERROR_ILLEGAL_RUN_TIMES				( _IDEA_ERROR_RUN_BC_ + 2L)

#define IDEA_ERROR_ILLEGAL_MINOR_FRAME_TIME			( _IDEA_ERROR_DEF_MINOR_ + 1L)

#define IDEA_ERROR_LOC_BIT_TIME_OUT					( _IDEA_ERROR_RD_DATA_ + 1L)

#define IDEA_ERROR_ILLEGAL_GAP_ERROR				( _IDEA_ERROR_DEF_RT_ + 1L)
#define IDEA_ERROR_ILLEGAL_RESPONSE_TIME			( _IDEA_ERROR_DEF_RT_ + 2L)
#define IDEA_ERROR_ILLEGAL_ALTER_TADR				( _IDEA_ERROR_DEF_RT_ + 3L)
#define IDEA_ERROR_ILLEGAL_LENGTH_ERROR				( _IDEA_ERROR_DEF_RT_ + 4L)
#define IDEA_ERROR_ILLEGAL_STATUS_ENC_ERR			( _IDEA_ERROR_DEF_RT_ + 5L)
#define IDEA_ERROR_ILLEGAL_WORD_ENC_ERR				( _IDEA_ERROR_DEF_RT_ + 6L)
#define IDEA_ERROR_ILLEGAL_ERROR_TYPE				( _IDEA_ERROR_DEF_RT_ + 7L)
#define IDEA_ERROR_ILLEGAL_DBC_DELAY				( _IDEA_ERROR_DEF_RT_ + 8L)
#define IDEA_ERROR_ILLEGAL_DBC_FLAG					( _IDEA_ERROR_DEF_RT_ + 9L)

#define IDEA_ERROR_ILLEGAL_BUFFERING_MODE			( _IDEA_ERROR_DEF_BUFF_ + 1L)

#define IDEA_ERROR_NOT_INIT_STATE					( _IDEA_ERROR_GENERAL_ + 1L)
#define IDEA_ERROR_NOT_HALT_STATE					( _IDEA_ERROR_GENERAL_ + 2L)
#define IDEA_ERROR_NOT_RUN_STATE					( _IDEA_ERROR_GENERAL_ + 3L)
#define IDEA_ERROR_NOT_HALT_OR_RUN_STATE			( _IDEA_ERROR_GENERAL_ + 4L)
#define IDEA_ERROR_NOT_BCRT_HALT_STATE				( _IDEA_ERROR_GENERAL_ + 5L)
#define IDEA_ERROR_NOT_MON_HALT_STATE				( _IDEA_ERROR_GENERAL_ + 6L)
#define IDEA_ERROR_NOT_BCRT_RUN_STATE				( _IDEA_ERROR_GENERAL_ + 7L)
#define IDEA_ERROR_NOT_MON_RUN_STATE				( _IDEA_ERROR_GENERAL_ + 8L)
#define IDEA_ERROR_CARD_IS_NOT_READY				( _IDEA_ERROR_GENERAL_ + 10L)

#define IDEA_ERROR_SYMBOL_CHECK_SUM_1				( _IDEA_ERROR_READ_SYM_ + 1L)
#define IDEA_ERROR_SYMBOL_CHECK_SUM_2				( _IDEA_ERROR_READ_SYM_ + 2L)
#define IDEA_ERROR_SYMBOL_CHECK_SUM_3				( _IDEA_ERROR_READ_SYM_ + 3L)
#define IDEA_ERROR_SYMBOL_CHECK_SUM_4				( _IDEA_ERROR_READ_SYM_ + 4L)
#define IDEA_ERROR_SYMBOL_CHECK_SUM_5				( _IDEA_ERROR_READ_SYM_ + 5L)
#define IDEA_ERROR_SYMBOL_CHECK_SUM_6				( _IDEA_ERROR_READ_SYM_ + 6L)
#define IDEA_ERROR_SYMBOL_CHECK_SUM_7				( _IDEA_ERROR_READ_SYM_ + 7L)
#define IDEA_ERROR_SYMBOL_CHECK_SUM_8				( _IDEA_ERROR_READ_SYM_ + 8L)
#define IDEA_ERROR_CANNOT_OPEN_IDEA_SYM				( _IDEA_ERROR_READ_SYM_ + 9L)

#define IDEA_ERROR_ILLEGAL_LEGALITY					( _IDEA_ERROR_DEF_MC_ + 1L)
#define IDEA_ERROR_ILLEGAL_MODE_CODE				( _IDEA_ERROR_DEF_MC_ + 2L)
#define IDEA_ERROR_ILLEGAL_FIRST_MC_IMR				( _IDEA_ERROR_DEF_MC_ + 3L)
#define IDEA_ERROR_ILLEGAL_SECOND_MC_IMR			( _IDEA_ERROR_DEF_MC_ + 4L)

#define IDEA_ERROR_ILLEGAL_EMUL_TADR				( _IDEA_ERROR_DEF_EMUL_ + 1L)
#define IDEA_ERROR_ILLEGAL_EMULATE_BC				( _IDEA_ERROR_DEF_EMUL_ + 2L)
#define IDEA_ERROR_ILLEGAL_EMULATE_RT				( _IDEA_ERROR_DEF_EMUL_ + 3L)

#define IDEA_ERROR_NO_SUCH_CODE_FILE				( _IDEA_ERROR_INIT_CRD_ + 1L)
#define IDEA_ERROR_NO_BCRT_CODE_FILE 				( _IDEA_ERROR_INIT_CRD_ + 2L)
#define IDEA_ERROR_NO_IMRLIB_CODE_FILE				( _IDEA_ERROR_INIT_CRD_ + 3L)
#define IDEA_ERROR_NO_MON_CODE_FILE					( _IDEA_ERROR_INIT_CRD_ + 4L)
#define IDEA_ERROR_ZBL_NOT_FOUND					( _IDEA_ERROR_INIT_CRD_ + 12L)


#define IDEA_ERROR_ILLEGAL_MESSAGE_ID				( _IDEA_ERROR_DEF_MSG_ + 1L)
#define IDEA_ERROR_ILLEGAL_CMD_TYPE					( _IDEA_ERROR_DEF_MSG_ + 2L)
#define IDEA_ERROR_ILLEGAL_MSG_ERR_TYPE				( _IDEA_ERROR_DEF_MSG_ + 3L)
#define IDEA_ERROR_ILL_MSG_BIT_WORD_ERR				( _IDEA_ERROR_DEF_MSG_ + 4L)
#define IDEA_ERROR_ILL_MSG_BIT_IN_WRD_ERR			( _IDEA_ERROR_DEF_MSG_ + 5L)
#define IDEA_ERROR_ILLEGAL_ENCODE_ERR_TYPE			( _IDEA_ERROR_DEF_MSG_ + 6L)
#define IDEA_ERROR_ILLEGAL_GAP_TIME_ERROR			( _IDEA_ERROR_DEF_MSG_ + 7L)
#define IDEA_ERROR_ILLEGAL_GAP_WORD_ERROR			( _IDEA_ERROR_DEF_MSG_ + 8L)
#define IDEA_ERROR_ILL_MSG_EN_BEGIN_WRD				( _IDEA_ERROR_DEF_MSG_ + 9L)
#define IDEA_ERROR_ILL_MSG_EN_WIDTH_ERR				( _IDEA_ERROR_DEF_MSG_ + 10L)
#define IDEA_ERROR_ILLEGAL_MESSAGE_BUS				( _IDEA_ERROR_DEF_MSG_ + 11L)
#define IDEA_ERROR_ILLEGAL_FIRST_MSG_IMR			( _IDEA_ERROR_DEF_MSG_ + 13L)
#define IDEA_ERROR_ILLEGAL_SECOND_MSG_IMR			( _IDEA_ERROR_DEF_MSG_ + 14L)
#define IDEA_ERROR_IMR_NOT_COMPATIBLE				( _IDEA_ERROR_DEF_MSG_ + 15L)
#define IDEA_ERROR_ILLEGAL_SUBADDRESS				( _IDEA_ERROR_DEF_MSG_ + 16L)

#define IDEA_ERROR_IN_COMMAND						( _IDEA_ERROR_DRV_INST_ + 2L)
#define IDEA_ERROR_COMMAND_DID_NOT_START			( _IDEA_ERROR_DRV_INST_ + 3L)
#define IDEA_ERROR_COMMAND_DID_NOT_END				( _IDEA_ERROR_DRV_INST_ + 4L)

#define IDEA_ERROR_ILLEGAL_BCST						( _IDEA_ERROR_DEF_BCST_ + 1L)
#define IDEA_ERROR_ILLEGAL_AMPLITUDE				( _IDEA_ERROR_SET_AMP_ + 1L)
#define IDEA_ERROR_ILLEGAL_RESP_TIMEOUT				( _IDEA_ERROR_SET_RESP_ + 1L)
#define IDEA_ERROR_ILLEGAL_LAST_STATUS_TADR			( _IDEA_ERROR_DEF_LAST_ + 1L)
#define IDEA_ERROR_ILLEGAL_MAP_TADR					( _IDEA_ERROR_DEF_MAP_ + 1L)
#define IDEA_ERROR_ILLEGAL_MAP_SADR					( _IDEA_ERROR_DEF_MAP_ + 2L)
#define IDEA_ERROR_ILLEGAL_MAP_TR_BIT				( _IDEA_ERROR_DEF_MAP_ + 3L)
#define IDEA_ERROR_ILLEGAL_MAP_TABLE_ID				( _IDEA_ERROR_DEF_MAP_ + 4L)

#define IDEA_ERROR_ILLEGAL_WR_ID					( _IDEA_ERROR_WR_DATA_ + 1L)
#define IDEA_ERROR_ILLEGAL_WR_COUNT					( _IDEA_ERROR_WR_DATA_ + 3L)
#define IDEA_ERROR_ILLEGAL_WR_POS					( _IDEA_ERROR_WR_DATA_ + 4L)

#define IDEA_ERROR_ILLEGAL_WR_MC_TADR				( _IDEA_ERROR_WR_MDATA_ + 1L)
#define IDEA_ERROR_ILLEGAL_WR_MODE_CODE				( _IDEA_ERROR_WR_MDATA_ + 2L)

#define IDEA_ERROR_ILLEGAL_RD_MC_TADR				( _IDEA_ERROR_RD_MDATA_ + 1L)
#define IDEA_ERROR_ILLEGAL_RD_MODE_CODE				( _IDEA_ERROR_RD_MDATA_ + 2L)


#define IDEA_ERROR_NO_NEW_MESSAGES					( _IDEA_ERROR_RD_LMSG_ + 1L)
#define IDEA_ERROR_THIRD_BUFF_INT_ENABLED			( _IDEA_ERROR_RD_LMSG_ + 2L)

#define IDEA_ERROR_INSUFFICIENT_MEMORY				( _IDEA_ERROR_RESET_ + 2L)
#define IDEA_ERROR_ILLEGAL_RESET_MODE				( _IDEA_ERROR_RESET_ + 4L)

#define IDEA_ERROR_ILLEGAL_SET_BIT_TADR				( _IDEA_ERROR_SET_BIT_ + 1L)
#define IDEA_ERROR_ILLEGAL_SET_BIT_NO				( _IDEA_ERROR_SET_BIT_ + 2L)
#define IDEA_ERROR_NOT_READY						( _IDEA_ERROR_SET_BIT_ + 3L)
#define IDEA_ERROR_ILLEGAL_IMMEDIATE				( _IDEA_ERROR_SET_BIT_ + 4L)

#define IDEA_ERROR_ILLEGAL_SELECT_TADR				( _IDEA_ERROR_SELECT_ + 1L)
#define IDEA_ERROR_ILLEGAL_SELECT_TR				( _IDEA_ERROR_SELECT_ + 2L)
#define IDEA_ERROR_ILLEGAL_SELECT_SADR				( _IDEA_ERROR_SELECT_ + 3L)

#define IDEA_ERROR_ILLEGAL_CAPTURE_TYPE				( _IDEA_ERROR_CAPTURE_ + 1L)
#define IDEA_ERROR_ILLEGAL_CAPTURE_EXCEPTION		( _IDEA_ERROR_CAPTURE_ + 2L)
#define IDEA_ERROR_ILLEGAL_CAPTURE_TRIGGER			( _IDEA_ERROR_CAPTURE_ + 3L)

#define IDEA_ERROR_ILLEGAL_EXCEPTION_BITS			( _IDEA_ERROR_DEF_EXCP_ + 1L)

#define IDEA_ERROR_ILLEGAL_STACK_TYPE				( _IDEA_ERROR_MON_STCK_ + 1L)
#define IDEA_ERROR_CANNOT_READ_STACK				( _IDEA_ERROR_MON_STCK_ + 2L)

#define IDEA_ERROR_UNDEFINED_CARD_STATE				( _IDEA_ERROR_CRD_STATE_ + 1L)

#define IDEA_ERROR_ILLEGAL_FIRST_DTAB_IMR			( _IDEA_ERROR_DEF_TB_R_ + 1L)
#define IDEA_ERROR_ILLEGAL_SECOND_DTAB_IMR			( _IDEA_ERROR_DEF_TB_R_ + 2L)

#define IDEA_ERROR_ILLEGAL_IN_MSG_POS				( _IDEA_ERROR_IN_MSG_ + 1L)
#define IDEA_ERROR_IN_MSG_NOT_READY					( _IDEA_ERROR_IN_MSG_ + 2L)
#define IDEA_ERROR_IN_MSG_BC_NOT_EMULATED			( _IDEA_ERROR_IN_MSG_ + 3L)

#define IDEA_ERROR_ILLEGAL_CARD_NUMBER				( _IDEA_ERROR_CARD_NUM_ + 1L)

#define IDEA_ERROR_RESET_BEFORE_LOAD_FAILED			( _IDEA_ERROR_INIT_IDEA_ + 1L)
#define IDEA_ERROR_MON_SHARED_MEM_ADR_ERR			( _IDEA_ERROR_INIT_IDEA_ + 3L)
#define IDEA_ERROR_BCRT_SHARED_MEM_ADR_ERR			( _IDEA_ERROR_INIT_IDEA_ + 4L)
#define IDEA_ERROR_MON_SHARED_MEM_DATA_ERR			( _IDEA_ERROR_INIT_IDEA_ + 5L)
#define IDEA_ERROR_BCRT_SHARED_MEM_DATA_ERR			( _IDEA_ERROR_INIT_IDEA_ + 6L)
#define IDEA_ERROR_NOT_IDEA_HW						( _IDEA_ERROR_INIT_IDEA_ + 7L)
#define IDEA_ERROR_SELF_TEST_FAILED					( _IDEA_ERROR_INIT_IDEA_ + 8L)
#define IDEA_ERROR_NOT_MARK3_HW						( _IDEA_ERROR_INIT_IDEA_ + 9L)

#define IDEA_ERROR_NO_PARAMETER_MONITOR				( _IDEA_ERROR_MON_PAR_ + 1L)
#define IDEA_ERROR_ILLEGAL_RT_FOR_PARAMETER			( _IDEA_ERROR_MON_PAR_ + 2L)
#define IDEA_ERROR_ILLEGAL_SADR_FOR_PARAMETER		( _IDEA_ERROR_MON_PAR_ + 3L)
#define IDEA_ERROR_ILLEGAL_TR_FOR_PARAMETER			( _IDEA_ERROR_MON_PAR_ + 4L)
#define IDEA_ERROR_ILLEGAL_TABLE_ID_FOR_PARAMETER	( _IDEA_ERROR_MON_PAR_ + 5L)

#define IDEA_ERROR_UNDEFINED_PART_NUMBER			( _IDEA_ERROR_DRV_STATUS_ + 1L)

#define IDEA_ERROR_ILLEGAL_MON_BIN					( _IDEA_ERROR_MON_SELECT_ + 1L)
#define IDEA_ERROR_ILLEGAL_BUS						( _IDEA_ERROR_MON_SELECT_ + 2L)
#define IDEA_ERROR_ILLEGAL_BUS_STATE				( _IDEA_ERROR_MON_SELECT_ + 3L)

#define IDEA_ERROR_ILLEGAL_IGNORE_CMD				( _IDEA_ERROR_IGNORE_MON_ + 1L)

#define IDEA_ERROR_STP_CANNOT_OPEN_FILE				( _IDEA_ERROR_SETUP_ + 2L)

#define IDEA_ERROR_NT_OPEN_REG_KEY_ERROR			( _IDEA_ERROR_NT_SYS_ + 8L)
#define IDEA_ERROR_NT_READ_REG_KEY_ERROR			( _IDEA_ERROR_NT_SYS_ + 9L)

#define IDEA_ERROR_NT_LOCATE_CARDNO					( _IDEA_ERROR_NT_SYS_ + 15L)
#define IDEA_ERROR_NT_DRIVER_START					(_IDEA_ERROR_NT_SYS_ + 16L)

#define IDEA_ERROR_NT_THREAD_CREATE					( _IDEA_ERROR_NT_INTR_HANDLER_ + 1L)
#define IDEA_ERROR_NT_THREAD_SYNC					( _IDEA_ERROR_NT_INTR_HANDLER_ + 2L)

/*+ MEMORY MAP OF BC/RT WITH DOUBLE BUFFERED DATA */

#define IDEA_FRAME_POINTER					0x0013
#define IDEA_INTERRUPT_POINTER				0x0014
#define IDEA_INTERRUPT_MASK					0x0015
#define IDEA_MASK_WORD						0x0016
#define IDEA_COMPARE_WORD					0x0017
#define IDEA_EXCEP_BITS_IN_STATUS			0x0018
#define IDEA_TASK_ID						0x001B
#define IDEA_MONITOR_RTC_COPY				0x001D
#define IDEA_BCRT_RTC_COPY					0x001F
#define IDEA_DOWN_LOAD_BUFF_1 				0x0028
#define IDEA_DOWN_LOAD_BUFF_2				0x0050
#define IDEA_DATA_TABLES_BUFFER_I			0x0180
#define IDEA_DOUBLE_MODE_DATA 				0x0E00
#define IDEA_DOUBLE_FRAME_ADR				0x1000
#define IDEA_DATA_TABLES_BUFFER_II			0x1180
#define IDEA_INTERRUPT_QUEUE				0x1E00
#define IDEA_DATA_TABLE_FLAGS				0x1E80
#define IDEA_DATA_TABLE_POINTERS			0x1F00


/*+ MEMORY MAP OF BC/RT WITH SINGLE BUFFERED DATA */

#define IDEA_SINGLE_FRAME_ADR				0x0100
#define IDEA_SINGLE_MODE_DATA				0x0CC0
#define IDEA_SINGLE_DATA_TABLE_BUFF			0x0EC0

/*+ MEMORY MAP OF MONITOR */

#define IDEA_CAPTURE_FLAG					0x000F
#define IDEA_INTERRUPT_COUNTER				0x000F
#define IDEA_LAST_MESSAGE_POINTER			0x0010
#define IDEA_START_OF_NEXT_MESSAGE			0x0011
#define IDEA_END_OF_BUFFER					0x0012
#define IDEA_MON_INTERRUPT_MASK				0x0015
#define IDEA_ERROR_COUNTERS					0x0028
#define IDEA_CIRCULAR_BUFFER				0x0838

#define IDEA_REAL_TIME_CLOCK				0x0002
#define IDEA_RESET_INTERRUPT_REQUEST		0x0005

/*+ Intermessage routines */

#define IDEA_IMR_NO_OPERATION				1
#define IDEA_IMR_RTRY_ONCE_ALT_BUS			2
#define IDEA_IMR_RTRY_REMAIN_ALT_BUS		3
#define IDEA_IMR_RTRY_SAME_BUS				4
#define IDEA_IMR_INT_ON_END_OF_MESSAGE		5
#define IDEA_IMR_INT_FRAME_SYMBOL			6
#define IDEA_IMR_SET_SERQ_BIT				7
#define IDEA_IMR_RESET_SERQ_BIT				8
#define IDEA_IMR_RT_INT_AFTER_DATA			9
#define IDEA_IMR_RT_INT_AFTER_MODE			10
#define IDEA_IMR_RT_INT_CMD_MATCH			11
#define IDEA_IMR_RT_INT_MODE_MATCH			12
#define IDEA_IMR_TIME_TAG					13
#define IDEA_IMR_RESERVE_14					14
#define IDEA_IMR_RTRY_TWICE					15
#define IDEA_IMR_SET_STATUS_BIT				16
#define IDEA_IMR_RESET_STATUS_BIT			17
#define IDEA_IMR_SET_TRIGGER				18
#define IDEA_IMR_RESET_TRIGGER				19
#define IDEA_IMR_WAIT_TRIGGER				20
#define IDEA_IMR_RESERVE_21					21
#define IDEA_IMR_DISABLE_RT					22
#define IDEA_IMR_SET_BUSY					23
#define IDEA_IMR_RESET_BUSY					24
#define IDEA_IMR_SET_BUSY_RESET_SRQ			25
#define IDEA_IMR_SET_SRQ_RESET_BUSY			26
#define IDEA_IMR_RESERVE_27					27
#define IDEA_IMR_RESERVE_28					28
#define IDEA_IMR_RESERVE_29					29
#define IDEA_IMR_RESERVE_30					30
#define IDEA_IMR_SKIP_NEXT					31
#define IDEA_IMR_RESERVE_32					32
#define IDEA_IMR_RESERVE_33					33
#define IDEA_IMR_RESERVE_34					34
#define IDEA_IMR_RESERVE_35					35
#define IDEA_IMR_RESERVE_36					36
#define IDEA_IMR_RESERVE_37					37
#define IDEA_IMR_RESERVE_38					38
#define IDEA_IMR_RESERVE_39					39
#define IDEA_IMR_RESERVE_40					40
#define IDEA_IMR_RESERVE_41					41
#define IDEA_IMR_RESERVE_42					42
#define IDEA_IMR_RESERVE_43					43
#define IDEA_IMR_SKIP_NEXT_ONCE				44
#define IDEA_IMR_RESERVE_45					45
#define IDEA_IMR_RESERVE_46					46
#define IDEA_IMR_BLOCK_DATA_BC				47
#define IDEA_IMR_BLOCK_DATA_RT				48
#define IDEA_IMR_RESERVE_49					49
#define IDEA_IMR_RESERVE_50					50

/* when new IMR added - must be updated ! */
#define IDEA_MAX_IMR_INDEX					50


#define IDEA_WRONG_RT						1
#define IDEA_EXCEPTION_BIT					2
#define IDEA_TYPE_NO_RESPONSE				1
#define IDEA_TYPE_EARLY_RESPONSE			2
#define IDEA_TYPE_GAP						3
#define IDEA_TYPE_MANCHESTER				4
#define IDEA_TYPE_LONG_WORD					5
#define IDEA_TYPE_SHORT_WORD				6
#define IDEA_TYPE_PARITY					7
#define IDEA_TYPE_EXTRA_WORD				8
#define IDEA_TYPE_INVERSE					9
#define IDEA_WORD_CMD						1
#define IDEA_WORD_RX						2
#define IDEA_WORD_TX						3
#define IDEA_WORD_DATA						4

/*+ card instructions */
#define IDEA_SET_CONT_ROUTINE				0
#define IDEA_START_WAIT						1000
#define IDEA_SET_NO_OF_BUFFERS				3
#define IDEA_SET_LEGALITY					4
#define IDEA_SET_LAST_STATUS				5
#define IDEA_SET_LAST_COMMAND				6
#define IDEA_END_BIT						3
#define IDEA_IMMEDIATE_EXECUTE				128

/*+ set - reset bit */
#define  IDEA_RESEVED_BIT					5
#define  IDEA_BROADCAST_BIT					4
#define  IDEA_BUSY_BIT						3
#define  IDEA_DBC_ACCEPTANCE_BIT			1
#define  IDEA_INSTRUMENTATION_BIT			9
#define  IDEA_MESSAGE_ERROR_BIT				10
#define  IDEA_SERQ_BIT						8
#define  IDEA_SUBSYSTEM_BIT					2
#define  IDEA_TERMINAL_FLAG_BIT				0
#define  IDEA_SET_BUSY_RESET_SRQ			25
#define  IDEA_RESET_BUSY_SET_SRQ			26

#define IDEA_TABLE_LEN						0x22
#define IDEA_ERROR_COUNTERS_AREA_LENGTH		0x440

#define IDEA_CODE_invalid_cmd				0x3

#define IDEA_CODE_invalid_data1				0xF
#define IDEA_CODE_invalid_data2				0x10
#define IDEA_CODE_invalid_data3				0x13
#define IDEA_CODE_invalid_data4				0x14
#define IDEA_CODE_invalid_data5				0x15
#define IDEA_CODE_invalid_data6				0x16
#define IDEA_CODE_invalid_data7				0x1F
#define IDEA_CODE_invalid_data8				0x20
#define IDEA_CODE_invalid_data9				0x21

#define IDEA_CODE_gap1						0xE
#define IDEA_CODE_gap2						0x11
#define IDEA_CODE_gap3						0x12

#define IDEA_CODE_no_resp1					0x8
#define IDEA_CODE_no_resp2					0x9
#define IDEA_CODE_no_resp3					0xC
#define IDEA_CODE_no_resp4					0xD
#define IDEA_CODE_no_resp5					0x17
#define IDEA_CODE_no_resp6					0x18
#define IDEA_CODE_no_resp7					0x1B
#define IDEA_CODE_no_resp8					0x1C

#define IDEA_CODE_invalid_stat1				0xA
#define IDEA_CODE_invalid_stat2				0xB
#define IDEA_CODE_invalid_stat3				0x19
#define IDEA_CODE_invalid_stat4				0x1A

#define IDEA_CODE_wrong_tadr1				0x6
#define IDEA_CODE_wrong_tadr2				0x7
#define IDEA_CODE_wrong_tadr3				0x1D

#define IDEA_CODE_bit_stat1					0x4
#define IDEA_CODE_bit_stat2					0x5
#define IDEA_CODE_bit_stat3					0x1E

#define IDEA_CODE_illegal_mode				0x2

#define IDEA_CODE_total_mess				0x0

#define IDEA_READ_BC_MSG					5


/************************************************************************/
/*      Type Definitions                                                */
/************************************************************************/

typedef ViInt16 (_VI_FUNCH *IDEA_MON_INTERRUPT_HANDLER) ( void );
typedef ViInt16 (_VI_FUNCH *IDEA_BCRT_INTERRUPT_HANDLER) ( ViInt16, ViInt16 );

/* COMMAND bit fields  */
#ifdef VME
typedef struct _tagIDEA_CMD  {
	
	ViUInt16 tadr	: 5;
	ViUInt16 t_r	: 1;
	ViUInt16 subadr	: 5;
	ViUInt16 wcnt	: 5;

} IDEA_CMD;
#else
typedef struct _tagIDEA_CMD  {
	ViUInt16 wcnt	: 5;
	ViUInt16 subadr	: 5;
	ViUInt16 t_r	: 1;
	ViUInt16 tadr	: 5;
} IDEA_CMD;
#endif

#ifdef VME
typedef struct _tagIDEA_CMD_FLG {
		
	ViUInt16 spare	:10;
	ViUInt16 crun	: 1;
	ViUInt16 bc		: 1;
	ViUInt16 ccapture: 1;
	ViUInt16 error	: 1;
	ViUInt16 end	: 1;
	ViUInt16 start	: 1;
} IDEA_CMD_FLG;
#else
typedef struct _tagIDEA_CMD_FLG {
	ViUInt16 start	: 1;
	ViUInt16 end	: 1;
	ViUInt16 error	: 1;
	ViUInt16 ccapture: 1;
	ViUInt16 bc		: 1;
	ViUInt16 crun	: 1;
	ViUInt16 spare	:10;
} IDEA_CMD_FLG;
#endif

typedef struct _tagIDEA_INJ_ERR {
	ViInt16 error;
	ViInt16 sub_error_1;
	ViInt16 sub_error_2;
	ViInt16 sub_error_3;
} IDEA_INJ_ERR, *IDEA_PINJ_ERR;

typedef struct _tagIDEA_ERR_ST {
	ViInt16  code;
	ViInt16  illegal;
	ViInt16  rx_status;
	ViInt16  tx_status;
	ViInt16  type;
	ViInt16  word_type;
	ViInt16  word_number;
} IDEA_ERR_ST;

typedef struct _tagIDEA_MESSAGE {		/* IDEA_MESSAGE typedef */
	ViByte comm_type;					/* type of communication */
	IDEA_CMD cmd_1;						/* command to be tranmitted by card */
	IDEA_CMD cmd_2;						/* TX command in case of RT to RT */
	ViUInt16 time_to_next_message;		/* time from the start of
											the present message to
											the start of  the next
											message */
	ViInt16 data_table_no;				/* data table number 
											This is the first data table number
											in the case of BLOCK_DATA_BC */
	ViByte bus;							/* bus A/B */
	ViByte first_intermessage_routine;	/* routines to be */
	ViByte second_intermessage_routine;	/* executed by Z8002 in
											the gap following the message */
	IDEA_INJ_ERR *inj_error_ptr;		/* pointer to structure
											defining error injection */
	ViInt16 det_error;					/* detected error - updated by BC */
	ViInt16 rx_status;					/* returned RX status - updated by BC */
	ViInt16 tx_status;					/* returned TX status - updated by BC */

	ViInt16 last_data_table_no;			/* last data table number, to be
											used in case of BLOCK_DATA_BC */
} IDEA_MESSAGE, *IDEA_PMESSAGE;

typedef struct _tagIDEA_RT_DEFS {		/* RT_DEFS typedef */
	ViInt16 basic_status;				/* basic status */
	ViInt16 dbca;						/* bus control acceptance */
	ViInt32 dbc_delay;					/* delay before activating BC */
	IDEA_INJ_ERR *inj_error;
} IDEA_RT_DEFS, *IDEA_PRT_DEFS;

typedef struct _tagIDEA_MON_MSG {
	ViInt16	comm_type,
			bus,
			capture,
			broadcast;
	ViInt32	rtc;
	ViInt16	cmd_1,
			cmd_2,
			rx_status,
			tx_status,
			word_count,
			aux_word_count,
			error;
	ViInt16	*data_buf,
			*next_msg;

	IDEA_ERR_ST	error_inf;

	IDEA_CMD	s_cmd_1,
				s_cmd_2;
	ViInt16		cmd_1_flag,
				cmd_2_flag,
				stat_t_flag,
				stat_r_flag;
} IDEA_MON_MSG, _VI_PTR IDEA_PMON_MSG;

/* structure not containing bit-fields */
/** VB_TYPE **/
typedef struct _tagIDEA_MON_MSG_VI {
	ViInt16		comm_type;
	ViInt16		bus;
	ViInt16		capture;
	ViInt16		broadcast;
	ViInt32		rtc;
	ViInt16		cmd_1;
	ViInt16		cmd_2;
	ViInt16		rx_status;
	ViInt16		tx_status;
	ViInt16		word_count;
	ViInt16		aux_word_count;
	ViInt16		error;
	IDEA_ERR_ST	error_inf;
	ViInt16		cmd_1_flag;
	ViInt16		cmd_2_flag;
	ViInt16		stat_t_flag;
	ViInt16		stat_r_flag;
} IDEA_MON_MSG_VI, _VI_PTR IDEA_PMON_MSG_VI;

typedef struct _tagIDEA_MONITOR_STATISTICS {
	ViUInt16 invalid_cmd;

	ViUInt16 invalid_data1;
	ViUInt16 invalid_data2;
	ViUInt16 invalid_data3;
	ViUInt16 invalid_data4;
	ViUInt16 invalid_data5;
	ViUInt16 invalid_data6;
	ViUInt16 invalid_data7;
	ViUInt16 invalid_data8;
	ViUInt16 invalid_data9;

	ViUInt16 gap1;
	ViUInt16 gap2;
	ViUInt16 gap3;

	ViUInt16 no_resp1;
	ViUInt16 no_resp2;
	ViUInt16 no_resp3;
	ViUInt16 no_resp4;
	ViUInt16 no_resp5;
	ViUInt16 no_resp6;
	ViUInt16 no_resp7;
	ViUInt16 no_resp8;

	ViUInt16 invalid_stat1;
	ViUInt16 invalid_stat2;
	ViUInt16 invalid_stat3;
	ViUInt16 invalid_stat4;

	ViUInt16 wrong_tadr1;
	ViUInt16 wrong_tadr2;
	ViUInt16 wrong_tadr3;

	ViUInt16 bit_stat1;
	ViUInt16 bit_stat2;
	ViUInt16 bit_stat3;

	ViUInt16 illegal_mode;

	ViUInt32 total_mess;
} IDEA_MONITOR_STATISTICS, _VI_PTR IDEA_P_MONITOR_STATISTICS;

typedef struct _tagIDEA_CARD_STATE
{
	ViInt16 bc_emulated;	/* specifies whether or not the card emulates a BC */
	ViInt16 bcrt;		/* specifies the bcrt_function state; possible values are
							INIT_STATE, HALTED, RUNNING */
	ViInt16 mon;		/* specifies the monitor's state; possible values are
							INIT_STATE, HALTED, RUNNING */
	ViInt16 card;		/* specifies the card's state; possible values are
							INIT_STATE,  HALT_STATE, RUN_BCRT_STATE,
							RUN_MONITOR_STATE, RUN_BCRT_MONITOR_STATE */
} IDEA_CARD_STATE, _VI_PTR IDEA_PCARD_STATE;

#if !defined (_MAX_PATH)
#define _MAX_PATH 255
#endif

typedef struct _tagIDEA_REGISTRY_INFO {
	ViChar   BinPath[_MAX_PATH];
	ViInt32  BusNumber;
	ViChar   CardName[_MAX_PATH];
	ViInt32  InterfaceType;
	ViInt32  Interrupt;
	ViInt32  IoPortAddress;
	ViInt32  IoPortCount;
	ViChar   LibPath[_MAX_PATH];
	ViInt32  MemoryAddress;
	ViInt32  MemoryCount;
	ViChar   Protocol[4];
	ViInt32  Type;
} IDEA_REGISTRY_INFO, *IDEA_PREGISTRY_INFO;

/************************************************************************/
/*      Function Definitions                                            */
/************************************************************************/

ViStatus _VI_FUNC IdeaCaptureEvent		( ViInt16 CardNo, ViInt16 EvtType, 
											ViInt16 Param1, ViInt16 Param2 );
ViStatus _VI_FUNC IdeaCaptureOccurred	( ViInt16 CardNo, ViPChar Flag );
ViStatus _VI_FUNC IdeaCardState			( ViInt16 CardNo, IDEA_PCARD_STATE State );
ViStatus _VI_FUNC IdeaCardStateVi		( ViInt16 CardNo, ViPInt16 BcEmulated, 
											ViPInt16 Bcrt, ViPInt16 Mon, 
											ViPInt16 Card );
ViStatus _VI_FUNC IdeaCheckImrCompatibility( ViInt16 CardNo, ViInt16 FirstImrId, 
												ViInt16 SecondImrId );
ViStatus _VI_FUNC IdeaDbgOSGetCounts	( ViInt16 CardNo, ViPInt32 tot, ViPInt32 mon_host,
											ViPInt32 mon_card, ViPInt32 bc_host, ViPInt32 bc_card );
/*ViStatus _VI_FUNC IdeaDbgOSReadMk3Register	( ViInt16 CardNo, ViInt16 reg_byte_address, ViPByte val );
Status _VI_FUNC IdeaDbgOSReadVxiRegister	( ViInt16 CardNo, ViInt16 reg_byte_address, ViPUInt16 val );
ViStatus _VI_FUNC IdeaDbgReadShmemWord	( ViInt16 CardNo, ViInt16 Part, ViUInt16 OffsetInWords, 
											ViPUInt16 Word );*/
ViStatus _VI_FUNC IdeaDecodeMonMessage	( ViInt16 CardNo, ViPInt16 Message,
											IDEA_PMON_MSG DecodedMessage );

ViStatus _VI_FUNC IdeaDecodeMonMessageVi( ViInt16 CardNo, ViPInt16 Message,
											IDEA_PMON_MSG_VI DecodedMessage );
ViStatus _VI_FUNC IdeaDefEmulateBc		( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Emulate );

ViStatus _VI_FUNC IdeaDefDataBuffering	( ViInt16 CardNo, ViBoolean DataBufferingMode );

ViStatus _VI_FUNC IdeaDefTableRoutine	( ViInt16 CardNo, ViInt16 TableId, ViInt16 FirstImr,
											ViInt16 SecondImr );

ViStatus _VI_FUNC IdeaDefTableSize		( ViInt16 CardNo, ViInt16 TableId, ViInt16 Size );

ViStatus _VI_FUNC IdeaDefBcExceptionStatus( ViInt16 CardNo, ViUInt16 ExceptionBits );

ViStatus _VI_FUNC IdeaDefMonExceptionStatus( ViInt16 CardNo, ViUInt16 ExceptionBits );

ViStatus _VI_FUNC IdeaDefFrame			( ViInt16 CardNo, ViInt16 Length, ViPChar FrameItem );

ViStatus _VI_FUNC IdeaDefFrameTime		( ViInt16 CardNo, ViInt16 Length, 
											ViPByte FrameItem, ViPUInt32 frame_time );
ViStatus _VI_FUNC IdeaDefModeRoutine	( ViInt16 CardNo, ViInt16 ModeCode, ViInt16 FirstImr,
											ViInt16 SecondImr );
ViStatus _VI_FUNC IdeaDefIntCmdTemplate	( ViInt16 CardNo, ViUInt16 MaskWord, 
											ViUInt16 CompareWord );
ViStatus _VI_FUNC IdeaDefIntMask		( ViInt16 CardNo, ViUInt16 MaskReg );
ViStatus _VI_FUNC IdeaDefIntMaskBcrt	( ViInt16 CardNo, ViUInt16 MaskReg );
ViStatus _VI_FUNC IdeaDefIntMaskMonitor	( ViInt16 CardNo, ViUInt16 MaskReg );
ViStatus _VI_FUNC IdeaDefLastCmd		( ViInt16 CardNo, ViInt16 Tadr, ViUInt16 LastCmd );
ViStatus _VI_FUNC IdeaDefLastStatus		( ViInt16 CardNo, ViInt16 Tadr, ViUInt16 LastStatus );
ViStatus _VI_FUNC IdeaDefLegalityDetection( ViInt16 CardNo, ViBoolean Detect );
ViStatus _VI_FUNC IdeaDefLegalityBcst	( ViInt16 CardNo, ViBoolean Legality );
ViStatus _VI_FUNC IdeaDefMessage		( ViInt16 CardNo, ViInt16 MessageId,
											IDEA_MESSAGE *message );
ViStatus _VI_FUNC IdeaDefMinorFrameTime	( ViInt16 CardNo, ViInt32 MinorFrameTime );
ViStatus _VI_FUNC IdeaDefModeLegality	( ViInt16 CardNo, ViInt16 ModeCode, 
											ViBoolean Legality );
ViStatus _VI_FUNC IdeaDefMonitorStack	( ViInt16 CardNo, ViInt16 StackType );
ViStatus _VI_FUNC IdeaDefRt				( ViInt16 CardNo, ViInt16 RtId,
											IDEA_PRT_DEFS rt_defs );
ViStatus _VI_FUNC IdeaDefEmulateRt		( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Emulate );
ViStatus _VI_FUNC IdeaDefRtMap			( ViInt16 CardNo, ViInt16 tadr, 
											ViInt16 tr_bit, ViInt16 sadr, 
											ViInt16 table_id );
ViStatus _VI_FUNC IdeaDefRtMapBlock		( ViInt16 CardNo, ViInt16 Tadr, ViInt16 TrBit,
											ViInt16 Sadr, ViInt16 TableIdFirst,
											ViInt16 TableIdSecond );
ViStatus _VI_FUNC IdeaDefRtVi			( ViInt16 CardNo, ViInt16 RtId, 
											ViInt16 BasicStatus, ViInt16 Dbca,
											ViInt32 DbcDelay, ViInt16 ErrorType, 
											ViInt16 SubError_1, ViInt16 SubError_2, 
											ViInt16 SubError_3 );
ViStatus _VI_FUNC IdeaDeselectAllMessages( ViInt16 CardNo, ViInt16 Tadr );
ViStatus _VI_FUNC IdeaDeselectMessage	( ViInt16 CardNo, ViInt16 Tadr,
											ViInt16 TrBit, ViInt16 Sadr );
/*ViStatus _VI_FUNC IdeaEnableSharedMemory( ViInt16 CardNo, ViInt16 EnableDisable );*/
ViStatus _VI_FUNC IdeaEndInitIdea		( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaGetBcDataTable	( ViInt16 CardNo, ViInt16 MessageId, ViInt16 TrBit,
											ViPInt16 DataTableNumber );
ViStatus _VI_FUNC IdeaGetCommandVi		( ViUInt16 Command, ViPUInt16 Wcnt, ViPUInt16 Subadr,
											ViPUInt16 Tr, ViPUInt16 Tadr );
ViStatus _VI_FUNC IdeaGetErrorMessageVi	( ViInt16 CardNo, ViInt32 status,
											ViPChar astring, ViInt16 Length );
ViStatus _VI_FUNC IdeaGetIdeaInfoVi		( ViInt16 CardNo, ViPUInt16 Paragraph, ViPInt16 Std_1553,
											ViPInt16 IntReq, ViPInt16 CardType,
											ViPInt16 MonType, ViPInt16 BcrtType,
											ViPInt16 SelfTest, ViPInt32 PartNumber,
											ViPChar DriverVer, ViPInt16 PartIdx );
ViStatus _VI_FUNC IdeaGetRegistryInfo ( ViInt16 CardNo, IDEA_PREGISTRY_INFO Info );
ViStatus _VI_FUNC IdeaHaltBcrt			( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaHaltIdea			( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaHaltMon			( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaInsertMessage		( ViInt16 CardNo, ViInt16 Pos );
/*ViStatus _VI_FUNC IdeaLoadSetupToCard	( ViInt16 CardNo, 
											ViPChar FileName, ViInt16 Part_Of_Card, 
											ViInt16 ResetMode );*/
ViStatus _VI_FUNC IdeaMonitorStatistics	( ViInt16 CardNo, IDEA_P_MONITOR_STATISTICS mon_stat );
ViStatus _VI_FUNC IdeaPrintErrorMessage	( ViInt16 CardNo, ViInt32 status, ViPChar astring );
ViStatus _VI_FUNC IdeaReadData			( ViInt16 CardNo, ViInt16 TableId, ViPInt16 Buffer, ViInt16 Count );
ViStatus _VI_FUNC IdeaReadLastMonMessage( ViInt16 CardNo, ViPInt16 Buffer, ViPInt16 Count );
ViStatus _VI_FUNC IdeaReadLastStatusCmd	( ViInt16 CardNo, ViInt16 Tadr, ViPUInt16 LastStatus,
											ViPUInt16 LastCmd );
ViStatus _VI_FUNC IdeaWriteData			( ViInt16 CardNo, ViInt16 TableId, 
											ViPInt16 Buffer, ViInt16 Count,
											ViInt16 Pos );
ViStatus _VI_FUNC IdeaReadMessageVi		( ViInt16 CardNo, ViInt16 MessageId, ViPInt16 DetError,
											ViPInt16 RxStatus, ViPInt16 TxStatus  );
ViStatus _VI_FUNC IdeaReadModeData		( ViInt16 CardNo, ViInt16 Tadr, ViInt16 ModeCode, ViPInt16 Data );
ViStatus _VI_FUNC IdeaReadMonRtc		( ViInt16 CardNo, ViPUInt32 Rtc );
ViStatus _VI_FUNC IdeaReadMonStack		( ViInt16 CardNo, ViPInt16 Buffer, ViPInt16 Count );
ViStatus _VI_FUNC IdeaReadMonStackPart	( ViInt16 CardNo, ViPInt16 Buffer, ViPInt16 Count,
											ViPInt16 Message );
ViStatus _VI_FUNC IdeaReadTimeTags		( ViInt16 CardNo, ViPInt16 Buffer, ViPUInt16 Count );
ViStatus _VI_FUNC IdeaResetCard			( ViInt16 CardNo, ViInt16 Mode );
ViStatus _VI_FUNC IdeaRunBc				( ViInt16 CardNo, ViInt16 Position, ViInt32 Times );
ViStatus _VI_FUNC IdeaRunContinue		( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaRunMon			( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaRunRt				( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaSelectAllMessages	( ViInt16 CardNo, ViInt16 Tadr );
ViStatus _VI_FUNC IdeaSelectBus			( ViInt16 CardNo, ViUInt16 BusAState, ViUInt16 BusBState );
ViStatus _VI_FUNC IdeaSelectMessage		( ViInt16 CardNo, ViInt16 Tadr, ViInt16 TrBit,
											ViInt16 Sadr );
ViStatus _VI_FUNC IdeaSetAmp			( ViInt16 CardNo, ViInt16 Val );
ViStatus _VI_FUNC IdeaSetBcrtEvent		( ViInt16 CardNo, IDEA_BCRT_INTERRUPT_HANDLER fnHandler );
ViStatus _VI_FUNC IdeaSetCommandVi		( ViPUInt16 Command, ViUInt16 Wcnt, ViUInt16 Subadr,
											ViUInt16 Tr, ViUInt16 Tadr );
ViStatus _VI_FUNC IdeaSetMessageVi		(ViInt16 CardNo, ViInt16 MessageId, 
											ViInt16 CommType, ViUInt16 Cmd1, ViUInt16 Cmd2,
											ViUInt16 TimeToNextMessage, ViInt16 data_table_no,
											ViInt16 last_data_table_no, ViInt16 Bus,
											ViInt16 FirstIntermessageRoutine,
											ViInt16 SecondIntermessageRoutine,
											ViInt16 DetError, ViInt16 RxStatus, ViInt16 TxStatus,
											ViInt16 ErrorType, ViInt16 SubError_1,
											ViInt16 SubError_2, ViInt16 SubError_3 );
ViStatus _VI_FUNC IdeaSetMonEvent		( ViInt16 CardNo, IDEA_MON_INTERRUPT_HANDLER fnHandler );
ViStatus _VI_FUNC IdeaSetBroadcast		( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetBroadcast	( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetBusy			( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetBusy			( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetDbcAcceptance	( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetDbcAcceptance( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetInstrumentation( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetInstrumentation( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetMessageError	( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetMessageError	( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetReservedBit	( ViInt16 CardNo, ViInt16 Tadr, ViInt16 TrBit, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetReservedBit	( ViInt16 CardNo, ViInt16 Tadr, ViInt16 TrBit, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetSerq			( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetSerq			( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetSubsystem		( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetSubsystem	( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetTerminalFlag	( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaResetTerminalFlag	( ViInt16 CardNo, ViInt16 Tadr, ViBoolean Immediate );
ViStatus _VI_FUNC IdeaSetRespTimeout	( ViInt16 CardNo, ViInt16 ResponseTimeout );
ViStatus _VI_FUNC IdeaSetRtc			( ViInt16 CardNo, ViUInt32 Rtc );
/*ViStatus _VI_FUNC IdeaSetStartErrorResolution( ViInt16 CardNo, ViInt16 Resolution );*/
ViStatus _VI_FUNC IdeaSkipNextMessage	( ViInt16 CardNo, ViBoolean YesNo );
ViStatus _VI_FUNC IdeaStartInitIdea		( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaWriteModeData		( ViInt16 CardNo, ViInt16 TableId,
											ViInt16 ModeCode, ViInt16 Data );
ViStatus _VI_FUNC IdeaGetBcHaltParameters( ViInt16 CardNo, ViPInt32 RemainTimes,
											ViPInt16 FramePosition,
											ViPInt16 EmulatedBcTadr );
ViStatus _VI_FUNC IdeaShutdown			( ViInt16 CardNo );
/*ViStatus _VI_FUNC IdeaDefMonMap		( ViInt16 CardNo, ViInt16 Tadr, ViInt16 Tr,
											ViInt16 Subadr, ViInt16 TableId );
ViStatus _VI_FUNC IdeaGetSetupValue		( ViPChar FileName, ViPChar Key, ViPChar Buffer, 
											ViInt16 BufLength );
ViStatus _VI_FUNC IdeaDecodeMonError	( IDEA_PMON_MSG decoded_message  );*/

ViStatus _VI_FUNC IdeaEnableDma			( ViInt16 CardNo , ViInt32 Size );
ViStatus _VI_FUNC IdeaFlushDmaBuffer	( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaDisableDma		( ViInt16 CardNo );
ViStatus _VI_FUNC IdeaEnableIrig		( ViInt16 CardNo , ViBoolean Enable );
ViStatus _VI_FUNC IdeaSetIrig			( ViInt16 CardNo , ViInt16 Day, ViInt16 Hour, ViInt16 Minute, ViInt16 Second );
ViStatus _VI_FUNC IdeaReadIrig			( ViInt16 CardNo , ViPInt16 IrigMissing, ViPInt16 Day, ViPInt16 Hour, ViPInt16 Minute, ViPInt16 Second );
ViStatus _VI_FUNC IdeaReadAmp			( ViInt16 CardNo , ViPInt16 Amplitude );
ViStatus _VI_FUNC IdeaSetCoupling		( ViInt16 CardNo , ViInt16 Coupling, ViInt16 Termination );
ViStatus _VI_FUNC IdeaReadCoupling		( ViInt16 CardNo , ViPInt16 Coupling );
#endif /* VIFUNC_H */

