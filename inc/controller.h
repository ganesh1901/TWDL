/*
 * controller.h
 *
 *  Created on: Jun 17, 2019
 *      Author: root
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#define errExitEN(en, msg) \
                   do { errno = en; perror(msg); exit(EXIT_FAILURE); \
               } while (0)


#include <DDC1553.h>
#include <libconfig.h++>
#include <flatBuffer.h>
#include <limits.h>
#include <advantech.h>

using namespace libconfig;
using namespace ViewController;
using namespace AIO_global;
using namespace DDC1553_global;

#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <Timer.h>

#include <ctime>




#define DEV_CONFIG_FILE "../conf/devconf.cfg"
#define PATTERN "^([A-Z]{6,6})(_{1,1})([0-9]{5,5})(_{1,1})([0-9|a-f|A-F]{4,4})(_{1,1})([0-9]{2,2})$"


#define FAILURE -1
#define DATA_READY 1

#define MAX_BUF 1024
#define DATA_READY 1
#define NODATA -1
#define TIMEDOUT 0

#define MAX_1553_DATA 32
#define CMD_KEY_LENGTH 20
#define SKIP_WORDS 2

#define TRANSMIT 1
#define RECEIVE 0
#define SUCCESS 0
#define FAILED  -1
#define FIFO_PERMISSION 600

#define DEV_CONFIG_FILE "../conf/devconf.cfg"


typedef struct {
		unsigned short i_type;
		unsigned short slot;
		unsigned short port;
		unsigned short channel;
		unsigned short value;
		float value2;
}RRUPacket_t;

typedef struct Pipe_t{
	string cmd_pipe;
	string rsp_pipe;
	string err_pipe;
	string lock_filename;
}Pipe_t;


#define LOG_FILE_NAME "../out/log.bin"
#define INS_LOG_NAME  "../out/ins.bin"
#define EMA_FILE_NAME "../out/ema.bin"
#define SUR_LOG_NAME "../out/sur.bin"
#define NAV_LOG_NAME "../out/nav.bin"
#define LEV_LOG_NAME "../out/lev.bin"
#define TWDL_LOG_NAME "../out/twdl.bin"

#define NAV_19_FILE_NAME "../out/gps-19.bin"
#define NAV_20_FILE_NAME "../out/gps-20.bin"
#define NAV_21_FILE_NAME "../out/gps-21.bin"
#define NAV_22_FILE_NAME "../out/gps-22.bin"


#define NAV_17_FILE_NAME "../out/nav-17.bin"
#define NAV_18_FILE_NAME "../out/nav-18.bin"


#define TWDL_A_FILE_NAME "../out/twdl-A.bin"
#define TWDL_HEALTH_FILE_NAME "../out/twdl-health.bin"
#define TWDL_STATUS_FILE_NAME "../out/twdl-status.bin"

#define TWDL_C_FILE_NAME "../out/twdl-C.bin"
#define SEEKER_A_FILE_NAME "../out/seekerA.bin"
#define SEEKER_B_FILE_NAME "../out/seekerB.bin"


#define ADC_FILE_NAME "../out/ADC.bin"

#define FLAT_OBJ_SIZE 160



pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
bool async = TRUE;




	struct timeval time1;



	char data_array[32] = {0};
	unsigned char buffer[8+2+64];
	int rc;
	short ddc_result = 0;

	class Controller
	{

	public:
			Controller();
			Pipe_t pipe_info;

			typedef struct {
				unsigned char object[512];
			}pipe_data_t;
			pipe_data_t pipe_data;

			int  CMD_PIPE;
			int  RSP_PIPE;
			int  SUR_PIPE;
			int  ERR_PIPE;
			int  ADC_PIPE;
			int  EMA_PIPE;



			int LOG_FP;
			int INS_LOG_FP;
			int SUR_LOG_FP;
			int EMA_FP;
			int NAV_LOG_FP;
			int NAV_17_FP, NAV_18_FP, NAV_19_FP, NAV_20_FP, NAV_21_FP, NAV_22_FP;
			int TWDL_A_FP, TWDL_S_FP, TWDL_C_FP, TWDL_H_FP;
			int ADC_FP ;
			int SeekerA_FP, SeekerB_FP;
			int LEV_LOG_FP;
			int TWDL_LOG_FP;


			fd_set readfds;

			struct timeval timeout;


			Config cfg;
			DevConfig_t DevConfig;
			RRUPacket_t RRUPacket[3];

			int  CommandHandler( int , fd_set);
			void GetConfigParams();
			void Initialize_Data_Pipes();

			pthread_t threadId, readId, emaId;
			pthread_t twdlId, surId, insId, NavId, GpsId, seekerId;

			bool survelliance_ready;
			bool ins_survelliance;
			bool normal_data;
			bool ema_ready;
			bool ins_ready;
			bool nav_ready;
			bool twdl_uplink, twdl_downlink;


			unsigned short demand_subaddress;
			unsigned short freq;
			unsigned short total_samples;


			/* function pointer */
			void (*FunctionPointer[MAX_SUBADDRESS])(void);

			void Create_log_files();


			timer_t t_ema, t_lev, t_nav, t_ins, t_adc, t_obc_surv, t_seeker, t_twdl, t_onesec;
			unsigned long count_ema, count_ins;

			AIO_t adc_channel;

	};

	DDC1553_global::config_params_t	*device_params[32];
	void Read1553Data(DDC1553_global::config_params_t *, int, int);

	enum{

		INS_Ready = 9,
		EMA_Ready = 8,
		OBC_Servelliance = 4,
		INS_Servelliance = 2,
		NAV_Ready = 10,
		TWDL_Helath = 12,
		TWDL_Status = 13,
		TWDL_PktA = 4,
		TWDL_PktB = 22,
		TWDL_PktC = 14
	};



#endif /* CONTROLLER_H_ */