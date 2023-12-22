#include "widget.h"

#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.qss");
    if (file.exists() ) {
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
    Widget w;
    w.show();
    return a.exec();
}
