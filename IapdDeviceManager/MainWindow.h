#pragma once
#include "DeviceEnumerator.h"
#define WINDOW_TITLE "Device Manager"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 500
#define WINDOW_SPLITTER_DISTANCE 70
namespace IapdDeviceManager {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainWindow : public System::Windows::Forms::Form
	{	
	private:
		SplitContainer ^splitContainer;
		TreeView ^tree;
		System::ComponentModel::Container ^components;

		void InitializeComponent();
		void SetTree();
		void SetTreeItems();

	protected:
		~MainWindow();

	public:
		MainWindow();
	};
}
