/* ILC Data Device Corp.
 *
 *        BU-65550 MIL-STD-1553 BC/RT/MT PCMCIA Interface Card
 *        PCMCIA Enabler Software
 *
 *        rel 1.63 3-MAR-98
 *
 *        Copyright (c) 1995, 1996, 1997, 1998 by ILC Data Device Corp.
 *        All Rights Reserved.
 *
 * 65550ENA.C (PCMCIA ENABLER MODULE)
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for 65550ENA.C (HEADER FILE)
 *
 *   Release   Date            Description
 *
 *   1.40      22-Feb-94 mh/kl Default interrupt level for Card Info
 *                             structure was changed from 5 to 0xffff.
 *                             0xffff will request that Card Services
 *                             select an unused IRQ level.
 *
 *   1.51      27-Feb-94 mh/kl Corrected error in error string table.
 *
 *   1.61      01-Sep-95 bp    none
 *
 *   1.62      26-Jan-96 bp    none
 *
 *   1.63      03-Mar-98 dl    Replaced far with __BUFAR
 */

#ifndef TS_ENA_H
#define TS_ENA_H

/* Microsoft C defaults to word alignment on strucuture members. This will
 * corrupt the format of out card services strucutres which must support
 * byte aligned struture members. This pragma will set alignment to 1 byte. */

/* #define BU_DISPLAY_MESSAGES 1   for debuggin */

#ifdef _MSC_VER
#pragma pack(1)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef U8BIT
#define U8BIT unsigned char
#endif

#ifndef U16BIT		
#define U16BIT unsigned
#endif

#ifndef U32BIT
#define U32BIT unsigned long
#endif

/* Card Service Functions */
#define GetCardServicesInfo     0x0B
#define RegisterClient          0x10
#define DeregisterClient        0x02
#define GetStatus               0x0C
#define ResetCard               0x11
#define SetEventMask            0x31
#define GetEventMask            0x2E
#define RequestIO               0x1F
#define ReleaseIO               0x1B
#define RequestIRQ              0x20
#define ReleaseIRQ              0x1C
#define RequestWindow           0x21
#define ReleaseWindow           0x1D
#define ModifyWindow            0x17
#define MapMemPage              0x14
#define RequestSocketMask       0x22
#define ReleaseSocketMask       0x2F
#define RequestConfiguration    0x30
#define GetConfigurationInfo    0x04
#define ModifyConfiguration     0x27
#define ReleaseConfiguration    0x1E
#define GetFirstTuple           0x07
#define GetNextTuple            0x0A
#define GetTupleData            0x0D

/* Card Service Return Codes */
#define SUCCESS 0x00

/* Card Services Call Back Function Codes */
#define CARD_INSERTION          0x40
#define REGISTRATION_COMPLETE   0x82

#define __BUFAR far

typedef struct
{
	U16BIT InfoLen;
	U16BIT Signature;
	U16BIT Count;
	U16BIT Revision;
	U16BIT CSLevel;
	U16BIT VStrOff;
	U16BIT VStrLen;
	char VendorString[256];
} CardInfoStruct;

/* Client Registration Information Structure passed to RegisterClient function*/
typedef struct
{
	U16BIT Attributes;
	U16BIT EventMask;
	struct
	{
		U16BIT Data;
		U16BIT Segment;
		U16BIT Offset;
		U16BIT Reserved;
	} ClientData;
	U16BIT Version;
} ClientRegInfoStruct;

/* Socket and PC Card Configuration information structure */
typedef struct
{
	U16BIT Socket;
	U16BIT Attributes;
	U8BIT Vcc;
	U8BIT Vpp1;
	U8BIT Vpp2;
	U8BIT IntType;
	U32BIT ConfigBase;
	U8BIT Status;
	U8BIT Pin;
	U8BIT Copy;
	U8BIT Option;
	U8BIT Present;
	U8BIT FirstDevType;
	U8BIT FuncCode;
	U8BIT SysInitMask;
	U16BIT ManufCode;
	U16BIT ManufInfo;
	U8BIT CardValues;
	U8BIT AssignedIRQ;
	U16BIT IRQAttributes;
	U16BIT BasePort1;
	U8BIT NumPorts1;
	U8BIT Attributes1;
	U16BIT BasePort2;
	U8BIT NumPorts2;
	U8BIT Attributes2;
	U8BIT IOAddrLines;
} ConfigurationInfoStruct;

/* request configuration structure */
typedef struct
{
	U16BIT Socket;
	U16BIT Attributes;
	U8BIT Vcc;
	U8BIT Vpp1;
	U8BIT Vpp2;
	U8BIT IntType;
	U32BIT ConfigBase;
	U8BIT Status;
	U8BIT Pin;
	U8BIT Copy;
	U8BIT ConfigIndex;
	U8BIT Present;
} RequestConfigStruct;

/* Window information structure */
typedef struct
{
	U16BIT Socket;
	U16BIT Attributes;
	U32BIT Base;
	U32BIT Size;
	U8BIT AccessSpeed;
} WindowInfoStruct;

typedef struct
{
	U32BIT CardOffset;
	U8BIT  PageNumber;
} MapMemStruct;

/* Tuple Data structure (CIS - Card Information Structure */
typedef struct
{
	U16BIT Socket;
	U16BIT Attributes;
	U8BIT DesiredTuple;
	U8BIT TupleOffset;
	U16BIT Flags;
	U32BIT LinkOffset;
	U32BIT CISOffset;
	U16BIT TupleDataMax;
	U16BIT TupleDataLen;
	U8BIT TupleData[256];
} TupleDataStruct;

/* PC Card Status structure */
typedef struct
{
	U16BIT Socket;
	U16BIT CardState;
	U16BIT SocketState;
} PCCardStatusStruct;

typedef struct
{
	U16BIT Socket;
	U16BIT BasePort1;
	U8BIT NumPorts1;
	U8BIT Attributes1;
	U16BIT BasePort2;
	U8BIT NumPorts2;
	U8BIT Attributes2;
	U8BIT IOAddressLines;
} IOInfoStruct;

typedef struct
{
	U16BIT Socket;
	U16BIT Attributes;
	U8BIT  AssignedIRQ;
	U8BIT  IRQInfo1;
	U16BIT IRQInfo2;
} IRQInfoStruct;

typedef struct
{
	U16BIT socket;
	U16BIT attributes;
	U8BIT  assignedIRQ;
} ReleaseIRQStruct;

typedef struct
{
	U16BIT Socket;
	U16BIT BasePort1;
	U8BIT NumPorts1;
	U8BIT Attributes1;
	U16BIT BasePort2;
	U8BIT NumPorts2;
	U8BIT Attributes2;
	U8BIT IOAddressLines;
} ReleaseIOStruct;

typedef struct
{
	U16BIT Socket;
	U16BIT IrqLevel;
	U32BIT MemBase;
	U32BIT IoBase;
	U32BIT MemWinSize;
	U32BIT IoBaseSize;
	U16BIT NumMemWins;
	U16BIT IrqRegistered;
	U16BIT PCCardConfigured;
	U16BIT IoWindowHandle;
	U16BIT IoWindowAllocated;
	U16BIT MemWindow1Handle;
	U16BIT MemWindow1Allocated;
	U16BIT MemWindow2Handle;
	U16BIT MemWindow2Allocated;
	U16BIT MemWindow3Handle;
	U16BIT MemWindow3Allocated;
	U16BIT __BUFAR *MemPointer;
	U16BIT __BUFAR *RegPointer;
	#ifdef _UseWindows
	 DDC_HANDLE MemSelector;
	 DDC_HANDLE RegSelector;
	#endif
} PCCardStruct;


/* Default interrupt level 0xffff (any available interrupt level) */
#ifdef _UseWindows
#define PC_CARD_DEFAULTS {0,0xFFFF,0l,0x0l,0x8000l,0x0008l,1,0,0,0,0,0,0,0,0,0,0, 0l, 0l, 0, 0}
#else
#define PC_CARD_DEFAULTS {0,0xFFFF,0l,0x0l,0x8000l,0x0008l,1,0,0,0,0,0,0,0,0,0,0, 0l, 0l}
#endif

U16BIT CardServices(U16BIT Function, U16BIT *Handle, void __BUFAR *Pointer,
					U16BIT *ArgLength, void __BUFAR *ArgPointer);

/* PC Card Functions */
unsigned PCMCIAMapIO(void);
unsigned PCMCIAMapMemory(void);
unsigned CheckSlot(void);
unsigned RequestIrq(PCCardStruct *PCCardInfo);
unsigned ReleaseIrq(PCCardStruct *PCCardInfo);
unsigned ReleaseConfig(PCCardStruct *PCCardInfo);
unsigned RequestConfig(PCCardStruct *PCCardInfo);
unsigned GetMemWindow(PCCardStruct *PCCardInfo);
unsigned GetIOWindow(PCCardStruct *PCCardInfo);
unsigned Enable_BU65550(PCCardStruct *PCCardInfo);
unsigned Disable_BU65550(PCCardStruct *PCCardInfo);
unsigned BU_RegisterClient(void);
unsigned BU_DeregisterClient(void);
U16BIT InqConfig(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef TS_ENA_H */
