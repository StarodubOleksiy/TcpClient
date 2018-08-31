#include <QApplication>
#include "MyClient.h"
#include <QtGui>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MyClient     client("localhost", 4200);

    client.show();
    return app.exec();
}

