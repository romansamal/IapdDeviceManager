#include "DeviceEnumerator.h"

vector<DEVICE_INFO> DeviceEnumerator::vectorDeviceInfo;
DeviceEnumerator::DeviceEnumerator()
{
}


DeviceEnumerator::~DeviceEnumerator()
{
}


vector<DEVICE_INFO> DeviceEnumerator::getDevices()
{
	vectorDeviceInfo.clear();
	DEV_INFO deviceInfo;
	HDEVINFO hDevInfo = 0;
	SP_DEVINFO_DATA spDevInfoData = { 0 };
	SP_DRVINFO_DATA spDrvInfoData = { 0 };
	SP_DRVINFO_DETAIL_DATA spDrvInfoDetail = { 0 };
	int index = 0;
	hDevInfo = SetupDiGetClassDevs(0, 0, NULL, DIGCF_PRESENT |
		DIGCF_ALLCLASSES | DIGCF_PROFILE);


	deviceInfo.hDevInfo = hDevInfo;
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	spDrvInfoData.cbSize = sizeof(SP_DRVINFO_DATA);
	while (1)
	{
		
		if (SetupDiEnumDeviceInfo(hDevInfo,
			index,
			&spDevInfoData))
		{
			deviceInfo.spDevInfoData = spDevInfoData;
			deviceInfo.classDescription = Device::getDeviceClassDescription(spDevInfoData);
			deviceInfo.deviceName = Device::getDeviceName(hDevInfo, spDevInfoData);
			deviceInfo.guid_string = Device::getGUID(hDevInfo, spDevInfoData);
			deviceInfo.guid = spDevInfoData.ClassGuid;
			//Device::getDriverInfo(deviceInfo.guid, &deviceInfo.hardwareID, &deviceInfo.manufacturer, &deviceInfo.provider, &deviceInfo.driverDescription);
			deviceInfo.devicePath = Device::getDevicePath(hDevInfo, spDevInfoData);
			deviceInfo.driverFullName = Device::getDriverFullName(hDevInfo, spDevInfoData);
			deviceInfo.isEnabled = Device::isEnabled(spDevInfoData);
			vectorDeviceInfo.push_back(deviceInfo);
			index++;
		}
		else
			break;
	}
	return vector<DEV_INFO>(vectorDeviceInfo); 
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