#pragma once
#include <QtWidgets>
#include "ui_WindowsProcessCreator.h"
#include <string>
#include <vector>

class WindowsProcessCreator : public QMainWindow
{
	Q_OBJECT

public:
	WindowsProcessCreator(QWidget* parent = nullptr);
	~WindowsProcessCreator();

private:
	Ui::WindowsProcessCreatorClass ui{};
	std::wstring file{};
	bool enable_base_settings{ false };
	size_t parent_pid{ 0 };

	void setProcessesCombo();

private slots:
	void choose();
	void launch();
	void applyBaseSettings();
};
