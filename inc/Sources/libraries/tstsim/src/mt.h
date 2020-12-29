#ifndef MT_H
#define MT_H

typedef struct {
	      S16BIT code,
		    illegal,
		    rx_status,
		    tx_status,
		    type,
		    word_type,
		    word_number;
	       } ERR_ST;

#ifdef VME
typedef struct com {				/* COMMAND bit fields */
			 unsigned tadr : 5;
			 unsigned tr   : 1;
			 unsigned sadr : 5;
		    unsigned wcnt : 5;
		   } COM_ST;
#else
typedef struct com {		       /* COMMAND bit fields */
		    unsigned wcnt : 5;
		    unsigned sadr : 5;
		    unsigned tr   : 1;
		    unsigned tadr : 5;
#ifdef __BORLANDC__
			unsigned PAD  :  16;
#endif
		   } COM_ST;
#endif

typedef struct tagMON_MSG {
	     S16BIT  comm_type,
		    bus,
		    capture,
		    broadcast;
	     S32BIT   rtc;
	     S16BIT  cmd_1,
		    cmd_2,
		    rx_status,
		    tx_status,
		    word_count,
		    aux_word_count,
		    error;

	     S16BIT
		    *data_buf,
		    *next_msg;

	     ERR_ST error_inf;
	 struct com s_cmd_1,
		    s_cmd_2;
	      S16BIT cmd_1_flag,
		    cmd_2_flag,
		    stat_t_flag,
		    stat_r_flag;
		  S32BIT IrigTime;
	       } MON_MSG;


/* MONITOR  STACK */

#define     CYCLIC_STACK            1
#define     NON_CYCLIC_STACK        0

/* MONITOR  CAPTURE */

#define   CAPTURE_IMMEDIATE          0
#define   CAPTURE_COMMAND_TEMPLATE   1
#define   CAPTURE_EXCEPTION          2
#define   CAPTURE_TRIGGER            3

#define   BUS_OPEN                   TRUE
#define   BUS_CLOSE                  FALSE

#define WRONG_RT            1
#define EXCEPTION_BIT       2
#define TYPE_NO_RESPONSE    1
#define TYPE_EARLY_RESPONSE 2
#define TYPE_GAP            3
#define TYPE_MANCHESTER     4
#define TYPE_LONG_WORD      5
#define TYPE_SHORT_WORD     6
#define TYPE_PARITY         7 
#define TYPE_EXTRA_WORD     8
#define TYPE_INVERSE        9
#define WORD_CMD            1
#define WORD_RX             2
#define WORD_TX             3    
#define WORD_DATA           4

/* globals */
extern U16BIT last_read_mon_message_ptr;

Error_t __DECL decode_mon_message(U16BIT *buf,MON_MSG *msg);
Error_t __DECL capture_event(U16BIT type,U16BIT par1, U16BIT par2);
Error_t __DECL capture_occurred(S8BIT *flag);
Error_t __DECL def_mon_exception_status(U16BIT val);
Error_t __DECL def_int_mask_monitor(U16BIT maskreg);
Error_t __DECL def_monitor_stack(U16BIT type);
Error_t __DECL halt_mon(void);
Error_t __DECL read_last_mon_message(U16BIT *buf,U16BIT *count);
Error_t __DECL read_mon_stack(S16BIT *buf,S16BIT *count);
Error_t __DECL read_mon_stack_part(S16BIT *buf,S16BIT *count,S16BIT *messages);
Error_t __DECL read_mon_rtc(U32BIT *val);
Error_t __DECL reset_mon(S8BIT *bcrt_file,S8BIT *mon_file,S8BIT *imrlib_file);
Error_t __DECL run_mon(void);
Error_t __DECL select_bus(U16BIT bus_a_state,U16BIT bus_b_state);
Error_t __DECL select_all_messages(U16BIT tadr);
Error_t __DECL deselect_all_messages(U16BIT tadr);
Error_t __DECL select_message(U16BIT tadr,U16BIT tr,U16BIT sadr);
Error_t __DECL deselect_message(U16BIT tadr,U16BIT tr,U16BIT sadr);
Error_t __DECL ddcDecode_mon_message(Device_p pCrd,U16BIT *buf,MON_MSG *msg);
Error_t __DECL ddcCapture_event(Device_p pCrd,U16BIT type,U16BIT par1, U16BIT par2);
Error_t __DECL ddcCapture_occurred(Device_p pCrd,S8BIT *flag);
Error_t __DECL ddcDef_mon_exception_status(Device_p pCrd,U16BIT val);
Error_t __DECL ddcDef_int_mask_monitor(Device_p pCrd, U16BIT maskreg);
Error_t __DECL ddcDef_monitor_stack(Device_p pCrd,U16BIT type);
Error_t __DECL ddcHalt_mon(Device_p pCrd);
Error_t __DECL ddcRead_last_mon_message(Device_p pCrd,U16BIT *buf,U16BIT *count);
Error_t __DECL ddcRead_mon_stack(Device_p pCrd,S16BIT *buf,S16BIT *count);
Error_t __DECL ddcRead_mon_stack_part(Device_p pCrd,S16BIT *buf,S16BIT *count,S16BIT *messages);
Error_t __DECL ddcRead_mon_rtc(Device_p pCrd,U32BIT *val);
Error_t __DECL ddcReset_mon(Device_p pCrd,S8BIT *bcrt_file,S8BIT *mon_file,S8BIT *imrlib_file);
Error_t __DECL ddcRun_mon(Device_p pCrd);
Error_t __DECL ddcSelect_bus(Device_p pCrd,U16BIT bus_a_state,U16BIT bus_b_state);
Error_t __DECL ddcSelect_all_messages(Device_p pCrd,U16BIT tadr);
Error_t __DECL ddcDeselect_all_messages(Device_p pCrd,U16BIT tadr);
Error_t __DECL ddcSelect_message(Device_p pCrd,U16BIT tadr,U16BIT tr,U16BIT sadr);
Error_t __DECL ddcDeselect_message(Device_p pCrd,U16BIT tadr,U16BIT tr,U16BIT sadr);
Error_t __DECL _message_select(Device_p pCrd,U16BIT tadr,U16BIT tr,U16BIT sadr,U16BIT select,U16BIT all_bits);
Error_t __DECL ddcGetMTCounters(Device_p pCrd,U32BIT *invalid, U32BIT *total, U16BIT *buff);
Error_t __DECL GetMTCounters(U32BIT *invalid, U32BIT *total, U16BIT *buff);
Error_t __DECL ddcEnableDma(Device_p pCrd, U32BIT dwSize);
Error_t __DECL ddcDisableDma(Device_p pCrd);
Error_t __DECL ddcFlushDmaBuffer(Device_p pCrd);
Error_t __DECL ddcEnableIrig(Device_p pCrd, U16BIT Enable);
Error_t __DECL ddcSetIrig(Device_p pCrd, S16BIT Day, S16BIT Hour, S16BIT Minute, S16BIT Second);
Error_t __DECL ddcReadIrig(Device_p pCrd, S16BIT *IrigMissing, S16BIT *Day, S16BIT *Hour, S16BIT *Minute, S16BIT *Second);
Error_t __DECL ddcReadDmaStack(Device_p pCrd,S16BIT *buf,S32BIT *count);

#endif /* MT_H */

