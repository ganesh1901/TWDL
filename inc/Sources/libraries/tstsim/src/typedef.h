	/************************************************************************
	 *                                                                      *
	 *     BU-65558 SIMULATOR/TESTER                                        *
	 *                                                                      *
	 *     FILE: Typedef.h                                                  *
	 *     PURPOSE: Variable type definitions for TI DSP C compiler         *
	 *                                                                      *
	 *     Revision history:                                                *
	 *     created: 2/2/98  Adam Molny                                      *
	 *     modified: 9/4/98  KH    Changed definitions from                 *
	 *                             '#define's to 'typedef's                 *
	 ************************************************************************/

#ifndef TYPEDEF_H
#define TYPEDEF_H

/*  Variable Type Definitions  */
#ifndef DOS

#if 0
typedef unsigned char  U8BIT;
typedef char  S8BIT;
typedef unsigned short U16BIT;
typedef short S16BIT;
typedef unsigned long  U32BIT;
typedef signed   long  S32BIT;
#endif

#else
#define U8BIT  unsigned char
#define S8BIT  signed   char
#define U16BIT unsigned int
#define S16BIT signed   int
#define U32BIT unsigned long int
#define S32BIT signed   long int
#endif

#if defined( DOS )
typedef enum { FALSE, TRUE } BOOL;
#endif

#if defined( LINUX )
#define HKEY int
#endif

#endif /* TYPEDEF_H */

