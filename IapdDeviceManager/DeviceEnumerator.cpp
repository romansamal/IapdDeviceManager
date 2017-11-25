#include "DeviceEnumerator.h"


DeviceEnumerator::DeviceEnumerator()
{
}


DeviceEnumerator::~DeviceEnumerator()
{
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
			deviceInfo.classDescription = Device::getDeviceClassDescription(spDevInfoData);
			deviceInfo.deviceName = Device::getDeviceName(hDevInfo, spDevInfoData);

			vectorBuff.push_back(deviceInfo);
			index++;
		}
		else
			break;
	}
	return vectorBuff; 
}

set<string> DeviceEnumerator::getDeviceTypes()
{
	set<string> temp;
	vector<DEVICE_INFO> vectorBuff = getDevices();
	for (int i = 0; i < vectorBuff.size(); i++)
	{
		temp.insert(vectorBuff.at(i).classDescription);
	}
	return temp;
}