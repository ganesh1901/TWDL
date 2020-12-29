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
 * IRQ.H (INTERRUPT MODULE)
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for IRQ.H (HEADER FILE)
 *
 *   Release   Date           Description
 *
 *   1.0       01-DEC-1997    initial release
 *   4.1.1     04-SEP-2001    Updated Linux support
 *
 */

#ifndef IRQ_H
#define IRQ_H

enum{
_DISPATCH=0,
_WORKER=1
};


/* INTERRUPT FUNCTION PROTOTYPES ----------------------------------------------*/

/* set BC/RT event to which interrupts will be generated */
Error_t __DECL set_bcrt_event(S16BIT(__stdcall *usr_handler)(S16BIT type,S16BIT param));
Error_t __DECL ddcSetBCRTEvent(Device_p pCrd,Error_t(*usr_handler)(Device_p pCrd, S16BIT type, S16BIT param));
Error_t __DECL ddcSetBCRTEventEx(Device_p pCrd,Error_t(*usr_handler)(Device_p pCrd, S16BIT Type, S16BIT SubType, S16BIT param));

/* set BC/RT event to which interrupts will be generated */
Error_t __DECL set_mon_event(S16BIT (__stdcall *usr_handler)(void));
Error_t __DECL ddcSetMONEvent(Device_p pCrd,Error_t(*usr_handler)(Device_p pCrd));

/* Initializes card with ability to service interrupts */
#ifndef DOS
Error_t __DECL _InstallIRQ(Device_p pCrd, U16BIT LogNum);
#else
Error_t __DECL _InstallIRQ(Device_p pCrd, int Irq, void interrupt (*Isr)(void));
#endif

/* Cleans up thread loop with process interrupts */
Error_t __DECL _UninstallIRQ(Device_p pCrd);

/* Hardware interrupt procedure handler waits and calls system ISR */
DWORD __DECL _IrqProcHandler(Device_p pCrd);

#if (defined( _WIN32 ) || defined( LINUX ))
void __DECL _SystemIsr(DWORD Status);  /* changed from DWORD */

#elif defined( DOS )

void interrupt _ddcSystemIsr_Irq3(void);
void interrupt _ddcSystemIsr_Irq4(void);
void interrupt _ddcSystemIsr_Irq5(void);
void interrupt _ddcSystemIsr_Irq6(void);
void interrupt _ddcSystemIsr_Irq7(void);
void interrupt _ddcSystemIsr_Irq8(void);
void interrupt _ddcSystemIsr_Irq9(void);
void interrupt _ddcSystemIsr_Irq10(void);
void interrupt _ddcSystemIsr_Irq11(void);
void interrupt _ddcSystemIsr_Irq12(void);
void interrupt _ddcSystemIsr_Irq13(void);
void interrupt _ddcSystemIsr_Irq14(void);
void interrupt _ddcSystemIsr_Irq15(void);

void SystemIsr(int IrqLevel);

#endif    /* System interrupt service routine */

#endif /* INT_H */

