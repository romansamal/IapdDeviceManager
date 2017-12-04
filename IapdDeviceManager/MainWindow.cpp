#include "MainWindow.h"
using namespace IapdDeviceManager;

MainWindow::MainWindow()
{
	InitializeComponent();
}

MainWindow::~MainWindow()
{
	if (components)
	{
		delete components;
	}
}

void MainWindow::InitializeComponent()
{
	this->components = gcnew System::ComponentModel::Container();
	this->Size = System::Drawing::Size(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->Text = WINDOW_TITLE;
	this->Padding = System::Windows::Forms::Padding(0);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	list = gcnew ListView();
	list->View = View::Details;
	list->Columns->Add(LISTVIEW_COL1, LISTVIEW_COL1_WIDTH, HorizontalAlignment::Center);
	list->Columns->Add(LISTVIEW_COL2, LISTVIEW_COL2_WIDTH, HorizontalAlignment::Center);
	list->Dock = DockStyle::Fill;
	tree = gcnew TreeView();
	tree->Dock = DockStyle::Fill;
	tree->AfterSelect += gcnew TreeViewEventHandler(this, &MainWindow::Tree_Click);
	tree->DoubleClick += gcnew EventHandler(this, &MainWindow::Tree_DoubleClick);
	splitContainer = gcnew SplitContainer();
	splitContainer->Panel1->Controls->Add(tree);
	splitContainer->Panel2->Controls->Add(list);
	splitContainer->Dock = DockStyle::Fill;
	splitContainer->SplitterDistance = WINDOW_SPLITTER_DISTANCE;
	Controls->Add(splitContainer);
	SetTree();
}

void MainWindow::SetTree()
{
	set<string> deviceInfo = DeviceEnumerator::getDeviceTypes();
	for (set<string>::iterator iter = deviceInfo.begin(); iter != deviceInfo.end(); iter++)
	{
		string item = *iter;
		if (item != "")
		{
			String ^classDescription = gcnew String(item.c_str());
			TreeNode ^newNode = gcnew TreeNode(classDescription);
			newNode->Name = classDescription;
			tree->Nodes->Add(newNode);
		}
	}
	SetTreeItems();
}

void MainWindow::SetTreeItems()
{
	vector<DEVICE_INFO> deviceInfo = DeviceEnumerator::getDevices();
	for (int i = 0; i < deviceInfo.size(); i++)
	{
		String ^classDescription = gcnew String(deviceInfo.at(i).classDescription.c_str());
		cli::array<TreeNode ^> ^nodes = tree->Nodes->Find(classDescription, false);
		String ^deviceDescription = gcnew String(deviceInfo.at(i).deviceName.c_str());
		TreeNode ^newNode = gcnew TreeNode(deviceDescription);
		newNode->Name = deviceDescription;
		String ^deviceGuid = gcnew String(deviceInfo.at(i).guid_string.c_str());
		newNode->Tag = deviceGuid;
		if (nodes->Length > 0)
			nodes[0]->Nodes->Add(newNode);

	}
}

void MainWindow::Tree_Click(System::Object^ Sender, TreeViewEventArgs ^e)
{
	vector<DEVICE_INFO> deviceInfo = DeviceEnumerator::getDevices();
	TreeNode ^ node = tree->SelectedNode;
	list->Items->Clear();
	for (vector<DEVICE_INFO>::iterator it = deviceInfo.begin(); it != deviceInfo.end() ; it++)
	{
		DEV_INFO info = *it;
		String ^name = gcnew String(info.deviceName.c_str());
		String ^guid = gcnew String(info.guid_string.c_str());
		String ^tag = (String ^)node->Tag;
		if (node->Text == name && tag == guid)
		{
			Device::getDriverInfo(info.guid, &info.hardwareID, &info.manufacturer, &info.provider, &info.driverDescription);
			ListViewItem ^guidItem = gcnew ListViewItem("GUID");
			String ^guidM = gcnew String(info.guid_string.c_str());
			guidItem->SubItems->Add(guidM);
			list->Items->Add(guidItem);
			
			ListViewItem ^HIDItem = gcnew ListViewItem("Hardware ID");
			String ^HIDM = gcnew String(info.hardwareID.c_str());
			HIDItem->SubItems->Add(HIDM);
			list->Items->Add(HIDItem);

			ListViewItem ^manufacturerItem = gcnew ListViewItem("Manufacturer");
			String ^manufacturerM = gcnew String(info.manufacturer.c_str());
			manufacturerItem->SubItems->Add(manufacturerM);
			list->Items->Add(manufacturerItem);

			ListViewItem ^providerItem = gcnew ListViewItem("Provider");
			String ^providerM = gcnew String(info.provider.c_str());
			providerItem->SubItems->Add(providerM);
			list->Items->Add(providerItem);

			ListViewItem ^driverDescItem = gcnew ListViewItem("Driver description");
			String ^driverDescM = gcnew String(info.driverDescription.c_str());
			driverDescItem->SubItems->Add(driverDescM);
			list->Items->Add(driverDescItem);

			ListViewItem ^driverFullNameItem = gcnew ListViewItem("Driver full name");
			String ^driverFullNameM = gcnew String(info.driverFullName.c_str());
			driverFullNameItem->SubItems->Add(driverFullNameM);
			list->Items->Add(driverFullNameItem);

			ListViewItem ^devicePathItem = gcnew ListViewItem("Device path");
			String ^devicePathM = gcnew String(info.devicePath.c_str());
			devicePathItem->SubItems->Add(devicePathM);
			list->Items->Add(devicePathItem);

			ListViewItem ^isEnabledItem = gcnew ListViewItem("Is enabled");
			String ^isEnabledM = gcnew String(info.isEnabled ? "true" : "false");
			isEnabledItem->SubItems->Add(isEnabledM);
			list->Items->Add(isEnabledItem);
			return;
		}
	}
}

void MainWindow::Tree_DoubleClick(System::Object^ Sender, EventArgs ^e)
{
	vector<DEVICE_INFO> deviceInfo = DeviceEnumerator::getDevices();
	TreeNode ^ node = tree->SelectedNode;
	int index = 0;
	for (vector<DEVICE_INFO>::iterator it = deviceInfo.begin(); it != deviceInfo.end(); it++)
	{
		DEV_INFO info = *it;
		String ^name = gcnew String(info.deviceName.c_str());
		if (node->Text == name)
		{
			if (Device::deviceChangeStatus(info.hDevInfo, info.spDevInfoData, !info.isEnabled))
			{
				tree->Nodes->Clear();
				SetTree();
				list->Items->Clear();
				return;
			}
			
			MessageBox::Show("Can't disable device.", "Error", MessageBoxButtons::OK);
			return;
		}
		index++;
	}
}