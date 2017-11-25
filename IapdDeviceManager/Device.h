#pragma once
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
	~Device();
};

