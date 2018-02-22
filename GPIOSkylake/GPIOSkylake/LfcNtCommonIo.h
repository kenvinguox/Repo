//*****************************************************************************
// Copyright (c) 2012 - 2013, Hefei LCFC Information Technology Co.Ltd. 
// And/or its affiliates. All rights reserved. 
// Hefei LCFC Information Technology Co.Ltd. PROPRIETARY/CONFIDENTIAL. 
// Use is subject to license terms.         
// Note: this driver is based on WinIo(Copyright 1998-2010 Yariv Kaplan)
//******************************************************************************
 /*++
Abstract:
  This driver will handle common IO request, such as IO, Memory, PCI, CMOS Ram
EC Ram, MSR register.
 
History:
  Date          Name          Version    Change Notes
  2014.08.19    Steven Wang   V1.00      Initial release.

Module Name:
  LfcNtCommonIoDrv.h
  
Compile Method:
  run "build" at Windows Win7 X64 checked Build Environment => X64 driver
  run "build" at Windows Win7 X86 checked Build Environment => X86 driver
 --*/
#ifndef LFC_NT_COMMON_IO_DRV_H
#define LFC_NT_COMMON_IO_DRV_H

// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.

#define FILE_DEVICE_WINIO 0x00008010

// Macro definition for defining IOCTL and FSCTL function control codes.
// Note that function codes 0-2047 are reserved for Microsoft Corporation,
// and 2048-4095 are reserved for customers.

#define WINIO_IOCTL_INDEX 0x810

// Define our own private IOCTL

#define IOCTL_WINIO_MAPPHYSTOLIN     CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX,      \
	METHOD_BUFFERED,        \
	FILE_ANY_ACCESS)

#define IOCTL_WINIO_UNMAPPHYSADDR    CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 1,  \
	METHOD_BUFFERED,        \
	FILE_ANY_ACCESS)

#define IOCTL_WINIO_ENABLEDIRECTIO   CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 2,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)

#define IOCTL_WINIO_DISABLEDIRECTIO  CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 3,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)

#define IOCTL_WINIO_READPORT		 CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 4,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)

#define IOCTL_WINIO_WRITEPORT		 CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 5,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)
	              
#define IOCTL_LFC_PCI_FUNCTION_DWORD  CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 6,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)	            
	  
#define IOCTL_LFC_EC_FUNCTION         CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 7,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)	

#define IOCTL_LFC_EC_RAM_RW_FUNCTION  CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 8,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)
	
#define IOCTL_LFC_EC_DIAG_RAM_RW_FUNCTION  CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 9,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)
	
#define IOCTL_LFC_CMOS_FUNCTION       CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 10,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)	
	
#define IOCTL_LFC_MSR_FUNCTION       CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 11,   \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)	
	
#define IOCTL_LFC_EC_SMBUS_FUNCTION  CTL_CODE (FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 12,  \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)		
	
#define IOCTL_LFC_GETPHYSADDR		 CTL_CODE(FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 13,  \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)

#define IOCTL_LFC_TRIGGERSMI		 CTL_CODE(FILE_DEVICE_WINIO,  \
	WINIO_IOCTL_INDEX + 14,  \
	METHOD_BUFFERED,         \
	FILE_ANY_ACCESS)
//
// Debug definition section
//
extern ULONG		STVDebug;
#define STVDbgPrint(a,b)    if(a & STVDebug) DbgPrint b

#define STV_TRACKING                           0x00000001
#define STV_WARNNING                           0x00000002
#define STV_ERROR                              0x00000004
#define STV_IRP_ERROR                          0x00000008
#define STV_INVALID_PARA                       0x12340001     
#define STV_INVALID_EC_SMBUS_CMD               0x12340002
#define STV_READ_PCI_FAIL                      0x12340003

#pragma pack(push)
#pragma pack(1)
struct tagPhysStruct
{
	DWORD64 dwPhysMemSizeInBytes;
	DWORD64 pvPhysAddress;
	DWORD64 PhysicalMemoryHandle;
	DWORD64 pvPhysMemLin;
	DWORD64 pvPhysSection;
};

struct tagPortStruct
{
	USHORT wPortAddr;
	ULONG dwPortVal;
	UCHAR bSize;
};

struct RegisterTable
{
  ULONG RegEax;
  ULONG RegEbx;
  ULONG RegEcx;
  ULONG RegEdx;
  ULONG RegEsi;
  ULONG RegEdi;
};

typedef struct _PCI_EXCHANGE_BUFFER
{
	ULONG BusNum;
	ULONG DeviceNum;
	ULONG FunctionNum;
	ULONG Offset;
	ULONG RWOperation; //0: read, 1: write
	ULONG Value;
} PCI_EXCHANGE_BUFFER, *PPCI_EXCHANGE_BUFFER; 

typedef struct _EC_EXCHANGE_BUFFER
{
	UINT8 CommandPort;
	UINT8 DataPort;
	UINT8 EcCommand;
	UINT8 EcCommandData;
	UINT8 EcCommandDataNum;
	UINT8 DataReturnFlag;
	UINT8 Offset;
	UINT8 Data;
} EC_EXCHANGE_BUFFER, *PEC_EXCHANGE_BUFFER; 

typedef struct _EC_RAM_RW_BUFFER
{
	UINT8 EcCommand; //0x80: read, 0x81: write
	UINT8 Offset;
	UINT8 Data;
} EC_RAM_RW_BUFFER, *PEC_RAM_RW_BUFFER; 

typedef struct _EC_DIAG_RAM_RW_BUFFER
{
	UINT8 EcDiagCommand; //0x90: read, 0x91: write
	UINT8 Offset;
	UINT8 Data;
} EC_DIAG_RAM_RW_BUFFER, *PEC_DIAG_RAM_RW_BUFFER; 

typedef struct _CMOS_EXCHANGE_BUFFER
{
	UINT8 CmosRamIndexPort;
	UINT8 CmosRamDataPort;
	UINT8 CMosRamOffset;
	UINT8 CmosRamData;
	UINT8 CmosCommand;
} CMOS_EXCHANGE_BUFFER, *PCMOS_EXCHANGE_BUFFER;

typedef struct _MSR_EXCHANGE_BUFFER
{
	ULONG MSRAddress;
	ULONG MSRRwOperation;
	ULONG MSR_HI;
	ULONG MSR_LOW;
} MSR_EXCHANGE_BUFFER, *PMSR_EXCHANGE_BUFFER;

typedef struct _EC_SMBUS_EXCHANGE_BUFFER
{
	UINT8 EcRamCommandPort;
	UINT8 EcRamDataPort;
	UINT8 SlaveAddress;
	UINT8 Command;
	UINT8 Offset;
	UINT8 Data[31];
} EC_SMBUS_EXCHANGE_BUFFER, *PEC_SMBUS_EXCHANGE_BUFFER;

#define PCI_CONFIG_ADDRESS(bus, dev, func, reg) \
   (0x80000000 + (((ULONG) bus) << 16) + (((ULONG) dev) << 11) + (((ULONG) func) << 8) + ((ULONG) reg))
   
#define PCI_CONFIG_INDEX       0xcf8
#define PCI_CONFIG_DATA        0xcfc
#define PCI_READ_REQ           0x00
#define PCI_WRITE_REQ          0x01
#define EC_CMD_PORT            0x66
#define EC_DATA_PORT           0x62
#define EC_CMD_PORT_EX         0x6C
#define EC_DATA_PORT_EX        0x68
#define EC_DIAG_SPACE_READ     0x90
#define EC_DIAG_SPACE_WRITE    0x91
#define CMOS_READ_REQ          0x0
#define CMOS_WRITE_REQ         0x1
#define MSR_READ_REQ           0x0
#define MSR_WRITE_REQ          0x1
#define KBC_CMD_PORT           0x64
#define KBC_DATA_PORT          0x60

#define EC_READ_ECRAM_CMD      0x80
#define EC_WRITE_ECRAM_CMD     0x81
#define EC_SMBUS_COMMNAD       0x60
#define EC_SMBUS_SLAVE_ADR     0x62
#define EC_SMBUS_OFFSET        0x63
#define EC_SMBUS_DATA0         0x64
#define EC_SMBUS_BLOCK_LEN     31
#define EC_SMBUS_READ_BYTE     0x07
#define EC_SMBUS_WRITE_BYTE    0x06
#define EC_SMBUS_READ_WORD     0x09
#define EC_SMBUS_WRITE_WORD    0x08
#define EC_SMBUS_READ_BLOCK    0x0B
#define EC_SMBUS_WRITE_BLOCK   0x0A

#define EC_DIAG_SPACE_CMD      0x00
#define EC_DIAG_SPACE_DATA     0x01
#define EC_DIAG_SPACE_ADR_LOW  0x02
#define EC_DIAG_SPACE_ADR_HIGH 0x03

#define KEY_OBF                1
#define KEY_IBF                2
#define KBC_TIME_OUT           0x10000

#pragma pack(pop)
#endif
