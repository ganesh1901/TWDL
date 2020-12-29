/*
 * DDC1553.cpp
 *
 *  Created on: Jun 6, 2019
 *      Author: Aviral
 */

#include <DDC1553.h>
using namespace DDC1553_global;



static void PrintOutError(S16BIT nResult)
{
    char buf[80];
    aceErrorStr(nResult, buf, 80);
    printf("RTL Function Failure-> %s.\n", buf);
}


DDC1553::~DDC1553(){

	printf(" at destructor \n");
	aceBCStop(DevNum);
	aceFree(DevNum);

}

DDC1553::DDC1553()
{
	DevNum = 0;
	dwMsgCount = 0;
	dwHBufLost = 0x00000000;
	wMsgGapTime = 100;
	wFrmTime = 1000;
	SubaddCount= 0;
	wSelection = 0;
	nResult = 0;
	wResult = 0;
	wRepeatCount = 0;
	pCmdWrd = 0;

	for(int i=0; i<MAX_SUBADDRESS; i++)
	{
		MsgId[i] = 350+i;
		BlkId[i] = 450+i;
		XeqOpcode[i] = 550+i;
	}
	

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
	Subaddress[SubaddCount++] = 28;
	Subaddress[SubaddCount++] = 29;
	Subaddress[SubaddCount++] = 25;
	
	

}

DDC1553 *DDC1553::getDDC1553Instance()
{
	if (ddc1553_instance==NULL)
	{
		ddc1553_instance=new DDC1553();
	}
	return ddc1553_instance;
}



void DDC1553::setReadArray(char *tmp, int size){

	memcpy(&ReadArray, tmp, size);

}

int DDC1553::initDDC()
{
	DevNum = 0;

	wResult = aceInitialize(DevNum, ACE_ACCESS_CARD, ACE_MODE_BC, 0, 0, 0);
	if (wResult)
	{
		//printf("ERROR ace Initialization Error\n");
		return FAILED;
	}
	else
	{
		printf("DDC1553 Initialized\n");
	}
	aceBCInstallHBuf(DevNum, 24* 40 * 256);
	
	
	wResult = aceBCConfigure(DevNum, ACE_BC_ASYNC_HMODE); //ACE_BC_ASYNC_BOTH); //ACE_BC_ASYNC_HMODE);
	if (wResult)
	{
		return FAILED;
	}

	aceBCSetMsgRetry(DevNum, ACE_RETRY_TWICE, ACE_RETRY_SAME, ACE_RETRY_ALT, 0);  //For sending data on redundant channel
	//aceBCSetMsgRetry(DevNum, ACE_RETRY_ONCE, ACE_RETRY_SAME, ACE_RETRY_ALT, 0);  //For sending data on redundant channel 
	createFrames();

	return SUCCESS;
}

void DDC1553::createFrames()
{
	DDC1553::createMessage();
	for(int i=0; i<SubaddCount; i++)
	{
		cout<<"val=="<<Subaddress[i]<<endl;
	}

	DDC1553::createMajorFrame4();
}

void DDC1553::createMessage()
{
	for(int i=0; i<SubaddCount; i++)
	{
		wResult=aceBCDataBlkCreate(DevNum, BlkId[i], 32, wBuffer, 32);
		if(wResult){
			printf("aceBCDataBlkCreate DBLK1 ");
			PrintOutError(wResult);
		}
		


			/* Create message block RT-BC */
			nResult = aceBCMsgCreateRTtoBC(
				DevNum,              			/* Device number                    */
				MsgId[i],             				/* Message ID to create             */
				BlkId[i],          				/* Message will use this data block */
				1,               				/* RT address                       */
				Subaddress[i],             				/* RT subaddress                    */
				32,           				/* Word count                       */
				0,           				/* Default message timer            */
				//ACE_BCCTRL_CHL_A       /* use chl A options                */
				ACE_BCCTRL_CHL_A|ACE_BCCTRL_RETRY_ENA       /* use chl A options                */
			);

			if(nResult)
			{
				printf(" Error at aceBCMsgCreateBCtoRT \n");
				PrintOutError(nResult);
			}

			nResult = aceBCOpCodeCreate(DevNum, XeqOpcode[i], ACE_OPCODE_XEQ, ACE_CNDTST_ALWAYS, MsgId[i], 0, 0);
			if(nResult)
			{
				printf(" Error at XEQ aceBCOpCodeCreate for OP2 \n");
				PrintOutError(nResult);
			}
			printf(" Xeq--code -- %d\n", XeqOpcode[i]);


	}

}


void DDC1553::createMajorFrame4(){
	int msg10msec[]={MSG_EMA, MSG_OBC_SURV, MSG_INS_17};
	int msg20msec[]={MSG_INS_LEV, MSG_INS_18, MSG_28, MSG_29};
	int msg100msec[]={MSG_TWDL_PktA, MSG_TWDL_PktC};
	int msg1sec[]={MSG_TWDL_Health, MSG_TWDL_Status, MSG_INS_19, MSG_INS_20, MSG_INS_21, MSG_INS_22};



	S16BIT MinorCode[400];
	S16BIT MinorOpcode[400];

		S16BIT Opcode[400][3];
		int i, l=0;

		for(unsigned long ii=0; ii<sizeof MinorCode / sizeof MinorCode[0]; ii++){
					MinorCode[ii] = 1000 + ii;
					MinorOpcode[ii] = 2000 + ii;

				}


		for(i=0; i<400; i++)
		{
			Opcode[i][0] = XeqOpcode[MSG_INS_SURV];;
			Opcode[i][1] = XeqOpcode[MSG_DUMMY];
			Opcode[i][2] = XeqOpcode[MSG_DUMMY];
		}

		//for 10 msec
		for(i=0; i<400; i=i+4)
		{
			for(unsigned long k=0; k<(sizeof(msg10msec)/ sizeof(msg10msec[0])); k++)
			{
				Opcode[i+k][1] = XeqOpcode[msg10msec[k]];

			}

		}

		//for 20 msec
		for(i=0; i<400; i=i+8)
		{
			for(unsigned long k=0; k<(sizeof(msg20msec)/ sizeof(msg20msec[0])); k++)
			{
				Opcode[i+k][2] = XeqOpcode[msg20msec[k]];
				//printf("%d\t\t%d\t\t",(i+k),Opcode[i+k][2]);
			}
			//printf("\n");
		}

		int sizeof20msecmsg=sizeof(msg20msec)/ sizeof(msg20msec[0]);
		int sizeof100msecmsg=sizeof(msg100msec)/ sizeof(msg100msec[0]);
		int sizeof1secmsg = sizeof(msg1sec) / sizeof(msg1sec[0]);

		//for 100 msec & 1 Sec
		for(i=0; i<400; i=i+40)
		{
			for(unsigned long  k=0; k<(sizeof(msg100msec)/ sizeof(msg100msec[0])); k++)
			{
				Opcode[i+ sizeof20msecmsg+ k][2] = XeqOpcode[msg100msec[k]];
			}

			if(l<sizeof1secmsg){
				Opcode[i+ sizeof20msecmsg+ sizeof100msecmsg][2] = XeqOpcode[msg1sec[l]];
				l+=1;
			}
		}

//		for(i=0; i<400; i++)
//		{
//			for(j=0; j<3; j++)
//			{
//				printf("%d\t\t",Opcode[i][j]);
//			}
//			printf("\n");
//		}

		for(int jj=0; jj<400; jj++){

			nResult = aceBCFrameCreate(DevNum, MinorCode[jj], ACE_FRAME_MINOR, Opcode[jj], 3, 22, 0);
			if(nResult)
			{
				printf(" Error Minor Frame --%d [%d]  %p \n", MinorCode[jj], jj, Opcode[jj]);
				PrintOutError(nResult);
			}

			nResult = aceBCOpCodeCreate(DevNum, MinorOpcode[jj], ACE_OPCODE_CAL, ACE_CNDTST_ALWAYS, MinorCode[jj], 0, 0);
			if(nResult)
			{
				printf(" Error at CAL aceBCOpCodeCreate %d [%d] %d\n", nResult, jj, MinorCode[jj]);
				PrintOutError(nResult);
			}

		}

		/* 1 sec major frame*/
		nResult = aceBCFrameCreate(DevNum, MJR4, ACE_FRAME_MAJOR, MinorOpcode, sizeof MinorCode / sizeof MinorCode[0], 25, 0); /* 100 Usec resolutiondd*/
		if(nResult){
			printf(" Error at aceBCFrameCreate for MAJOR 4 \n");
			PrintOutError(nResult);
		}

		nResult=aceBCStart(DevNum, MJR4, -1);
		if(nResult < 0)
		{
			printf("Error in aceBCStart() function \n");
			PrintOutError(nResult);
		}
		else{
			printf("Major frame 4 scheduled\n");
		}

}


void DDC1553::scheduleAsyncReceiveMsg(config_params_t *p1)
{

	printf(" At Sending async msg -- rt--%d, subaddr--%d \n", p1->Rtnum , p1->Subaddr);
	wResult = aceBCAsyncMsgCreateBCtoRT(
		DevNum,             			/* Device number                    */
		p1->MsgID,               			/* Message ID to create             */
		p1->DataBlk,              			/* Data Block ID                    */
		p1->Rtnum,                  			/* RT address                       */
		p1->Subaddr,                  			/* RT subaddress                    */
		p1->wordcount,                 			/* Word Count                       */
		0,                  			/* Devault Message Timer            */
		//ACE_BCCTRL_CHL_A,  /* use chl A options                */
		ACE_BCCTRL_CHL_A|ACE_BCCTRL_RETRY_ENA,  /* use chl A options                */
		wBuffer	
	);

	if(wResult){
		printf(" Error at aceBCAsyncMsgCreateBCtoRT  ");
		PrintOutError(wResult);
	}

	nResult = aceBCDataBlkWrite(DevNum, p1->DataBlk, p1->payload, sizeof p1->payload, 0);
	if(nResult){
		printf(" Error at aceBCDataBlkWrite  ");
		PrintOutError(nResult);
	}

	wResult = aceBCSendAsyncMsgHP(DevNum, p1->MsgID, wFrmTime);
	if (wResult < 0)
	{
		printf("Error occurred in aceBCSendAsyncMsgHP function\n");
		PrintOutError(wResult);
	}

	nResult = aceBCMsgDelete(DevNum, p1->MsgID);
	if(nResult){
		printf(" Error at  aceBCMsgDelete ");
		PrintOutError(nResult);
	}

	nResult = aceBCDataBlkDelete(DevNum, p1->DataBlk);
	if(nResult){
		printf(" Error at  aceBCDataBlkDelete ");
		PrintOutError(nResult);
	}
}



void DDC1553::scheduleAsyncTransmitMsg(config_params_t *p1)
{

	int counter = 0;

	wResult = aceBCAsyncMsgCreateRTtoBC(
		DevNum,             			/* Device number                    */
		p1->MsgID,              		/* Message ID to create             */
		p1->DataBlk,              		/* Data Block ID                    */
		p1->Rtnum,                  	/* RT address                       */
		p1->Subaddr,                 	/* RT subaddress                    */
		p1->wordcount,                 	/* Word Count                       */
		0,                  			/* Devault Message Timer            */
		ACE_BCCTRL_CHL_A|ACE_BCCTRL_RETRY_ENA,  /* use chl A options                */
		p1->payload
		);

	if(wResult){
		printf("Error occurred in aceBCAsyncMsgCreateRTtoBC [%d] function\n", p1->Subaddr);
		PrintOutError(wResult);

	}

	wResult = aceBCSendAsyncMsgHP(DevNum, p1->MsgID, wFrmTime);
	if (wResult < 0)
	{
		printf("Error occurred in aceBCSendAsyncMsgHP function\n");
		PrintOutError(wResult);
	}


	do{
			/* Check host buffer for msgs */
			nResult = aceBCGetMsgFromIDDecoded(DevNum, p1->MsgID, &sMsg, FALSE);
			if (nResult < 0 ){
				printf("Error in aceBCGetMsgFromIDDecoded() function \n");
				PrintOutError(nResult);
			}
			else if(nResult == 1){
				p1->CommandWord = sMsg.wCmdWrd1;
				memcpy(p1->payload, &sMsg.aDataWrds, sizeof p1->payload);
			}
			counter++;

	 }while( (nResult != 1) && (counter < 100));



	U16BIT wRT, wTR1, wSA, wWC;
	/* Display command word info */
	wResult = aceCmdWordParse(sMsg.wCmdWrd1, &wRT, &wTR1, &wSA, &wWC);
	if(wResult){
		printf(" aceCmdWordParse error \n");
		PrintOutError(wResult);
	}

	p1->Rtnum = wRT;
	p1->Subaddr = wSA;
	p1->wordcount = wWC;
	p1->CommandWord = sMsg.wCmdWrd1;
	memcpy(p1->payload, &sMsg.aDataWrds, sizeof p1->payload);

	nResult = aceBCMsgDelete(DevNum, p1->MsgID);
	if(nResult){
		printf(" Error at  aceBCMsgDelete ");
		PrintOutError(nResult);

	}
	nResult = aceBCDataBlkDelete(DevNum, p1->DataBlk);
	if(nResult){
		printf(" Error at  aceBCDataBlkDelete ");
		PrintOutError(nResult);
	}

}


void DDC1553::getUpdatedData(config_params_t *p1, int index)
{

	MSGSTRUCT l1;
	nResult = 0;
	int counter = 0;

	do{
			/* Check host buffer for msgs */
			nResult = aceBCGetMsgFromIDDecoded(DevNum, MsgId[index], &l1, FALSE);
			if (nResult < 0 ){
				printf("Error in aceBCGetMsgFromIDDecoded() function \n");
				PrintOutError(nResult);
			}

			else if(nResult == 1){
				p1->CommandWord = l1.wCmdWrd1;
				memcpy(p1->payload, &l1.aDataWrds, sizeof p1->payload);
			}
			counter++;

	 }while(nResult != 1 && counter < 20);//while(nResult != 1 && counter<25);

}

void DDC1553::DDCStateChange(unsigned short Majornum){

    aceBCStop(DevNum);
    aceBCStart(DevNum, Majornum, -1);

}


void DDC1553::check()
{
	cout <<"DDC Object Created"<<endl;
}

