#include "mcp2200_simpleio.h"
#include <unistd.h>

namespace SimpleIO {
#define MCP2200_HID_REPORT_SIZE 16

#define MCP2200_HID_COMMAND_SET_CLEAR_OUTPUT 0x08
#define MCP2200_HID_COMMAND_CONFIGURE 0x10
#define MCP2200_HID_COMMAND_READ_EE 0x20
#define MCP2200_HID_COMMAND_WRITE_EE 0x40
#define MCP2200_HID_COMMAND_READ_ALL 0x80

#define MCP2200_SECRET_CONFIGURE    0x01
#define MCP2200_CFG_PID_VID     0x00 
#define MCP2200_CFG_MANU        0x01
#define MCP2200_CFG_PROD        0x02

	hid_device *SimpleIOClass::handle=NULL;
	unsigned char SimpleIOClass::buf[MCP2200_HID_REPORT_SIZE];
	int SimpleIOClass::needRead=1;

	void SimpleIOClass::SetCommand(int cmd)
	{
		buf[0]=cmd;
	}

	bool SimpleIOClass::WriteBuf(void) {
		needRead=1;
		if(hid_write(handle, buf, MCP2200_HID_REPORT_SIZE) < 0)
			return false;
		else
			return true;
	}


	void SimpleIOClass::ReadAll()
	{
		int res;
	
		if(needRead) {
			SetCommand(MCP2200_HID_COMMAND_READ_ALL);
    
			hid_write(handle, buf, MCP2200_HID_REPORT_SIZE);
			res=hid_read(handle,buf,MCP2200_HID_REPORT_SIZE);
	
			needRead=0;
		}
	
	}

	void SimpleIOClass::SetRxLedModeBuf(unsigned int mode) {
		switch(mode)
		{
			case OFF:
			buf[5]=buf[5] & ~0b1000;
			buf[7]=buf[7] & ~0b10100000;
			break;
			case ON:
			buf[5]=buf[5] | 0b1000;
			buf[7]=buf[7] & ~0b10100000;
			break;			
			case TOGGLE:
			buf[5]=buf[5] | 0b1000;
			buf[7]=buf[7] & ~0b10100000;
			buf[7]=buf[7] | 0b10100000;
			break;			
			case BLINKSLOW:
			buf[5]=buf[5] | 0b1000;
			buf[7]=buf[7] & ~0b10100000;
			buf[7]=buf[7] | 0b00100000;
			break;			
			case BLINKFAST:
			buf[5]=buf[5] | 0b1000;
			buf[7]=buf[7] & ~0b10100000;
			buf[7]=buf[7] | 0b00000000;
			break;
		}
	}
	void SimpleIOClass::SetTxLedModeBuf(unsigned int mode) {
		switch(mode)
		{
			case OFF:
			buf[5]=buf[5] & ~0b100;
			buf[7]=buf[7] & ~0b01100000;
			break;
			case ON:
			buf[5]=buf[5] | 0b100;
			buf[7]=buf[7] & ~0b01100000;
			break;
			case TOGGLE:
			buf[5]=buf[5] | 0b100;
			buf[7]=buf[7] & ~0b01100000;
			buf[7]=buf[7] | 0b01100000;
			break;
			case BLINKSLOW:
			buf[5]=buf[5] | 0b100;
			buf[7]=buf[7] & ~0b01100000;
			buf[7]=buf[7] | 0b00100000;
			break;			
			case BLINKFAST:
			buf[5]=buf[5] | 0b100;
			buf[7]=buf[7] & ~0b01100000;
			buf[7]=buf[7] | 0b00000000;
			break;
		}
	}

	void SimpleIOClass::SetBaudRateBuf(unsigned long BaudRateParam) {
		unsigned long BaudRateDivisor=12000000/BaudRateParam - 1;
		 
		buf[8]=BaudRateDivisor >> 8;
		buf[9]=BaudRateDivisor & 0xFF;
	}
	
	void SimpleIOClass::SetIoMapBuf(unsigned char ioMap)	
	{
		buf[4]=ioMap;
	}

	void SimpleIOClass::SetHardwareFlowControlBuf(unsigned int onOff)
	{
		if(onOff) {
			buf[7]=buf[7] | 1;
		} 
		else
		{
			buf[7]=buf[7] & ~1;		
		}
	}

	void SimpleIOClass::SetSuspendBuf(unsigned int onOff)
	{
		if(onOff) {
			buf[5]=buf[5] | 0b10000000;
		} 
		else
		{
			buf[5]=buf[5] & ~0b10000000;		
		}
	}

	void SimpleIOClass::SetULoadBuf(unsigned int onOff)
	{
		if(onOff) {
			buf[5]=buf[5] | 0b01000000;
		} 
		else
		{
			buf[5]=buf[5] & ~0b01000000;		
		}
	}

	bool SimpleIOClass::ClearPin(unsigned int pin) 
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_SET_CLEAR_OUTPUT);
		buf[11] = buf[10];
		buf[12] = ~buf[11] | (1 << pin);
	
		return WriteBuf();	
	}

	bool SimpleIOClass::ConfigureIO (unsigned char IOMap) 
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetIoMapBuf(IOMap);

		return WriteBuf();	
	}

	bool SimpleIOClass::ConfigureIoDefaultOutput(unsigned char ucIoMap, unsigned char ucDefValue)
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetIoMapBuf(ucIoMap);
		buf[6]=ucDefValue;

		return WriteBuf();
	}

	bool SimpleIOClass::ConfigureMCP2200 (unsigned char IOMap, unsigned long BaudRateParam, unsigned int RxLEDMode, unsigned int TxLEDMode, bool FLOW, bool ULOAD,bool SSPND) 
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetIoMapBuf(IOMap);
		SetBaudRateBuf(BaudRateParam);
		SetRxLedModeBuf(RxLEDMode);
		SetTxLedModeBuf(TxLEDMode);
		SetHardwareFlowControlBuf(FLOW);
		SetULoadBuf(ULOAD);
		SetSuspendBuf(SSPND);
	
		return WriteBuf();	
	}

	bool SimpleIOClass::fnHardwareFlowControl (unsigned int onOff) 
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);	
		SetHardwareFlowControlBuf(onOff);

		return WriteBuf();
	}

	bool SimpleIOClass::fnRxLED (unsigned int mode) 
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetRxLedModeBuf(mode);

		return WriteBuf();		
	}

	bool SimpleIOClass::fnSetBaudRate (unsigned long BaudRateParam) 
	{
		ReadAll();
		 
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetBaudRateBuf(BaudRateParam);

		return WriteBuf();	
	}

	bool SimpleIOClass::fnSuspend(unsigned int onOff) 
	{
		ReadAll();	
		 
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetSuspendBuf(onOff);

		return WriteBuf();
	}

	bool SimpleIOClass::fnTxLED (unsigned int mode) 
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetTxLedModeBuf(mode);

		return WriteBuf();
	}

	bool SimpleIOClass::fnULoad(unsigned int onOff) 
	{
		ReadAll();	
		 
		SetCommand(MCP2200_HID_COMMAND_CONFIGURE);
		SetULoadBuf(onOff);

		return WriteBuf();
	}

	string SimpleIOClass::GetDeviceInfo(unsigned int uiDeviceNo) 
	{	
	}

	unsigned int SimpleIOClass::GetNoOfDevices(void) 
	{
		return 0;	
	}

	int SimpleIOClass::GetSelectedDevice(void) 
	{
		return 0;	
	}

	string SimpleIOClass::GetSelectedDeviceInfo(void) 
	{	
	}

	void SimpleIOClass::InitMCP2200 (unsigned int VendorID, unsigned int ProductID) 
	{

		handle = hid_open(VendorID, ProductID, NULL);	
		//	if(handle) {
		//		hid_set_nonblocking(handle, 1);
		//	}
	}

	bool SimpleIOClass::IsConnected() 
	{
		if(handle)
			return true;
		else
			return false;
	}

	int SimpleIOClass::ReadEEPROM(unsigned int uiEEPAddress) 
	{
		int res;
	
		SetCommand(MCP2200_HID_COMMAND_READ_EE);
		buf[1]=uiEEPAddress;
  
		hid_write(handle, buf, MCP2200_HID_REPORT_SIZE);
		res=hid_read(handle,buf,MCP2200_HID_REPORT_SIZE);
  
		return buf[3];
	}

	bool SimpleIOClass::ReadPin(unsigned int pin, unsigned int *returnvalue) {
		unsigned int v=ReadPinValue(pin);
		*returnvalue=v;
		return true;
	} 

	int SimpleIOClass::ReadPinValue(unsigned int pin) 
	{
		if (ReadPortValue() & (1 << pin))
			return 1;
		else
			return 0;
	}

	bool SimpleIOClass::ReadPort(unsigned int *returnvalue) 
	{
		unsigned int v=ReadPortValue();
		*returnvalue=v;
		return true;
	}

	int SimpleIOClass::ReadPortValue() 
	{
		ReadAll();
		return buf[10];
	}

	int SimpleIOClass::SelectDevice(unsigned int uiDeviceNo) 
	{
		return 0;
	}

	bool SimpleIOClass::SetPin(unsigned int pin) 
	{
		ReadAll();
	
		SetCommand(MCP2200_HID_COMMAND_SET_CLEAR_OUTPUT);

		buf[11] = buf[10] | (1 << pin);
		buf[12] = ~buf[11];
	
		return WriteBuf();
	}

	int SimpleIOClass::WriteEEPROM(unsigned int uiEEPAddress, unsigned char ucValue) 
	{
		SetCommand(MCP2200_HID_COMMAND_WRITE_EE);
		buf[1]=uiEEPAddress;
		buf[2]=ucValue;
  
		return WriteBuf();
	}

	bool SimpleIOClass::WritePort(unsigned int portValue) 
	{
		SetCommand(MCP2200_HID_COMMAND_SET_CLEAR_OUTPUT);
		buf[11] = portValue;
		buf[12] = ~buf[11];
	
		return WriteBuf();
	}


	// extended


	// adapted from http://stackoverflow.com/a/23106926 reverse engineering
	bool SimpleIOClass::SetString(int strType, char *str) {
		unsigned char **bufA;
	
		char strA[64];
		unsigned int i, k = 0;
		unsigned char tmp = 0;

		memset (strA, 0x00, sizeof(strA));

		// allocate bufA buffer, 2-dim array with 16 x 16 chars
		if (( bufA = ( unsigned char** )malloc( 16*sizeof( unsigned char* ))) == NULL ) {
			// error
		}

		for ( i = 0; i < 16; i++ )
		{
			if (( bufA[i] = ( unsigned char* )malloc( 16 )) == NULL ) {
				/* error */ 
			}
			/* init the rows here */
			memset (bufA[i], 0x00, sizeof(&bufA[i]));
		}

		// strA holds (strlen(manuStr) * 2) + 2 in byte[0] and manufacturer string from byte[1] on
		strcpy (&strA[1], str);
		strA[0] = ((strlen (&strA[1]) * 2) + 2);

		// build manufacturer / product strings
		for (i=0, k=0; i<16; i++, k+=4) {
			if (i==0) {
				tmp = 0x03;
			} else {
				tmp = 0x00;
			}
			// string
			bufA[i][0] = MCP2200_SECRET_CONFIGURE; 
			bufA[i][1] = strType;
			bufA[i][2] = i; 
			bufA[i][3] = strA[k];
			bufA[i][4] = tmp;
			bufA[i][5] = strA[k+1];
			bufA[i][6] = 0x00;
			bufA[i][7] = strA[k+2];
			bufA[i][8] = 0x00;
			bufA[i][9] = strA[k+3];
			bufA[i][10] = 0x00;
			bufA[i][11] = 0xff;
			bufA[i][12] = 0xff;
			bufA[i][13] = 0xff;
			bufA[i][14] = 0xff;
			bufA[i][15] = 0xff;

		}
	
		for (i=0; i<16; i++) {
			hid_write (handle, bufA[i], 16);
		}		
	
		return true;
	}

	bool SimpleIOClass::SetManufacturerString(char *manufacturer) {
		return SetString(MCP2200_CFG_MANU,manufacturer);
	}

	bool SimpleIOClass::SetProductString(char *manufacturer) {
		return SetString(MCP2200_CFG_PROD,manufacturer);
	}

	char *SimpleIOClass::GetManufacturerString(void) {
		char *str;
		wchar_t wstr[255];
		hid_get_manufacturer_string(handle, wstr, 255);
		//	wprintf(L"Manufacturer String: %ls\n",wstr);
		str=(char*)malloc(wcslen(wstr) + 1);
		memset (str, 0x00, wcslen(wstr)+1);
		int ret=wcstombs(str, wstr, wcslen(wstr));
		return (char *)str;
	}

	char *SimpleIOClass::GetProductString(void) {
		char *str;
		wchar_t wstr[255];
		hid_get_product_string(handle, wstr, 255);
		//		wprintf(L"Product String: %ls (%d)\n", wstr,wcslen(wstr));
		str=(char*)malloc(wcslen(wstr) + 1);
		memset (str, 0x00, wcslen(wstr)+1);
		int ret=wcstombs(str, wstr, wcslen(wstr));
		return (char *)str;
	}

	bool SimpleIOClass::SetVendorIDProductID(int vendorID, int productID) {
		unsigned char vpbuf[16];
	
		vpbuf[0]=MCP2200_SECRET_CONFIGURE;
		vpbuf[1]=MCP2200_CFG_PID_VID;
		vpbuf[2]=vendorID & 0xFF;
		vpbuf[3]=vendorID >> 8;
		vpbuf[4]=productID & 0xFF;
		vpbuf[5]=productID >> 8;
		vpbuf[6]=0x00;
		vpbuf[7]=0x00;
		vpbuf[8]=0x00;
		vpbuf[9]=0x00;
		vpbuf[10]=0x00;
		vpbuf[11]=0x00;
		vpbuf[12]=0x00;
		vpbuf[13]=0x00;
		vpbuf[14]=0x00;
		vpbuf[15]=0x00;
	
		if(hid_write(handle, vpbuf, MCP2200_HID_REPORT_SIZE) < 0)
			return false;
		else
			return true;	
	}

	int SimpleIOClass::GetBaudRate(void) {
		ReadAll();
		return 12000000/((buf[8] << 8)+buf[9] + 1);	
	}
	
	int SimpleIOClass::GetRxLED(void) {
		ReadAll();
		if(buf[5] & 0b00001000)
		{
			if(buf[7] & 0b10000000 ) {
				return 3;
			}
			else
			{
				if(buf[7] & 0b00100000) {
					return 4;
				}
				else
				{
					return 5;
				}
			}
		}
		else
		{
			return 0;
		}
	}
	
	int SimpleIOClass::GetTxLED(void) {
		ReadAll();
		if(buf[5] & 0b00000100)
		{
			if(buf[7] & 0b01000000 ) {
				return 3;
			}
			else
			{
				if(buf[7] & 0b00100000) {
					return 4;
				}
				else
				{
					return 5;
				}
			}
		}
		else
		{
			return 0;
		}
	}

	int SimpleIOClass::GetHardwareFlowControl(void) {
		ReadAll();
		if(buf[7] & 0b00000001)
			return 1;
		else
			return 0;
	}
	int SimpleIOClass::GetULoad(void) {
		ReadAll();
		if(buf[5] & 0b01000000)
			return 1;
		else
			return 0;
		
	}
	int SimpleIOClass::GetSuspend(void) {
		ReadAll();
		if(buf[5] & 0b10000000)
			return 1;
		else
			return 0;
	}


	int SimpleIOClass::GetIO(void) {
		ReadAll();
		return buf[4];
	}
	
	int SimpleIOClass::GetDefaultOutput(void) {
		ReadAll();
		return buf[6];
	}

}