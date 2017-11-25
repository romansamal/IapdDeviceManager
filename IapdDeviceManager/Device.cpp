#include "Device.h"


Device::Device()
{
}


Device::~Device()
{
}


string Device::getDeviceClassDescription(SP_DEVINFO_DATA spDevInfoData)
{
	char  buff[MAX_PATH] = { 0 };
	DWORD requireSize;

	if (!SetupDiGetClassDescriptionA(&spDevInfoData.ClassGuid,
		buff,
		MAX_PATH,
		&requireSize))
		return string("\0");
	else
		return string(buff);
}

string Device::getDeviceName(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData)
{
	char  buff[MAX_PATH] = { 0 };
	
	if (!SetupDiGetDeviceRegistryPropertyA(hDevInfo,
		&spDevInfoData,
		SPDRP_FRIENDLYNAME,
		0,
		(PBYTE)buff,
		MAX_PATH,
		0))
	{
		SetupDiGetDeviceRegistryPropertyA(hDevInfo,
			&spDevInfoData,
			SPDRP_DEVICEDESC,
			0,
			(PBYTE)buff,
			MAX_PATH,
			0);
	}
	return string(buff);
}

string Device::getGUID(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData)
{
	char  buff[MAX_PATH] = { 0 };

	SetupDiGetDeviceRegistryPropertyA(hDevInfo,
		&spDevInfoData,
		SPDRP_DRIVER,
		0,
		(PBYTE)buff,
		MAX_PATH,
		0);
	int s = 0;
	return string(buff);
}