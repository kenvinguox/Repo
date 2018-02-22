/***********************************************************************************************/
//ec lib release note
//�汾��   V1.0.0.1 by xiangxiang 2015.06.08  first release
/***********************************************************************************************/

#define EC_OBF                  0x01
#define EC_IBF                  0x02

#define RD_EC                   0x80        // read EC Ram  
#define WR_EC                   0x81        // write to EC
#define BE_EC                   0x82        // enable burst mode
#define BD_EC                   0x83        // disable burst mode
#define QR_EC                   0x84        // query EC event


BOOL InitializeWinIO();
void ShutdownWinIO();
void wait_EC_OBF(BYTE Port);
void wait_EC_OBE(BYTE Port);
void wait_EC_IBE(BYTE Port);


/*==============================================================
	����:	send_EC_cmd
	����:	write command into command Port
	����:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Command:       command
	˵��:   
--------------------------------------------------------------*/
void send_EC_cmd(BYTE CMD_port, BYTE Data_port, BYTE Command);


/*==============================================================

	����:	send_EC_data
	����:	write data into data Port
	����:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Data:          Data
	˵��:   
--------------------------------------------------------------*/
void send_EC_data(BYTE CMD_port, BYTE Data_port, BYTE Data);


/*==============================================================

	����:	write_Byte_cmd
	����:	write ec command & data
	����:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Command:       Command
	        Data:          Data
	˵��:   
--------------------------------------------------------------*/
void write_Byte_cmd(BYTE CMD_port, BYTE Data_port, BYTE Command, BYTE Data);


/*==============================================================

	����:	Read_Byte_cmd
	����:	write ec command & data, read a byte from data Port
	����:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Command:       Command
	        Data:          Data
	˵��:   
--------------------------------------------------------------*/
BYTE Read_Byte_cmd(BYTE CMD_port, BYTE Data_port, BYTE EcCmd, BYTE EcData);


/*==============================================================

	����:	read_EC_data
	����:	read a byte from ec namespace
	����:   CMD_port:       Command Port
	        Data_port:      Data Port
	        ECAddr:         namespace addr
	˵��:   
--------------------------------------------------------------*/
BYTE read_EC_data(BYTE CMD_port, BYTE Data_port, BYTE ECAddr);


/*==============================================================

����:	write_EC_data
����:	write a byte to EC namespace
����:   CMD_port:       Command Port
        Data_port:      Data Port
        ECAddr:         namespace addr
		ECAddr:         data
˵��:
--------------------------------------------------------------*/
void write_EC_data(BYTE CMD_port, BYTE Data_port, BYTE ECAddr, BYTE ECData);

/*==============================================================

����:	GetPhysValue
����:	read a DWORD from Physical address
����:    pbPhysAddr:       Physical address Pointer
˵��:    return a DWORD Value
--------------------------------------------------------------*/
DWORD GetPhysValue(PBYTE pbPhysAddr);

/*==============================================================

����:	SetPhysValue
����:	write a DWORD into Physical address
����:   pbPhysAddr:       Physical address Pointer
        dwPhysValue:      a DWORD value
˵��:   
--------------------------------------------------------------*/
BOOL SetPhysValue(PBYTE pbPhysAddr, DWORD dwPhysValue);