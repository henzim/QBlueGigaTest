#include "blewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BLEWindow w;
    w.show();

    return a.exec();
}
