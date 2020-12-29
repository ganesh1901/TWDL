#ifndef __excalibur_h__
#define __excalibur_h__ 


#include <galahad.h>

//#include <errorinfo.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRANSMIT 1
#define RECEIVE 0
#define FAILURE -1
#define ErrDevnotFound 10

#define DEFAULT_RTNUM 1
#define DEFAULT_SUBADDRESS 0
#define DEFAULT_WORDCOUNT 32
#define DEFAULT_SUR_SUBADDRESS 16

#define FAILED -1
#define SUCCESS 0

namespace excalibur_global{
#pragma pack(1)

	typedef struct{
			unsigned short Devnum;
			unsigned short BusSelection;
			unsigned short Rtnum;
			unsigned short Subaddr;
			unsigned short MsgID;
			unsigned short msg_count;
			unsigned short DataBlk;
			unsigned short wordcount;
			unsigned short Msg_timer;
			unsigned short FrameID;
			unsigned short direction;
			unsigned short payload[32];
			unsigned short frequency;
			unsigned short CommandWord;
		}config_params_t;


	typedef struct excalibur_devconf_t {
		int modulenum;
		int devicenum;
		int handle;
		int errinfonum;
	}excalibur_devconf_t;

	 typedef enum sequence_t{
		Receive_Tr,
		Transmit_Tr,
		dummy_Tr,
		Start_Survalliance_Tr,
		Stop_Survalliance_Tr,
		Start_Levelling_Tr,
		Stop_Levelling_Tr,
		Start_Navigation_Tr,
		Stop_Navigation_Tr
	}sequence_t;


	class excalibur{

	private:
		static excalibur *excalibur_instance;
		excalibur();
		unsigned short module,device;
		int status;
		unsigned char errstr[80];


		short Msgid[32];
		int Subaddress[32];
		short SubaddCount = 0;
		struct FRAME frame_st[32];
		short  minor_id = 0;
		long time_gap = 0;
		int excalibur_state = 0;
		unsigned short statusword = 0;
		int major_frameid[16];
		int receive_frameid = 0, transmit_frameid = 0;
		short transmit_msgid = 0, receive_msgid = 0;

		void CreateMessage();
		void CreateReceiveFrame();
		void CreateTransmitFrame();
		void CreateMajorframe1();
		void CreateMajorframe2();
		void CreateMajorframe3();
		void CreateMajorframe4();
		void CreateMajorframe5();
		void printTransmitMessage(usint *, int);
		void Go();

		excalibur_devconf_t *local_devconf;

	public:
		~excalibur();
		static excalibur *getexcaliburInstance();
		int initExcalibur(excalibur_devconf_t *);

		void scheduleAsyncReceiveMsg(config_params_t *);
		void scheduleAsyncTransmitMsg(config_params_t *);
		void getUpdatedData(config_params_t *, int);
		void ExcaliburStateChange(int);

	};



	enum{
		MSG_INS_LEV = 0,
		MSG_EMA ,
		MSG_OBC_SURV,
		MSG_TWDL_PktA,
		MSG_TWDL_Health,
		MSG_TWDL_Status,
		MSG_TWDL_PktC,
		MSG_INS_SURV,
		MSG_INS_17,
		MSG_INS_18,
		MSG_INS_19,
		MSG_INS_20,
		MSG_INS_21,
		MSG_INS_22,
		MSG_DUMMY

	};
}
using namespace excalibur_global;

#endif

