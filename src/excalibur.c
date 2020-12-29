#include <excalibur.h>



excalibur *excalibur::getexcaliburInstance()
{
	if (excalibur_instance == NULL)
	{
		excalibur_instance = new excalibur();
	}
	return excalibur_instance;
}

excalibur::excalibur(){
	module = device = status = 0;
	local_devconf = (excalibur_devconf_t *)malloc(sizeof excalibur_devconf_t);


	Subaddress[SubaddCount++] = 1;  //levelling(0)

	Subaddress[SubaddCount++] = 5;  //EMA(1)

	Subaddress[SubaddCount++] = 8;  //OBC Surveillance  (2)


	Subaddress[SubaddCount++] = 4; //TWDL   (3-6)
	Subaddress[SubaddCount++] = 12;
	Subaddress[SubaddCount++] = 13;
	Subaddress[SubaddCount++] = 14;

	Subaddress[SubaddCount++] = 16; //Navigation(16-22) (7-13)
	Subaddress[SubaddCount++] = 17;
	Subaddress[SubaddCount++] = 18;

	Subaddress[SubaddCount++] = 19;
	Subaddress[SubaddCount++] = 20;
	Subaddress[SubaddCount++] = 21;
	Subaddress[SubaddCount++] = 22;
	Subaddress[SubaddCount++] = 23;



}


int excalibur::initExcalibur(excalibur_devconf_t *excalibur_devconf){

		module = excalibur_devconf->modulenum;
		device = excalibur_devconf->devicenum;

		printf("\n");

		printf("Calling Init_Module_Px with device %d module %d\n\n", device, module);

		status = Init_Module_Px(device, module);

		if (status < 0)
		{

			//Dump_ErrorInfo(Err_DevNotFound);
			//Get_Error_String_Px(status, errstr);
			printf("Init_Module_Px Failure. %s == %d \n", errstr,status);
			excalibur_devconf->errinfonum = status;
			return FAILED;
		}

		excalibur_devconf->handle = status;


		status = Set_Mode_Px (excalibur_devconf->handle, BC_RT_MODE);
		if (status < 0)
		{
			printf("Set_Mode returned error: %s\n",Print_Error_Px(status));
			printf("Press any key to exit the program ... ");
			Release_Module_Px(excalibur_devconf->handle);
		}

		status = Get_Mode_Px (excalibur_devconf->handle);
		if (status == BC_RT_MODE) /*this will be true */
			printf ("Board is configured as Bus Controller concurrent RT(s)\n");
		else if (status == RT_MODE)
			printf ("Board is configured as Remote Terminal(s)\n");
		else if (status == MON_BLOCK)
			printf ("Board is configured as Monitor in Sequential Mode\n");
		else if (status == MON_LOOKUP)
			printf ("Board is configured as Monitor in Table Lookup Mode\n");
		else if (status == BC_MODE)
			printf ("Board is configured as Bus Controller\n");
		else{
			printf ("Bad configuration = %x\n",status);
			return FAILED;
		}

		/* and finally we get the board revision level */
		status = Get_Rev_Level_Px (excalibur_devconf->handle);
		printf("Board revision level is %d.%d\n",status/16,status%16);

		memcpy(local_devconf, excalibur_devconf, sizeof local_devconf);


		CreateMessage();
		CreateReceiveFrame();
		CreateTransmitFrame();
		CreateMajorframe1();
		CreateMajorframe2();
		CreateMajorframe3();
		CreateMajorframe4();
		CreateMajorframe5();
		Go();
		return SUCCESS;

}


void excalibur::Go(){

	excalibur_state = major_frameid[4];

	status = Start_Frame_Px(local_devconf->handle, excalibur_state, FULLFRAME);
	if(status <  0){
		printf(" Error at Start_Frame_Px -- %d staus -- %d \n", excalibur_state, status);
	}

	status = Set_Frame_Time_Px(local_devconf->handle, 10l);
	if(status <  0){
		printf(" Error at Set_Frame_Time_Px -- staus -- %d \n", status);
	}

	status = Run_BC_Px(local_devconf->handle, 0);
	if(status < 0){
		printf(" Error at Run_BC_Px -- staus -- %d \n", status);
	}

}

void excalibur::CreateMessage(){
	unsigned short buffer[32] = {0};
	unsigned short commnad_word = 0;

	for(int kk = 0; kk < SubaddCount-1; kk++ ){
		commnad_word = 0;

		status = Command_Word_Px(DEFAULT_RTNUM, 1, Subaddress[kk], DEFAULT_WORDCOUNT, &commnad_word);
		if(status < 0){
			printf("ERROR  Command_Word_Px at %d  status = %d \n", SubaddCount[kk], status);
		}
		else{
			printf(" Comnmand word -- %04X \n", commnad_word);
			buffer[0] = commnad_word;
		}

		status = Create_1553_Message_Px(local_devconf->handle, 1, &buffer[0], &Msgid[kk]);
		if(status < 0){
			printf(" ERROR Create_1553_Message_Px -- %d status -- %d\n", kk, status);
		}
		else{
			printf(" Message PX -- %d \n", Msgid[kk]);
		}

	}


	status = Command_Word_Px(DEFAULT_RTNUM, RT2BC, DEFAULT_SUBADDRESS, DEFAULT_WORDCOUNT, &commnad_word);
	if(status < 0){
		printf("ERROR  Command_Word_Px at Transmit command word %d \n", status);
	}
	else{
		printf(" Comnmand word -- %04X \n", commnad_word);
		buffer[0] = commnad_word;
	}

	status = Create_1553_Message_Px(local_devconf->handle, RT2BC, &buffer[0], &transmit_msgid);
	if(status < 0){
		printf(" ERROR Create_1553_Message_Px -- transmit status -- %d\n", status);
	}
	else{
		printf(" Message PX -- %d \n", transmit_msgid);
	}


	//
	status = Command_Word_Px(DEFAULT_RTNUM, BC2RT, DEFAULT_SUBADDRESS, DEFAULT_WORDCOUNT, &commnad_word);
	if(status < 0){
		printf("ERROR  Command_Word_Px at Receive command word %d \n", status);
	}
	else{
		printf(" Comnmand word -- %04X \n", commnad_word);
		buffer[0] = commnad_word;
	}

	status = Create_1553_Message_Px(local_devconf->handle, BC2RT, &buffer[0], &receive_msgid);
	if(status < 0){
		printf(" ERROR Create_1553_Message_Px -- transmit status -- %d\n", status);
	}
	else{
		printf(" Message PX -- %d \n", receive_msgid);
	}

	// Minor frame indication  msgid
	status = Create_1553_Message_Px(local_devconf->handle, MINOR_FRAME, &buffer[0], &minor_id);
	if(status < 0){
		printf(" ERROR Create_1553_Message_Px -- %d status -- %d\n", status);
	}
	else{
		printf(" Message PX -- %d \n", minor_id);
	}

}



void excalibur::CreateMajorframe1(){


	/*OBC Survelliance (10 msec) + INS Survelliance(2.5 msec) + EMA(2.5msec)
	 *
	 * + TWDl-A(10 msec) + TWDL-C(10 msec)
	 */
	time_gap = 0;

	// minor frame1
	frame_st[0].id = minor_id;
	frame_st[0].gaptime = time_gap;

	frame_st[1].id = MSG_EMA;
	frame_st[1].gaptime = time_gap;

	frame_st[2].id = MSG_INS_SURV;
	frame_st[2].gaptime = time_gap;

	frame_st[3].id = MSG_OBC_SURV;
	frame_st[3].gaptime = time_gap;

	// minor frame2
	frame_st[4].id = minor_id;
	frame_st[4].gaptime = time_gap;

	frame_st[5].id = MSG_EMA;
	frame_st[5].gaptime = time_gap;

	frame_st[6].id = MSG_INS_SURV;
	frame_st[6].gaptime = time_gap;

	frame_st[7].id = MSG_TWDL_PktA;
	frame_st[7].gaptime = time_gap;

	// minor frame3
	frame_st[8].id = minor_id;
	frame_st[8].gaptime = time_gap;

	frame_st[9].id = MSG_EMA;
	frame_st[9].gaptime = time_gap;

	frame_st[10].id = MSG_INS_SURV;
	frame_st[10].gaptime = time_gap;

	frame_st[11].id = MSG_TWDL_PktC;
	frame_st[11].gaptime = time_gap;


	// minor frame4
	frame_st[12].id = minor_id;
	frame_st[12].gaptime = time_gap;

	frame_st[13].id = MSG_EMA;
	frame_st[13].gaptime = time_gap;

	frame_st[14].id = MSG_INS_SURV;
	frame_st[14].gaptime = time_gap;

	frame_st[15].id = 0;

	major_frameid[0] = Create_Frame_Px(local_devconf->handle, &frame_st[0]);
	if(major_frameid[0] < 0){
		printf(" Error Create_Frame_Px 1 -- %d \n", major_frameid[0]);
	}
	else{
		printf(" major Frame 1 frameid -- %d \n",major_frameid[0]);
	}
}

void excalibur::CreateMajorframe2(){


	/*OBC Survelliance (10 msec) + INS Survelliance(2.5 msec) + INS LEV(10 msec)
	 *
	 * + TWDl-A(10 msec) + TWDL-C(10 msec)
	 */
	time_gap = 0;

	// minor frame1
	frame_st[0].id = minor_id;
	frame_st[0].gaptime = time_gap;

	frame_st[1].id = MSG_INS_LEV;
	frame_st[1].gaptime = time_gap;

	frame_st[2].id = MSG_INS_SURV;
	frame_st[2].gaptime = time_gap;

	frame_st[3].id = MSG_OBC_SURV;
	frame_st[3].gaptime = time_gap;

	// minor frame2
	frame_st[4].id = minor_id;
	frame_st[4].gaptime = time_gap;

	frame_st[5].id = MSG_TWDL_PktA;
	frame_st[5].gaptime = time_gap;

	frame_st[6].id = MSG_INS_SURV;
	frame_st[6].gaptime = time_gap;


	// minor frame3
	frame_st[7].id = minor_id;
	frame_st[7].gaptime = time_gap;

	frame_st[8].id = MSG_INS_SURV;
	frame_st[8].gaptime = time_gap;

	frame_st[9].id = MSG_TWDL_PktC;
	frame_st[9].gaptime = time_gap;


	// minor frame4
	frame_st[10].id = minor_id;
	frame_st[10].gaptime = time_gap;

	frame_st[11].id = MSG_INS_SURV;
	frame_st[11].gaptime = time_gap;

	frame_st[12].id = 0;

	major_frameid[1] = Create_Frame_Px(local_devconf->handle, &frame_st[0]);
	if(major_frameid[1] < 0){
		printf(" Error Create_Frame_Px 2 -- %d \n", major_frameid[1]);
	}
	else{
		printf(" major Frame 2 frameid -- %d \n",major_frameid[1]);
	}
}

void excalibur::CreateMajorframe3(){


	/*OBC Survelliance (10 msec) + INS Survelliance(2.5 msec) + INS NAV(10 msec)
	 *
	 * + TWDl-A(10 msec) + TWDL-C(10 msec)
	 */
	time_gap = 0;

	// minor frame1
	frame_st[0].id = minor_id;
	frame_st[0].gaptime = time_gap;

	frame_st[1].id = MSG_INS_17;
	frame_st[1].gaptime = time_gap;

	frame_st[2].id = MSG_INS_SURV;
	frame_st[2].gaptime = time_gap;

	frame_st[3].id = MSG_OBC_SURV;
	frame_st[3].gaptime = time_gap;

	// minor frame2
	frame_st[4].id = minor_id;
	frame_st[4].gaptime = time_gap;

	frame_st[5].id = MSG_INS_18;
	frame_st[5].gaptime = time_gap;

	frame_st[6].id = MSG_INS_SURV;
	frame_st[6].gaptime = time_gap;

	frame_st[7].id = MSG_TWDL_PktA;
	frame_st[7].gaptime = time_gap;

	// minor frame3
	frame_st[8].id = minor_id;
	frame_st[8].gaptime = time_gap;

	frame_st[9].id = MSG_INS_SURV;
	frame_st[9].gaptime = time_gap;

	frame_st[10].id = MSG_TWDL_PktC;
	frame_st[10].gaptime = time_gap;


	// minor frame4
	frame_st[11].id = minor_id;
	frame_st[11].gaptime = time_gap;

	frame_st[12].id = MSG_EMA;
	frame_st[12].gaptime = time_gap;

	frame_st[13].id = MSG_INS_SURV;
	frame_st[13].gaptime = time_gap;

	frame_st[14].id = 0;

	major_frameid[2] = Create_Frame_Px(local_devconf->handle, &frame_st[0]);
	if(major_frameid[2] < 0){
		printf(" Error Create_Frame_Px 3 -- %d \n", major_frameid[2]);
	}
	else{
		printf(" major Frame 3 frameid -- %d \n",major_frameid[2]);
	}
}

void excalibur::CreateMajorframe4(){


	/*OBC Survelliance (10 msec) + INS Survelliance(2.5 msec)
	 *
	 * + TWDl-A(10 msec) + TWDL-C(10 msec)
	 */
	time_gap = 0;

	// minor frame1
	frame_st[0].id = minor_id;
	frame_st[0].gaptime = time_gap;

	frame_st[1].id = MSG_INS_SURV;
	frame_st[1].gaptime = time_gap;

	frame_st[2].id = MSG_OBC_SURV;
	frame_st[2].gaptime = time_gap;

	frame_st[3].id = MSG_TWDL_PktA;
	frame_st[3].gaptime = time_gap;

	// minor frame2
	frame_st[4].id = minor_id;
	frame_st[4].gaptime = time_gap;

	frame_st[5].id = MSG_INS_SURV;
	frame_st[5].gaptime = time_gap;

	frame_st[6].id = MSG_TWDL_PktC;
	frame_st[6].gaptime = time_gap;


	// minor frame3
	frame_st[7].id = minor_id;
	frame_st[7].gaptime = time_gap;

	frame_st[8].id = MSG_INS_SURV;
	frame_st[8].gaptime = time_gap;



	// minor frame4
	frame_st[9].id = minor_id;
	frame_st[9].gaptime = time_gap;

	frame_st[10].id = MSG_INS_SURV;
	frame_st[10].gaptime = time_gap;

	frame_st[11].id = 0;

	major_frameid[3] = Create_Frame_Px(local_devconf->handle, &frame_st[0]);
	if(major_frameid[3] < 0){
		printf(" Error Create_Frame_Px 4 -- %d \n", major_frameid[3]);
	}
	else{
		printf(" major Frame 4 frameid -- %d \n",major_frameid[3]);
	}
}


void excalibur::CreateMajorframe5(){
	/*OBC Survelliance (10 msec) + INS Survelliance(2.5 msec)
		 *
		 * + TWDl-A(10 msec) + TWDL-C(10 msec) + navigation(10 msec) + EMA( 5 msec)
		 */
		time_gap = 0;

		// minor frame1
		frame_st[0].id = minor_id;
		frame_st[0].gaptime = time_gap;

		frame_st[1].id = MSG_INS_SURV;
		frame_st[1].gaptime = time_gap;

		frame_st[2].id = MSG_OBC_SURV;
		frame_st[2].gaptime = time_gap;

		frame_st[3].id = MSG_EMA;
		frame_st[3].gaptime = time_gap;

		// minor frame2
		frame_st[4].id = minor_id;
		frame_st[4].gaptime = time_gap;

		frame_st[5].id = MSG_INS_SURV;
		frame_st[5].gaptime = time_gap;

		frame_st[6].id = MSG_TWDL_PktA;
		frame_st[6].gaptime = time_gap;

		frame_st[7].id = MSG_EMA;
		frame_st[7].gaptime = time_gap;


		// minor frame3
		frame_st[8].id = minor_id;
		frame_st[8].gaptime = time_gap;

		frame_st[9].id = MSG_INS_SURV;
		frame_st[9].gaptime = time_gap;

		frame_st[10].id = MSG_INS_17;
		frame_st[10].gaptime = time_gap;


		// minor frame4
		frame_st[11].id = minor_id;
		frame_st[11].gaptime = time_gap;

		frame_st[12].id = MSG_INS_SURV;
		frame_st[12].gaptime = time_gap;

		frame_st[13].id = MSG_INS_18;
		frame_st[13].gaptime = time_gap;

		frame_st[14].id = MSG_TWDL_PktC;
		frame_st[14].gaptime = time_gap;

		frame_st[15].id = 0;

		major_frameid[4] = Create_Frame_Px(local_devconf->handle, &frame_st[0]);
		if(major_frameid[4] < 0){
			printf(" Error Create_Frame_Px 5 -- %d \n", major_frameid[4]);
		}
		else{
			printf(" major Frame 5 frameid -- %d \n",major_frameid[4]);
		}

}

void excalibur::CreateReceiveFrame(){

	time_gap = 0;
	frame_st[0].id = receive_msgid;
	frame_st[0].gaptime = time_gap;

	receive_frameid = Create_Frame_Px(local_devconf->handle, &frame_st[0]);
	if(receive_frameid < 0){
		printf(" Error Create_Frame_Px receive_frameid-- %d \n", receive_frameid);
	}
	else{
		printf(" major Frame receive_frameid  frameid -- %d \n",receive_frameid);
	}



}

void excalibur::CreateTransmitFrame(){

	time_gap = 0;
	frame_st[0].id = transmit_msgid;
	frame_st[0].gaptime = time_gap;

	transmit_frameid = Create_Frame_Px(local_devconf->handle, &frame_st[0]);
	if(transmit_frameid < 0){
		printf(" Error Create_Frame_Px transmit_frameid-- %d \n", transmit_frameid);
	}
	else{
		printf(" major Frame transmit_frameid frameid -- %d \n",transmit_frameid);
	}


}

void excalibur::ExcaliburStateChange(int  frameid){

	excalibur_state = frameid;

	Stop_Px(local_devconf->handle);

	status = Start_Frame_Px (local_devconf->handle, frameid,FULLFRAME);

#ifdef DEBUG
	printf(" Start_Frame_Px status == %d \n",status);
#endif

	status = Run_BC_Px(local_devconf->handle, 0);

#ifdef DEBUG
	printf(" Run_BC_Px returns %d  frame_id %d \n",status,Transaction->frame_id);
#endif



}

void excalibur::scheduleAsyncReceiveMsg(config_params_t *p1){

	/* to change the command word respective with message id */
	status = Alter_Cmd_Px(local_devconf->handle, receive_msgid, p1->Rtnum, p1->Subaddr);
#ifdef DEBUG
		printf("Alter_Cmd_Px status == %d \n",status);
#endif

	/* to change the msg data respective with message id*/
	status = Alter_Message_Px(local_devconf->handle, receive_msgid, &p1->payload[0]);

#ifdef DEBUG
		printf(" Alter_Message_Px %d == %d\n",status,Transaction->message_id);
#endif

	status = Select_Async_Frame_Px(local_devconf->handle, receive_frameid, 1);
	if (status < 0)
		printf("Error returned by call to Select_Async_Frame  Receive --%d\n", status);

	status = Send_Async_Frame_Px(local_devconf->handle);
	if (status < 0)
		printf("Error returned by call to Send_Async_Frame_Px receive  %d\n", status);


}


void excalibur::scheduleAsyncTransmitMsg(config_params_t *p1){


	/* to change the command word respective with message id */
	status = Alter_Cmd_Px(local_devconf->handle, transmit_msgid, p1->Rtnum, p1->Subaddr);
#ifdef DEBUG
		printf("Alter_Cmd_Px status == %d \n",status);
#endif

	status = Select_Async_Frame_Px(local_devconf->handle, transmit_frameid, 1);
	if (status < 0){
		printf("Error returned by call to Select_Async_Frame  Transmit --%d\n", status);
	}

	do {
			status = Get_Msgentry_Status_Px(local_devconf->handle, excalibur_state, 1, &statusword);
 	}while ((statusword & END_OF_MSG) != END_OF_MSG);

	status = Send_Async_Frame_Px(local_devconf->handle);
	if (status < 0){
		printf("Error returned by call to Send_Async_Frame_Px Transmit  %d\n", status);

	}
	memset(p1->payload, 0x00, sizeof p1->payload);

	status = Read_Message_Px(local_devconf->handle, transmit_msgid, &p1->payload[0]);

#ifdef DEBUG
		printTransmitMessage(p1->payload[0],sizeof p1->payload[0]);
#endif

}


void excalibur::getUpdatedData(config_params_t *p1, int msgid){

	do {
		status = Get_Msgentry_Status_Px(local_devconf->handle, excalibur_state, Msgid[msgid], &statusword);
 	}while ((statusword & END_OF_MSG) != END_OF_MSG);

	memset(p1->payload, 0x00, sizeof p1->payload);

	status = Read_Message_Px(local_devconf->handle, transmit_msgid, &p1->payload[0]);

#ifdef DEBUG
	printTransmitMessage(p1->payload[0],sizeof p1->payload[0]);
#endif
}

excalibur::~excalibur()
{
	 /* Stop the card */
	Stop_Px (local_devconf->handle);
	Release_Module_Px(local_devconf->handle);

}

void excalibur::printTransmitMessage(usint *msgdata, int wordCount)
{
	int i,j;
	
	if (wordCount == 0) {
		wordCount = 32;
	}
				
	printf("\n");
	printf("Command word is %04X, Message Status Word is %04X\n", msgdata[0], msgdata[1]);

	for (i=0; i<4; i++)	{
		printf("%04X     ", i*8);

		for (j=0; j<8; j++) {
			if (((i*8) + j) >= wordCount) {
				printf("\n");
			}
			else {
				printf("%04X  ", msgdata[(i*8)+j +2]);  /* the +2 is to get past the CW and MSW */
			}
		}
		printf("\n");
	}

}

