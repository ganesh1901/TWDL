/* ILC Data Device Corp.
 *
 *        MIL-STD-1553 Simulator/Tester
 *        'C' Software Library
 *
 *        BUS-69068 rel 4.30
 *
 *        Copyright (c) 2000 by ILC Data Device Corp.
 *        All Rights Reserved.
 *
 * Flash.h
 *
 * ILC Data Device Corp.
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (631) 567-5600
 *
 * Revision History for INT.H (HEADER FILE)
 *
 *   Release   Date           Description
 *	Original
 */

#ifndef FLASH_H
#define FLASH_H

/* library is written in 'C' */
#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_ILLEGAL_DESTINATION 821
#define FLASH_ILLEGAL_LENGTH	822
#define FLASH_FILE_MISMATCH		823

Error_t __DECL ddcFlashReset  (Device_p *pCrd, U16BIT LogNum); /* load flash embedded code */
Error_t __DECL ddcEraseRom       (Device_p pCrd);
Error_t __DECL ddcProgramRomRange(Device_p pCrd, U16BIT *source, U32BIT destination, U16BIT length);
Error_t __DECL ddcProgramDsp     (Device_p pCrd, S8BIT *file);
Error_t __DECL ddcProgramFpga    (Device_p pCrd, S8BIT *file);
Error_t __DECL ddcReadRomRange   (Device_p pCrd, U32BIT source, U16BIT *destination, U16BIT length);
Error_t __DECL ddcReadDsp        (Device_p pCrd, S8BIT *file);
Error_t __DECL ddcReadFpga       (Device_p pCrd, S8BIT *file);

#ifdef __cplusplus
}
#endif
#endif /* FLASH_H */

