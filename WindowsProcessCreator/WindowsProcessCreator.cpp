#include "WindowsProcessCreator.h"
#include "ProcessWrapper.h"
WindowsProcessCreator::WindowsProcessCreator(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.choose_button, &QPushButton::clicked, this, &WindowsProcessCreator::choose);
	connect(ui.launch_button, &QPushButton::clicked, this, &WindowsProcessCreator::launch);

#ifdef _DEBUG
	file = LR"(C:\Windows\notepad.exe)";
	ui.path_edit->setText(QString::fromStdWString(file));
#endif
}

WindowsProcessCreator::~WindowsProcessCreator()
{}

void WindowsProcessCreator::choose()
{
	auto file_qstring = QFileDialog::getOpenFileName(this, "Please choose executable file.", R"(C:\Windows)", "File (*.exe)");
	ui.path_edit->setText(file_qstring);
	file = file_qstring.toStdWString();
}

void WindowsProcessCreator::launch()
{
	createProcess(file);
}