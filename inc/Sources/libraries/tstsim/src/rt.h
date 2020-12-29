#ifndef RT_H
#define RT_H

typedef struct _tagRT_DEFS      {               /* RT_DEFS typedef */
	   S16BIT     basic_status;  /* basic status */
	   INJ_ERR   *inj_error;    /* INJ_ERR - see drv_msg.in */

	   S16BIT     dbca;          /* bus control acceptance */
       S32BIT      dbc_delay;     /* delay before activating BC */
       } RT_DEFS;

typedef struct _tagRT_DEFS_NEW  {               /* RT_DEFS typedef */
	   S16BIT     basic_status;  /* basic status */

	   S16BIT     dbca;          /* bus control acceptance */
           S32BIT      dbc_delay;     /* delay before activating BC */
	   INJ_ERR   inj_error;     /* INJ_ERR - see drv_msg.in */
           } RT_DEFS_NEW;

/* defines */
#define TL_RT_IMR_INT_AFTER_MODE    0x00400000
#define TL_RT_IMR_INT_MODE_MATCH    0x00100000

/* globals */
extern U16BIT last_read_time_tag;

Error_t __DECL def_emulate_rt(S16BIT tadr,S16BIT emulate);
Error_t __DECL def_last_cmd(S16BIT tadr,U16BIT val);
Error_t __DECL def_last_status(S16BIT tadr,U16BIT val);
Error_t __DECL def_mode_routine(S16BIT mode_code,S16BIT im_routine_1,S16BIT im_routine_2);
Error_t __DECL def_rt(S16BIT tadr,struct _tagRT_DEFS *rt);
Error_t __DECL def_rt_map(S16BIT tadr,S16BIT tr_bit,S16BIT sadr,S16BIT table_id);
Error_t __DECL def_rt_map_block(S16BIT tadr,S16BIT tr_bit,S16BIT sadr,S16BIT first_table_id, S16BIT last_table_id);
Error_t __DECL read_last_status_cmd(S16BIT tadr,U16BIT *last_status,U16BIT *last_cmd);
Error_t __DECL read_time_tags(void *buf,S16BIT *count);
Error_t __DECL set_broadcast(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_broadcast(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_busy(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_busy(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_dbc_acceptance(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_dbc_acceptance(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_instrumentation(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_instrumentation(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_message_error(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_message_error(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_serq(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_serq(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_subsystem(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_subsystem(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_terminal_flag(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_terminal_flag(S16BIT tadr,S16BIT immediate);
Error_t __DECL set_reserved_bit(S16BIT tadr,S16BIT bit,S16BIT immediate);
Error_t __DECL reset_reserved_bit(S16BIT tadr,S16BIT bit,S16BIT immediate);
Error_t __DECL set_busy_reset_srq(S16BIT tadr,S16BIT immediate);
Error_t __DECL reset_busy_set_srq(S16BIT tadr,S16BIT immediate);
Error_t __DECL run_rt(void);
Error_t __DECL read_mode_data(S16BIT tadr,S16BIT mode_code,S16BIT *val);
Error_t __DECL write_mode_data(S16BIT tadr,S16BIT mode_code,S16BIT val);
Error_t __DECL def_legality_bcst(S16BIT legality);
Error_t __DECL def_legality_detection(S16BIT detect);
Error_t __DECL def_mode_legality(S16BIT mode_code,S16BIT legality);
Error_t __DECL set_rtc(U32BIT val);
Error_t __DECL def_table_routine(S16BIT id,S16BIT im_routine_1,S16BIT im_routine_2);
Error_t __DECL def_int_cmd_template(U16BIT maskwd,U16BIT cmpwd);
Error_t __DECL ddcDef_emulate_rt(Device_p pCrd,S16BIT tadr,S16BIT emulate);
Error_t __DECL ddcDef_last_cmd(Device_p pCrd,S16BIT tadr,U16BIT val);
Error_t __DECL ddcDef_last_status(Device_p pCrd,S16BIT tadr,U16BIT val);
Error_t __DECL ddcDef_mode_routine(Device_p pCrd,S16BIT mode_code,S16BIT im_routine_1,S16BIT im_routine_2);
Error_t __DECL ddcDef_rt(Device_p pCrd,S16BIT tadr,struct _tagRT_DEFS *rt);
Error_t __DECL ddcDef_rt_map(Device_p pCrd,S16BIT tadr,S16BIT tr_bit,S16BIT sadr,S16BIT table_id);
Error_t __DECL ddcDef_rt_map_block(Device_p pCrd,S16BIT tadr,S16BIT tr_bit,S16BIT sadr,S16BIT start_table_id, S16BIT end_table_id);
Error_t __DECL ddcRead_last_status_cmd(Device_p pCrd,S16BIT tadr,U16BIT *last_status,U16BIT *last_cmd);
Error_t __DECL ddcRead_time_tags(Device_p pCrd,void *buf,S16BIT *count);
Error_t __DECL ddcSet_broadcast(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_broadcast(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_busy(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_busy(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_dbc_acceptance(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_dbc_acceptance(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_instrumentation(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_instrumentation(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_message_error(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_message_error(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_serq(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_serq(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_subsystem(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_subsystem(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_terminal_flag(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_terminal_flag(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcSet_reserved_bit(Device_p pCrd,S16BIT tadr,S16BIT bit,S16BIT immediate);
Error_t __DECL ddcReset_reserved_bit(Device_p pCrd,S16BIT tadr,S16BIT bit,S16BIT immediate);
Error_t __DECL ddcSet_busy_reset_srq(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcReset_busy_set_srq(Device_p pCrd,S16BIT tadr,S16BIT immediate);
Error_t __DECL ddcRun_rt(Device_p pCrd);
Error_t __DECL ddcRead_mode_data(Device_p pCrd,S16BIT tadr,S16BIT mode_code,S16BIT *val);
Error_t __DECL ddcWrite_mode_data(Device_p pCrd,S16BIT tadr,S16BIT mode_code,S16BIT val);
Error_t __DECL ddcDef_legality_bcst(Device_p pCrd,S16BIT legality);
Error_t __DECL ddcDef_legality_detection(Device_p pCrd,S16BIT detect);
Error_t __DECL ddcDef_mode_legality(Device_p pCrd,S16BIT mode_code,S16BIT legality);
Error_t __DECL ddcSet_rtc(Device_p pCrd,U32BIT val);
Error_t __DECL ddcDef_table_routine(Device_p pCrd,S16BIT id,S16BIT im_routine_1,S16BIT im_routine_2);
Error_t __DECL ddcDef_int_cmd_template(Device_p pCrd,U16BIT maskwd,U16BIT cmpwd);
Error_t __DECL ddcSynchronizeAllCards();
#endif /* RT_H */

