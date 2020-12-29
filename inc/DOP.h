/*
 * DDC1553.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Aviral
 */

#ifndef INC_DOP_H_
#define INC_DOP_H_


#include<global.h>
#include<ip445.h>



class DOP
{
	static DOP *dop_instance;
	
	private:
		DOP();



	private:
	    //optoDo cards//
	    long addr, baddr4451, baddr4452, baddr4453, baddr4454;    /* hold carrier address  & board address*/
	    struct sblk445 s_block445_1, s_block445_2, s_block445_3, s_block445_4;              /* allocate status param. blk */

	public:
		static DOP *getDopInstance();
		void check();
		int initDo();
		int initDO_SLOT(struct sblk445*,  int, int , char, long *);
		void sendDO();
		void writeToDo_SLOT(int, int, int, int);
		long readDO(int, int, int, int);
		void closeDo();

		map<int, struct sblk445 *> slotToStructMap;





};

#endif /* INC_DOP_H_ */
