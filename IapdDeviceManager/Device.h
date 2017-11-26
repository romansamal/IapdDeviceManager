#pragma once
#define BUFFERL_SIZE 2048
#define REG_PATH "SYSTEM\\CurrentControlSet\\Services\\\0"
#define REG_IMAGE "ImagePath"
#define DISC_DISABLE 2
#define DISC_ENABLE 1
#define PROBLEM_CODE 22
#include <string>
#include <Windows.h>
#include <SetupApi.h>
#include <Cfgmgr32.h>
using namespace std;

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "Advapi32.lib") 

class Device
{
public:
	Device();
	static string getDeviceClassDescription(SP_DEVINFO_DATA spDevInfoData);
	static string getDeviceName(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData);
	static string getGUID(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData);
	static void getDriverInfo(GUID guid, string *hardwareID, string *manufacturer, string *provider, string *driverDescription);
	static string getDevicePath(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData);
	static string getDriverFullName(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData);
	static bool deviceChangeStatus(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData, bool status);
	static bool isEnabled(SP_DEVINFO_DATA spDevInfoData);
	~Device();
};

