/***********************************************************************************************/
//ec lib release note
//版本：   V1.0.0.1 by xiangxiang 2015.06.08  first release
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
	函数:	send_EC_cmd
	功能:	write command into command Port
	参数:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Command:       command
	说明:   
--------------------------------------------------------------*/
void send_EC_cmd(BYTE CMD_port, BYTE Data_port, BYTE Command);


/*==============================================================

	函数:	send_EC_data
	功能:	write data into data Port
	参数:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Data:          Data
	说明:   
--------------------------------------------------------------*/
void send_EC_data(BYTE CMD_port, BYTE Data_port, BYTE Data);


/*==============================================================

	函数:	write_Byte_cmd
	功能:	write ec command & data
	参数:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Command:       Command
	        Data:          Data
	说明:   
--------------------------------------------------------------*/
void write_Byte_cmd(BYTE CMD_port, BYTE Data_port, BYTE Command, BYTE Data);


/*==============================================================

	函数:	Read_Byte_cmd
	功能:	write ec command & data, read a byte from data Port
	参数:   CMD_port:      Command Port
	        Data_port:     Data Port
	        Command:       Command
	        Data:          Data
	说明:   
--------------------------------------------------------------*/
BYTE Read_Byte_cmd(BYTE CMD_port, BYTE Data_port, BYTE EcCmd, BYTE EcData);


/*==============================================================

	函数:	read_EC_data
	功能:	read a byte from ec namespace
	参数:   CMD_port:       Command Port
	        Data_port:      Data Port
	        ECAddr:         namespace addr
	说明:   
--------------------------------------------------------------*/
BYTE read_EC_data(BYTE CMD_port, BYTE Data_port, BYTE ECAddr);


/*==============================================================

函数:	write_EC_data
功能:	write a byte to EC namespace
参数:   CMD_port:       Command Port
        Data_port:      Data Port
        ECAddr:         namespace addr
		ECAddr:         data
说明:
--------------------------------------------------------------*/
void write_EC_data(BYTE CMD_port, BYTE Data_port, BYTE ECAddr, BYTE ECData);

/*==============================================================

函数:	GetPhysValue
功能:	read a DWORD from Physical address
参数:    pbPhysAddr:       Physical address Pointer
说明:    return a DWORD Value
--------------------------------------------------------------*/
DWORD GetPhysValue(PBYTE pbPhysAddr);

/*==============================================================

函数:	SetPhysValue
功能:	write a DWORD into Physical address
参数:   pbPhysAddr:       Physical address Pointer
        dwPhysValue:      a DWORD value
说明:   
--------------------------------------------------------------*/
BOOL SetPhysValue(PBYTE pbPhysAddr, DWORD dwPhysValue);