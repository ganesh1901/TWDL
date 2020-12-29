#ifndef VBFUNC_H
#define VBFUNC_H

/************************************************************************/
/*      Type Definitions                                                */
/************************************************************************/

/* structure not containing bit-fields */
/** VB_TYPE **/


/* CMD_VB typedef */
typedef struct {             
    U16BIT  wcnt;
    U16BIT  subadr;
    U16BIT  t_r;
    U16BIT  tadr;
} CMD_VB;

/*used by MON_MSG_VB*/
typedef struct {				/* COMMAND bit fields */ 
	U16BIT tadr;
	U16BIT tr;
	U16BIT sadr;
	U16BIT wcnt;
} com_VB;


typedef struct tagMON_MSG_VB {
	S16BIT		comm_type;
	S16BIT		bus;
	S16BIT		capture;
	S16BIT		broadcast;
	S32BIT		rtc;
	S16BIT		cmd_1;
	S16BIT		cmd_2;
	S16BIT		rx_status;
	S16BIT		tx_status;
	S16BIT		word_count;
	S16BIT		aux_word_count;
	S16BIT		error;
	S16BIT		data_buf[32];
	S16BIT		next_msg;
	ERR_ST		error_inf;

	com_VB		s_cmd_1;
	com_VB		s_cmd_2;

	S16BIT		cmd_1_flag;
	S16BIT		cmd_2_flag;
	S16BIT		stat_t_flag;
	S16BIT		stat_r_flag;
	S32BIT		IrigTime;
} MON_MSG_VB; 



/* MESSAGE of VB typedef */
typedef struct tagMESSAGE_VB{             
   char      comm_type;					/* type of communication */
   
   CMD_VB    cmd_1;						/* command to be tranmitted by card */
   CMD_VB    cmd_2;						/* TX command in case of RT to RT */
   
   unsigned short time_to_next_message; /* time from the start of the present message to the start of  the next message */
   unsigned short data_table_no;		/* data table number */
   unsigned short last_data_table_no;
   
   char bus;							/* bus A/B */
   char	first_intermessage_routine;     /* routines to be */
   char	second_intermessage_routine;    /* executed by Z8002 in the gap following the message */

   INJ_ERR	inj_error_ptr;				/* pointer to a structure defining  error injection */

   unsigned short det_error;			/* detected error - updated by BC */
   unsigned short rx_status;			/* returned RX status - updated by BC */
   unsigned short tx_status;			/* returned TX status - updated by BC */

} MESSAGE_VB;


/************************************************************************/
/*     VB Function Definitions                                          */
/************************************************************************/

Error_t __DECL ddcCapture_event_VB		(S32BIT pCrd, S16BIT type,S16BIT par1, S16BIT par2 );
Error_t __DECL ddcCapture_occurred_VB	(S32BIT pCrd, S8BIT *flag );
Error_t __DECL ddcCardState_VB			(S32BIT pCrd, CARD_STATE *state);
Error_t __DECL ddcDecode_mon_message_VB (S32BIT pCrd, U16BIT *buf,MON_MSG_VB *msg );
Error_t __DECL ddcDef_emulate_bc_VB		(S32BIT pCrd, S16BIT tadr, S16BIT emulate );
Error_t __DECL ddcDef_data_buffering_VB	(S32BIT pCrd, S16BIT type );
Error_t __DECL ddcDef_table_routine_VB	(S32BIT pCrd, S16BIT id, S16BIT im_routine_1,S16BIT im_routine_2 );
Error_t __DECL ddcDef_table_size_VB		(S32BIT pCrd, S16BIT id, S16BIT size );
Error_t __DECL ddcDef_bc_exception_status_VB	(S32BIT pCrd, U16BIT val );
Error_t __DECL ddcDef_mon_exception_status_VB	(S32BIT pCrd, U16BIT val );
Error_t __DECL ddcDef_frame_VB			(S32BIT pCrd, S16BIT length, U16BIT *frame );
Error_t __DECL ddcDef_frame_time_VB		(S32BIT pCrd, S16BIT length, U16BIT *frame, U32BIT *frame_time );
Error_t __DECL ddcDef_mode_routine_VB	(S32BIT pCrd, S16BIT mode_code, S16BIT im_routine_1,S16BIT im_routine_2 );
Error_t __DECL ddcDef_int_cmd_template_VB	(S32BIT pCrd, U16BIT maskwd,U16BIT cmpwd );
Error_t __DECL ddcDef_int_mask_VB		(S32BIT pCrd, U16BIT maskreg );
Error_t __DECL ddcDef_int_mask_bcrt_VB	(S32BIT pCrd, U16BIT maskreg );
Error_t __DECL ddcDef_int_mask_monitor_VB(S32BIT pCrd, U16BIT maskreg );
Error_t __DECL ddcDef_last_cmd_VB		(S32BIT pCrd, S16BIT tadr, U16BIT val );
Error_t __DECL ddcDef_last_status_VB		(S32BIT pCrd, S16BIT tadr, U16BIT val );
Error_t __DECL ddcDef_legality_detection_VB	(S32BIT pCrd, S16BIT detect );
Error_t __DECL ddcDef_legality_bcst_VB	(S32BIT pCrd, S16BIT legality );
Error_t __DECL ddcDef_minor_frame_time_VB	(S32BIT pCrd, S32BIT val );
Error_t __DECL ddcDef_mode_legality_VB	(S32BIT pCrd, S16BIT mode_code,S16BIT legality );
Error_t __DECL ddcDef_monitor_stack_VB	(S32BIT pCrd, S16BIT type );
Error_t __DECL ddcDef_emulate_rt_VB		(S32BIT pCrd, S16BIT tadr, S16BIT emulate );
Error_t __DECL ddcDef_rt_map_VB			(S32BIT pCrd, S16BIT tadr,S16BIT tr_bit, S16BIT sadr, S16BIT table_id );
Error_t __DECL ddcDef_rt_map_block_VB	(S32BIT pCrd, S16BIT tadr, S16BIT tr_bit,S16BIT sadr, S16BIT start_table_id,S16BIT end_table_id );
Error_t __DECL ddcDef_rt_VB				(S32BIT pCrd, S16BIT tadr, RT_DEFS *rt);
Error_t __DECL ddcDeselect_all_messages_VB	(S32BIT pCrd, S16BIT tadr );
Error_t __DECL ddcDeselect_message_VB	(S32BIT pCrd, S16BIT tadr,S16BIT tr, S16BIT sadr );
Error_t __DECL ddcGet_bc_data_table_VB	(S32BIT pCrd, S16BIT message_id, S16BIT t_r,U16BIT *data_table_number );
Error_t __DECL ddcGetErrorMessage_VB	(S32BIT pCrd, Error_t error_number,char *error_string, S16BIT Length );
Error_t __DECL ddcPrintErrorMessage_VB	(S32BIT pCrd, Error_t errcode, S8BIT *user_string );
Error_t __DECL ddcHalt_bcrt_VB			(S32BIT pCrd );
Error_t __DECL ddcHaltIdea_VB			(S32BIT pCrd );
Error_t __DECL ddcHalt_mon_VB			(S32BIT pCrd );
Error_t __DECL ddcInsert_message_VB		(S32BIT pCrd, S16BIT pos );
Error_t __DECL ddcRead_data_VB			(S32BIT pCrd, S16BIT id, S16BIT *buf, S16BIT count );
Error_t __DECL ddcRead_last_mon_message_VB	(S32BIT pCrd, U16BIT *buf, U16BIT *count );
Error_t __DECL ddcRead_last_status_cmd_VB	(S32BIT pCrd, S16BIT tadr, U16BIT *last_status,U16BIT *last_cmd );
Error_t __DECL ddcRead_message_VB		(S32BIT pCrd, S16BIT id, MESSAGE_VB *message );
Error_t __DECL ddcRead_mode_data_VB		(S32BIT pCrd, S16BIT tadr, S16BIT mode_code, S16BIT *val);
Error_t __DECL ddcRead_mon_rtc_VB		(S32BIT pCrd, U32BIT *val );
Error_t __DECL ddcRead_mon_stack_VB		(S32BIT pCrd, S16BIT *buf, S16BIT *count );
Error_t __DECL ddcRead_mon_stack_part_VB	(S32BIT pCrd, S16BIT *buf, S16BIT *count,S16BIT *messages );
Error_t __DECL ddcRead_time_tags_VB		(S32BIT pCrd, void *buf, S16BIT *count );
Error_t __DECL ddcResetCard_VB			(S32BIT *pCrd, DRV_CONFIG *config, U16BIT LogNum);
Error_t __DECL ddcRun_bc_VB				(S32BIT pCrd, S16BIT position, S32BIT times );
Error_t __DECL ddcWrite_data_VB			(S32BIT pCrd, S16BIT id, S16BIT *buf, S16BIT count,S16BIT pos );
Error_t __DECL ddcRun_continue_VB		(S32BIT pCrd );
Error_t __DECL ddcRun_mon_VB			(S32BIT pCrd );
Error_t __DECL ddcRun_rt_VB				(S32BIT pCrd );
Error_t __DECL ddcSelect_all_messages_VB	(S32BIT pCrd, S16BIT tadr );
Error_t __DECL ddcSelect_bus_VB			(S32BIT pCrd, U16BIT bus_a_state, U16BIT bus_b_state );
Error_t __DECL ddcSelect_message_VB		(S32BIT pCrd, S16BIT tadr, S16BIT tr,S16BIT sadr );
Error_t __DECL ddcSetAmp_VB				(S32BIT pCrd, S16BIT val );
Error_t __DECL ddcSetBCRTEvent_VB		(S32BIT pCrd, Error_t(*usr_handler)(S32BIT pCrd, S16BIT type, S16BIT param));
Error_t __DECL ddcDef_message_VB		(S32BIT pCrd,S16BIT id, MESSAGE_VB *message);
Error_t __DECL ddcSetMONEvent_VB		(S32BIT pCrd, Error_t(*usr_handler)(S32BIT pCrd));
Error_t __DECL ddcSet_broadcast_VB		(S32BIT pCrd, S16BIT tadr, S16BIT immediate );
Error_t __DECL ddcReset_broadcast_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate );
Error_t __DECL ddcSet_busy_VB			(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcReset_busy_VB			(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcSet_dbc_acceptance_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcReset_dbc_acceptance_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcSet_instrumentation_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcReset_instrumentation_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcSet_message_error_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcReset_message_error_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcSet_reserved_bit_VB	(S32BIT pCrd, S16BIT tadr, S16BIT bit,S16BIT immediate );
Error_t __DECL ddcReset_reserved_bit_VB	(S32BIT pCrd, S16BIT tadr, S16BIT bit,S16BIT immediate );
Error_t __DECL ddcSet_serq_VB			(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcReset_serq_VB			(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcSet_subsystem_VB		(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcReset_subsystem_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcSet_terminal_flag_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcReset_terminal_flag_VB	(S32BIT pCrd, S16BIT tadr, S16BIT immediate  );
Error_t __DECL ddcSetRespTimeout_VB		(S32BIT pCrd, S16BIT val );
Error_t __DECL ddcSet_rtc_VB			(S32BIT pCrd, U32BIT val );
Error_t __DECL ddcSkip_next_message_VB	(S32BIT pCrd, S16BIT yes_no );
Error_t __DECL ddcWrite_mode_data_VB	(S32BIT pCrd, S16BIT tadr,S16BIT mode_code, S16BIT val );
Error_t __DECL ddcShutDownIdea_VB		(S32BIT pCrd );
Error_t __DECL ddcGet_bc_halt_parameters_VB	(S32BIT pCrd, S32BIT *rem_times,S16BIT *frame_pos,S16BIT *bc_tadr );
Error_t __DECL ddcEnableDma_VB			(S32BIT pCrd , S32BIT dwSize );
Error_t __DECL ddcFlushDmaBuffer_VB		(S32BIT pCrd );
Error_t __DECL ddcDisableDma_VB			(S32BIT pCrd );
Error_t __DECL ddcEnableIrig_VB			(S32BIT pCrd , S16BIT Enable );
Error_t __DECL ddcSetIrig_VB			(S32BIT pCrd , S16BIT Day, S16BIT Hour, S16BIT Minute, S16BIT Second );
Error_t __DECL ddcReadIrig_VB			(S32BIT pCrd ,S16BIT *IrigMissing, S16BIT *Day, S16BIT *Hour, S16BIT *Minute, S16BIT *Second  );
Error_t __DECL ddcReadAmp_VB			(S32BIT pCrd , S16BIT *val );
Error_t __DECL ddcSetCoupling_VB		(S32BIT pCrd , S16BIT Coupling, S16BIT Termination );
Error_t __DECL ddcReadCoupling_VB		(S32BIT pCrd , S16BIT *val );
Error_t __DECL ddcRead_data_part_VB		(S32BIT pCrd, S16BIT id, S16BIT *buf, S16BIT count, S16BIT pos);
Error_t __DECL ddcGetMTCounters_VB		(S32BIT pCrd, U32BIT *invalid,  U32BIT *total, U16BIT *buff);
Error_t __DECL ddcIdeaVersion_VB		(S32BIT pCrd, char *rtl_ver, char *VXD_ver, char *hardware_ver);
Error_t __DECL ddcReadDmaStack_VB		(S32BIT pCrd,S16BIT *buf,S32BIT *count);
Error_t __DECL ddcReplayReset_VB		(S32BIT pCrd, U16BIT LogNum);
Error_t __DECL ddcReplayFillStack_VB	(S32BIT pCrd, U16BIT StackNum);
Error_t __DECL ddcReplayRunBC_VB		(S32BIT pCrd, const char *file, S32BIT times, U16BIT immediate);
Error_t __DECL ddcReplayRunRTs_VB		(S32BIT pCrd, const char *file, S32BIT times, U16BIT immediate);
Error_t __DECL ddcReplaySetRespDelay_VB	(S32BIT pCrd, U16BIT delay);
Error_t __DECL ddcReplayRTActive_VB		(S32BIT pCrd, U16BIT RTNum, U16BIT Active);
Error_t __DECL ddcResetCardBCRT_VB		(S32BIT pCrd, DRV_CONFIG *config, U16BIT LogNum);
Error_t __DECL ddcResetCardMonitor_VB	(S32BIT pCrd, DRV_CONFIG *config, U16BIT LogNum);
Error_t __DECL ddcSet_busy_reset_srq_VB	(S32BIT pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSetBCRTEventEx_VB     (S32BIT pCrd,Error_t(*usr_handler)(S32BIT pCrd, S16BIT Type, S16BIT SubType, S16BIT Param));
Error_t __DECL ddcSynchronizeAllCards	();
Error_t __DECL ddcReset_busy_set_srq_VB	(S32BIT pCrd, S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_mon_VB			(S32BIT pCrd,S8BIT *bcrt_file,S8BIT *mon_file,S8BIT *imrlib_file); 
#endif /* VBFUNC_H */