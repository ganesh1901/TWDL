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
 * MEM.H (MEMORY MODULE)
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for MEM.H (HEADER FILE)
 *
 *   Release   Date           Description
 *
 *   1.0       01-DEC-1997    initial release
 *   4.1.1     04-SEP-2001    Updated Linux support
 *
 */

#ifndef MEM_H
#define MEM_H

/* MEMORY CONSTANTS ----------------------------------------------------*/

/* define the page options*/
#define FOUR_16K_PAGES		0x00
#define TWO_32K_PAGES		0x01
#define ONE_64K_PAGE		0x02

/* DSP IRQ requests */
#define IRQ_REQUEST			0x00
#define IRQ_CLEAR			0x01

/* define the PAGE select constants, some may be invalid depending on pagination
 * option being used.
 */
#define PAGE0				0x00
#define PAGE1				0x01 
#define PAGE2				0x02
#define PAGE3				0x03


/* MEMORY FUNCTION PROTOTYPES ------------------------------------------*/

/* write ram,(offset) words */
Error_t __DECL _WriteMem(Device_p pCrd,U16BIT offset,U16BIT data);

/* read ram,(offset) words */
U16BIT __DECL _ReadMem(Device_p pCrd,U16BIT offset);

/* sets or clears the reset line of the DSP */
Error_t __DECL _ResetDSP(Device_p pCrd,S16BIT value);

/* set the pagination option on the card  */
Error_t __DECL _SetPageMode(Device_p pCrd);

/* set the current page on TestSim card */
Error_t __DECL _PageSelect(Device_p pCrd);

/* command the DSP to perform a particular function */
Error_t __DECL _CommandDSP(Device_p pCrd,U16BIT func,
						   U16BIT arg1,U16BIT arg2,U16BIT arg3,U16BIT arg4, U32BIT delay);

/* command the DSP to perform and finish a particular function */
Error_t __DECL _CommandDSPWait(Device_p pCrd,U16BIT func,
						   U16BIT arg1,U16BIT arg2,U16BIT arg3,U16BIT arg4, U32BIT delay);

/* loads the DSP with program code */
Error_t __DECL _LoadDSP(Device_p pCrd,S8BIT* filename);

/* loads the DSP with program code */
Error_t __DECL _LoadFPGA(Device_p pCrd,S8BIT* filename, U16BIT *size);

/* returns DSP revision string */
S8BIT* __DECL getDSPRevString(Device_p pCrd);

/* returns DSP revision number */
U16BIT __DECL getFPGARevNumber(Device_p pCrd);

/* Selftest, provides insurance that card is working */
Error_t __DECL _SelfTest(Device_p pCrd);

#endif /* MEM_H */

