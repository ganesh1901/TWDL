/*            IOCTL Definitions           */
#define IOCTL_INDEX                       0x0800
#define IOCTL_DEVICE_TYPE                 0x8000
#define IOCTL_REGISTER_ACCESS             0
#define IOCTL_MEMORY_ACCESS               1

#define IOCTL_IMP_MRT_CMD                 0
#define IOCTL_IMP_MRT_DATA                1
#define IOCTL_IMP_BC_CMD                  2
#define IOCTL_IMP_BC_DATA                 3

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/*                             THIS IS BLOCK #1 TO BE MERGED                                    */
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/* KMDF IOCTL COMMANDS */

/*#define IOCTL_DDCSFPUSB_GET_CONFIG_DESCRIPTOR  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 0, METHOD_BUFFERED, FILE_ANY_ACCESS)*/
#define IOCTL_GETINFO                       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 0,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BLOCK_ON_IRQ                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 3,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_RELEASE_IRQ                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 4,  METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_INFOREG                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 16, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_CONFIG_DESCRIPTOR         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 20, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_RESET_DEVICE                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_RESET_PIPE                    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 22, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_USB_DEVICE_CAPS           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 25, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_USB_VENDOR_OUT                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 32, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BOARD_COMPONENT_RESET         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 33, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ARINC_429_PROGRAMABLE_CONFIG  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 34, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_REG_READ                      CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 50, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_WRITE                     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 51, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MEM_READ                      CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 52, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MEM_WRITE                     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 53, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_STACK_READ                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 54, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_CMD_STACK_WRITE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 55, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DATA_STACK_READ               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 56, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DATA_STACK_WRITE              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 57, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DATA_SEND_STREAM              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 58, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_DATA_RECEIVE_STREAM           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 59, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_REG_READ32                    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 60, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REG_WRITE32                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 61, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MEM_READ32                    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 62, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_MEM_WRITE32                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 63, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_REG_READ_BLK                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 64, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_REG_WRITE_BLK                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 65, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_MEM_READ_BLK                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 66, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_MEM_WRITE_BLK                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 67, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_GET_TIME_TAG                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 70, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_TIME_TAG                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 71, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_IRIG_TX                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 72, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_IRIG_TX                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 73, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_IRIG_ID                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 74, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_SET_RESP_TIME_OUT             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 80, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_WAIT                          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 85, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_SET_TT_RESOLUTION             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 90, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_TT_ROLLOVER_POINT         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 91, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_TT_INTERRUPT              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 92, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_EXT_TT_CNT_CTRL               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 93, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_SET_RAM_PARITY_CHECKING       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 95, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_GET_CMD_STK_AREA              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 100, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_IMP_READ                      CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 110, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_DEVICE_CONFIGURE              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 119, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_BC_GET_ASYNC_COUNT            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 120, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_GET_DATA_BUFFER            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 121, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_GET_FRAME_TO_HBUF          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 122, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_GET_HBUF_MESSAGE           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 123, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_GET_METRICS                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 124, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_GET_MESSAGE                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 125, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_GPQ_CLEAR                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 126, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_GPQ_COUNT                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 127, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_GPQ_READ                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 128, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_POST_ASYNC_MESSAGE         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 129, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_SET_MSG_BUFFER             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 130, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_SET_DATA_BUFFER            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 131, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_STATE                      CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 132, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_DATASTR_INIT               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 133, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_DATASTR_STATUS             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 134, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_DATA_ARRAY_INIT            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 135, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_DATA_ARRAY_STATUS          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 136, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_EXT_TRIGGER_AVAIL          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 137, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_ASYNC_QUEUE_INFO           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 138, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_CMD_STACK_READ             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 139, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_DATA_STACK_READ            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 140, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_CLEAR_MEMORY                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 150, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DEVICE_INIT                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 151, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_GET_AIO_OUTPUT                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 153, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_AIO_DIRECTION             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 154, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_AIO_INPUT                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 155, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_AIO_ALL                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 156, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_AIO_OUTPUT                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 157, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_AIO_DIRECTION             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 158, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_AIO_ALL                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 159, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_GET_DIO_OUTPUT                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 160, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_DIO_DIRECTION             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 161, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_DIO_INPUT                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 162, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_DIO_ALL                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 163, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_DIO_OUTPUT                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 164, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_DIO_DIRECTION             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 165, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_DIO_ALL                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 166, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_GET_IO_COUNT                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 167, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_GET_MTI_CH10_DATA_PKT         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 170, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_MTI_CH10_TIME_PKT         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 171, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_SET_MTI_STATE                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 181, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_MTI_CH10_DATA_PKT_FREE    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 182, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_MTI_CH10_TIME_PKT_FREE    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 183, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_MTI_FREE                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 184, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_MTI_STROBE_REG            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 185, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_MTI_CH10_STATUS           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 186, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_MTI_FREE_MEM_COUNT            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 187, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_MTI_INTERUPT_CONFIG           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 188, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_MTI_GET_METRICS_CONFIG        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 189, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_IRIG_SET_INT_STATE            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 190, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ARINC_LOADTXONE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 195, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_GET_TT_CONFIG           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 196, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_SET_TT_CONFIG           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 197, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_GET_PGRM_LOOPBACK       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 198, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_SET_PGRM_LOOPBACK       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 199, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_REG_READ                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 200, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_REG_WRITE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 201, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_MEM_READ                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 202, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_MEM_WRITE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 203, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_BLOCK_ON_IRQ            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 204, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ARINC_RELEASE_IRQ             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 205, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ARINC_MEM_READ_MORE           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 206, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_SET_OLD_LOOPBACK        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 207, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_GET_OLD_LOOPBACK        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 208, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_LOADTXMORE              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 209, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_BLK_READ                      CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 210, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BLK_WRITE                     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 211, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_CAPABILITIES              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 212, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_GET_ENHANCED_CAPABILITIES     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 213, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ACTEL_REG_READ                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 220, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ACTEL_REG_WRITE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 221, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_FLASH_MEM_READ                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 222, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_FLASH_MEM_WRITE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 223, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_FLASH_MEM_BLOCK_ERASE         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 224, METHOD_IN_DIRECT,  FILE_ANY_ACCESS)
#define IOCTL_FLASH_MEM_WRITE_PROTECTED     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 225, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_FLASH_MEM_INFO                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 226, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_GET_INTERRUPT_MSG             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 227, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* IMPROVEMENTS BLOCK SPECIFIC */
#define IOCTL_IMP_CFG                       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 228, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_GET_HW_VERSION_INFO           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 230, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_DEVICE_STATUS                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 231, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_DATA_ARCHIVE              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 232, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_GET_COUPLING                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 240, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_COUPLING                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 241, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_GET_AMPLITUDE                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 243, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_AMPLITUDE                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 244, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_MODULE_INTERRUPT_STATE    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 250, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SET_IRQ_CONDITIONS            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 251, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_RESERVED_01                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 255, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Multi-Function support*/
#define IOCTL_CHECK_MF_CAPABLE              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 260, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_REPLAY_STATUS              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 261, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_REPLAY_BLOCK_ON_IRQ        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 262, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_REPLAY_RELEASE_IRQ         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 263, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_REPLAY_DATA                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 264, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_BC_REPLAY_RAW                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 265, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BC_REPLAY_WRITE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 266, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_TRG_RESET                     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 270, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_TRG_IRQ_STATUS                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 271, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ARINC_717_LOAD_TX             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 290, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ARINC_717_GET_RX              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 291, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_CAN_BUS_TX_DATA               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 295, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_CAN_BUS_RX_DATA               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 296, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ARINC_RX_HBUF_CTRL            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 500, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ARINC_GET_QUEUE_STATUS        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 501, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

/* control operations */
#define ACEX_MRT_HBUF_ENABLE                   0
#define ACEX_MRT_HBUF_DISABLE                  1
#define ACEX_MRT_HBUF_GET                      2
#define ACEX_MRT_RTCMD_HIGH_PCT_RESET          3
#define ACEX_MRT_STREAMING_ENABLE_RX           4
#define ACEX_MRT_STREAMING_ENABLE_TX           5
#define ACEX_MRT_STREAMING_DISABLE_RX          6
#define ACEX_MRT_STREAMING_DISABLE_TX          7
#define ACEX_MRT_DATA_ARRAY_ENABLE             8
#define ACEX_MRT_DATA_ARRAY_DISABLE            9
#define ACEX_MRT_DATA_ARRAY_ENABLE_CONTINUOUS  10
#define ACEX_MRT_DATA_ARRAY_DISABLE_INT        11

/************************************************************************************************/
/************************************************************************************************/
/*                             SUB-COMMANDS!!!!!!!!!!!!!!!!!                                    */
/************************************************************************************************/
/************************************************************************************************/

/* IOCTL_DDCSFP_1553_CONFIGURE SUB-COMMANDS
   type definitions associated with ACEX_CONFIG_ID */
#define ACEX_MRT_CONFIG                 1
#define ACEX_MRT_FREE                   2
#define ACEX_RT_OPEN                    3
#define ACEX_RT_CONFIG_SET              4 /* enable configuration features after open */
#define ACEX_RT_CONFIG_CLR              5 /* disable configuration features after open */
#define ACEX_RT_BITWD_RD                6
#define ACEX_RT_START                   7
#define ACEX_RT_STOP                    8
#define ACEX_RT_CLOSE                   9
#define ACEX_RT_LATCH_CTRL              10
#define ACEX_RT_SOURCE_CTRL             11
#define ACEX_MRT_HBUF_CTRL              12
#define ACEX_MRT_CMDSTK_STATS           13
#define ACEX_MRT_CMDSTK_SET_TO_LATEST   14
#define ACEX_MRT_GET_BRDCST_ADDRS       15
#define ACEX_RT_STREAM_CTRL             16
#define ACEX_RT_START_RESET_CMDSTK      17
#define ACEX_RT_DATA_ARRAY_CTRL         18
#define ACEX_MRT_CONFIG_UPDATE          19 /* this command is used to update mrt global config after
                                              initial configuration is completed */
#define ACEX_UMT_CONFIG                 20
#define ACEX_UMT_FREE                   21
#define ACEX_UMT_INIT                   22

#define BC_HOST_ID_MSG     0
#define BC_HOST_ID_STREAM  1
#define BC_HOST_ID_ARRAY   2
#define BC_HOST_ID_LP      3
#define BC_HOST_ID_HP      4

#define ACEX_BC_INIT                    40
#define ACEX_BC_CONFIG                  41
#define ACEX_BC_FREE                    42
#define ACEX_BC_HBUF_INSTALL            43
#define ACEX_BC_HBUF_UNINSTALL          44
#define ACEX_BC_GPQ_INIT                45
#define ACEX_BC_LPQ_INIT                46
#define ACEX_BC_HPQ_INIT                47
#define ACEX_BC_REPLAY_INIT             48
#define ACEX_BC_MUX_INIT                49
#define ACEX_ARINC_429_PROGRAMABLE      50

#define ACEX_ARINC_717_CONFIG           70
#define ACEX_ARINC_717_FREE             71
#define ACEX_ARINC_717_STATE            72
#define ACEX_ARINC_717_INTERRUPTS       73

#define ACEX_CAN_BUS_CONFIG             74
#define ACEX_CAN_BUS_FREE               75
#define ACEX_CAN_BUS_STATE              76
#define ACEX_CAN_FIRWARE_VERSION        77

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/*                             THIS IS BLOCK #2 TO BE MERGED                                    */
/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
#define IOCTL_ACENT_GETINFO                     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 0, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ACENT_BLOCK_ON_IRQ                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ACENT_RELEASE_IRQ                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 4, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_ACENT_REG_READ                    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 50, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_REG_WRITE                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 51, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_MEM_READ                    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 52, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_MEM_WRITE                   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 53, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_CMD_STACK_READ              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 54, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_CMD_STACK_WRITE             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 55, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_DATA_STACK_READ             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 56, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_DATA_STACK_WRITE            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 57, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_REG_READ32                  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 60, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_REG_WRITE32                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 61, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_TIME_TAG                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 70, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_TIME_TAG                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 71, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_IRIG_TX                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 72, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_IRIG_TX                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 73, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_IRIG_ID                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 74, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_CMD_STK_AREA            CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 100, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_BC_DATA_BLOCK_ADDR      CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 105, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_MT_CMD_STK_PTR          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 110, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_MT_CMD_STK_NEXT         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 111, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_MT_CMD_STK_LATEST       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 112, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_MT_CMD_STK_DATA         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 113, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_MT_CMD_STK_DATA_COUNT   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 114, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_RT_CMD_STK_PTR          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 120, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_RT_CMD_STK_NEXT         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 121, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_RT_CMD_STK_LATEST       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 122, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_RT_CMD_STK_DATA         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 123, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_RT_DATA_BLK_OFFSET      CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 126, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_RT_DATA_BLK_PTR         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 127, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_RT_SUB_ADDR_CTL_WORD    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 128, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_RT_AUTO_BOOT_INFO       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 129, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_SET_MT_CMD_STK_PURGE        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 130, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MT_CMD_STK_PTR          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 131, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MT_DATA_STK_PTR         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 132, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MT_CMD_STK_SIZE         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 133, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MT_DATA_STK_SIZE        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 134, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_SET_RT_CMD_STK_PURGE        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 140, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_RT_CMD_STK_PTR          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 141, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_RT_DATA_BLK_PTR         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 142, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_RT_CMD_STK_SIZE         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 143, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_RT_SUB_ADDR_CTL_WORD    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 145, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_CLEAR_MEMORY                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 150, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_DEVICE_INIT                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 151, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_SET_BC_MESSAGE_BLK          CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 160, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_BC_MESSAGE_DATA_PTR     CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 161, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_BC_EXT_TRIGGER_AVAIL    CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 162, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_MTI_CH10_DATA_PKT       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 170, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_MTI_CH10_TIME_PKT       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 171, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_SET_MTI_CONFIG              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 180, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MTI_STATE               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 181, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MTI_CH10_DATA_PKT_FREE  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 182, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MTI_CH10_TIME_PKT_FREE  CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 183, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_MTI_FREE                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 184, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GP_REG_READ                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 210, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GP_REG_WRITE                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 211, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_FLASH_MEM_READ              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 222, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_FLASH_MEM_WRITE             CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 223, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_FLASH_MEM_BLOCK_ERASE       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 224, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_FLASH_MEM_WRITE_PROTECTED   CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 225, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_FLASH_MEM_INFO              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 226, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_HW_VERSION_INFO         CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 230, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_DIO_TT_CFG                        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 275, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_DIO_TT_CTL                        CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 276, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_DIO_TT_READ                       CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 277, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_DIO_OUTPUT              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 300, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_DIO_DIRECTION           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 301, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_DIO_INPUT               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 302, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_DIO_ALL                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 303, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_SET_DIO_OUTPUT              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 310, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_DIO_DIRECTION           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 311, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_DIO_ALL                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 312, METHOD_IN_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_IO_COUNT                CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 315, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_GET_AIO_OUTPUT              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 400, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_AIO_DIRECTION           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 401, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_AIO_INPUT               CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 402, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_GET_AIO_ALL                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 403, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_ACENT_SET_AIO_OUTPUT              CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 410, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_AIO_DIRECTION           CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 411, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_ACENT_SET_AIO_ALL                 CTL_CODE(IOCTL_DEVICE_TYPE, IOCTL_INDEX + 412, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
