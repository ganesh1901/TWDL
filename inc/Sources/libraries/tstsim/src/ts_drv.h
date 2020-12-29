/* ILC Data Device Corp.
 *
 *        MIL-STD-1553 Simulator/Tester
 *        'C' Software Library
 *
 *        BUS-69068 rel 2.00
 *
 *        Copyright (c) 1997 by ILC Data Device Corp.
 *        All Rights Reserved.
 *
 * TS_drv.h (Main include module)
 *
 * This is the master header file for the Tester/Simulator Library. It contains compiler
 * directives used throughout the library. It makes the necessary defines for
 * the dynamic link library compilation and use. In addition, macros are
 * defined for standard data types and dynamic memory allocation to make
 * the Tester/Simulator library transportable.
 *
 * NOTE: Any changes to this file will require re-compilation of the
 *       Tester/Simulator Library for the changes to take effect.
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for TS_drv.h (HEADER FILE)
 *
 *   Release   Date           Description
 *
 *   1.0       01-DEC-1997    initial release
 *   2.00	   21-DEC-1998
 *   4.1.1	   04-SEP-2001  Updated Linux support
 */

#ifndef TS_DRV_H
#define TS_DRV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#ifndef LABWINDOWS_CVI
#include <conio.h>
#endif
#endif /* _WIN32 */

#ifdef DOS
#include <dos.h>
#endif /* DOS */

/* Determine if we are running Windows, if so define include file */
#if defined( _WINDOWS ) || defined( _WIN32 )
#define _UseWindows 1
#endif

/* If we are compiling on Linux, define our local directive. */
#ifdef linux
#ifndef LINUX
#define LINUX
#endif
#endif /* linux */

/* If we are compiling on VXWorks, include the appropriate headers. */
#ifdef VX_WORKS
#include <vxworks.h>
#include "semlib.h"
#endif /* VX_WORKS */

/* Definitions not provided by UDL */
#define DWORD U32BIT
#define WORD  U16BIT

#include "stdemace.h"
#include "typedef.h"

#ifdef _UseWindows
#include <windows.h>
#else
#if (!defined(LINUX)) && (!defined(VX_WORKS))
typedef const void* DDC_HANDLE;
#endif /* LINUX */
/*typedef void*       LPVOID; */
#endif

/* library is written in 'C' */
#ifdef __cplusplus
extern "C" {
#endif

/* If compiling a dll then define __IMPORTDLL__ */
/* added _DLL for MSC WIN32 compatability       */

#ifdef _WIN32
   /* Function calling convention */
   #define __DECL WINAPI
   #define far		/* To NULL */
#elif defined( LINUX )
   #define __DECL
   #define __stdcall	/* To NULL */
   #define far		/* To NULL */
   #define interrupt	/* To NULL */
   #define OSVERSIONINFO struct utsname
#elif defined( VX_WORKS )
   #define __DECL	/* To NULL */
   #define __stdcall	/* To NULL */
   #define far		/* To NULL */
#elif defined( DOS )
   #define __DECL far pascal _export
   #define __stdcall	/* To NULL */
#else
   #if defined(__DLL__) || defined(_DLL) || defined(_WINDLL)
      #define __IMPORTDLL__
   #endif

   /* __IMPORTDLL__ creates exportable function declarations */
   #ifdef __IMPORTDLL__
      /* WIN16 DLL     */
     #define __DECL far pascal
   #else
      /* 16Bit application  */
      #ifdef __UseWindows
         #define __DECL
      #else
         #define __DECL far pascal _export
      #endif
   #endif
#endif


/* Memcpy is not reliable so we create our own */
#define _MemCpy(src,dest,size) \
	{int i;for(i=0;i<size;i++)*(dest+i)=*(src+i);}  /* size,words */

/* The standard boolean integer/states can be changed here. */
#define BOOLEAN char

#if (!defined(VX_WORKS))
#define TRUE                         1
#define FALSE                        0
#endif 

#define ON                           1
#define OFF                          0

#define Error_t		signed long
#define MAX_CARDS	32	/* Maximum number of TS devices. */

/* user define structures */

#ifndef __IDEA1553_DRV_MSG_IN__
#define __IDEA1553_DRV_MSG_IN__

#ifdef VME
  typedef struct {
	      unsigned tadr    : 5;
              unsigned t_r     : 1;
              unsigned subadr  : 5;
              unsigned wcnt    : 5;
             } CMD;
#else
  typedef struct {             /* CMD typedef */
              unsigned  wcnt   : 5;
              unsigned  subadr : 5;
              unsigned  t_r    : 1;
              unsigned  tadr   : 5;
#ifdef __BORLANDC__
			  unsigned  PAD    : 16;
#endif
             } CMD;
#endif

  typedef struct tagINJ_ERR {             /* INJ_ERR typedef */
	     unsigned short   error;
	     unsigned short   sub_error_1;
	     unsigned short   sub_error_2;
	     unsigned short   sub_error_3;
  } INJ_ERR;


  typedef struct tagMESSAGE {             /* MESSAGE typedef */
   char      comm_type;        /* type of communication */
   CMD       cmd_1;            /* command to be tranmitted by card */
   CMD       cmd_2;            /* TX command in case of RT to RT */
   unsigned  short time_to_next_message; /* time from the start of
                                           the present message to
                                           the start of  the next
                                           message */
   unsigned short     data_table_no;     /* data table number */
   unsigned short    last_data_table_no;
   char      bus;               /* bus A/B */
   char      first_intermessage_routine;    /* routines to be */
   char      second_intermessage_routine;   /* executed by Z8002 in
                                   the gap following the message */

   INJ_ERR   *inj_error_ptr;   /* pointer to a structure defining
                                  error injection */

   unsigned short       det_error; /* detected error - updated by BC */
   unsigned short       rx_status; /* returned RX status - updated by BC */
   unsigned short       tx_status; /* returned TX status - updated by BC */

} MESSAGE;

  typedef struct {             /* MESSAGE typedef */
   char      comm_type;        /* type of communication */
   CMD       cmd_1;            /* command to be tranmitted by card */
   CMD       cmd_2;            /* TX command in case of RT to RT */
   unsigned  short time_to_next_message; /* time from the start of
                                           the present message to
                                           the start of  the next
                                           message */
/* short     data_table_no;   */ /* data table number */
   char      bus;               /* bus A/B */
   char      first_intermessage_routine;    /* routines to be */
   char      second_intermessage_routine;   /* executed by Z8002 in
                                   the gap following the message */

   unsigned short       det_error; /* detected error - updated by BC */
   unsigned short       rx_status; /* returned RX status - updated by BC */
   unsigned short       tx_status; /* returned TX status - updated by BC */

   INJ_ERR   inj_error;   /* structure defining error injection */
} MESSAGE_NEW;

#endif

/* All module header files are should be included here.*/

#include "defines.h"
#include "timer.h"
#include "int.h"
#ifdef DOS
	extern Error_t __DECL _WriteMem(Device_p pCrd,U16BIT offset,U16BIT data);
	extern U16BIT __DECL _ReadMem(Device_p pCrd,U16BIT offset);
	extern Error_t __DECL _ResetDSP(Device_p pCrd,int value);
	extern Error_t __DECL _SetPageMode(Device_p pCrd);
	extern Error_t __DECL _PageSelect(Device_p pCrd);
	extern Error_t __DECL _CommandDSP(Device_p pCrd,U16BIT func,
			U16BIT arg1,U16BIT arg2,U16BIT arg3,U16BIT arg4, U32BIT delay);
	extern Error_t __DECL _LoadDSP(Device_p pCrd,char* filename);
	extern Error_t __DECL _LoadFPGA(Device_p pCrd,char* filename, U16BIT *Size);
	extern char* __DECL getDSPRevString(Device_p pCrd);
	extern U16BIT __DECL getFPGARevNumber(Device_p pCrd);
#else	/* Block this \/ out for Linux ???  kwh */

Error_t __DECL _libStartService(char *Srvc);
Error_t __DECL _libStopService(char *Srvc);

#endif /* DOS */

#define V_WINNT 0
#define V_WIN9X 1
#define V_WIN2K 2
#define V_LINUX 3
#define V_VXWRX 4
S16BIT _libPlatform(void);
U16BIT _libInp(Device_p pCrd, U16BIT addr);
void _libOutp(Device_p pCrd, U16BIT addr, U16BIT b);

#include "irq.h"
#include "ddcmem.h"
#include "err.h"
#include "bc.h"
#include "rt.h"
#include "mt.h"
#include "rep.h"

#ifndef DOS
#include "vifunc.h"
#endif

#ifdef DOS
#include "ts_ena.h"
#endif /* DOS */

#ifdef _WIN32
#include "TS_NTdrv.h"
#endif /* _WIN32 */

#include "driver.h"

#ifdef __cplusplus
}
#endif
#endif /* TS_DRV_H */

