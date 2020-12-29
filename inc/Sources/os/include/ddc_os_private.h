/*******************************************************************************
 * FILE: ddc_os_private.h
 *
 * DESCRIPTION:
 *
 *  This file contains all OS specific include files and defines.
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

#ifndef _DDC_OS_PRIVATE_H_
#define _DDC_OS_PRIVATE_H_

#include <stddef.h>

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
#include <generated/utsrelease.h>
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18)
#include <linux/utsrelease.h>
#else
#include <linux/config.h>
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18) */

#if defined (CONFIG_SMP)
#define __SMP__
#endif /* CONFIG_SMP */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/dma-mapping.h>  /* for dma_alloc_coherent */
#include <linux/list.h>         /* for DDC_COPY_FROM_USER */
#include <linux/spinlock.h>     /* for DDC_COPY_FROM_USER */
#include <linux/vmalloc.h>      /* for vmalloc */
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/delay.h>

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 0)
#include <linux/interrupt.h>
#endif /* LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0) */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
#include <asm/system.h>
#endif

#include <asm/irq.h>
#include <asm/uaccess.h>        /* for DDC_COPY_FROM_USER */


/* ========================================================================== */
/*                                  DEFINES                                   */
/* ========================================================================== */

#define ARINC_MINOR                 128
#define DISCRETE_MINOR              192

#endif /* _DDC_OS_PRIVATE_H_ */
