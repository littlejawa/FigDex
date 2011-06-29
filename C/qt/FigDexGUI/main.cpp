#include <QtGui/QApplication>
#include "displaywindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DisplayWindow w;
    w.show();

    return a.exec();
}
