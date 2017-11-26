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

void Device::getDriverInfo(GUID guid , string *hardwareID, string *manufacturer, string *provider, string *driverDescription)
{
	HDEVINFO hDevInfo = 0;
	SP_DEVINFO_DATA  spDevInfoData = { 0 };
	SP_DRVINFO_DATA_A spDrvInfoData = { 0 };
	SP_DRVINFO_DETAIL_DATA_A spDrvInfoDetail = { 0 };
	int index = 0;

	hDevInfo = SetupDiGetClassDevs(&guid, 0, NULL, DIGCF_PRESENT);

	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	if (SetupDiEnumDeviceInfo(hDevInfo,
		0,
		&spDevInfoData))
	{

		if (SetupDiBuildDriverInfoList(hDevInfo,
			&spDevInfoData,
			SPDIT_COMPATDRIVER))
		{
			spDrvInfoData.cbSize = sizeof(SP_DRVINFO_DATA_A);
			while (1)
			{
				if (SetupDiEnumDriverInfoA(hDevInfo,
					&spDevInfoData,
					SPDIT_COMPATDRIVER,
					index,
					&spDrvInfoData))
				{
					*manufacturer = string(spDrvInfoData.MfgName);
					*provider = string(spDrvInfoData.ProviderName);

					char szBuf[BUFFERL_SIZE] = { 0 };
					memcpy(&spDrvInfoDetail, szBuf, sizeof(SP_DRVINFO_DETAIL_DATA_A));
					spDrvInfoDetail.cbSize = sizeof(SP_DRVINFO_DETAIL_DATA_A);
					DWORD dwRequireSize = 0;
					if (SetupDiGetDriverInfoDetailA(hDevInfo,
						&spDevInfoData,
						&spDrvInfoData,
						&spDrvInfoDetail,
						BUFFERL_SIZE,
						&dwRequireSize))
					{
						*hardwareID = string(spDrvInfoDetail.HardwareID);
						*driverDescription = string(spDrvInfoDetail.DrvDescription);
						index++;
					}
				}
				else
					break;
				
			}
			
		}

		
	}
	
}


string Device::getDevicePath(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData)
{
	SP_DEVICE_INTERFACE_DATA spDevInterfaceData = { 0 };
	DWORD required = 0;
	string result;

	spDevInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	if (SetupDiCreateDeviceInterface(hDevInfo,
		&spDevInfoData,
		&spDevInfoData.ClassGuid,
		0,
		0,
		&spDevInterfaceData))

	{
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &spDevInterfaceData, 0, 0, &required, 0);

		SP_DEVICE_INTERFACE_DETAIL_DATA_A *spDevInterfaceDetail;
		spDevInterfaceDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA_A*)LocalAlloc(LPTR,
			sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A)*required);

		spDevInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
		if (SetupDiGetDeviceInterfaceDetailA(hDevInfo,
			&spDevInterfaceData,
			spDevInterfaceDetail,
			required,
			&required,
			0))
		{
			result = string(spDevInterfaceDetail->DevicePath);
			if (spDevInterfaceDetail)
				LocalFree(spDevInterfaceDetail);
			
		}
	}
	return result;
}