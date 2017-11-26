#pragma once
#define BUFFERL_SIZE 2048
#include <string>
#include <Windows.h>
#include <setupapi.h>
using namespace std;

#pragma comment(lib, "setupapi.lib")

class Device
{
public:
	Device();
	static string getDeviceClassDescription(SP_DEVINFO_DATA spDevInfoData);
	static string getDeviceName(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData);
	static string getGUID(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData);
	static void getDriverInfo(GUID guid, string *hardwareID, string *manufacturer, string *provider, string *driverDescription);
	static string getDevicePath(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData);
	~Device();
};

