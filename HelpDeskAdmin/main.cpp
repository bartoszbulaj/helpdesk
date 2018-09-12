#include "helpdeskadmin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HelpDeskAdmin w;
    w.show();

    return a.exec();
}
