/*
 Driver.h: PCMCIA device driver header file
*/


#ifndef TS_DRIVER_H
#define TS_DRIVER_H
/* IO register constants */
#define IO_DSP_RESET		0x00
#define IO_PAGE_OPT			0x02
#define IO_PAGE_SEL			0x04
#define IO_IRQ_REQ			0x06
#define IO_DAC				0x08
#define IO_RELAYS			0x0a
#define IO_IRIG				0x0c
#define IO_IRQ_CHAN			0x0e
#define IO_IRIG_LOW			0x10
#define IO_IRIG_HIGH		0x12
#define IO_GFPGA_VER		0x14
#define IO_DMA_DEST_LOW		0x20
#define IO_DMA_DEST_HIGH	0x22
#define IO_DMA_BLOCK_SIZE	0x24
#define IO_DMA_STATUS		0x26
#define IO_PCI_OFFSET		0x10000		/* PCI cards have I/O mapped 64K Words above mem base address */

/* Case statement labels for DeviceIOCtrl calls */
#define PCIDEA_GET_VERSION		0x01  
#define PCIDEA_GET_DEVNODE		0x02
#define PCIDEA_GET_ATTRADDR		0x03  
#define PCIDEA_GET_MEMADDR		0x04  
#define PCIDEA_GET_IOPORT		0x05
#define PCIDEA_GET_IRQLEVEL		0x06   
#define PCIDEA_GET_MEMLENGTH	0x07
#define PCIDEA_GET_SOCKET		0x08   
#define PCIDEA_GET_STATUS		0x09
#define PCIDEA_SET_PAGE			0x11
#define PCIDEA_SET_MAILBOX		0x14
#define PCIDEA_SETUP_DMA		0x15
#define PCIDEA_HALTDMA			0x16
#define PCIDEA_CLOSE_DEVICE		0x31

/* The following calls are included for legacy support */
#define PCIDEA_SET_HIRQ			0x12   
#define PCIDEA_SET_SAME_HIRQ	0x13
#define PCIDEA_SIMULATE_IRQ		0x21


/* New deviocontrols for multiple card support */
#define IOCTL_GET_VERSION_EX		0x101  
#define IOCTL_DEVNODE_FROM_ID		0x150
#define IOCTL_INSTALL_IRQEVENT		0x1A0
#define IOCTL_UNINST_IRQEVENT		0x1A1
#define IOCTL_IRQ_BLOCK				0x1A2
#define IOCTL_READ_FILE				0x1A3
#define IOCTL_INSTALL_DMAEVENT		0x1A4
#endif /* TS_DRIVER_H */

