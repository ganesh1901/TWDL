
#define USING_MAIN_MENU


/*
{+D}
    SYSTEM:		Acromag Pmc730 Multifunction Board

    FILENAME:		Pmc730.h

    MODULE NAME:	Pmc730.h

    VERSION:		D

    CREATION DATE:	04/24/02

    CODED BY:		FJM

    ABSTRACT:		This "inlcude" file contains all the major data
			structures and memory map used in the software

    CALLING
		SEQUENCE:	#include "Pmc730.h"

    MODULE TYPE:	include file

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

    DATE       BY		PURPOSE
    --------   -----	---------------------------------------------------
06/30/06 FJM   Support for multiple apmc730 devices
01/11/07 FJM   Add support x86_64
02/18/08 FJM   Added define to support signed & unsigned A/D data formats

{-D}
*/

#define DEVICE_NAME	"apmc730_"	/* the name of the device */



/* //////////////////////////////////////////////////////////////////////// */
/* Unsigned AD output data format (straight binary) is selected by using    */ 
/* the UNSIGNED_AD_DATA define below.    Default is signed AD data format.  */
/* Any change here requires the complete rebuild of the demo application.   */
/* //////////////////////////////////////////////////////////////////////// */


/*#define UNSIGNED_AD_DATA 0		/ * unsigned AD data format to be used unipolar ranges */


#ifdef UNSIGNED_AD_DATA
typedef unsigned short SDATA;
typedef unsigned int LDATA;
#else								/* signed AD data format */
typedef short SDATA;
typedef int LDATA;
#endif /* UNSIGNED_AD_DATA */



typedef enum
{
	Success = 0,
	ParameterOutOfRange = 1,	/* Parameter in error */
	InvalidPointer = 2,		/* Flag NULL pointers */
	DataDirection = 3,		/* data direction error */
	TimeOut = 4			/* time-out error */
} PMCSTAT;


#define MAXSIZE	1024		/* data buffer size */
#define NUM_CHAN 8		    /* number of channels present */


/*
    Defined below is the memory map template for the Pmc730 Board.
    This structure provides access to the various registers on the board.
*/

struct map730		   /* Memory map of the I/O board */
{
	/* Common to all */
	word InterruptRegister;
	word unusedw1;
	/* Analog input section */
	word ADControlStatusRegister;
	word unusedw2;
	byte ADStartChannel;
	byte ADEndChannel;
	word unusedw3;
	byte ADTimerPrescaler;
	byte unusedb1;
	word ADConversionTimer;
	word ADCThreshold;
	word unusedw4;
	word ADStartConversion;
	word unusedw5[5];
	/* Analog output section */
	word DACControlStatus;
	word unusedw6;
	uint32_t DACConversionTimer;
	word DACFIFOThreshold;
	word unusedw7;
	uint32_t DACStartConversion;
	uint32_t DACFIFO;
	uint32_t unusedl1;
	/* Digital I/O section */
	byte IOPort[2];
	word unusedw8;
	word Direction;
	word unusedw9;
	byte InterruptEnableReg[2];
	byte InterruptStatusReg[2];
	byte InterruptTypeReg[2];
	byte InterruptPolarityReg[2];
	uint32_t DebounceDurationReg[2];
	/* Counter/Timer section */
	uint32_t CounterControl;
	uint32_t CounterReadBack;
	uint32_t CounterConstant[2];
	uint32_t TriggerControl;
	/* Calibration access section */
	word CoefficientAccess;
	word unusedw10;
	word CoefficientStatus;
	word unusedw11;
	word CoefficientWriteEnable;
	word unusedw12[0x3C9];
	/* Analog input data */
	uint32_t ADMailBoxArray[512];
};


/*
    Defined below is the Interrupt Handler Data Structure.
    The interrupt handler is provided with a pointer that points
    to this structure.  From this the handler has a link back to
    its related process and common data area.  The usage of the members
    is not absolute.  Their typical uses are described below.
*/


/*
    Defined below is the structure which is used for the read analog command
*/

struct ai730
{
    LDATA *s_cor_buf[32];/* corrected buffer start */
    SDATA *s_raw_buf[32];/* raw buffer start */
    SDATA s_az_buf;	    /* auto zero data storage area */
    SDATA s_cal_buf;		/* calibration buffer */
    word *s_count;		/* number of samples in each array */
    word conv_timer;	/* conversion timer */
    byte data_format;	/* SB or BTC */
    byte trigger;	    /* triggering option software/external */
    byte acq_mode;	    /* the acquisition mode */
    byte scan_mode;	    /* the scan mode */
    byte timer_ps;	    /* timer prescaler */
    byte timer_en;	    /* timer enable */
    byte int_mode;	    /* the interrupt mode */
    byte range; 	    /* input range setting of the board */
    byte s_channel;	    /* the start channel */
    byte e_channel;	    /* the end channel */
    word control;	    /* board control register used by read status */
    word threshold;		/* threshold */
    byte mem_bank_switch;/* enable/disable conversions on memory bank switch */
    byte FIFO_Reset;	/* enable/disable FIFO pointer reset */
    double ref_v[4];	/* Volt reference constant */
};



/*
    Defined below is the structure which is used for the analog output section
*/

struct ao730
{
    byte mode;							/* conversion mode */
    byte trigger;						/* channel triggering */
    byte interrupt_enable;				/* interrupt enable */
    word threshold;						/* threshold */
    byte FIFOReset;						/* FIFO pointer reset */
    uint32_t conv_timer;				/* conversion timer */
    short coef_buf[NUM_CHAN][2];		/* offset/gain buffer start */
    byte channel_numbers[MAXSIZE];		/* allocate data storage area */
    byte sample_flags[MAXSIZE];			/* allocate data storage area */
    uint32_t ideal_data[MAXSIZE];		/* allocate data storage area */
    uint32_t cor_data[MAXSIZE];			/* allocate data storage area */
    float fb[MAXSIZE];					/* storage buffer for floats */
    uint32_t *head_ptr;					/* head pointer of write buffer */
    uint32_t *tail_ptr;					/* tail pointer of write buffer */
    uint32_t *current_ptr;				/* current data pointer of write buffer */
    int write_size;						/* write buffer data size */
    byte interrupt_flag;				/* interrupt status flag */
};


/*
    Defined below is the structure which is used to hold the
    Digital I/O board's status information.
*/

struct sdio730
{
    word direction;			/* data direction register */
    byte int_enable[2];	/* interrupt enable register */
    byte int_status[2];		/* interrupt status register */
    byte int_type[2];		/* interrupt type select register */
    byte int_polarity[2];	/* interrupt input polarity register */
    uint32_t debounce_duration[2]; /* debounce_duration registers */
};

/*
    Defined below is the structure which is used to hold the
    Digital I/O section of the board's configuration information.
*/

struct dio730
{
    word param;			/* parameter mask for configuring board */
    word direction;		/* data direction register */
    byte int_enable[2];	/* interrupt enable register */
    byte int_status[2];		/* interrupt status register */
    byte int_type[2];		/* interrupt type select register */
    byte int_polarity[2];	/* interrupt input polarity register */
    uint32_t debounce_duration[2]; /* debounce_duration registers */
};



/*
    Defined below is the structure which is used to hold the
    Counter/Timer I/O section of the board's configuration information.
*/

struct ct730
{
    uint32_t CounterConstant[2];	 /* constant registers are write only copies are here */
    byte Mode;					 /* the counter mode */
    BOOL OutputPolarity;	 	 /* output polarity */
    byte InputPolarity;			 /* input polarity */
    byte Trigger;				 /* triggering internal/external-polarity */
    byte ClockSource;			 /* clock source */
    byte GateSource;			 /* gate source */
    BOOL WDLoad;				 /* watchdog timer internal/external loaded */
    BOOL InterruptEnable;		 /* interrupt enable */
    byte event_status;			 /* interrupt event status */
};


/*
    Defined below is the structure that holds everything together.
*/

struct pmc730
{
    struct map730 *brd_ptr;		/* base address of the I/O board */
    struct dio730 *dio730ptr;	/* digital I/O section pointer */
    struct sdio730 *sdio730ptr;	/* Status block pointer */
	struct ct730 *ct730ptr;		/* counter/timer section pointer */
	struct ai730 *ai730ptr;		/* analog input section pointer */
	struct ao730 *ao730ptr;		/* analog output section pointer */
	int nHandle;				/* handle to an open board */
    BOOL bPmc;					/* flag indicating a board is open */
    BOOL bInitialized;			/* flag indicating ready to talk to board */
};





/* ************************** Start of Digital I/O section defines */

/*
    Parameter mask bit positions
*/

#define INT_STATUS     1	/* interrupt status register */
#define INT_ENAB       2	/* interrupt enable register */
#define INT_POLARITY   4	/* interrupt polarity register */
#define INT_TYPE       8	/* interrupt type register */
#define DIRECTION      0x10	/* direction register */
#define DEBOUNCE	   0x20	/* debounce registers */



/* Declare digital I/O functions called */

void scfgdio(struct pmc730 *ptr);				/* routine to set up the configuration param. block */
void pstsdio(struct pmc730 *ptr);				/* routine which calls the Read Status Command */
void cnfgdio(struct pmc730 *c_blk);				/* routine to configure the digital I/O section of the board */
void rstsdio(struct pmc730 *c_blk);				/* routine to read status information */
long rpntdio(struct pmc730 *c_blk,
			unsigned port, unsigned point);		/* routine to read a input point */
long rprtdio(struct pmc730 *c_blk, unsigned port); /* routine to read the input port */
long wpntdio(struct pmc730 *c_blk,
			unsigned port, unsigned point,
			unsigned value);					/* routine to write to a output point */
long wprtdio(struct pmc730 *c_blk,
			unsigned port, unsigned value);		/* routine to write to the output port */

/* ************************** End of Digital I/O section defines */



/* ************************** Start of Counter/Timer I/O section defines */

/*
	Mode and option selection enumerations
*/

enum
	{
		None	  = 0,          /* disable counter */
		pwm = 2,		        /* pwm */
		Watchdog  = 3,          /* watchdog function */
		InEvent   = 4,          /* input event */
		InWidth   = 5,          /* input width */
		InPeriod  = 6,          /* input period */
		OneShot   = 7,          /* One-Shot output pluse */

		OutPolLow = 0,          /* output polarity active low */
		OutPolHi  = 1,          /* output polarity active high */

		InPolDisabled = 0,
		InPolLow  = 1,          /* input polarity active low */
		InPolHi   = 2,          /* input polarity active high */

		InTrig    = 0,          /* internal trigger */
		ExtPolLow = 1,          /* external trigger polarity */
		ExtPolHi  = 2,          /* external trigger polarity */

		IntDisable= 0,          /* disable interrupt */
		IntEnable = 1,          /* interrupt enabled */

		InC1Mhz   = 0,          /* internal 1MHZ clock */
		InC4Mhz   = 1,          /* internal 4MHZ clock */
		InC8Mhz   = 2,          /* internal 8MHZ clock */
		ExClock   = 3,          /* external clock */

		WDIntLd   = 0,          /* watchdog timer autoloaded (internal ) */
		WDExtLd   = 1,          /* watchdog loaded when externally triggered */

		GateOffDis = 0,			/* Gate Off disabled - not used */
		GateOffLow = 1,			/* Gate Off active low */
		GateOffHi  = 2			/* Gate Off active high */
	};


/* Declare functions called */

void trigct(struct pmc730 *c_blk);				/* routine to software trigger */
void set_ct_config(struct pmc730 *c_blk);			/* routine to set up the configuration param. block */
void selectcon(struct pmc730 *c_blk);				/* routine to examine/change the counter constant*/
uint32_t build_ct_control(struct pmc730 *c_blk);/* builds counter control words */

PMCSTAT ConfigureCounterTimer(struct pmc730 *c_blk);/* Configure counter timer */
PMCSTAT WriteCounterConstant(struct pmc730 *c_blk);	/* Write counter constant to board register */
PMCSTAT StopCounter(struct pmc730 *c_blk);			/* Stop a counter */
PMCSTAT DisableInterrupt(struct pmc730 *c_blk);		/* Disable counter interrupts */
PMCSTAT GetMode(struct pmc730 *c_blk, byte *mode);	/* get mode */
PMCSTAT SetMode(struct pmc730 *c_blk, byte mode);	/* set mode */
PMCSTAT GetInterruptEnable(struct pmc730 *c_blk,
							BOOL *enable);			/* get interrupt enable */
PMCSTAT SetInterruptEnable(struct pmc730 *c_blk,
							BOOL enable);			/* set interrupt enable */
PMCSTAT GetTriggerSource(struct pmc730 *c_blk,
							byte *trigger);			/* get trigger source */	
PMCSTAT SetTriggerSource(struct pmc730 *c_blk,
							byte trigger);			/* set trigger source */	
PMCSTAT GetClockSource(struct pmc730 *c_blk,
							byte *source);			/* get clock source */
PMCSTAT SetClockSource(struct pmc730 *c_blk,
							byte source);			/* set clock source */
PMCSTAT GetWatchdogLoad(struct pmc730 *c_blk,
							BOOL *load);			/* get Watchdog load */
PMCSTAT SetWatchdogLoad(struct pmc730 *c_blk,
							BOOL load);				/* set Watchdog load */
PMCSTAT GetOutputPolarity(struct pmc730 *c_blk,
							BOOL *polarity);		/* get output polarity */
PMCSTAT SetOutputPolarity(struct pmc730 *c_blk,
							BOOL polarity);			/* set output polarity */
PMCSTAT GetInputPolarity(struct pmc730 *c_blk,
							byte *polarity);		/* get input polarity */
PMCSTAT SetInputPolarity(struct pmc730 *c_blk,
							BOOL polarity);			/* set input polarity */
PMCSTAT StartCounter(struct pmc730 *c_blk);			/* start counter */
PMCSTAT GetCounterConstant(struct pmc730 *c_blk,
							int counter, uint32_t *val);/* get constant */
PMCSTAT SetCounterConstant(struct pmc730 *c_blk,
							int counter, uint32_t val);	/* set constant */

PMCSTAT GetGateSource( struct pmc730 *c_blk,
							byte *source);

PMCSTAT SetGateSource(  struct pmc730 *c_blk,
							byte source);


/* ************************** End of Counter/Timer I/O section defines */

/* ************************** Start of Analog Input section defines */


#define SA_SIZE 32		/* # of channels in data array */
#define FIFO_SIZE 512	/* FIFO size */
#define RANGE_0TO5   0  /* input range */
#define RANGE_5TO5   1	/* input range */
#define RANGE_10TO10 2	/* input range */
#define RANGE_0TO10  3	/* input range */
#define RANGE_3TO3   4	/* input range */

#define TC_SELECT   0	/* two's compliment */
#define SB_SELECT   1	/* straight binary */

#define TD_SELECT   0	/* trigger disabled */
#define TI_SELECT   1	/* trigger direction (input)*/
#define TO_SELECT   2	/* trigger direction (output)*/

#define DI_SELECT   0	/* differential select */
#define SE_SELECT   1	/* single ended select */
#define CAL_SELECT  2	/* calibration select */
#define AZ_SELECT   3	/* auto-zero select */

#define DISABLE     0	/* disable scan */
#define UN_CONT     1	/* uniform continuous */
#define UN_SNGL     2	/* uniform single */
#define BU_CONT     3	/* burst continuous */
#define BU_SNGL     4	/* burst single */
#define EX_TRIG     5	/* external trigger */

#define TIMER_OFF   0	/* disable timer */
#define TIMER_ON    1	/* enable timer */
#define INT_DIS     0	/* disable interrupt */
#define INT_THRESH  1	/* interrupt after threshold */

#define MEM_BANK_ENABLE 0	/* continued analog input conversions */
#define MEM_BANK_DISABLE 1	/* disable analog input conversions on memory bank switch */

#define FIFO_WRTPTR_ENABLE 0/* FIFO write pointer enable */
#define FIFO_WRTPTR_DISABLE 1/* disable FIFO write pointer */

#define START_CONV  0x0001	/* start conversion */

#define WRITE 0
#define READ  1
#define MagicCode (word)0x00A3



/* Declare functions called */

//void GetADReferenceValues(struct pmc730 *c_blk );
void cnfgai(struct pmc730 *c_blk);			/* configure board */
void set_ai_config(struct pmc730 *c_blk);	/* routine to set up the configuration param. block */
void readaistat(struct pmc730 *c_blk);		/* routine which calls the Read Status Command */
int mccdai(struct pmc730 *c_blk);			/* routine to correct input data */
void calibrate(struct pmc730 *c_blk, int mode);					/* get calibration data */
void convert(struct pmc730 *c_blk);			/* get input data */
void move_data(struct pmc730 *c_blk);		/* move data from FIFO */
word build_ai_control(struct pmc730 *c_blk);/* builds analog input control word */
word ReadCoefficientStatus(struct pmc730 *c_blk); /* read access register status */
void WriteCoefficientAccess(struct pmc730 *c_blk,
				word Address, byte Data,
				byte Cycle);				/* write access register */

/* ************************** End of Analog Input section defines */

/* ************************** Start of Analog Output section defines */

#define BIPOLAR_10  1
#define VOLT_SPAN_B10 20	/* constant for ideal volt span of bipolar 10V board */


/* Declare functions called */

int ReadOffsetError(struct pmc730 *c_blk,
				int channel, int Range);		/* Read the offset error for the specified channel */
int ReadGainError(struct pmc730 *c_blk,
				int channel, int Range);		/* Read the gain error for the specified channel */
void WriteCoefficients(struct pmc730 *c_blk,
				int channel, word Offset,
				word Gain, int Range);			/* write offset & gain calibration coefficients */
int ReadOffsetError(struct pmc730 *c_blk,
				int channel, int Range);		/* read offset calibration coefficients */
int ReadGainError(struct pmc730 *c_blk,
				int channel, int Range);		/* read gain calibration coefficients */
void readaostat(struct pmc730 *c_blk);			/* read status */
long get_ao_param(void);					    /* input a parameter */
void set_ao_config(struct pmc730 *c_blk);		/* routine to set up the configuration param. block */
void cnfgao(struct pmc730 *c_blk);
void cdao(struct pmc730 *c_blk);
void wroao(struct pmc730 *c_blk);
void start_conversion(struct pmc730 *c_blk);
void stop_conversions(struct pmc730 *c_blk);
/* ************************** End of Analog Output section defines */


byte input_byte(int nHandle, byte*);/* function to read an input byte */
word input_word(int nHandle, word*);/* function to read an input word */
void output_byte(int nHandle, byte*, byte);	/* function to output a byte */
void output_word(int nHandle, word*, word);	/* function to output a word */
long input_long(int nHandle, long*);			/* function to read an input long */
void output_long(int nHandle, long*, long);	/* function to output a long */


/* declare interrupt handler */

void isr_pmc730(void* pAddr);	/* interrupt handler for Pmc730 */

