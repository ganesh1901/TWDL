/*
 * RS422.h
 *
 *  Created on: Jun 7, 2019
 *      Author: checkout
 */

#ifndef INC_RS422_H_
#define INC_RS422_H_


#include<global.h>

class RS422
{
	static RS422 *rs422_instance;

private:
	RS422();
public:
	static RS422 *getRS422Instance();
	void check();
};




#endif /* INC_RS422_H_ */
