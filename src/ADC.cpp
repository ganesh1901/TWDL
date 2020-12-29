/*
 * ADC.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: checkout
 */




#include<ADC.h>

ADC::ADC()
{
	gNumberPmcs = -1;
}

ADC *ADC::getADCInstance()
{
	if (adc_instance==NULL)
	{
		adc_instance=new ADC();
	}
	return adc_instance;
}


PSTATUS ADC::InitPmcLib(void)
{
    int i;				/* General purpose index */

    if( gNumberPmcs == -1)		/* first time used - initialize pointers to 0 */
    {
        gNumberPmcs = 0;		/* Initialize number of Pmcs to 0 */

        /* initialize the pointers to the Pmc data structure */
        for(i = 0; i < MAX_PMCS; i++)
            gpPmcs[i] = 0;		/* set to a NULL pointer */
    }
    return (PSTATUS)S_OK;
}


PSTATUS ADC::PmcOpen(int nDevInstance, int* pHandle, char* devname)
{
		PMCDATA_STRUCT* pPmc;		/* local pointer */
	    unsigned long data[MAX_PMCS];
	    char devnamebuf[64];
	    char devnumbuf[8];

	    *pHandle = -1;		/* set callers handle to an invalid value */

	    if(gNumberPmcs == MAX_PMCS)
	        return E_OUT_OF_PMCS;

	    /* Allocate memory for a new Pmc structure */
	    pPmc = (PMCDATA_STRUCT*)malloc(sizeof(PMCDATA_STRUCT));

	    if(pPmc == 0)
	        return (PSTATUS)E_OUT_OF_MEMORY;

	    pPmc->nHandle = -1;
	    pPmc->bInitialized = FALSE;
	    pPmc->bIntEnabled = FALSE;
	    pPmc->nPmcDeviceHandle = 0;
	    pPmc->lBaseAddress = 0;
	    pPmc->nInteruptID = 0;
	    pPmc->nIntLevel = 0;

	    memset( &pPmc->devname[0], 0, sizeof(pPmc->devname));
	    memset( &devnamebuf[0], 0, sizeof(devnamebuf));
	    memset( &devnumbuf[0], 0, sizeof(devnumbuf));

	    strcpy(devnamebuf, "/dev/");
	    strcat(devnamebuf, devname);
	    sprintf(&devnumbuf[0],"%d",nDevInstance);
	    strcat(devnamebuf, devnumbuf);

	    pPmc->nPmcDeviceHandle = open( devnamebuf, O_RDWR );

	    if( pPmc->nPmcDeviceHandle < 0 )
	    {
	        free((void*)pPmc);		/* delete the memory for this Pmc */
	        return (PSTATUS)ERROR;
	    }
	    strcpy(&pPmc->devname[0], &devnamebuf[0]);	/* save device name */

	    /* Get Base Address of Pmc */
	    memset( &data[0], 0, sizeof(data)); /* no mem if data[x] returns 0 from ioctl() */
	    ioctl( pPmc->nPmcDeviceHandle, 5, &data[0] );		/* get address cmd */
	    pPmc->lBaseAddress = data[nDevInstance];

	    /* Get IRQ Number from Pmc */
	    ioctl( pPmc->nPmcDeviceHandle, 6, &data[0] );		/* get IRQ cmd */
	    pPmc->nIntLevel = ( int )( data[nDevInstance] & 0xFF );

	    AddPmc(pPmc);                  /* call function to add Pmc to array and set handle */
	    *pHandle = pPmc->nHandle;      /* return our addpmc handle */

	    return (PSTATUS)S_OK;
}

void ADC::calibrate(pmc730 *c_blk,int acq_mode,int scanMode,int aChnlNum)
{
		struct ai730 tai730;			/* copy of analog input data structure */
	    struct ai730 *ai730;			/* pointer to saved structure */
	    double f_cor;
	    int data;
	    int j;

	    ai730 = c_blk->ai730ptr;	/* save pointer to configuration source structure */
	    memcpy( &tai730, c_blk->ai730ptr, sizeof( tai730 ) ); /* copy structure */
	    c_blk->ai730ptr = &tai730;	/* insert copied configuration settings into configuration block pointer */

	    /* modify settings in the copied structure to acquire auto-zero or calibration data */
	    c_blk->ai730ptr->FIFO_Reset =  FIFO_WRTPTR_ENABLE;	/* allow reset on trigger */
	    c_blk->ai730ptr->mem_bank_switch = MEM_BANK_DISABLE;


	    c_blk->ai730ptr->int_mode = INT_DIS;		/* disable interrupts */


	    //    c_blk->ai730ptr->acq_mode = mode;			/* input mode */
	    c_blk->ai730ptr->acq_mode = acq_mode;			/* input mode */



	    // c_blk->ai730ptr->scan_mode = BU_CONT;		/* new scan mode */
	    c_blk->ai730ptr->scan_mode = scanMode;			/* input mode */




	    c_blk->ai730ptr->threshold =511;			/* new threshold */
	    c_blk->ai730ptr->trigger = TD_SELECT;		/* no triggering */



	    //    c_blk->ai730ptr->s_channel = 0;
	    //    c_blk->ai730ptr->e_channel = 0;


	    c_blk->ai730ptr->s_channel = aChnlNum;
	    c_blk->ai730ptr->e_channel = aChnlNum;



	    c_blk->ai730ptr->conv_timer = 15;			/* counter */


	    cnfgai( c_blk );							/* configure the board */


	    f_cor = 0.0;
	    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADStartConversion, 1);  /* start scan */
	    while((( input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADControlStatusRegister) & 0x8000) == 0))
	        ;

	    for( j = 0; j < c_blk->ai730ptr->threshold; j++) /* while < threshold get data */
	    {
	        data = input_long(c_blk->nHandle, (long*)&c_blk->brd_ptr->ADMailBoxArray[j]); /* read from board */
	        f_cor += (double)((SDATA)(data & 0xFFFF)); /* strip channel info from data */
	    }
	    f_cor /= (double)j;

	    if( f_cor < 0.0)               /* round */
	        f_cor -= 0.5;
	    else
	        f_cor += 0.5;

	    /* replace original configuration pointer into configuration block pointer */
	    c_blk->ai730ptr = ai730;

	    if(acq_mode == AZ_SELECT)
	        c_blk->ai730ptr->s_az_buf = (SDATA)f_cor;  /* auto-zero data */
	    else
	        c_blk->ai730ptr->s_cal_buf = (SDATA)f_cor; /* calibration data */
}


void ADC::convert(pmc730 *c_blk)
{
		cnfgai(c_blk);		/* configure the board */

	    if(c_blk->ai730ptr->scan_mode == DISABLE) /* stop scan request */
	        return;

	    /* Non-interrupt driven scan modes just poll for the end of scan */
	    /* then the data is moved from the board to the raw data array */
	    if(c_blk->ai730ptr->int_mode == INT_DIS) /* interrupts disabled */
	    {
	        /* If the trigger direction is set to output or the scan mode is external */
	        /* trigger only or disabled , the start conversion register is used to start the scan */
	        switch( c_blk->ai730ptr->trigger )
	        {
	        case TO_SELECT:
	        case EX_TRIG:
	        case TD_SELECT:
	            output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADStartConversion, 1); /* start scan */
	            break;
	        }
	        /*    Wait for scan complete */
	        while((( input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADControlStatusRegister) & 0x8000) == 0))
	            ;
	    }
	    else
	    {
	        printf("blocking_start_convert wait...");
	        fflush(stdout);
	        /* Interrupt driven scan mode will block until the end of scan */
	        /* then the data is moved from the board to the raw data array */
	        /* If the trigger direction is set to output or the scan mode is external */
	        /* trigger only or disabled , the start conversion register is used to start the scan */
	        switch( c_blk->ai730ptr->trigger )
	        {
	        case TO_SELECT:
	        case EX_TRIG:
	        case TD_SELECT:
	            blocking_start_convert(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADStartConversion, 1); /* start scan */
	            break;
	        }
	        printf(" done!\n");
	    }

	    move_data(c_blk);
}

void ADC::cnfgai(pmc730 *c_blk)
{
		int data __attribute__ ((unused));
	 	output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADControlStatusRegister, DISABLE | 0x38); /* stop all scanning */
	    data = input_long(c_blk->nHandle, (long*)&c_blk->brd_ptr->ADMailBoxArray[0]);		/* do read to clear transition status bit, if set */
	    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADControlStatusRegister, build_ai_control(c_blk)); /* initialize control register value */
	    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADCThreshold, c_blk->ai730ptr->threshold); /* initialize control register value */
	    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->ADStartChannel, c_blk->ai730ptr->s_channel);
	    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->ADEndChannel, c_blk->ai730ptr->e_channel);
	    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->ADTimerPrescaler, c_blk->ai730ptr->timer_ps);
	    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ADConversionTimer, c_blk->ai730ptr->conv_timer);
}

void ADC::move_data(pmc730 *c_blk)
{
	 int i, j;
	    int temp_data;

	    /*
	    ENTRY POINT OF ROUTINE:
	    Move data to array
	*/

	    for( j = 0; j < SA_SIZE; j++)
	        c_blk->ai730ptr->s_count[j] = 0;              /* clear counters */

	    for( j = 0; j <= c_blk->ai730ptr->threshold; j++) /* while < threshold get data */
	    {
	        temp_data = input_long(c_blk->nHandle, (long*)&c_blk->brd_ptr->ADMailBoxArray[ j ]); /* read from board */

	        i = (int)(( temp_data & 0x001F0000 ) >> 16);   /* get channel number from data */
	        if(c_blk->ai730ptr->s_count[i] > FIFO_SIZE - 1 )
	            c_blk->ai730ptr->s_count[i] = 0;

	        c_blk->ai730ptr->s_raw_buf[i][c_blk->ai730ptr->s_count[i]++] = (SDATA)temp_data;
	    }
}


int ADC::mccdai(pmc730* c_blk, int range)
{
		int denom;
	    LDATA *lp;
	    SDATA *wp;
	    double i_span;		/* ideal span value */
	    double i_zero;		/* ideal zero value */
	    double calhi;		/* high calibration input voltage */
	    double callo;		/* low calibration input voltage */
	    double slope;		/* slope from equation 2 */
	    double temp;
	    int chan;		/* current channel */
	    int i;				/* index */
	    /*
	    ENTRY POINT OF ROUTINE:
	    Initialize variables
	*/

	    denom = (int)c_blk->ai730ptr->s_cal_buf - (int)c_blk->ai730ptr->s_az_buf;

	    if( denom == 0 ) /* check for divide by zero */
	        return(-1);                              /* if so, abort */

	    /* Select calibration voltages and ideal zero and span values
	 */
	    //    switch(c_blk->ai730ptr->range)
	    switch(range)
	    {
	    case RANGE_3TO3:
	        callo = 0.00;
	        calhi = c_blk->ai730ptr->ref_v[2];
	        /* actual 2.47 V reference */
	        i_zero = -3.3333;
	        i_span = 6.6666;
	        break;

	    case RANGE_0TO5:
	        callo = c_blk->ai730ptr->ref_v[3];
	        /* actual 1.23 V reference */
	        calhi = c_blk->ai730ptr->ref_v[1];
	        /* actual 4.9 V reference */
	        i_zero = 0.0000;
	        i_span = 5.0000;
	        break;

	    case RANGE_5TO5:
	        callo = 0.00;
	        calhi = c_blk->ai730ptr->ref_v[1];
	        /* actual 4.9 V reference */
	        i_zero = -5.0000;
	        i_span = 10.0000;
	        break;

	    case RANGE_0TO10:
	        callo = c_blk->ai730ptr->ref_v[3];
	        /* actual 1.23 V reference */
	        calhi = c_blk->ai730ptr->ref_v[0];
	        /* actual 9.88 V reference */
	        i_zero =  0.0;
	        i_span = 10.0000;
	        break;

	    default:			/* RANGE_10TO10 */
	        callo = 0.00;
	        calhi = c_blk->ai730ptr->ref_v[0];
	        /* actual 9.88 V reference */
	        i_zero = -10.0000;
	        i_span =  20.0000;
	        break;
	    }

	    chan = c_blk->ai730ptr->s_channel; /* start channel/conversion loop */
	    slope =  (calhi - callo) / (double)(denom);

	    while(1)			/* correct data in scan array */
	    {
	        wp = c_blk->ai730ptr->s_raw_buf[chan]; 			/* pointer to buffer */
	        lp =  ( LDATA*)c_blk->ai730ptr->s_cor_buf[chan];			/* pointer to buffer */
	        for( i = 0; i < FIFO_SIZE; i++ ) /* correct all elements */
	        {
	            temp = (((double)65536 * slope) / i_span) * (wp[i] +
	                                                         ((callo - i_zero) / slope) - c_blk->ai730ptr->s_az_buf );

	            if( temp < 0.0)		/* round */
	                temp -= 0.5;
	            else
	                temp += 0.5;

	            lp[i] = (LDATA)temp;
	        }
	        if(++chan > c_blk->ai730ptr->e_channel)                     /* convert channels in array */
	            break;
	    }
	    return(0);
}


void ADC::blocking_start_convert(int nHandle, word *p, word v)
{
    PMCDATA_STRUCT* pPmc;	/*  local carrier */
    unsigned long data[2];

    pPmc = GetPmc(nHandle);
    if(pPmc == NULL)
        return;

    if( p )
    {
        /* place address to write word in data [0]; */
        data[0] = (unsigned long) p;
        /* place value to write @ address data [1]; */
        data[1] = (unsigned long) SwapBytes( v );
        /* pram3 = function: 8=blocking_start_convert */
        write( pPmc->nPmcDeviceHandle, &data[0], 8 );
    }
}

void ADC::AddPmc(PMCDATA_STRUCT* pPmc)
{
    int i, j;			/* general purpose index */
    BOOL bFound;			/* general purpose BOOL */

    for(i = 0; i < MAX_PMCS; i++)	/* Determine a handle for this Pmc */
    {
        bFound = TRUE;
        for(j = 0; j < gNumberPmcs; j++)
        {
            if(i == gpPmcs[j]->nHandle)
            {
                bFound = FALSE;
                break;
            }
        }

        if(bFound)
            break;
    }

    pPmc->nHandle = i;             	/* set new handle */
    gpPmcs[gNumberPmcs] = pPmc;	/* add Pmc to array */
    gNumberPmcs++;

}

PSTATUS ADC::PmcInitialize(int nHandle)
{
    PMCDATA_STRUCT* pPmc;
    PMC_BOARD_MEMORY_MAP* pPmcCard;

    pPmc = GetPmc(nHandle);

    if(pPmc == 0)
        return E_INVALID_HANDLE;

    /*  initialize Pmc */
    pPmcCard = (PMC_BOARD_MEMORY_MAP*)pPmc->lBaseAddress;
    output_word( nHandle, (word*)&pPmcCard->InterruptRegister, (word)APMC_INT_RELEASE);
    pPmc->bInitialized = TRUE;	/* Pmc is now initialized */

    return (PSTATUS)S_OK;
}


void ADC::DeletePmc(int nHandle)
{
    PMCDATA_STRUCT* pPmc;
    int i;

    if(gNumberPmcs == 0)
        return;

    pPmc = 0;			/* initialize pointer to null */
    for(i = 0; i < gNumberPmcs; i++)/* Find Pmc that has this handle */
    {
        if(nHandle == gpPmcs[i]->nHandle)
        {
            pPmc = gpPmcs[i];
            break;
        }
    }
    if(pPmc == 0)			/* return if no Pmc has been found */
        return;

    free((void*)pPmc);		/* delete the memory for this Pmc */

    /* Rearrange Pmc array */
    gpPmcs[i] = gpPmcs[gNumberPmcs - 1];
    gpPmcs[gNumberPmcs - 1] = 0;
    gNumberPmcs--;
}


PSTATUS ADC::PmcClose(int nHandle)
{
    /*  Delete the carrier with the provided handle */
    PMCDATA_STRUCT* pPmc;	/* local pmc pointer */

    pPmc = GetPmc(nHandle);

    if(pPmc == 0)
        return E_INVALID_HANDLE;

    if(pPmc->bInitialized == FALSE)
        return E_NOT_INITIALIZED;

    close( pPmc->nPmcDeviceHandle );
    pPmc->nPmcDeviceHandle = -1;

    DeletePmc(nHandle);		/*  Delete the Pmc with the provided handle */
    return (PSTATUS)S_OK;
}

PMCDATA_STRUCT* ADC::GetPmc(int nHandle)
{
    PMCDATA_STRUCT* pPmc;
    int i;				/* General purpose index */

    for(i = 0; i < gNumberPmcs; i++)/* Find Pmc that has this handle */
    {
        if(nHandle == gpPmcs[i]->nHandle)
        {
            pPmc = gpPmcs[i];
            return pPmc;
        }
    }
    return (PMCDATA_STRUCT*)0;	/* return null */
}




PSTATUS ADC::GetPmcAddress(int nHandle, long* pAddress)
{
	PMCDATA_STRUCT* pPmc;

	pPmc = GetPmc(nHandle);
	if(pPmc == 0)
		return E_INVALID_HANDLE;

	*pAddress = pPmc->lBaseAddress;
	return (PSTATUS)S_OK;
}



void ADC::output_word(int nHandle, word *p, word v)
{
    PMCDATA_STRUCT* pPmc;	/*  local */
    unsigned long data[2];

    pPmc = GetPmc(nHandle);
    if(pPmc == NULL)
        return;

    if( p )
    {
        /* place address to write word in data [0]; */
        data[0] = (unsigned long) p;
        /* place value to write @ address data [1]; */
        data[1] = (unsigned long) SwapBytes( v );
        /* pram3 = function: 1=write8bits,2=write16bits,4=write32bits */
        write( pPmc->nPmcDeviceHandle, &data[0], 2 );
    }
}


void ADC::output_byte(int nHandle, byte *p, byte v)
{
    PMCDATA_STRUCT* pPmc;	/*  local */
    unsigned long data[2];

    pPmc = GetPmc(nHandle);
    if(pPmc == NULL)
        return;

    if( p )
    {
        /* place address to write byte in data [0]; */
        data[0] = (unsigned long) p;
        /* place value to write @ address data [1]; */
        data[1] = (unsigned long) v;
        /* pram3 = function: 1=write8bits,2=write16bits,4=write32bits */
        write( pPmc->nPmcDeviceHandle, &data[0], 1 );
    }
}

long ADC::input_long(int nHandle, long *p)
{
	PMCDATA_STRUCT* pPmc;	/*  local */
	unsigned long data[2];

	pPmc = GetPmc(nHandle);
	if(pPmc == NULL)
		return((long)0);

	if( p )
	{
		/* place address to read word from in data [0]; */
		data[0] = (unsigned long) p;
		/* pram3 = function: 1=read8bits,2=read16bits,4=read32bits */
		read( pPmc->nPmcDeviceHandle, &data[0], 4 );
		return(  SwapLong( (long)data[1] ) );
	}
	return((long)0);
}


word ADC::input_word(int nHandle, word *p)
{
    PMCDATA_STRUCT* pPmc;	/*  local */
    unsigned long data[2];

    pPmc = GetPmc(nHandle);
    if(pPmc == NULL)
        return((word)0);

    if( p )
    {
        /* place address to read word from in data [0]; */
        data[0] = (unsigned long) p;
        /* pram3 = function: 1=read8bits,2=read16bits,4=read32bits */
        read( pPmc->nPmcDeviceHandle, &data[0], 2 );
        return(  SwapBytes( (word)data[1] ) );
    }
    return((word)0);
}




word ADC::SwapBytes( word v )
{
#ifdef SWAP_ENDIAN		/* endian correction if needed */
    word  Swapped;

    Swapped = v << 8;
    Swapped |= ( v >> 8 );
    return( Swapped );
#else				/* no endian correction needed */
    return( v );
#endif /* SWAP_ENDIAN */
}


long ADC::SwapLong( long v )
{
#ifdef SWAP_ENDIAN
    word Swap1, Swap2;
    long Swapped;

    Swap1 = (word)(v >> 16);
    Swap1 = SwapBytes( Swap1 );

    Swap2 = (word)v & 0xffff;
    Swap2 = SwapBytes( Swap2 );

    Swapped = (long)(Swap2 << 16);
    Swapped |= (long)(Swap1 & 0xffff);
    return( Swapped );
#else				/* no endian correction needed */
    return( v );
#endif
}

word ADC::build_ai_control(pmc730 *c_blk)
{
	 word control;		/* board control register */

	    /*
	    ENTRY POINT OF ROUTINE:
	    Build up control word
	*/

	    control = DISABLE;		/* initialize control register value */

	    if(c_blk->ai730ptr->data_format == SB_SELECT) /* select straight binary */
	        control |= 1;

	    switch(c_blk->ai730ptr->trigger)	/* select trigger */
	    {
	    case TI_SELECT:				/* trigger direction (input)*/
	        control |= (1 << 1);
	        break;

	    case TO_SELECT:				/* trigger direction (output)*/
	        control |= (1 << 2);
	        break;
	    }

	    switch(c_blk->ai730ptr->acq_mode)	/* select acquisition mode */
	    {
	    case SE_SELECT:
	        control |= (1 << 3);
	        break;

	    case AZ_SELECT:		/* select auto zero */
	        switch(c_blk->ai730ptr->range)
	        {
	        case RANGE_0TO5:		/* range 0 to 5 */
	        case RANGE_0TO10:	/* range 0 to 10 use 1.23V for zero */
	            control |= (6 << 3);
	            break;
	        default:
	            control |= (7 << 3);
	            break;
	        }
	        break;

	          case CAL_SELECT:		/* calibration mode */
	              switch(c_blk->ai730ptr->range)
	              {
	              case RANGE_5TO5:		/* range -5 to 5 */
	              case RANGE_0TO5:		/* range 0 to 5 */
	                  control |= (4 << 3);
	                  break;
	              case RANGE_10TO10:	/* range 10 to 10 */
	              case RANGE_0TO10:	/* range 0 to 10 */
	                  control |= (3 << 3);
	                  break;
	              default:				/* -3.3 to 3.3 */
	                  control |= (5 << 3);
	                  break;
	              }
	              break;

	          default:			/* differential input is default */
	              break;
	          }

	    control |= (c_blk->ai730ptr->scan_mode << 8); /* scan mode bits */

	    if(c_blk->ai730ptr->timer_en == TIMER_ON)	/* select timer enable */
	        control |= 0x0040;

	    control |= (c_blk->ai730ptr->int_mode << 11); /* interrupt mode bits */

	    if( c_blk->ai730ptr->mem_bank_switch == MEM_BANK_DISABLE ) /* Memory Bank Switching */
	        control |= (1 << 13);

	    if( c_blk->ai730ptr->FIFO_Reset == FIFO_WRTPTR_DISABLE )	/* dont reset on trigger */
	        control |= (1 << 14);

	    return(control);
}


void ADC::WriteCoefficientAccess( struct pmc730 *c_blk,word Address, byte Data, byte Cycle )
{
	    Address <<= 8;				/* position address */
	    Address &= 0x7f00;			/* save only address for now */
	    if( Cycle == WRITE )		/* if write cycle... */
	    {
	        /* Before a write cycle can be started the magic code must first
	                   be written to the coefficient write enable register
	                */
	        output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->CoefficientWriteEnable, MagicCode);
	        Address |= (word)Data;	/* ... OR in the data to write */
	    }
	    else						/* else a read cycle... */
	        Address |= 0x8000;		/* ... set read cycle bit */

	    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->CoefficientAccess, Address); /* do read or write cycle */

	    /* if a write cycle, remove the magic code from the coefficient write enable register */
	    if( Cycle == WRITE )
	        output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->CoefficientWriteEnable, 0);
}


word ADC::ReadCoefficientStatus(struct pmc730 *c_blk )
{
    word data;

    // now read status
    usleep( 12 );				/* sleep time */

    data = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->CoefficientStatus);

    usleep( 12 );				/* sleep time */

    return ((word)data);
}


double ADC::ReadADReferenceValue( pmc730 *c_blk , word Address )
{
    char ValueString[32];
    unsigned int i;
    word data;
    double result;

    memset(ValueString, 0, sizeof(ValueString));

    for( i = 0; i < sizeof(ValueString); i++ )
    {
        /* Read numeric string */
        WriteCoefficientAccess( c_blk, (word)(Address + i), 0, READ );// read data at address 1-Read
        do
        {
            data = ReadCoefficientStatus( c_blk );
        } while( ( data & 1 ) == 0 );				// wait for read complete

        data >>= 8;
        data &= 0x00FF;

        ValueString[i] = (char)data;

        if(ValueString[i] == 0)					// quit if end of string
            break;
    }
    sscanf(ValueString,"%lf",&result);

    return (result);
}

void ADC::GetADReferenceValues(pmc730 *c_blk)
{
    c_blk->ai730ptr->ref_v[0] = ReadADReferenceValue( c_blk, 0x20);	// 9.88
    c_blk->ai730ptr->ref_v[1] = ReadADReferenceValue( c_blk, 0x30);	// 4.94
    c_blk->ai730ptr->ref_v[2] = ReadADReferenceValue( c_blk, 0x40);	// 2.47
    c_blk->ai730ptr->ref_v[3] = ReadADReferenceValue( c_blk, 0x50);	// 1.23
}

int ADC::init_ADC_CARD()
{
	   //unsigned finished;		/* flag to exit program */
	    /*
	      ENTRY POINT OF ROUTINE:
	      INITIALIZATION
	  */

	    //finished = 0;	/* indicate not finished with program */
	    hflag = 0;		/* indicate interrupt handler not installed yet */

	    memset(raw_data, 0, sizeof(raw_data));
	    memset(cor_data, 0, sizeof(cor_data));
	    memset(count, 0, sizeof(count));

	    /*
	      Initialize the Configuration Parameter Block to default values.
	  */
	    c_block.bPmc = FALSE;	/* indicate no initialization set up yet */
	    c_block.bInitialized = FALSE;/* indicate not ready to talk */
	    c_block.nHandle = 0;	/* make handle to a closed board */
	    c_block.ai730ptr = &ai730;		/* analog input section pointer */



	    /* REFERENCE ...*/

	    /*  RANGE_0TO5   0   input range
	        RANGE_5TO5   1	 input range
	        RANGE_10TO10 2	 input range
	        RANGE_0TO10  3	 input range
	        RANGE_3TO3   4	 input range */



	    c_block.ai730ptr->range = 2;		/* default +-5 V */

	// DI_SELECT   0	/* differential select */
	// SE_SELECT   1	/* single ended select */
	// CAL_SELECT	2	/* calibration select */
	// AZ_SELECT   3

	    c_block.ai730ptr->acq_mode = 0;		/* mode */


	//#define DISABLE     0	/* disable scan */
	//#define UN_CONT     1	/* uniform continuous */
	//#define UN_SNGL     2	/* uniform single */
	//#define BU_CONT     3	/* burst continuous */
	//#define BU_SNGL     4	/* burst single */
	//#define EX_TRIG     5  /* */

	    c_block.ai730ptr->scan_mode = 1	;	/* scan mode */

	    //c_block.ai730ptr->s_channel = ui->cmb_start_chnl->currentIndex();			/* start channel */

	    //c_block.ai730ptr->e_channel = ui->cmb_start_chnl->currentIndex();			/* end channel*/


	//#define TC_SELECT   0	/* two's compliment */
	//#define SB_SELECT   1	/* straight binary */


	    c_block.ai730ptr->data_format = 0;		/* A/D data format */


	    c_block.ai730ptr->timer_ps = 0xff;			/* prescaler */

	    c_block.ai730ptr->conv_timer = 0xff;		/* counter */

	    c_block.ai730ptr->timer_en = TIMER_ON;		/* timer on */


	    c_block.ai730ptr->trigger = TD_SELECT;		/* trigger I/O is disabled */


	    c_block.ai730ptr->int_mode = INT_DIS;	 	/* disable interrupt mode */

	    c_block.ai730ptr->s_az_buf =  0;			/* auto zero buffer start */

	    c_block.ai730ptr->s_cal_buf = 0;			/* calibration buffer start */

	    c_block.ai730ptr->control = 0;			/* control register used by read only*/

	    c_block.ai730ptr->threshold = 511;		 	/* threshold */

	    c_block.ai730ptr->FIFO_Reset =  FIFO_WRTPTR_ENABLE;	/* allow reset on trigger */

	    c_block.ai730ptr->mem_bank_switch = MEM_BANK_ENABLE;



	    c_block.ai730ptr->s_count = &count[0];        /* sample counts */
	    for(int j = 0; j < SA_SIZE; j++)
	    {
	        c_block.ai730ptr->s_raw_buf[j] = &raw_data[j][0];   /* raw buffer start */
	        c_block.ai730ptr->s_cor_buf[j] = &cor_data[j][0];   /* corrected buffer start */
	    }

	    /*
	          Initialize the Pmc library
	  */



	    if(InitPmcLib() != S_OK)
	    {
	    	return FAILED;
	        //printf("\nUnable to initialize the Pmc library. Exiting program.\n");
	        //exit(0);
	    }
	    /*
	          Open an instance of a Pmc device
	          Other device instances can be obtained
	          by changing parameter 1 of PmcOpen()
	  */
	    if((PmcOpen(0, &c_block.nHandle, DEVICE_NAME)) != S_OK)
	    {
		return FAILED;
	        //printf("\nUnable to Open instance of Pmc730.\n");
	        //finished = 1;	 /* indicate finished with program */
	    }
	    else
	    {
	        if(PmcInitialize(c_block.nHandle) == S_OK)/* Initialize Pmc */
	        {
	            GetPmcAddress(c_block.nHandle, &addr);	/* Read back address */
	            c_block.brd_ptr = (struct map730 *)addr;
	            c_block.bInitialized = TRUE;
	            c_block.bPmc = TRUE;
	        }
	    }



	    GetADReferenceValues( &c_block );		/* read AD Reference Values */
	    return SUCCESS;
}


float ADC::readADCVolatage(int adcChnlNumber)
{


		c_block.ai730ptr->s_channel = adcChnlNumber;			/* start channel */
		c_block.ai730ptr->e_channel = adcChnlNumber;



		if(!c_block.bInitialized)
		{
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
			//break;
		}

	        calibrate(&c_block,2,1,adcChnlNumber);   /* get auto-zero values */

		/* read calibration data values */
		if(!c_block.bInitialized)
		{
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");

			// break;
		}

		calibrate(&c_block,3,1,adcChnlNumber);

		/* read raw input data values */
		if(!c_block.bInitialized)
		{
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");

			// break;
		}
		if(hflag == 0 && c_block.ai730ptr->int_mode != 0)
		{
			printf("\n>>> ERROR: NO INTERRUPT HANDLERS ATTACHED <<<\n");

			// break;
		}
		convert(&c_block);                /* convert the board */

		mccdai(&c_block, 2);       /* correct input data */

		double voltage = display_ADC_Result(adcChnlNumber);

		memset(raw_data, 0, sizeof(raw_data));
		memset(cor_data, 0, sizeof(cor_data));
		memset(c_block.ai730ptr->s_count, 0, sizeof(c_block.ai730ptr->s_count));
		c_block.ai730ptr->s_az_buf = 0;		/* clear auto zero buffer */

		c_block.ai730ptr->s_cal_buf = 0;	/* clear calibration buffer */


		return voltage;
		// PMC CLOSE

		//if(c_block.bPmc == TRUE)
			//PmcClose(c_block.nHandle);
}


float ADC::display_ADC_Result(int aChnlNum)
{

    aChnlNum &= 0x1f;     /* limit range */


    string dataStr;
    string temp;

    ostringstream str1,str2,str3,str4;

    str1<<aChnlNum;

    double val=0;

    for(i = 0; i < FIFO_SIZE;i++)
    {
        /*
        check for modulo 4 to see if we need to print title info.
    */

        if((i & 0x3) == 0)
        {
		/*
        	  printf("\nCh %X  Volts[",aChnlNum);
                  printf("%X",(i & 0xF00) >> 8);
                  printf("%x",(i & 0xf0) >> 4);
                  printf("%x] ",i & 0xf);
		*/
        	//temp=" Ch " + str1.str() +" [ " + str2.str() +"  "+ str3.str() +"  "+ str4.str() + " ] ";
        }
        //dataStr.append(temp);

        //        switch(c_block.ai730ptr->range)
        switch(c_block.ai730ptr->range)
        {
        case RANGE_3TO3:
            z = -3.3333;
            s = 6.6666;
            break;

        case RANGE_0TO5:
            z = 0.0000;
            s = 5.0000;
            break;

        case RANGE_5TO5:
            z = -5.0000;
            s = 10.0000;
            break;

        case RANGE_0TO10:
            z =  0.0;
            s = 10.0000;
            break;

        default:			/* RANGE_10TO10 */
            z = -10.0000;
            s =  20.0000;
            break;
        }


        val=((double)cor_data[aChnlNum][i] / (double)65536 * s + z);
	//printf(" %12.6f ",val);
        //dataStr.append(temp);
		



        //cout<<dataStr<<endl;
        //dataStr.clear();
    }

   float voltage =0 ;
   for ( i = 0; i < FIFO_SIZE;i++){
		voltage += ((double)cor_data[aChnlNum][i] / (double)65536 * s + z);
   }
   voltage /= FIFO_SIZE ;

  cout << " Volage == "<< voltage << endl;
  return voltage;
}


void ADC::check()
{
	cout<<"ADC Object Created"<<endl;
}

