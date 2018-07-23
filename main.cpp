#include "pidwidget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PIDWidget w;

    w.show();

    return a.exec();
}
