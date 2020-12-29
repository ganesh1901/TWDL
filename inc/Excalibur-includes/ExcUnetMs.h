#pragma once

//
// UNet Public Interface Prototype Functions
//
// The bulk of these functions define a bridge interface between the SW tools and the target raw memory of the device
// that enables a raw reading and writing of bytes (8 bit) and words (16 bits) to and from the raw memory of the device
//

#ifdef _WIN32
#include <windows.h>
#else
#include "WinTypes.h"
#endif

// Future...?
// #define UNET
// #define HOST_COMMAND_CLEAR_STATUS 0x0040

#ifndef NUMCARDS
/* These flags are used internally for compilation of galahad dll */
#define NUMBOARDS 16  
#define NUMMODULES 16
#define NUMCARDS ((NUMBOARDS * NUMMODULES) + 1) // as defined in flags_px.h (with the "+ 1"
#endif

#ifndef eboardnotalloc
#define eboardnotalloc	-24
#endif

#define MODULE_0 0
#define MODULE_1 1
#define MODULE_2 2
#define MODULE_3 3
#define MODULE_4 4

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#define __declspec(dllexport) 
#endif
void __declspec(dllexport) Get_Error_String_UNET(int errorNum, int errStringLen, char *pErrorString);

// Bridge2DeviceRawMemory

void			__declspec(dllexport) b2drmSetBaseAddress( unsigned int address, int remoteModuleHandle );
void			__declspec(dllexport) b2drmSetTestMacroWPciBaseAddress( unsigned int address, int remoteModuleHandle );
unsigned char	__declspec(dllexport) b2drmReadByteFromDpr( unsigned int offsetAddress, char * paramName, int remoteModuleHandle  );
unsigned short	__declspec(dllexport) b2drmReadWordFromDpr( unsigned int offsetAddress, char * paramName, int remoteModuleHandle  );
unsigned int	__declspec(dllexport) b2drmReadDWordFromDpr( unsigned int offsetAddress, char * paramName, int remoteModuleHandle  );
void			__declspec(dllexport) b2drmWriteByteToDpr ( unsigned int	offsetAddress,
										unsigned char dataByte,
										char * paramName,
										int remoteModuleHandle );
void			__declspec(dllexport) b2drmWriteWordToDpr ( unsigned int	offsetAddress,
										unsigned short dataWord,
										char * paramName,
										int remoteModuleHandle );
void			__declspec(dllexport) b2drmWriteDWordToDpr ( unsigned int	offsetAddress,
										unsigned int dataDWord,
										char * paramName,
										int remoteModuleHandle );
void			__declspec(dllexport) b2drmReadWordBufferFromDpr( unsigned int offsetAddress,
										unsigned short * wordBufferToFill,
										unsigned int numWordsToRead,
										char * paramName,
										int remoteModuleHandle );
void			 __declspec(dllexport) b2drmReadWordBufferFromDpr_AndClearEOMStatus( unsigned int offsetAddress,
										unsigned short * wordBufferToFill,
										unsigned int numWordsToRead,
										char * paramName,
										int remoteModuleHandle );
void			__declspec(dllexport) b2drmWriteWordBufferToDpr ( unsigned int offsetAddress,
										unsigned short * wordBufferToWrite,
										unsigned int numWordsToWrite,
										char * paramName,
										int remoteModuleHandle );
int				__declspec(dllexport) b2drmOpenReadWORDClose (
										int deviceNum,
										int moduleNum,
										unsigned int offset,
										char *paramName,
										unsigned short *pData,
										int remoteModuleHandle );
int				 __declspec(dllexport) b2drmOpenWriteWORDClose (
										int deviceNum,
										int moduleNum,
										unsigned int offset,
										char * paramName,
										unsigned short data,
										int remoteModuleHandle );
void			__declspec(dllexport) b2drmStartDebug(int remoteModuleHandle);
void			__declspec(dllexport) b2drmStopDebug(int remoteModuleHandle);
void			__declspec(dllexport) b2drmStartLocalAccessToPx(int remoteModuleHandle);
int				__declspec(dllexport) b2drmIsRemoteAccessFlag(int remoteModuleHandle);
int				__declspec(dllexport) b2drmIsRemoteDeviceReachable (int deviceNumber);
BOOL			__declspec(dllexport) b2drmIsCmdClearEOMStatusSupported (int deviceNumber);

	// b2drmIsRemoteDevice is just a wrapper for RegTools IsRemoteDevice(int deviceNumber)
	// This function (b2drmIsRemoteDevice) is different from the next
	// (b2drmDeviceIsRemote) in that:
	// (1) b2drmDeviceIsRemote plans to return an instance variable rather
	//     than rereading the registry if it's already connected.
	// (2) b2drmDeviceIsRemote uses remoteModuleHandle to do some logging.
	// Returns TRUE if the device is a UNET device
BOOL			__declspec(dllexport) b2drmIsRemoteDevice (int deviceNumber);
int				__declspec(dllexport) b2drmDeviceIsRemote(unsigned short deviceNum, int remoteModuleHandle); 
unsigned int	__declspec(dllexport) b2drmIsUSB(int remoteModuleHandle );

int				__declspec(dllexport) b2drmInitRemoteAccessToDevice(unsigned short deviceNum, unsigned short moduleNum, int remoteModuleHandle);
int				__declspec(dllexport) b2drmInitRemoteAccessToDevicePlus(unsigned short deviceNum, unsigned short moduleNum, int remoteModuleHandle, char * pTextInfo);
void			__declspec(dllexport) b2drmReleaseRemoteAccessToDevice(unsigned short deviceNum, unsigned short moduleNum, int remoteModuleHandle);

int				__declspec(dllexport) b2drmWaitForModuleInterrupt(int remoteModuleHandle, unsigned int timeOutInMilliSeconds);
int				__declspec(dllexport) b2drmGetModuleInterruptCount(int remoteModuleHandle, unsigned int *pInterruptCount);

int				__declspec(dllexport) b2drmExc_Request_Interrupt_Notification(int nDevice, int nModule, HANDLE hEvent, int remoteModuleHandle);
int				__declspec(dllexport) b2drmExc_Cancel_Interrupt_Notification(int nDevice, int nModule, int remoteModuleHandle);

int				__declspec(dllexport) b2drmGetEventHandleForModule(int deviceNum, int moduleNum, HANDLE * phEvent);
#ifndef _WIN32
int __declspec(dllexport) b2drmWait_For_Multiple_Interrupts_Px(int numints, int *handle_array, unsigned int timeout, unsigned long *pInterrupt_Bitfield);
#endif

// BEWARE: Do not use the Read/Write BYTE Buffer functions for now.
// We have found suspicious problems when using them, but have not
// isolated exactly how and when!
// Use the Read/Write WORD Buffer functions instead!
void			b2drmReadByteBufferFromDpr( unsigned int offsetAddress,
											unsigned char * byteBufferToFill,
											unsigned int numBytesToRead,
											char * paramName,
											int remoteModuleHandle );
void			b2drmWriteByteBufferToDpr ( unsigned int offsetAddress,
											unsigned char * byteBufferToWrite,
											unsigned int numBytesToWrite,
											char * paramName,
											int remoteModuleHandle );



#ifdef __cplusplus
} //of extern "C"
#endif

//
//  Device Raw Memory Access Macros
//

// Read and return BYTE data from Device Raw Memory to Host
#define RBYTE_D2H( MemoryMapField )  b2drmReadByteFromDpr( (DWORD_PTR) &( MemoryMapField ) , #MemoryMapField, B2DRMHANDLE )

// Write BYTE data from Host to Device Raw Memory
#define WBYTE_H2D( MemoryMapField , byteData)  b2drmWriteByteToDpr( (DWORD_PTR) &( MemoryMapField ) , (unsigned char) (byteData) , #MemoryMapField, B2DRMHANDLE  )

// Read and return WORD data from Device Raw Memory to Host
#define RWORD_D2H( MemoryMapField )  b2drmReadWordFromDpr( (DWORD_PTR) &( MemoryMapField ) , #MemoryMapField, B2DRMHANDLE )

// Write WORD data from Host to Device Raw Memory
#define WWORD_H2D( MemoryMapField , wordData)  b2drmWriteWordToDpr( (DWORD_PTR) &( MemoryMapField ) , (unsigned short) (wordData) , #MemoryMapField, B2DRMHANDLE  )

// Read and return DWORD data from Device Raw Memory to Host
#define RDWORD_D2H( MemoryMapField )  b2drmReadDWordFromDpr( (DWORD_PTR) &( MemoryMapField ) , #MemoryMapField, B2DRMHANDLE )

// Write DWORD data from Host to Device Raw Memory
#define WDWORD_H2D( MemoryMapField , dwordData)  b2drmWriteDWordToDpr( (DWORD_PTR) &( MemoryMapField ) , (unsigned int) (dwordData) , #MemoryMapField, B2DRMHANDLE  )

// OpenUNet-ReadWORD-CloseUNet data from Device MODULE 4 (EXC4000 interface) to Host (returns an errorNumber)
#define ORWordC_M42H( Device, MemoryMapField, pWordData ) b2drmOpenReadWORDClose ( Device, MODULE_4, (unsigned long) &( MemoryMapField ), #MemoryMapField, pWordData, (NUMCARDS - 1) - Device )

// OpenUNet-WriteWORD-CloseUNet data from Device MODULE 4 (EXC4000 interface) to Host (returns an errorNumber)
#define OWWordC_H2M4( Device, MemoryMapField, wordData) b2drmOpenWriteWORDClose ( Device, MODULE_4, (unsigned long) &( MemoryMapField ), #MemoryMapField, wordData, (NUMCARDS - 1) - Device )


