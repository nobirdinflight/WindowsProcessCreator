#pragma once
#include <QtWidgets>
#include "ui_WindowsProcessCreator.h"
#include <string>

class WindowsProcessCreator : public QMainWindow
{
	Q_OBJECT

public:
	WindowsProcessCreator(QWidget* parent = nullptr);
	~WindowsProcessCreator();

private:
	Ui::WindowsProcessCreatorClass ui;
	std::wstring file;

private slots:
	void choose();
	void launch();
};
