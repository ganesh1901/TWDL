/*
 * global.h
 *
 *  Created on: Jun 6, 2019
 *      Author: checkout
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include<stdio.h>
#include<iostream>
#include<sstream>
#include<map>
#include<iterator>
#include<pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
using namespace std;
#include<apc8620.h>

#define VECTOR 192
#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE false
#endif

#define FAILED -1
#define SUCCESS 0


#define Err15553Data                 "Check 1553 Interface"
#define ErrSetCancellationThread     "Pthread Set Cancellation Type Error"
#define ErrCreateThread              "Pthread Creation Error"
#define ErrJoinThread                "Pthread Join Error"
#define ErrSetNameThread             "Pthread Set Name NP Error"
#define ErrDopInit                   "DOP Initialization Error"
#define ErrDipInit                   "DIP Initialization Error"
#define ErrAdcInit                   "ADC Initialization Error"
#define ErrDdcInit                   "DDC Initialization Error"
#define ErrFolderCreate              "Folder Creation Error => "
#define ErrFileCreate                "File Creation Error => "
#define ErrLinkCreate                "File Link Creation Error => "
#define ErrLibConfigIO               "Lib Configuration File IO Error"
#define ErrLibConfigParse            "Lib Configuration File Parse Error"
#define ErrLibConfigSnf              "Lib Configuration File Setting Not Found Error"
#define ErrAceBcStart                "DDC Major Frame Scheduling Error  =>"
#define ErrInvalidOpcode             "Invalid OpCode Found From View => "



typedef struct {
		string Dev_Make;
		string Dev_Interface;
		int modulenum;
		int devicenum;
		string cmd_pipe;
		string rsp_pipe;
		string err_pipe;
		string sur_pipe;
		string adc_pipe;
		string ema_pipe;
		string lock_filename;
		string description;
}DevConfig_t;




#endif /* INC_GLOBAL_H_ */
