/*******************************************************************************
 * FILE: ddc_udl_serial_io_private.h
 *
 * DESCRIPTION:
 *
 *  The purpose of this module is to provide functions to support serialIO
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

#ifndef _DDC_UDL_SERIAL_IO_PRIVATE_H_
#define _DDC_UDL_SERIAL_IO_PRIVATE_H_

#include "ddc_udl_serial_io.h"

#include "driver_sdk/ddc_udl_private.h"
#include "core/1553/ddc_udl_1553_common_private.h"
#include "core/arinc429/ddc_udl_arinc429.h"
#include "include/ddc_ioctl.h"
#include "driver_sdk/ddc_udl_sdk.h"

typedef enum _CAST_SERIAL_PROTOCOL
{
    CAST_PROTOCOL_UART = 0,
    CAST_PROTOCOL_SDLC,
    CAST_PROTOCOL_HDLC

} CAST_SERIAL_PROTOCOL;  /* set the following in the protocol mode reg */


typedef struct _CAST_SERIAL_IO_CHAN_MATRIX
{
    ACEX_429_UART_TYPE serialAsyncUart;     /* Address       */
    ACEX_429_UART_TYPE serialSDLC;          /* Address       */
    ACEX_429_UART_TYPE serialHDLC;          /* Address       */
    CAST_SERIAL_PROTOCOL castSerialMode;

} CAST_SERIAL_IO_CHAN_MATRIX;


#endif /* _DDC_UDL_SERIAL_IO_PRIVATE_H_ */
