/*
 * DDC1553.h
 *
 *  Created on: Jun 6, 2019
 *      Author: checkout
 */

#ifndef INC_DIP_H_
#define INC_DIP_H_


#include<global.h>
#include<ip470.h>
#define INTERRUPT_LEVEL 5


class DIP
{
  static DIP *dip_instance;
  private:
  	DIP();
  	
  	long addr1,addr2,baddr4701,baddr4702,baddr4703,baddr4704;    /* hold carrier address  & board address*/
	struct cblk470 c_block470_1,c_block470_2,c_block470_3,c_block470_4;

  public:
  	static DIP *getDipInstance();
  	int initDI(void);
  	int initDI_SLOT(struct cblk470*,  int, char, long *);
  	long read_DI(int, int, int);
  	void check(void);
  	void closeDI(void);

  	map<int, struct  cblk470*> slotToStructMap;

};

#endif /* INC_DIP_H_ */
