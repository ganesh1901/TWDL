/*******************************************************************************
 * FILE: ddcsockmsg.c
 *
 * DESCRIPTION:
 *
 *      DDC socket interface for remote devices.
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

#include "os/include/ddc_os_types.h"
#include "ddcsockmsg.h"

#ifdef LINUX
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#endif /* LINUX */

#ifdef VX_WORKS
#include <iolib.h>
#include <string.h>
#include <sys/socket.h>
#endif

/* ========================================================================== */
/* common to all OS's other than Windows */
/* ========================================================================== */
#ifndef WIN32

/* ========================================================================== */
/* ========================================================================== */
int DDC_WSAClearLastError
(
    void
)
{
    errno = 0;

    return 0;
}

/* ========================================================================== */
/* ========================================================================== */
int DDC_WSAGetLastError
(
    void
)
{
    return errno;
}

/* ========================================================================== */
/* ========================================================================== */
int DDC_WSAStartup
(
    DDC_SOCK_MSG_WORD wVersionRequested,
    DDC_LPWSADATA lpWSAData
)
{
    DDC_UNREFERENCED_PARAMETER(wVersionRequested);
    DDC_UNREFERENCED_PARAMETER(lpWSAData);
    return 0;
}

/* ========================================================================== */
/* ========================================================================== */
int DDC_WSACleanup
(
    void
)
{
    return 0;
}

/* ========================================================================== */
/* ========================================================================== */
int DDC_closesocket
(
    DDC_SOCKET socket
)
{
    close(socket);
    return 0;
}

#endif /* not WIN32 - common to all OS's other than Windows */


U16BIT gDdcSockDbgPriority = 0 /*DDC_SOCK_DBG_FUNC | DDC_SOCK_DBG_INFO | DDC_SOCK_DBG_ERROR*/;

/*******************************************************************************
 * Name:    DDCSockCreateMessageStruct
 *
 * Description:
 *      This function creates the message structure needed to store all
 *      message information. It will allocate a buffer of message Struct,
 *      then initialize information to it.
 *
 * In   MsgType     Message Type
 * In   MsgCmd      Message Command
 * In   MaxLen      maximum length of the message
 * Out  pMsgStruct  pointer to the message Struct
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockCreateMessageStruct
(
    U32BIT u32MsgType,
    U32BIT u32MsgCmd,
    U32BIT u32MaxLen,
    PDDC_SOCK_MSG_STRUCT *ppMsgStruct
)
{
    int i = 0;
    PDDC_SOCK_MSG_STRUCT pMsgStruct = NULL;
    U8BIT **pddcSockMsgFields;

    pMsgStruct = (PDDC_SOCK_MSG_STRUCT)malloc(sizeof(DDC_SOCK_MSG_STRUCT));
    if (pMsgStruct == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockCreateMessageStruct: malloc failed\n");
        return DDC_SOCK_MSG_ALLOC_FAIL;
    }

    pMsgStruct->ddcSockMsgHeader.ddcSockMsgMaxLen = u32MaxLen;
    pMsgStruct->ddcSockMsgHeader.ddcSockMsgType = u32MsgType;
    pMsgStruct->ddcSockMsgHeader.ddcSockMsgCmd = u32MsgCmd;
    pMsgStruct->ddcSockMsgHeader.ddcSockMsgFieldNum = 0;
    pMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen   = sizeof (DDC_SOCK_MSG_HEADER);

    pddcSockMsgFields = GET_DDC_SOCK_MSG_FIELDS_DATA_POINTER(pMsgStruct);

    /* Initialize Msg Field Pointers */
    for (i=0; i<DDC_SOCK_MSG_FIELD_NUM; i++)
    {
        pddcSockMsgFields[i] = NULL;
    }

    *ppMsgStruct = pMsgStruct;
    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockDestroyMessageStruct
 *
 * Description:
 *      This function destroys the message Struct created earlier.
 *
 * In   pMsgStruct      the pointer to the message
 * Out  none
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockDestroyMessageStruct
(
    PDDC_SOCK_MSG_STRUCT *ppMsgStruct
)
{
    int i = 0;
    PDDC_SOCK_MSG_STRUCT pMsgStruct = *ppMsgStruct;
    U8BIT **pddcSockMsgFields;

    if (pMsgStruct == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockDestroyMessageStruct: DDC_SOCK_MSG_ERROR\n");
        return DDC_SOCK_MSG_ERROR;
    }

    pddcSockMsgFields = GET_DDC_SOCK_MSG_FIELDS_DATA_POINTER(pMsgStruct);

    for (i=0; i<DDC_SOCK_MSG_FIELD_NUM; i++)
    {
        if (pddcSockMsgFields[i] != NULL)
        {
            free(pddcSockMsgFields[i]);
            pddcSockMsgFields[i] = NULL;
        }
    }
    free(pMsgStruct);
    pMsgStruct = NULL;

    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockAddFieldToMessageStruct
 *
 * Description:
 *      This function appends a field to the message Struct. Each field has
 *      following structure:
 *          - the 1st 32bits are always the Field Length
 *          - the 2nd 32bits are always the Field Type
 *          - the following bytes are the data
 *
 * In   FieldType       Field Type
 * In   FieldDataLen    Field Data Length
 * In   pDataBuf        pointer to the data buffer
 * In   pMsgStruct      pointer to the message Struct
 * Out  none
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockAddFieldToMessageStruct
(
    PDDC_SOCK_MSG_STRUCT pMsgStruct,
    U32BIT u32FieldType,
    U32BIT u32FieldDataLen,
    void * pDataBuf
)
{
    U32BIT u32MsgLen = 0;
    PDDC_SOCK_MSG_FIELD_HEADER pMsgField = NULL;
    void * pMsgFieldData = NULL;
    U8BIT **pddcSockMsgFields;

    if (pMsgStruct == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_FUNC, "DDCSockAddFieldToMessageStruct: DDC_SOCK_MSG_ERROR\n");
        return DDC_SOCK_MSG_ERROR;
    }

    /* check whether there is still room available for the new field */
    u32MsgLen = pMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen;
    if (u32MsgLen + u32FieldDataLen + sizeof(DDC_SOCK_MSG_FIELD_HEADER) > pMsgStruct->ddcSockMsgHeader.ddcSockMsgMaxLen)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockAddFieldToMessageStruct: no space for field leb %d\n", u32FieldDataLen);
        return DDC_SOCK_MSG_NO_SPACE;
    }

    /* user can pass in NULL pointer if there is no new data */
    if (pDataBuf == NULL)
    {
        /* make sure the length information is set to 0 */
        u32FieldDataLen = 0;
    }

    /* allocate memory for the field */
    pMsgField = (PDDC_SOCK_MSG_FIELD_HEADER)malloc(u32FieldDataLen + sizeof(DDC_SOCK_MSG_FIELD_HEADER));
    if (pMsgField == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockAddFieldToMessageStruct: malloc failed\n");
        return DDC_SOCK_MSG_ALLOC_FAIL;
    }

    /* fill in Field information */
    pMsgField->ddcSockMsgFieldLen = u32FieldDataLen + sizeof(DDC_SOCK_MSG_FIELD_HEADER);
    pMsgField->ddcSockMsgFieldType = u32FieldType;
    pMsgFieldData = (U8BIT *)pMsgField + sizeof(DDC_SOCK_MSG_FIELD_HEADER);

    if (pDataBuf)
    {
        /* copy data only when there is data */
        memcpy(pMsgFieldData, pDataBuf, u32FieldDataLen);
    }

    /* add field structure to the list in Struct */
    pddcSockMsgFields = GET_DDC_SOCK_MSG_FIELDS_DATA_POINTER(pMsgStruct);
    pddcSockMsgFields[pMsgStruct->ddcSockMsgHeader.ddcSockMsgFieldNum] = (U8BIT *)pMsgField;

    /* update msg Struct */
    pMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen += u32FieldDataLen + sizeof(DDC_SOCK_MSG_FIELD_HEADER);
    pMsgStruct->ddcSockMsgHeader.ddcSockMsgFieldNum++;

    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockGetFieldDataFromMessageStruct
 *
 * Description:
 *      This function get the data in a certain field.
 *
 * In   pMsgStruct          message Struct
 * In   FieldIndex          index of the field we want to get
 * In   ExpectFieldType     expected type of the field
 * In   ExpectFieldDataLen  expected data length of the field if unknown, input 0
 * Out  DataBuf             buffer to hold the field data
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockGetFieldDataFromMessageStruct
(
    PDDC_SOCK_MSG_STRUCT pMsgStruct,
    U8BIT u8FieldIndex,
    U32BIT u32ExpectFieldType,
    U32BIT u32ExpectFieldDataLen,
    void * pDataBuf
)
{
    PDDC_SOCK_MSG_FIELD_HEADER pMsgField = NULL;
    U8BIT * pFieldData = NULL;
    U32BIT u32DataLen = 0;
    U8BIT **pddcSockMsgFields;

    pddcSockMsgFields = GET_DDC_SOCK_MSG_FIELDS_DATA_POINTER(pMsgStruct);
    pMsgField = (PDDC_SOCK_MSG_FIELD_HEADER)(pddcSockMsgFields[u8FieldIndex]);

    if (pMsgField->ddcSockMsgFieldType != u32ExpectFieldType)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "Field#%d FieldType %d not expected %d\n", \
            u8FieldIndex, pMsgField->ddcSockMsgFieldType, u32ExpectFieldType);

        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockGetFieldDataFromMessageStruct: DDC_SOCK_MSG_ERROR\n");
        return DDC_SOCK_MSG_ERROR;
    }

    u32DataLen = pMsgField->ddcSockMsgFieldLen - sizeof(DDC_SOCK_MSG_FIELD_HEADER);
    if (u32ExpectFieldDataLen != 0)
    {
        if (u32DataLen != u32ExpectFieldDataLen)
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "Field#%d FieldDataLen %d not expected %d\n", \
                u8FieldIndex, u32DataLen, u32ExpectFieldDataLen);

            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockGetFieldDataFromMessageStruct-2: DDC_SOCK_MSG_ERROR\n");
            return DDC_SOCK_MSG_ERROR;
        }
    }

    pFieldData = (U8BIT *)pMsgField + sizeof(DDC_SOCK_MSG_FIELD_HEADER);
    memcpy(pDataBuf, pFieldData, u32DataLen);

    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockMsgEncode
 *
 * Description:
 *      Encode the socket message from message Struct.
 *
 * In   MsgStruct   pointer to the message Struct, including message information
 *                  and pointers to all field structures
 * Out  MsgBuf      pointer to the socket message
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockMsgEncode
(
    PDDC_SOCK_MSG_STRUCT pMsgStruct,
    char * pMsgBuf
)
{
    char * pOffset = NULL;
    PDDC_SOCK_MSG_FIELD_HEADER pMsgField = NULL;
    U32BIT i = 0;

    /* first copy the message header */
    memcpy(pMsgBuf, pMsgStruct, sizeof(DDC_SOCK_MSG_HEADER));

    /* then copy each field */
    pOffset = pMsgBuf + sizeof(DDC_SOCK_MSG_HEADER);

    for (i=0; i<pMsgStruct->ddcSockMsgHeader.ddcSockMsgFieldNum; i++)
    {
        U8BIT **pddcSockMsgFields = GET_DDC_SOCK_MSG_FIELDS_DATA_POINTER(pMsgStruct);

        pMsgField = (PDDC_SOCK_MSG_FIELD_HEADER)pddcSockMsgFields[i];

        memcpy(pOffset, pMsgField, pMsgField->ddcSockMsgFieldLen);
        pOffset += pMsgField->ddcSockMsgFieldLen;
    }

    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockMsgDecode
 *
 * Description:
 *      Decode the socket message into message struct.
 *
 * In   MsgBuf      pointer to the socket message
 * Out  DecodedMsgStruct    pointer to the decoded message Struct, including
 *                          message information and pointers to all field
 *                          structures
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockMsgDecode
(
    U8BIT * pMsgBuf,
    PDDC_SOCK_MSG_STRUCT pDecodedMsgStruct
)
{
    U8BIT * pMsgBufOffset = NULL;
    U8BIT * pDecodedMsgField = NULL;
    U32BIT i = 0;
    U32BIT u32FieldLen = 0;

    if ((pMsgBuf == NULL) || (pDecodedMsgStruct == NULL))
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgDecode: DDC_SOCK_MSG_ERROR\n");
        return DDC_SOCK_MSG_ERROR;
    }

    pMsgBufOffset = pMsgBuf;

     /* Decode message information */
    pDecodedMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen = ((PDDC_SOCK_MSG_STRUCT)pMsgBufOffset)->ddcSockMsgHeader.ddcSockMsgCurrentLen;
    pDecodedMsgStruct->ddcSockMsgHeader.ddcSockMsgMaxLen     = ((PDDC_SOCK_MSG_STRUCT)pMsgBufOffset)->ddcSockMsgHeader.ddcSockMsgMaxLen;
    pDecodedMsgStruct->ddcSockMsgHeader.ddcSockMsgType       = ((PDDC_SOCK_MSG_STRUCT)pMsgBufOffset)->ddcSockMsgHeader.ddcSockMsgType;
    pDecodedMsgStruct->ddcSockMsgHeader.ddcSockMsgCmd        = ((PDDC_SOCK_MSG_STRUCT)pMsgBufOffset)->ddcSockMsgHeader.ddcSockMsgCmd;
    pDecodedMsgStruct->ddcSockMsgHeader.ddcSockMsgFieldNum   = ((PDDC_SOCK_MSG_STRUCT)pMsgBufOffset)->ddcSockMsgHeader.ddcSockMsgFieldNum;

    pMsgBufOffset += sizeof(DDC_SOCK_MSG_HEADER);

    /* Decode each field */
    for (i=0; i<pDecodedMsgStruct->ddcSockMsgHeader.ddcSockMsgFieldNum; i++)
    {
        U8BIT **pddcSockMsgFields = GET_DDC_SOCK_MSG_FIELDS_DATA_POINTER(pDecodedMsgStruct);

        if (pMsgBufOffset == NULL)
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgDecode-2: DDC_SOCK_MSG_ERROR\n");
            return DDC_SOCK_MSG_ERROR;
        }

        u32FieldLen = ((PDDC_SOCK_MSG_FIELD_HEADER)pMsgBufOffset)->ddcSockMsgFieldLen;

        pDecodedMsgField = (U8BIT *)malloc(u32FieldLen);

        if (pDecodedMsgField == NULL)
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgDecode: malloc failed\n");
            return DDC_SOCK_MSG_ALLOC_FAIL;
        }

        memcpy(pDecodedMsgField, pMsgBufOffset, u32FieldLen);

        pddcSockMsgFields[i] = pDecodedMsgField;

        pMsgBufOffset += u32FieldLen;
    }

    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockMsgSend
 *
 * Description:
 *      This function constructs the socket message from the message Struct
 *      and sends the socket raw message to a socket.
 *
 * In   ConnectSocket   socket needs to send the message to
 * In   MsgStruct       message Struct
 * Out  none
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockMsgSend
(
    DDC_SOCKET ConnectSocket,
    PDDC_SOCK_MSG_STRUCT pMsgStruct
)
{
    char * psendBuf = NULL;
    int iResult = 0;

    if (pMsgStruct == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgSend: DDC_SOCK_MSG_ERROR\n");
        return DDC_SOCK_MSG_ERROR;
    }

    /* allocate memory */
    psendBuf = (char *)malloc(pMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen);

    if (psendBuf == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgSend: malloc failed\n");
        return DDC_SOCK_MSG_ALLOC_FAIL;
    }

    /* construct the sock message */
    DDCSockMsgEncode(pMsgStruct, psendBuf);

    /* Clear out possible stale errno. */
    DDC_WSAClearLastError();

    /* Send the message */
    iResult = send( ConnectSocket, psendBuf, pMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen, 0 );

    /* free buffer*/
    if (psendBuf)
    {
        free(psendBuf);
        psendBuf = NULL;
    }

    /* check status */
    if (iResult == -1)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgSend: sock %d has error %d\n", ConnectSocket, DDC_WSAGetLastError());
        return DDC_SOCK_MSG_SEND_FAIL;
    }
    else if (iResult != pMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgSend: sock %d send %d bytes, xpctd %d\n", ConnectSocket, iResult, pMsgStruct->ddcSockMsgHeader.ddcSockMsgCurrentLen);
        return DDC_SOCK_MSG_ERROR;
    }

    DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_INFO, "Bytes Sent: %d\n", iResult);
    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockRecv
 *
 * Description:
 *      This function receives certain length of data from the socket.
 *
 * In   ConnectSocket   socket that receives the message from
 * In   Length          length of the data expected
 * In   TimeOut         wait for how long in seconds, -1: wait for ever
 * Out  Buf             the received data
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockRecv
(
    DDC_SOCKET ConnectSocket,
    U32BIT u32Length,
    S32BIT s32TimeOut,
    U8BIT * pBuf
)
{
    u_long iMode = 0;
    int iExpectLen = 0;
    int iResult = 0;
    S32BIT s32TimePassed = 0;

    char *recvbuf;
    char * pBufTmp = NULL;
    int iError = 0;

    /* Debug only*/
#if 0
    static int  failCount = 0;
#endif

    /* allocate buffer */
    recvbuf = (char *) malloc(DEFAULT_BUFLEN);
    if (recvbuf == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockRecv: malloc recvbuf Failed!");
        return DDC_SOCK_ERROR;
    }

    if (s32TimeOut > 0)
    {
        /* enable non-blocking mode */
#ifdef WIN32
        iMode = 1;
        ioctlsocket(ConnectSocket, FIONBIO, &iMode);
#endif
#ifdef LINUX
        fcntl(ConnectSocket, F_GETFL, iMode);
        iMode |= O_NONBLOCK;
        fcntl(ConnectSocket, F_SETFL, iMode);
#endif
#ifdef VX_WORKS
        fcntl(ConnectSocket, F_GETFL, iMode);
        iMode |= O_NONBLOCK;
        fcntl(ConnectSocket, F_SETFL, iMode);
#endif
    }
    else if (s32TimeOut == -1)
    {
        /* disable non-blocking mode */
#ifdef WIN32
        iMode = 0;
        ioctlsocket(ConnectSocket, FIONBIO, &iMode);
#endif
#ifdef LINUX
        fcntl(ConnectSocket, F_GETFL, iMode);
        iMode &= (~O_NONBLOCK);
        fcntl(ConnectSocket, F_SETFL, iMode);
#endif
#ifdef VX_WORKS
        fcntl(ConnectSocket, F_GETFL, iMode);
        iMode &= (~O_NONBLOCK);
        fcntl(ConnectSocket, F_SETFL, iMode);
#endif
    }

    iExpectLen = u32Length;
    pBufTmp = (char *)pBuf;
    do
    {
        if (s32TimeOut > 0)
        {
            if (s32TimePassed >= s32TimeOut*1000)
            {
                if (recvbuf != NULL)
                {
                    free(recvbuf);
                    recvbuf = NULL;
                }

                DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockRecv: timeout\n");
                return DDC_SOCK_TIMEOUT;
            }
        }

        iResult = recv(ConnectSocket, recvbuf, iExpectLen, 0);

        if (iResult > 0)
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_INFO, "Bytes received: %d\n", iResult);

            if (iResult < iExpectLen)
            {
                DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_INFO, "Message Not Done: expect %d, recv %d\n", iExpectLen, iResult);
                memcpy(pBufTmp, recvbuf, iResult);
                pBufTmp = pBufTmp + iResult;
                iExpectLen = iExpectLen - iResult;

                continue;
            }
            else if (iResult == iExpectLen)
            {
                /* copy data over */
                memcpy(pBufTmp, recvbuf, iResult);

                if (recvbuf != NULL)
                {
                    free(recvbuf);
                    recvbuf = NULL;
                }

                return DDC_SOCK_SUCCESS;
            }
            else
            {
                DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "Message Length Error: expect %d, recv %d\n", iExpectLen, iResult);
                continue;
            }
        }
        else if ( iResult == 0 )
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_INFO, "Connection closed sock %d\n", (int)ConnectSocket);

            if (recvbuf != NULL)
            {
                free(recvbuf);
                recvbuf = NULL;
            }

            return DDC_SOCK_CLOSED;
        }
        else if (iResult < 0)
        {
            /* if no data available (non-blocking socket) */
            iError = DDC_WSAGetLastError();

#ifdef WIN32
            if (iError == WSAEWOULDBLOCK)
#endif /* WIN32 */
#ifdef LINUX
            if (iError == EAGAIN)
#endif /* LINUX */
#ifdef VX_WORKS
            if (iError == EWOULDBLOCK)
#endif /* VX_WORKS */
            {
                DDC_SOCK_SLEEP_MS(1);
                s32TimePassed++;
                continue;

            }
            else
            {
                /* doing nothing here. Open the debug only when it is needed */
#if 0
                /* print error info every 100 errors */
                failCount++;
                if (!((failCount+99) % 100))
                {
                    DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockRecv: recv failed with error: %d\n", iError);
                }
#endif

               break;
            }
        }

    } while (1);

    if (recvbuf != NULL)
    {
        free(recvbuf);
        recvbuf = NULL;
    }

    DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockRecv: recv failed with error: %d\n", iError);
    return DDC_SOCK_MSG_RECV_FAIL;
}

/*******************************************************************************
 * Name:    DDCSockMsgRecvRaw
 *
 * Description:
 *      This function receives one message from the socket.
 *
 * In   ConnectSocket   socket that receives the message from
 * In   MsgType         type of the expected message
 * In   MsgCmd          command of the expected message
 * In   TimeOut         wait for how long in seconds, -1: wait for ever
 * Out  MsgBuf          received message
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockMsgRecvRaw
(
    DDC_SOCKET ConnectSocket,
    U32BIT u32MsgType,
    U32BIT u32MsgCmd,
    S32BIT s32TimeOut,
    U8BIT * pMsgBuf
)
{
    S16BIT s16Result = 0;
    char *recvbuf    = NULL;
    U32BIT u32MsgHeaderLen = sizeof(DDC_SOCK_MSG_HEADER);  /* length of the message header */
    U32BIT u32MsgFieldsLen = 0;                            /* length of all message fields data */
    PDDC_SOCK_MSG_HEADER pRecvMsgHeader = NULL;

    /* allocate buffer */
    recvbuf = (char *) malloc(DEFAULT_BUFLEN);

    if (recvbuf == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgRecvRaw: malloc recvbuf Failed!");
        return DDC_SOCK_ERROR;
    }

    /* first get the message header */
    s16Result = DDCSockRecv(ConnectSocket, u32MsgHeaderLen, s32TimeOut, (U8BIT *)recvbuf);

    if (s16Result)
    {
        if (recvbuf != NULL)
        {
            free(recvbuf);
            recvbuf = NULL;
        }

        if (s16Result != DDC_SOCK_CLOSED)
        {
            /* doing nothing here. Open the debug only when it is needed */
            /*DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgRecvRaw: DDCSockRecv failed\n");*/
        }

        return s16Result;
    }

    pRecvMsgHeader = (PDDC_SOCK_MSG_HEADER)recvbuf;

    if (pRecvMsgHeader->ddcSockMsgType != u32MsgType)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "Message Type Error: in header %d, expect %d\n", pRecvMsgHeader->ddcSockMsgType, u32MsgType);

        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddcSockMsgCmd ......... %d (%08x)\n", pRecvMsgHeader->ddcSockMsgCmd, pRecvMsgHeader->ddcSockMsgCmd);
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddcSockMsgFieldNum .... %d (%08x)\n", pRecvMsgHeader->ddcSockMsgFieldNum, pRecvMsgHeader->ddcSockMsgFieldNum);
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddcSockMsgType ........ %d (%08x)\n", pRecvMsgHeader->ddcSockMsgType, pRecvMsgHeader->ddcSockMsgType);
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddcSockMsgCurrentLen .. %d (%08x)\n", pRecvMsgHeader->ddcSockMsgCurrentLen, pRecvMsgHeader->ddcSockMsgCurrentLen);
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "ddcSockMsgMaxLen ...... %d (%08x)\n", pRecvMsgHeader->ddcSockMsgMaxLen, pRecvMsgHeader->ddcSockMsgMaxLen);


        if (recvbuf != NULL)
        {
            free(recvbuf);
            recvbuf = NULL;
        }

        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgRecvRaw: DDC_SOCK_MSG_ERROR\n");

        return DDC_SOCK_MSG_ERROR;
    }

    if (u32MsgCmd != DDC_SOCK_MSG_CMD_ANY)
    {
        if (pRecvMsgHeader->ddcSockMsgCmd != u32MsgCmd)
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "Message Command Error: in header %d, expect %d\n", pRecvMsgHeader->ddcSockMsgCmd, u32MsgCmd);

            if (recvbuf != NULL)
            {
                free(recvbuf);
                recvbuf = NULL;
            }

            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgRecvRaw-2: DDC_SOCK_MSG_ERROR\n");
            return DDC_SOCK_MSG_ERROR;
        }
    }

    /* copy header over */
    memcpy(pMsgBuf, pRecvMsgHeader, u32MsgHeaderLen);

    /* then get all the fields of the message */
    u32MsgFieldsLen = pRecvMsgHeader->ddcSockMsgCurrentLen - u32MsgHeaderLen;
    memset(recvbuf, 0, DEFAULT_BUFLEN);
    s16Result = DDCSockRecv(ConnectSocket, u32MsgFieldsLen, s32TimeOut, (U8BIT *)recvbuf);

    if (s16Result)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgRecvRaw: call DDCSockRecv() failed, %d\n", s16Result);

        if (recvbuf != NULL)
        {
            free(recvbuf);
            recvbuf = NULL;
        }

        return s16Result;
    }

    /* copy all fields over */
    memcpy(pMsgBuf+u32MsgHeaderLen, recvbuf, u32MsgFieldsLen);

    if (recvbuf != NULL)
    {
        free(recvbuf);
        recvbuf = NULL;
    }

    return DDC_SOCK_SUCCESS;
}


/*******************************************************************************
 * Name:    DDCSockMsgRecv
 *
 * Description:
 *      This function receives one message from the socket and decode the message.
 *
 * In   ConnectSocket   socket that receives the message from
 * In   MsgType         type of the expected message
 * In   MsgCmd          command of the expected message
 * In   TimeOut         wait for how long in seconds, -1: wait for ever
 * Out  pMsgStruct      decoded received message
 *
 * Returns: error condition
 ******************************************************************************/
S16BIT DDCSockMsgRecv
(
    DDC_SOCKET ConnectSocket,
    U32BIT u32MsgType,
    U32BIT u32MsgCmd,
    S32BIT s32TimeOut,
    PDDC_SOCK_MSG_STRUCT pMsgStruct
)
{
    S16BIT s16Result = 0;
    U8BIT  *recvMsgBuf = NULL;

    /* allocate buffer */
    recvMsgBuf = (U8BIT *) malloc(DEFAULT_BUFLEN);

    if (recvMsgBuf == NULL)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgRecv: malloc recvMsgBuf Failed!");
        return DDC_SOCK_ERROR;
    }

    s16Result = DDCSockMsgRecvRaw(ConnectSocket, u32MsgType, u32MsgCmd, s32TimeOut, recvMsgBuf);

    if (s16Result)
    {
        if (s16Result == DDC_SOCK_CLOSED)
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_INFO, "Socket %d Closed\n", ConnectSocket);
        }
        else
        {
            DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgRecvRaw error %d\n", s16Result);
        }

        if (recvMsgBuf)
        {
            free(recvMsgBuf);
            recvMsgBuf = NULL;
        }

        return s16Result;
    }

    s16Result = DDCSockMsgDecode(recvMsgBuf, pMsgStruct);

    if (recvMsgBuf)
    {
        free(recvMsgBuf);
        recvMsgBuf = NULL;
    }

    if (s16Result)
    {
        DDC_SOCK_DBG_PRINT(DDC_SOCK_DBG_ERROR, "DDCSockMsgDecode error %d\n", s16Result);
        return s16Result;
    }

    return DDC_SOCK_SUCCESS;
}

/*******************************************************************************
 * Name:    DDCSockDataSet
 *
 * Description:
 *
 *
 * In   pData
 * In   u16Index
 * In   u32FieldType
 * In   u32FieldDataLen
 * In   pDataBuf
 * Out
 *
 * Returns: none
 ******************************************************************************/
void DDCSockDataSet
(
    DDC_SOCK_DATA_TYPE* pData,
    U16BIT u16Index,
    U32BIT u32FieldType,
    U32BIT u32FieldDataLen,
    void *pDataBuf
)
{
    pData[u16Index].u32Type = u32FieldType;
    pData[u16Index].u32Size = u32FieldDataLen;
    pData[u16Index].pBuf    = pDataBuf;
}
