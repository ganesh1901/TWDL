#include<iostream>
#include<DIP.h>

using namespace std;



DIP::DIP()
{
	slotToStructMap.insert(pair<int, struct cblk470 *> (65, &c_block470_1));
	slotToStructMap.insert(pair<int, struct cblk470 *> (66, &c_block470_2));
	slotToStructMap.insert(pair<int, struct cblk470 *> (67, &c_block470_3));
	slotToStructMap.insert(pair<int, struct cblk470 *> (68, &c_block470_4));
}

DIP *DIP::getDipInstance()
{
  if (dip_instance==NULL)
  {
  	dip_instance=new DIP();
  }
  
  return dip_instance;
}

int DIP::initDI()
{

	return DIP::initDI_SLOT(&c_block470_3, 1,  'C', &baddr4703);
	//DIP::initDI_SLOT(&c_block470_4, 1,  'D', &baddr4704);
}

int DIP::initDI_SLOT(struct  cblk470 *cblk, int devInstance, char slot_letter, long *board_address)
{
		cblk->param = 0;          /* parameter mask */
		cblk->e_mode = 0;         /* e_mode flag */
		cblk->mask_reg = 0;       /* output mask register */
		cblk->deb_control = 0;    /* debounce control register */
		cblk->deb_clock = 1;      /* debounce control register */
		cblk->enable = 0;         /* interrupt enable register */
		cblk->vector = VECTOR;    /* interrupt vector */

		cblk->bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
		cblk->bInitialized = FALSE;/* indicate not ready to talk to IP */
		cblk->slotLetter = slot_letter;
		cblk->nHandle = 0;	/* make handle to a closed carrier board */

	    for(int i = 0; i != 2; i++)
	    {
	    	cblk->ev_control[i] = 0;/* event control registers */
	    	cblk->deb_duration[i] = 0;/* debounce duration registers */
	    }

	    /*
	        Initialize the Carrier library
	*/
	    if(InitCarrierLib() != S_OK)
	    {
	        //printf("\nUnable to initialize the carrier library. Exiting program.\n");
	        //exit(0);
		return FAILED;
	    }

	    /*
	        Open an instance of a carrier devices
	*/
	    if(CarrierOpen(devInstance, &cblk->nHandle) != S_OK)
	    {
	        //printf("\nUnable to Open instance of carrier.\n");
		return FAILED;
	    }
	    else
	    {

		    cout<<"Carrier opened for DIP slot "<<slot_letter<<endl;

	    }


	    GetCarrierAddress(cblk->nHandle, board_address);	/* Read back carrier address */
	    printf("DIP address: %lX\n",*board_address);


	    SetCarrierAddress(cblk->nHandle, *board_address);	/* Set Carrier Address */
	    if(CarrierInitialize(cblk->nHandle) == S_OK)/* Initialize Carrier */
	    {
	    	cblk->bCarrier = TRUE;
	        SetInterruptLevel(cblk->nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
	    }

	    cblk->slotLetter = slot_letter;

	    /*
	Get the IPACK's base address based on the Slot letter,
	and initialize the IPACK's data structure with the returned address
	*/
	    if(GetIpackAddress(cblk->nHandle, cblk->slotLetter, board_address) != S_OK)
	    {
	        //printf("\nUnable to Get Ipack Address.\n");
	        cblk->bInitialized = false;
		return FAILED;
	    }
	    else
	    {
	    	cblk->brd_ptr = (struct map470 *)(*board_address);
	    	cblk->bInitialized = TRUE;
		return SUCCESS;
	    }

}


long DIP::read_DI(int slot, int port, int ch_no)
{
    long status=-1;        /* returned status of driver routines */

    if(!slotToStructMap[slot]->bInitialized)
    {
        printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
    }
    else
    {
    	cout<<"Initialized    "<<slotToStructMap[slot]<<"    "<<&c_block470_3<<endl;
        status = rpnt470(slotToStructMap[slot], port, ch_no);
        if(status == -1)
            printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
        else
            printf("\nValue of port %d point %d: %lx\n",port, ch_no, status);


    }
    return status;
}

void DIP::check()
{
	cout<<"DIP Object Created"<<endl;
}


