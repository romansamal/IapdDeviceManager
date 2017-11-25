#include "DeviceEnumerator.h"


DeviceEnumerator::DeviceEnumerator()
{
}


DeviceEnumerator::~DeviceEnumerator()
{
}


string DeviceEnumerator::getDeviceClassDescription(SP_DEVINFO_DATA spDevInfoData)
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

vector<DEVICE_INFO> DeviceEnumerator::getDevices()
{
	vector<DEVICE_INFO> vectorBuff;
	HDEVINFO hDevInfo = 0;
	SP_DEVINFO_DATA spDevInfoData = { 0 };
	int index = 0;

	hDevInfo = SetupDiGetClassDevs(0, 0, NULL, DIGCF_PRESENT |
		DIGCF_ALLCLASSES | DIGCF_PROFILE);

	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	while (1)
	{
		DEV_INFO deviceInfo;
		if (SetupDiEnumDeviceInfo(hDevInfo,
			index,
			&spDevInfoData))
		{
			deviceInfo.spDevInfoData = spDevInfoData;
			deviceInfo.classDescription = getDeviceClassDescription(spDevInfoData);
			vectorBuff.push_back(deviceInfo);
			index++;
		}
		else
			break;
	}
	return vectorBuff; 
}