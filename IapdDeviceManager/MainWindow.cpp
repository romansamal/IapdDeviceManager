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
	tree = gcnew TreeView();
	tree->Dock = DockStyle::Fill;
	splitContainer = gcnew SplitContainer();
	splitContainer->Panel1->Controls->Add(tree);
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
			String ^deviceDescription = gcnew String(item.c_str());
			TreeNode ^newNode = gcnew TreeNode(deviceDescription);
			tree->Nodes->Add(newNode);
		}
	}
}

void MainWindow::SetTreeItems()
{

}