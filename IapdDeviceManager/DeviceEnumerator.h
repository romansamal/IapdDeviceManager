#pragma once
#include <vector>
#include <string>
#include <set>
#include <Windows.h>
#include <setupapi.h>
#include "Device.h"
using namespace std;

#pragma comment(lib, "setupapi.lib")

typedef struct DEVICE_INFO
{
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA spDevInfoData;
	string classDescription;
	string deviceName;
	string guid_string;
	GUID guid;
	string hardwareID;
	string manufacturer;
	string provider;
	string driverDescription;
	string devicePath;
	string driverFullName;
	bool isEnabled;
}DEV_INFO;

class DeviceEnumerator
{
private:
	static vector<DEVICE_INFO> vectorDeviceInfo;
public:
	static vector<DEVICE_INFO> getDevices();
	static set<string> getDeviceTypes();
	DeviceEnumerator();
	~DeviceEnumerator();
};

