
#include "stdafx.h"
#include "8502EC.h"
#include "LfcNtCommonIoDll.h"
#pragma comment(lib, "LfcIo64.lib")

//------------------------------------
// wait EC input buffer empty
//------------------------------------
void wait_EC_IBE(BYTE Port)
{ 
	DWORD dwPortVal = 0x02;
	int i;
	for(i=0;i<0xFFFF;i++)
	{
		bool hr = GetPortVal(Port, (PDWORD)&dwPortVal, 1);
		if (!(dwPortVal & EC_IBF))
		{
			break;
		}
	}
}
//------------------------------------
// wait EC output buffer empty
//------------------------------------
void wait_EC_OBE(BYTE Port)
{
	DWORD dwPortVal = 0;
	int i;
	for (i = 0; i<0xFFFF; i++)
	{
		bool hr = GetPortVal(Port, &dwPortVal, 1);
		if (!(dwPortVal & EC_OBF))
		{
			break;
		}
	}
}
//------------------------------------
// wait EC output buffer full
//------------------------------------
void wait_EC_OBF(BYTE Port)
{
	DWORD dwPortVal = 0;
	int i;
	for(i=0;i<0xFFFF;i++)
	{
		bool hr = GetPortVal(Port,&dwPortVal, 1);
		if ((dwPortVal & EC_OBF))
		{
			break;
		}
	}
}
//------------------------------------
// InitializeWinIo
//------------------------------------
BOOL InitializeWinIO()
{
	if (InitializeWinIo())
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------
// ShutdownWinIo
//------------------------------------
void ShutdownWinIO()
{
	ShutdownWinIo();
}
//------------------------------------
// send EC command
//------------------------------------
void send_EC_cmd(BYTE CMD_port, BYTE Data_port, BYTE Command)
{
	wait_EC_IBE(CMD_port);
	SetPortVal(CMD_port, Command,1);
}
//------------------------------------
// send EC data
//------------------------------------
void send_EC_data(BYTE CMD_port, BYTE Data_port, BYTE Data)
{
	wait_EC_IBE(CMD_port);
	SetPortVal(Data_port, Data,1);
}

//------------------------------------
// write EC command/data
//------------------------------------
void write_Byte_cmd(BYTE CMD_port, BYTE Data_port, BYTE Command, BYTE Data)
{
	wait_EC_IBE(CMD_port);
	SetPortVal(CMD_port, Command,1);
	wait_EC_IBE(CMD_port);
	SetPortVal(Data_port, Data,1);
}
//------------------------------------
// read a byte from EC command 
//------------------------------------
BYTE Read_Byte_cmd(BYTE CMD_port, BYTE Data_port, BYTE EcCmd, BYTE EcData)
{
	BYTE bData;
	DWORD BuffData = 0;
	wait_EC_IBE(CMD_port);
	wait_EC_OBE(CMD_port);
	SetPortVal(CMD_port, EcCmd,1);
	wait_EC_IBE(CMD_port);
	SetPortVal(Data_port, EcData,1);
	wait_EC_OBF(CMD_port);
	GetPortVal(Data_port,&BuffData, 1);
	bData = (BYTE)BuffData;
	return bData;
}

//------------------------------------
// read a byte from EC namespace
//------------------------------------
BYTE read_EC_data(BYTE CMD_port, BYTE Data_port, BYTE ECAddr)
{
	BYTE bData;
	DWORD BuffData = 0;
	wait_EC_IBE(CMD_port);
	wait_EC_OBE(CMD_port);
	SetPortVal(CMD_port, 0x80,1);
	wait_EC_IBE(CMD_port);
	SetPortVal(Data_port, ECAddr, 1);
	wait_EC_OBF(CMD_port);
	GetPortVal(Data_port,&BuffData, 1);
	bData = (BYTE)BuffData;
	return bData;
}

//------------------------------------
// write a byte to EC namespace
//------------------------------------
void write_EC_data(BYTE CMD_port, BYTE Data_port, BYTE ECAddr,BYTE ECData)
{
	wait_EC_IBE(CMD_port);
	wait_EC_OBE(CMD_port);
	SetPortVal(CMD_port, 0x81, 1);
	wait_EC_IBE(CMD_port);
	SetPortVal(Data_port, ECAddr, 1);
	wait_EC_IBE(CMD_port);
	SetPortVal(Data_port, ECData, 1);
}

//------------------------------------
// read a DWORD from Physical address
//------------------------------------
DWORD GetPhysValue(PBYTE pbPhysAddr)
{
	DWORD val = 0;
	bool re = GetPhysLong(pbPhysAddr, &val);
	return val;
}


//------------------------------------
// write a DWORD into Physical address
//------------------------------------
BOOL SetPhysValue(PBYTE pbPhysAddr, DWORD dwPhysValue)
{
	bool re = SetPhysLong(pbPhysAddr, dwPhysValue);
	return re;
}


