/* BC.H (HARDWARE INTERFACE MODULE) */

#ifndef BC_H
#define BC_H

Error_t __DECL def_bc_exception_status(U16BIT val);
Error_t __DECL def_emulate_bc(S16BIT tadr,S16BIT emulate);
Error_t __DECL def_frame(S16BIT length,U8BIT *frame);
Error_t __DECL def_frame_time(S16BIT length,U8BIT *frame,U32BIT *frame_time);
Error_t __DECL def_message(S16BIT id,struct tagMESSAGE *message);
Error_t __DECL def_minor_frame_time(S32BIT val);
Error_t __DECL get_bc_halt_parameters(S32BIT *rem_times,S16BIT *frame_pos,S16BIT *bc_tadr);
Error_t __DECL halt_bcrt(void);
Error_t __DECL insert_message(S16BIT pos);
Error_t __DECL read_message(S16BIT id,struct tagMESSAGE *message);
Error_t __DECL run_bc(S16BIT position,S32BIT times);
Error_t __DECL run_continue(void);
Error_t __DECL skip_next_message(S16BIT yes_no);
Error_t __DECL def_table_size(S16BIT id,S16BIT size);
Error_t __DECL read_data(S16BIT id,S16BIT *buf,S16BIT count);
Error_t __DECL write_data(S16BIT id,S16BIT *buf,S16BIT count,S16BIT pos);
Error_t __DECL def_int_mask_bcrt(U16BIT maskreg);
Error_t __DECL def_int_mask(U16BIT maskreg);
Error_t __DECL def_data_buffering(S16BIT type);
Error_t __DECL ddcDef_bc_exception_status(Device_p pCrd,U16BIT val);
Error_t __DECL ddcDef_emulate_bc(Device_p pCrd,S16BIT tadr,S16BIT emulate);
Error_t __DECL ddcDef_frame(Device_p pCrd,S16BIT length,U16BIT *frame);
Error_t __DECL ddcDef_frame_time(Device_p pCrd,S16BIT length,U16BIT *frame,U32BIT *frame_time);
Error_t __DECL ddcDef_message(Device_p pCrd,S16BIT id,struct tagMESSAGE *message);
Error_t __DECL ddcDef_minor_frame_time(Device_p pCrd,S32BIT val);
Error_t __DECL ddcGet_bc_halt_parameters(Device_p pCrd,S32BIT *rem_times,S16BIT *frame_pos,S16BIT *bc_tadr);
Error_t __DECL ddcHalt_bcrt(Device_p pCrd);
Error_t __DECL ddcInsert_message(Device_p pCrd,S16BIT pos);
Error_t __DECL ddcRead_message(Device_p pCrd,S16BIT id,struct tagMESSAGE *message);
Error_t __DECL ddcRun_bc(Device_p pCrd,S16BIT position,S32BIT times);
Error_t __DECL ddcRun_continue(Device_p pCrd);
Error_t __DECL ddcSkip_next_message(Device_p pCrd,S16BIT yes_no);
Error_t __DECL ddcDef_table_size(Device_p pCrd,S16BIT id,S16BIT size);
Error_t __DECL ddcRead_data(Device_p pCrd,S16BIT id,S16BIT *buf,S16BIT count);
Error_t __DECL ddcRead_data_part(Device_p pCrd,S16BIT id,S16BIT *buf,S16BIT count,S16BIT pos);
Error_t __DECL ddcWrite_data(Device_p pCrd,S16BIT id,S16BIT *buf,S16BIT count,S16BIT pos);
Error_t __DECL ddcDef_int_mask_bcrt(Device_p pCrd,U16BIT maskreg);
Error_t __DECL ddcDef_int_mask(Device_p pCrd,U16BIT maskreg);
Error_t __DECL ddcDef_data_buffering(Device_p pCrd,S16BIT type);
Error_t	__DECL get_bc_data_table(S16BIT message_id, S16BIT t_r, S16BIT *data_table_number);
Error_t	__DECL ddcGet_bc_data_table(Device_p pCrd, S16BIT message_id, S16BIT t_r, S16BIT *data_table_number);
#endif /* BC_H */

