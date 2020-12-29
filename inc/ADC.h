/*
 * ADC.h
 *
 *  Created on: Jun 7, 2019
 *      Author: checkout
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include<global.h>
#include<pmcmulticommon.h>
#include<pmc730.h>

class ADC
{
	static ADC *adc_instance;
private:
	ADC();
	struct pmc730 c_block;



	//int	gNumberPmcs = -1;

	long addr;			/* holds board address */
	int i, j;			/* loop index */
	double s;			/* span value */
	double z;			/* zero value */
	int hflag;			/* interrupt handler installed flag */
	struct ai730 ai730;		/* analog input data structure */

	int current_channel;
	SDATA raw_data[SA_SIZE][FIFO_SIZE];	/* allocate data storage area */
	LDATA cor_data[SA_SIZE][FIFO_SIZE];	/* allocate data storage area */
	word count[SA_SIZE];                /* number of samples in array */

	PMCDATA_STRUCT *gpPmcs[MAX_PMCS];
	int	gNumberPmcs;

public:

	static ADC *getADCInstance();
	int init_ADC_CARD();
	//void calibrate(pmc730 *c_blk,int acq_mode,int scanMode,int aChnlNum);
	//int mccdai( pmc730 *c_blk,int range);
	void GetADReferenceValues( pmc730 *c_blk ) ;
	double ReadADReferenceValue( pmc730 *c_blk , word Address );
	void WriteCoefficientAccess( struct pmc730 *c_blk,word Address, byte Data, byte Cycle );
	word ReadCoefficientStatus(struct pmc730 *c_blk );

	PSTATUS InitPmcLib(void);
	PSTATUS PmcOpen(int , int*, char*);
	void calibrate(pmc730 *, int, int, int);
	void blocking_start_convert(int, word *, word );
	void convert(pmc730 *);
	void cnfgai(pmc730 *);
	void move_data(pmc730 *);
	int mccdai( pmc730 *, int );

	float display_ADC_Result(int);


	float readADCVolatage(int);




	PSTATUS PmcInitialize(int);
	void AddPmc(PMCDATA_STRUCT*);
	PSTATUS GetPmcAddress(int, long*);
	PMCDATA_STRUCT* GetPmc(int);
	PSTATUS PmcClose(int );
	void DeletePmc(int nHandle);

	void output_word(int, word *, word);
	void output_byte(int, byte *, byte);
	long input_long(int , long *);
	word input_word(int , word *);
	long SwapLong( long );
	word SwapBytes( word v );

	word build_ai_control(pmc730 *);

	void check();

};



#endif /* INC_ADC_H_ */
