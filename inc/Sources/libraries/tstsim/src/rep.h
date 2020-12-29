/* ILC Data Device Corp.
 *
 *        IDEA PC Card 1553 Simulator/Tester
 *        (PCIDEA) 'C' Software Library
 *
 *        BUS-69XXX rel 1.0
 *
 *        Copyright (c) 1997 by ILC Data Device Corp.
 *        All Rights Reserved.
 *
 * BC.C (BUS CONTROLLER MODULE)
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for BC.C (SOURCE FILE)
 *
 *   Release   Date           Description
 *
 *	1.0		01-DEC-1997		initial release
 *	1.01	18-JUN-1998		Added #include "common.h"	
 *	1.01	04-SEP-2001		Updated Linux support
 *
 */
#ifndef REP_H
#define REP_H

Error_t __DECL ddcReplayReset(Device_p *pCrd, U16BIT LogNum); /* load replay embedded code */
Error_t __DECL _ReplayFillStack(Device_p pCrd, U16BIT StackNum);
Error_t __DECL ddcReplayFillStack(Device_p pCrd, U16BIT StackNum);
Error_t __DECL ddcReplayRunBC(Device_p pCrd, const char *file, S32BIT times, U16BIT immediate);
Error_t __DECL ddcReplayRunRTs(Device_p pCrd, const char *file, S32BIT times, U16BIT immediate);
Error_t __DECL ddcReplaySetRespDelay(Device_p pCrd, U16BIT delay);
Error_t __DECL ddcReplayRTActive(Device_p pCrd, U16BIT RTNum, U16BIT Active);
#endif /* REP_H */

