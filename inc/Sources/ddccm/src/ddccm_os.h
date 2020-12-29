/*******************************************************************************
 * FILE: ddccm_os.h
 *
 * DESCRIPTION:
 *
 *  The purpose of this module is to define OS specific data types.
 *
 ******************************************************************************
 * Non-Disclosure Statement
 * ------------------------
 * This software is the sole property of Data Device Corporation.  All
 * rights, title, ownership, or other interests in the software remain
 * the property of Data Device Corporation.  This software may be used
 * in accordance with applicable licenses.  Any unauthorized use,
 * duplication, transmission, distribution, or disclosure is expressly
 * forbidden.
 *
 * This non-disclosure statement may not be removed or modified without
 * prior written consent of Data Device Corporation.
 ******************************************************************************
 * Data Device Corporation
 * 105 Wilbur Place
 * Bohemia N.Y. 11716
 * (631) 567-5600
 *
 * Copyright (c) 2014 by Data Device Corporation
 * All Rights Reserved.
 *****************************************************************************/

#ifndef _DDCCM_OS_H_
#define _DDCCM_OS_H_

#include <errno.h>

/* enable to use file I/O */
#define DDCCM_USE_FILE_SYSTEM

#define DDCCM_SUCCESS                               1
#define DDCCM_FAILURE                               0

#define DDCCM_ERROR_TYPE                            U32BIT

#define IDS_DRIVER_LIST_FILENAME                    "/usr/lib/ddc/drivers.ddc"

#endif /* _DDCCM_OS_H_ */
