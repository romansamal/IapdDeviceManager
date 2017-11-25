#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <setupapi.h>
using namespace std;

#pragma comment(lib, "setupapi.lib")

typedef struct DEVICE_INFO
{
	SP_DEVINFO_DATA spDevInfoData;
	string classDescription;
}DEV_INFO;

class DeviceEnumerator
{
private:
	static string getDeviceClassDescription(SP_DEVINFO_DATA spDevInfoData);
public:
	static vector<DEVICE_INFO> getDevices();
	DeviceEnumerator();
	~DeviceEnumerator();
};

