#include<DOP.h>
#include<iostream>
using namespace std;


DOP::DOP()
{
	slotToStructMap.insert(pair<int, struct sblk445 *> (65, &s_block445_1));
	slotToStructMap.insert(pair<int, struct sblk445 *> (66, &s_block445_2));
	slotToStructMap.insert(pair<int, struct sblk445 *> (67, &s_block445_3));
	slotToStructMap.insert(pair<int, struct sblk445 *> (68, &s_block445_4));

}

DOP *DOP::getDopInstance()
{
	if (dop_instance==NULL)
	{
		dop_instance=new DOP();
	}
	return dop_instance;
}



int DOP::initDo()
{

	//DOP::initDO_SLOT(&s_block445_1, nDevInstance, SLOT_A, 'A', &baddr4451);
	//DOP::initDO_SLOT(&s_block445_1, nDevInstance+1, SLOT_B, 'B', &baddr4452);
	return (DOP::initDO_SLOT(&s_block445_3, 0, SLOT_C, 'C', &baddr4453));
	//DOP::initDO_SLOT(&s_block445_4, 2, SLOT_D, 'D', &baddr4454);
}

int DOP::initDO_SLOT(struct sblk445 *sblk, int devInstance, int slot, char slot_letter, long *board_address)
{
		sblk->bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
		sblk->bInitialized = FALSE;	/* indicate not ready to talk to IP */
		sblk->slotLetter = slot_letter;
		sblk->nHandle = 0;		/* make handle to a closed carrier board */


	    if(InitCarrierLib() != S_OK)
	    {
	        //printf("\nUnable to initialize the carrier library for %c slot. Exiting program.\n", slot_letter);
	    	return FAILED;
	        //exit(0);
	    }
	    else
	    {
	    	cout<<"Carrier Initialize for DOP slot "<<slot_letter<<endl;

	    }

	    if(CarrierOpen(devInstance, &sblk->nHandle) != S_OK)
	    {
	        return FAILED;
	    }

	    GetCarrierAddress(sblk->nHandle, board_address);	/*  Read back carrier address */
	    //printf("DOP address: %lx\n",baddr4453);

	    SetCarrierAddress(sblk->nHandle, *board_address);	/* Set Carrier Address */
	    if(CarrierInitialize(sblk->nHandle) == S_OK)		/* Initialize Carrier */
	    {
	    	sblk->bCarrier = TRUE;

	    }

	    sblk->slotLetter = slot_letter;

	    /*
	Get the IPACK's base address based on the Slot letter,
	and initialize the IPACK's data structure with the returned address
	*/
	    if(GetIpackAddress(sblk->nHandle, sblk->slotLetter, board_address) != S_OK)
	    {
	        //printf("\nUnable to Get IP Address.\n");
	        sblk->bInitialized = false;
		return FAILED;
	    }
	    else
	    {
	    	sblk->brd_ptr = (struct map445 *)(*board_address);
	    	sblk->bInitialized = true;
		return SUCCESS;
	    }
}




void DOP::writeToDo_SLOT(int slot ,int port_Num, int ch_no, int on_off)
{
    int port, channel;

    port = port_Num;
    channel = ch_no % 8;
    int status;

    status = wpnt445(slotToStructMap[slot], port, (unsigned)channel, (unsigned)on_off);
 

    if(status == -1 )
    {
        cout<<"Written"<<endl;
    }

    else
    {
    	cout<<"DOP Write Success "<<endl;
    }

}

long DOP::readDO(int slot, int port, int channel, int on_off)
{
	channel = channel % 8;
	int status;
	status = rpnt445(slotToStructMap[slot], port, (unsigned)channel);
	long ret = (short)status;
	return ret;
}


void DOP::closeDo()
{
    //CSTATUS carrierStatus;
    BOOL carrierFlag;
    CSTATUS carrierStatus;
    carrierFlag = (s_block445_3.bCarrier || s_block445_4.bCarrier);
    if(carrierFlag)
    {
        carrierStatus = CarrierClose(s_block445_3.nHandle);
    }
}

void DOP::check()
{
	cout<<"DOP Object Created"<<endl;
}



