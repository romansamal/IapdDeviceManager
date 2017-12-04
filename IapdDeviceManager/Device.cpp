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
						continue;
					}
					
				}
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

string Device::getDriverFullName(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData)
{
	string result;
	char serviceName[64];
	//Get service name of device
	if (SetupDiGetDeviceRegistryPropertyA(hDevInfo,
		&spDevInfoData,
		SPDRP_SERVICE,
		0,
		(PBYTE)serviceName,
		63,
		0))
	{
		HKEY  hKey = 0;
		char szSubKey[128] = { REG_PATH };
		char szPath[MAX_PATH] = { 0 };
		DWORD cbData;
		DWORD dwType;
		strcat(szSubKey, serviceName);
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			szSubKey,
			0,
			KEY_ALL_ACCESS,
			&hKey) == ERROR_SUCCESS)
		{
			cbData = MAX_PATH - 1;
			dwType = REG_EXPAND_SZ;
			if (RegQueryValueExA(hKey, REG_IMAGE, 0L,
				&dwType,
				(unsigned char*)szPath,
				&cbData) == ERROR_SUCCESS)
			{
				char szRoot[MAX_PATH] = { 0 };
				GetWindowsDirectoryA(szRoot, MAX_PATH - 1);
				strcat(szRoot, "\\");
				strcat(szRoot, szPath);
				result = string(szRoot);
			}
		}
	}	
	return result;
}

bool Device::deviceChangeStatus(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData, bool status)
{
	SP_PROPCHANGE_PARAMS spPropChangeParams;

	spPropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	spPropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	spPropChangeParams.Scope = DICS_FLAG_GLOBAL;
	if (status)
		spPropChangeParams.StateChange = DISC_ENABLE; 
	else
		spPropChangeParams.StateChange = DISC_DISABLE;
	DWORD errorCode;
	//   
	if (SetupDiSetClassInstallParams(hDevInfo, &spDevInfoData, &spPropChangeParams.ClassInstallHeader, sizeof(spPropChangeParams)))
	{
		if (SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo, &spDevInfoData))
		{
			return true;
		}
	}
	return false;
}


bool Device::isEnabled(SP_DEVINFO_DATA spDevInfoData)
{
	ULONG status = 0, problem = 0;
	CONFIGRET cr = CM_Get_DevNode_Status(&status, &problem, spDevInfoData.DevInst, 0); 
	return problem != PROBLEM_CODE;
}