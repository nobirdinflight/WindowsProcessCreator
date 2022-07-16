#include "WindowsProcessCreator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowsProcessCreator w;
    w.show();
    return a.exec();
}
