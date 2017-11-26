#pragma once
#include "DeviceEnumerator.h"
#define WINDOW_TITLE "Device Manager"
#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 700
#define WINDOW_SPLITTER_DISTANCE 70
#define LISTVIEW_COL1 "Param"
#define LISTVIEW_COL2 "Value"
#define LISTVIEW_COL1_WIDTH 100
#define LISTVIEW_COL2_WIDTH 600
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
		ListView ^list;
		SplitContainer ^splitContainer;
		TreeView ^tree;
		System::ComponentModel::Container ^components;

		void InitializeComponent();
		void SetTree();
		void SetTreeItems();
		void Tree_Click(System::Object^ Sender, TreeViewEventArgs ^e);
		void Tree_DoubleClick(System::Object^ Sender, EventArgs ^e);

	protected:
		~MainWindow();

	public:
		MainWindow();
	};
}
