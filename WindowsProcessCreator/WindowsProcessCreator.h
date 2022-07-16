#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_WindowsProcessCreator.h"

class WindowsProcessCreator : public QMainWindow
{
    Q_OBJECT

public:
    WindowsProcessCreator(QWidget *parent = nullptr);
    ~WindowsProcessCreator();

private:
    Ui::WindowsProcessCreatorClass ui;
};
