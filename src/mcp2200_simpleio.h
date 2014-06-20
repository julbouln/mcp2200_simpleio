#ifndef MCP2200_SIMPLEIOCLASS_HPP
#define MCP2200_SIMPLEIOCLASS_HPP

#include <string>
#include <iostream>

#include <hidapi/hidapi.h>

using namespace std;

namespace SimpleIO {

const unsigned int OFF = 0;
const unsigned int ON = 1;
const unsigned int TOGGLE = 3;
const unsigned int BLINKSLOW = 4;
const unsigned int BLINKFAST = 5;

class SimpleIOClass {
private:
	static unsigned char buf[16];
	static hid_device *handle;
	static int needRead;
	
	static void SetCommand(int cmd);
	static bool WriteBuf(void);
	static void ReadAll(void);

	static void SetRxLedModeBuf(unsigned int mode);
	static void SetTxLedModeBuf(unsigned int mode);
	static void SetBaudRateBuf(unsigned long BaudRateParam);
	static void SetHardwareFlowControlBuf(unsigned int onOff);
	static void SetIoMapBuf(unsigned char ioMap);
	static void SetSuspendBuf(unsigned int onOff);
	static void SetULoadBuf(unsigned int onOff);

	static bool SetString(int strType, char *str);

public:
	static bool ClearPin(unsigned int pin);
	static bool ConfigureIO (unsigned char IOMap);
	static bool ConfigureIoDefaultOutput(unsigned char ucIoMap, unsigned char ucDefValue);
	static bool ConfigureMCP2200 (unsigned char IOMap, unsigned long BaudRateParam, unsigned int RxLEDMode, unsigned int TxLEDMode, bool FLOW, bool ULOAD,bool SSPND);
	static bool fnHardwareFlowControl (unsigned int onOff);
	static bool fnRxLED (unsigned int mode);
	static bool fnSetBaudRate (unsigned long BaudRateParam);
	static bool fnSuspend(unsigned int onOff);
	static bool fnTxLED (unsigned int mode);
	static bool fnULoad(unsigned int onOff);
	static string GetDeviceInfo(unsigned int uiDeviceNo);
	static unsigned int GetNoOfDevices(void);
	static int GetSelectedDevice(void);
	static string GetSelectedDeviceInfo(void);
	static void InitMCP2200 (unsigned int VendorID, unsigned int ProductID);
	static bool IsConnected();
	static int ReadEEPROM(unsigned int uiEEPAddress);
	static bool ReadPin(unsigned int pin, unsigned int *returnvalue); 
	static int ReadPinValue(unsigned int pin);
	static bool ReadPort(unsigned int *returnvalue);
	static int ReadPortValue();
	static int SelectDevice(unsigned int uiDeviceNo);
	static bool SetPin(unsigned int pin);
	static int WriteEEPROM(unsigned int uiEEPAddress, unsigned char ucValue);
	static bool WritePort(unsigned int portValue);
	
	// extended API
	
	static bool SetManufacturerString(char *manufacturer);
	static bool SetProductString(char *product);

	static char *GetManufacturerString(void);
	static char *GetProductString(void);

	static int GetBaudRate(void);
	static int GetRxLED(void);
	static int GetTxLED(void);
	static int GetHardwareFlowControl(void);
	static int GetULoad(void);
	static int GetSuspend(void);

	static int GetIO(void);
	static int GetDefaultOutput(void);

	static bool SetVendorIDProductID(int vendorID, int productID);
	
	
};
}
#endif

