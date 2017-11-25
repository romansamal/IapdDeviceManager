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
	this->Size = System::Drawing::Size(300, 300);
	this->Text = L"MainWindow";
	this->Padding = System::Windows::Forms::Padding(0);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
}
