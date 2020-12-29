#pragma once

// The general CHost2Dpr class

#ifdef _WIN32
#include <windows.h>
#else
#include "WinTypes.h"
#include <string.h>
#define MAX_PATH 260
#endif
#include "excdef.h"
#include "Logger.h"
#include "Host2Dpr_Error.h"
#ifdef _WIN32
#include <dbt.h>
#endif
#include "CriticalSection.h"

// #define OPTION_DEVELOPMENTAL_DEBUG 1
// #define OPTION_UNETLOG_ALWAYS_ON 1
// #define OPTION_LOG_HANDLE_AND_ENTRYEXIT_INFO 1
// #define OPTION_WHOCALLEDINIT_DEBUG 1
// #define OPTION_LOG_USB_WAITFORINT_TIMEOUTS 1


#define ERROR_STRING_LENGTH 255
#define MODULE_ZERO 0 //  interim for CheckNProcess_MsgRequest

#define DEFAULT_RW_TIMEOUT_MS 1000 // milliseconds. This needs to kept in sync with the define in Lib2232UsbPx.h
#define MAX_READ_ATTEMPT_FROM_DPR 5

// MAX_DEVICE_ID_LEN is defined as 200 in the DDK,
// but it is seems to be undefined without the DDK installed.
#ifndef MAX_DEVICE_ID_LEN
#define MAX_DEVICE_ID_LEN 200
#endif

#define HOST_COMMAND_WRITE_BITVAL 0x0008
#define HOST_COMMAND_READ_BITVAL 0x0000

#define HOST_COMMAND_WORD_BITVAL 0x0010
#define HOST_COMMAND_BYTE_BITVAL 0x0000

#define HOST_COMMAND_INTERRUPT_CHANNEL_BITVAL 0x0020
#define HOST_COMMAND_DATA_CHANNEL_BITVAL 0x0000

#define HOST_COMMAND_CLEAR_EOM_STATUS 0x0040

#define UNET_OPTION_MASK_CLEAR_EOM_STATUS 0x0001


#define INTERRUPT_TIMEOUT_INFINITE 0xFFFFFFFF
#define INTERRUPT_TIMEOUT_500MS 500 // 500 milliseconds

// we picked a default loop timeout of 500 ms instead of infinite for Request/Cancel Interrupt Notification
#define INTERRUPT_NOTIFICATION_LOOP_TIMEOUT INTERRUPT_TIMEOUT_500MS
// #define INTERRUPT_NOTIFICATION_LOOP_TIMEOUT INTERRUPT_TIMEOUT_INFINITE

#define INTERRUPT_REQUEST_ADDRESS 0x7012
#define STOP_INTERRUPT_EVENTS 0
#define REQUEST_INTERRUPT_EVENTS 1

#define INTERRUPT_EVENT_REQUEST_OFFSET_PX 0x7012
#define INTERRUPT_EVENT_REQUEST_OFFSET_DIO 0x7012
#define INTERRUPT_EVENT_REQUEST_OFFSET_RTX 0xff86

#define INTERRUPT_COUNTER_OFFSET_PX 0x7013
#define INTERRUPT_COUNTER_OFFSET_DIO 0x7013
#define INTERRUPT_COUNTER_OFFSET_RTX 0xff88

#define INTERRUPT_COUNTER_ADDRESS 0x7013

// This message should be bigger than any device buffers we're working with,
// so we should be okay. (Code should check GetMaxMessageBufferSize() also.)
#define MAX_MESSAGE_SIZE 2048
#define MESSAGE_HEADER_SIZE 6

#define DEVICE_CONNECTED 1
#define DEVICE_NOT_CONNECTED 0

#define SINGLE_DATA_ELEMENT 1

#define MAX_WRITES_WITHOUT_READ 10

#define MAX_ADDITIONAL_DEBUG_INFO_SIZE 512


#ifdef __cplusplus
extern "C" {
#endif


class CHost2Dpr {
	public:

		// Constructor / Destructor
		// Note that derived classes MUST call their .mConnect and .mOpen functions in their constructors
		CHost2Dpr();
		virtual ~CHost2Dpr() {}

		bool operator !() {if(IsReady()) return false; else return true;}

/* -
		void SelectModule (
			unsigned short module
		) {mModule = module;}
*/

		void SetRWTimeouts(unsigned int timeoutMillisec)
			{mRWTimeoutsMillisec = timeoutMillisec; return mSetRWTimeouts();}

//  - Do i need to worry about clearing this address any time?
		void SetMemorySpaceAddress(unsigned char * pMemBlockBeginAddr, unsigned int memBlockSize)
			{mpMemBlockBeginAddr = pMemBlockBeginAddr; mMemBlockSize = memBlockSize;}

		// Reads & Writes

		void WriteByteToDpr (
			unsigned short moduleNum,
			unsigned short offsetAddress,
			unsigned char dataByte
		);

		void WriteByteBufferToDpr (
			unsigned short moduleNum,
			unsigned short startOffsetAddress,
			const unsigned char * pDataByteBuffer,
			unsigned int numDataBytes
		);

		void WriteWordToDpr (
			unsigned short moduleNum,
			unsigned short offsetAddress,
			unsigned short dataWord
		);

		void WriteWordBufferToDpr (
			unsigned short moduleNum,
			unsigned short startOffsetAddress,
			const unsigned short * pDataWordBuffer,
			unsigned int numDataWords
		);


		unsigned char ReadByteFromDpr (
			unsigned short moduleNum,
			unsigned short offsetAddress
		);

		void ReadByteBufferFromDpr (
			unsigned short moduleNum,
			unsigned short startOffsetAddress,
			unsigned char * pDataByteBuffer,
			unsigned int numDataBytes
		);

		unsigned short ReadWordFromDpr (
			unsigned short moduleNum,
			unsigned short offsetAddress
		);

		void ReadWordBufferFromDpr (
			unsigned short moduleNum,
			unsigned short startOffsetAddress,
			unsigned short * pDataWordBuffer,
			unsigned int numDataWords,
			unsigned short commandFlags = 0
		);

		// Raw Buffer Read & Write (These do not error on timeout;
		//  they just return false.)
		void WriteBufferToDevice (
			unsigned char * pBuffer,
			unsigned int numCharsToWrite,
			unsigned int * pNumCharsWritten
		) {return mWriteBufferToDevice (pBuffer, numCharsToWrite, pNumCharsWritten);}

		bool ReadBufferFromDevice (
			unsigned char * pBuffer,
			unsigned int maxCharsToRead,
			unsigned int * pNumCharsRead
		) {return mReadBufferFromDevice(pBuffer, maxCharsToRead, pNumCharsRead);}

		// CheckNProcess_MsgRequest - Check for, get, and process a new message request.
		// The above listed Write and Read functions are used by clients making message
		// requests of a (service providing) device.
		// The CheckNProcess_MsgRequest function can be used by an application that wants to
		// offer a similar message-response service.
		bool CheckNProcess_MsgRequest(int msgEventSringSize, char * pMsgEventSring);

		// This lets everyone know how big the specific hardware buffers are.
		// (Assuming they're all the same size. Hardware buffer sizes vary;
		// this way the base class & anyone else can can tell how big the internal
		// buffers are and avoid overruning them.)
		unsigned int GetMaxMessageSizeSupported() {return mMaxMessageSizeSupported;}

		// Interrupts
		void SetInterruptState(bool enableFlag) {mInterruptState = enableFlag;}

		void Wait_For_Module_Interrupt(unsigned short moduleNum, unsigned int interruptEventReqOffset, unsigned int timeOutInMilliSeconds, int * pErrReturn) {mWait_For_Module_Interrupt(moduleNum, interruptEventReqOffset, timeOutInMilliSeconds, pErrReturn);}
		void Get_Module_Interrupt_Count(unsigned short moduleNum, unsigned int interruptCounterOffset, unsigned int * pInterruptCount);

		// Status / Errors / Logger

//  - WHAT if the open failed at construction time. Shall we have IsReady retry to open
// the device is if it is connected?
		bool IsReady(void) {return (mDeviceIsOpen && mMLoopThreadParams.deviceIsConnected);}
		bool IsConnected(void) {return mMLoopThreadParams.deviceIsConnected;}

		int GetLastError(void) {return mLastError;}
		void GetErrorString(int errorNum, int errStringLen, char *pErrorString) {Get_Error_String_UNET(errorNum, errStringLen, pErrorString);}
#ifdef _WIN32
		void GetLastErrorAdditionalDbgInfo(int errStringLen, char *pErrorString) {strcpy_s(pErrorString, errStringLen, mLastErrorAdditionalDbgInfo);}
#else
		void GetLastErrorAdditionalDbgInfo(int errStringLen, char *pErrorString) {strncpy(pErrorString, mLastErrorAdditionalDbgInfo, errStringLen);}
#endif

		void AssertOnError (bool assertFlag) {mAssertOnError = assertFlag;}

		// A logger helps with debugging
		void EnableErrorLog(char * pFileName);
		void AddToLog(char * szFormat, ...);
		void LogError(const char * pMsgText, int errorNum);
		void HexDumpMem(unsigned char * pMemStart, unsigned int offset, unsigned int numBytes);
		void HexByteString(unsigned char * pHexBytes, int numHexBytes, char * pHexString, int hexStringLen);
		void HexWordString(unsigned short * pHexWords, int numHexWords, char * pHexString, int hexStringLen);

		// Set up a Connection [Monitor] struct (also used by the message loop and WinProc CALLBACK threads)

		enum MLoopState
		{
			MLoopState_PreLaunch,
			MLoopState_Setup,
			MLoopState_Looping,
			MLoopState_Exiting,
			MLoopState_Error
		};

		typedef struct
		{
			CHost2Dpr * pSourceDev;
			enum MLoopState msgLoopState;
			volatile int lastError;
			volatile bool deviceIsConnected;
			void (*pConnectCallback)(int connectState);
			char deviceIdString[MAX_DEVICE_ID_LEN];
			volatile bool exitMLoopThread;
		} t_MLoopThreadParams;

		// Registration of User's callback functions
		void RegisterErrorCallback (void(*callbackPtr)(char * errorMessage)) {mpErrorCallback = callbackPtr;}
		void RegisterConnectCallback (void(*callbackPtr)(int connectState));

//  - Callback experiment
		void CallConnectCallback(int x) {(*mMLoopThreadParams.pConnectCallback)(x);}


	protected:

		// The hardware buffer size is set by the derived class;
		// the actual size depends on its buffers sizes.
		// The mMaxMessageSizeSupported is the minimum of mHardwareBufferSize
		// and MAX_MESSAGE_SIZE
		unsigned int mHardwareBufferSize;
		unsigned int mMaxMessageSizeSupported;

		int mLastError;
		char mLastErrorAdditionalDbgInfo[MAX_ADDITIONAL_DEBUG_INFO_SIZE + 1];

		// A few logger items to aid with debugging
		CLogger mErrorLog;
		char mLogFileName[MAX_PATH];
		bool mLoggerEnabled;

		// These are protected, because the base class FORCES the device-open flag to the closed state.
		// If the flag were private, then the derived classes would not be able to manipulate it
		//		without creating access functions.
		// Same goes for the mConnect function and attribute
		bool mDeviceIsOpen;

		// Interrupt channel support
		bool mInterruptState;

		// Set up the connection monitoring.
		t_MLoopThreadParams mMLoopThreadParams; // member variables used by the connection monitor threads
		void mConnect ();

		// Stop the connection monitoring.
		void mDisConnect();

		// Read and Write timeouts in milliseconds
		unsigned int mRWTimeoutsMillisec;

		// Keep these protected and no need to pass pointers to the derived classes
		unsigned char mWriteMessage[MAX_MESSAGE_SIZE];
		unsigned char * mpWriteBytesMessageBody;
		unsigned short * mpWriteWordsMessageBody;

		unsigned char mReadMessage[MAX_MESSAGE_SIZE];
		unsigned char * mpReadBytesMessageBody;
		unsigned short * mpReadWordsMessageBody;

		// Treat this as a "DprMsg" header, and not as a "DPR" message-header
		typedef struct
		{
			unsigned short cmdWord;
			unsigned short offsetAddress;
			unsigned short numDataElements;
		} t_DprMsgHeader;

		inline unsigned short mCreateCmdWord (
			unsigned short moduleNum,					// OR'ed command bits: the module number (valid values are 0-7)
			unsigned int readOrWriteFlag,				// OR'ed command bits: HOST_COMMAND_WRITE_BITVAL (0x8) or HOST_COMMAND_READ_BITVAL (0x0)
			unsigned int byteOrWordFlag,				// OR'ed command bits: HOST_COMMAND_WORD_BITVAL (0x10) or HOST_COMMAND_BYTE_BITVAL (0x0)
			unsigned int interruptOrDataChannelFlag		// OR'ed command bits: HOST_COMMAND_INTERRUPT_CHANNEL_BITVAL (0x20) or HOST_COMMAND_DATA_CHANNEL_BITVAL (0x0)
		) {return (moduleNum | readOrWriteFlag | byteOrWordFlag | interruptOrDataChannelFlag);}

		inline void mCreateMsgHeader (
			unsigned short cmdWord,
			unsigned short offsetAddress,
			unsigned short numDataElements,
			t_DprMsgHeader * pDprMsgHeader 
		) {pDprMsgHeader->cmdWord = cmdWord; pDprMsgHeader->offsetAddress = offsetAddress; pDprMsgHeader->numDataElements = numDataElements;}

	private:

	
		t_DprMsgHeader * mpWriteMessageHeader;
		t_DprMsgHeader * mpReadMessageHeader;

		unsigned char * mpMemBlockBeginAddr;
		unsigned int mMemBlockSize;
// 		unsigned short mModule;

		bool mAssertOnError;

		// Pointer to User's Error callback function; (the connection callback is buried in mMLoopThreadParams)
		void (*mpErrorCallback)(char * errorMessage);

		// Connection monitor thread handles
		HANDLE mhThreadMLoop;
		DWORD  mthreadIdMLoop;

		CCriticalSectionSynchronization mExclusiveReadWrite;

		// Open the device, and mark the connected flag.
		virtual void mOpen () {assert(Function_Not_Implemented); return;}

		// Close the device.
		virtual void mCloseDerived () {assert(Function_Not_Implemented); return;}

		virtual void mWriteMessageToDpr(unsigned int numCharsToWrite)  {assert(Function_Not_Implemented);}

		virtual void mReadMessageFromDpr(unsigned int numCharsToRead) {assert(Function_Not_Implemented);}

		virtual void mWriteBufferToDevice (
			unsigned char * pBuffer,
			unsigned int numCharsToWrite,
			unsigned int * pNumCharsWritten
		) {assert(Function_Not_Implemented); return;}

		virtual bool mReadBufferFromDevice (
			unsigned char * pBuffer,
			unsigned int maxCharsToRead,
			unsigned int * pNumCharsRead
		) {assert(Function_Not_Implemented); return false;}

		virtual void mSetRWTimeouts() {assert(Function_Not_Implemented);}

		// Protection from too many consecutive writes
		int mWriteThrottleCounter;
		void mPrep2Read() {mWriteThrottleCounter = 0;}
		void mPrep2Write(unsigned short moduleNum);

		virtual void mWait_For_Module_Interrupt(unsigned short moduleNum, unsigned int interruptEventReqOffset, unsigned int timeOutInMilliSeconds, int * pErrReturn) {assert(Function_Not_Implemented);}
};


#ifdef __cplusplus
}
#endif
