#include "oscillator_widget.h"
#include "gkickapi.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QVector>

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        MainWindow window;
        if (!window.init()) {
                qDebug() << "can't init main window";
                exit(1);
        }

        window.show();
        return a.exec();
}
