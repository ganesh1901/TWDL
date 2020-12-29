/*
 * DDC1553.h
 *
 *  Created on: Jun 6, 2019
 *      Author: Aviral
 */




#ifndef INC_DDC1553_H_
#define INC_DDC1553_H_

#define MSG_LEN_1553 32

//MessageBlock
#define DBLK1  400
#define DBLK2  401
#define DBLK3  402
#define DBLK4  403
#define DBLK5  404


//Message
#define MSG1 121
#define MSG2 122
#define MSG3 123
#define MSG4 124
#define MSG5 125

//XEQ opcode
#define OP1 41
#define OP2 42
#define OP3 43
#define OP4 44
#define OP5 45
#define OP6 46
#define OP7 47
#define OP8 48
#define OP9 49
#define OP10 50
#define OP11 51
#define OP12 52
#define OP13 53
#define OP14 54
#define OP15 55
#define OP16 56
#define OP17 57
#define OP18 58
#define OP19 59
#define OP20 60
#define OP21 61
#define OP22 62
#define OP23 63
#define OP24 64
#define OP25 65
#define OP26 66
#define OP27 67
#define OP28 68
#define OP29 69
#define OP30 70
#define OP31 71
#define OP32 72




/************FRAMES ****/
#define MNR1 81
#define MNR2 82
#define MNR3 83
#define MNR4 84
#define MNR5 85
#define MNR6 86
#define MNR7 87
#define MNR8 88
#define MNR9 89
#define MNR10 90
#define MNR11 91
#define MNR12 92
#define MNR13 93
#define MNR14 94
#define MNR15 95
#define MNR16 96
#define MNR17 97
#define MNR18 98
#define MNR19 99
#define MNR20 100
#define MNR21 101
#define MNR22 102
#define MNR23 103
#define MNR24 104
#define MNR25 105
#define MNR26 106
#define MNR27 107
#define MNR28 108
#define MNR29 109
#define MNR30 110
#define MNR31 111
#define MNR32 112



#define MJR1	201
#define MJR2	202
#define MJR3	203
#define MJR4	204
#define MJR5	205
#define MJR6	206
#define MJR7	207


#define CHL_OPTION  ACE_BCCTRL_CHL_A|ACE_BCCTRL_RETRY_ENA

#define RT_BUF_SIZE 256
#define MAX_SUBADDRESS 32

#define RECEIVE 0
#define TRANSMIT 1
#include <global.h>
#include <stdemace.h>

using namespace std;


namespace DDC1553_global{

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

	class DDC1553
	{
		private:
			static DDC1553 *ddc1553_instance;
			DDC1553();

			S16BIT wSelection;
			S16BIT wMsgGapTime;
			S16BIT wFrmTime;

			unsigned short wBuffer[32];
			S16BIT aOpCodes[15];
			MSGSTRUCT sMsg;
			U16BIT pCmdWrd;
			U16BIT wRepeatCount;
			U32BIT dwMsgCount;
			U32BIT dwHBufLost;

			char ReadArray[MAX_SUBADDRESS] = {};



		public:
			~DDC1553();
			S16BIT DevNum=0;
			S16BIT wResult;
			S16BIT nResult;
			static DDC1553 *getDDC1553Instance();
			int initDDC();
			void InitFrameValues(int, int, int, int, int);

			void createFrames();
			void createMessage();
			void craeateAsyncMessage();
			void createMajorFrame4();

			void scheduleAsyncReceiveMsg(config_params_t *);
			void scheduleAsyncTransmitMsg(config_params_t *);

			void getUpdatedData(config_params_t *, int);
			void setUpdatedData(config_params_t *, int);

			void DDCStateChange(unsigned short);


			void check();
			void DisplayDecodedMsg(S16BIT, MSGSTRUCT *);
			void DisplayNavigationDecodedMsg(S16BIT, MSGSTRUCT *, int);


			unsigned short Subaddress[32];
			unsigned short BlkId[32];
			unsigned short MsgId[32];
			unsigned short XeqOpcode[32];
			unsigned short SubaddCount=0;
			unsigned short rspDataBuffer[32][32];

			unsigned char STATE=4;
			bool async=false;
			bool async_operation=false;
			unsigned short ema_counter=0;

			void setReadArray(char *, int);



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
			MSG_28,
			MSG_29,
			MSG_DUMMY,
			MSG_25,

		};

};



#endif /* INC_DDC1553_H_ */
