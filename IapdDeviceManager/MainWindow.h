#pragma once

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
		System::ComponentModel::Container ^components;

		void InitializeComponent();

	protected:
		~MainWindow();

	public:
		MainWindow();
	};
}
