/* ILC Data Device Corp.
 *
 *        MIL-STD-1553 Simulator/Tester
 *        'C' Software Library
 *
 *        BUS-69068 rel 1.0
 *
 *        Copyright (c) 1997 by ILC Data Device Corp.
 *        All Rights Reserved.
 *
 * TIMER.H (TIMER MODULE)
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for TIMER.H (HEADER FILE)
 *
 *   Release   Date           Description
 *
 *   1.0       01-DEC-1997    initial release
 *   4.1.1     04-SEP-2001    Updated Linux support
 *
 */

#ifndef TIMER_H
#define TIMER_H

/* TIMER FUNCTION PROTOTYPES ------------------------------------------*/
#ifdef DOS          /* For 16-bit DOS delay function */
#include <time.h>
#endif /* DOS */

#ifdef LINUX
#include <sys/time.h>
#include <unistd.h>
#endif /* Linux */

/* This function produces a software delay of milliseconds */
Error_t __DECL _TimeDelay(U32BIT time);

#ifdef _WIN32
/* This function detects if a high resolution timer is supported */
void __DECL _FindHiResTimer();

/* This function enable/disable of using high resolution timer */
void __DECL _UseHiResTimer(U16BIT enable);

/* This function get a current time stamp. */
LARGE_INTEGER __DECL _GetStartTime();
#endif

/* This function checks to see if a certain delay has elapsed */
#ifdef _WIN32
BOOL __DECL _TimeCheck(LARGE_INTEGER start,U32BIT delay);
#else
BOOL __DECL _TimeCheck(U32BIT start,U32BIT delay);
#endif

#ifdef DOS
U32BIT GetTickCount(void);
#endif /* DOS */

#if defined( LINUX ) || defined( VX_WORKS )
void _epoc();
U32BIT GetTickCount(void);
#endif

#ifdef LINUX
BOOL _TimeCheckUsec(struct timeval tvStart, U32BIT dwDelay);
#endif /* LINUX */

#endif /* TIMER_H */

