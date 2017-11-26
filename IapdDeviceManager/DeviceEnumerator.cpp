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
	SP_DRVINFO_DATA spDrvInfoData = { 0 };
	SP_DRVINFO_DETAIL_DATA spDrvInfoDetail = { 0 };
	int index = 0;

	hDevInfo = SetupDiGetClassDevs(0, 0, NULL, DIGCF_PRESENT |
		DIGCF_ALLCLASSES | DIGCF_PROFILE);

	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	spDrvInfoData.cbSize = sizeof(SP_DRVINFO_DATA);
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
			deviceInfo.guid_string = Device::getGUID(hDevInfo, spDevInfoData);
			deviceInfo.guid = spDevInfoData.ClassGuid;
			Device::getDriverInfo(deviceInfo.guid, &deviceInfo.hardwareID, &deviceInfo.manufacturer, &deviceInfo.provider, &deviceInfo.driverDescription);
			deviceInfo.devicePath = Device::getDevicePath(hDevInfo, spDevInfoData);

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