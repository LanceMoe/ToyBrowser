#include "ToyBrowser.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToyBrowser w;
    w.show();
    return a.exec();
}
