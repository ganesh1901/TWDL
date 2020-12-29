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
 * TEST.H (TEST HARDWARE MODULE)
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (516) 567-5600
 *
 * Revision History for TEST.H (HEADER FILE)
 *
 *   Release   Date           Description
 *
 *   1.0       01-DEC-1997    initial release
 *
 */

#ifndef TEST_H
#define TEST_H

/* TEST CONSTANTS ----------------------------------------------------*/



/* TEST FUNCTION PROTOTYPES ------------------------------------------*/

/* Selftest, provides insurance that card is working */
Error_t __DECL _SelfTest(Device_p pCrd);

/* IRQTest, tests interrupt generation and handling */
Error_t __DECL _IRQTest(Device_p pCrd);
#endif /* #ifndef TEST_H */
