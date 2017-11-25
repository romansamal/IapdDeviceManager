#include "MainWindow.h"
#include "DeviceEnumerator.h"
using namespace System;
using namespace System::Windows::Forms;


[STAThread]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew IapdDeviceManager::MainWindow());
	vector<DEV_INFO> k = DeviceEnumerator::getDevices();
	return 0;
}

