#include "RkMain.h"
#include "RkWidget.h"
#include "RkLog.h"

#include <vector>

int main(int arc, char **argv)
{
    RkMain app(arc, argv);

    // Create main window.
    auto mainWindow = new RkWidget(&app);
    mainWindow->setTitle("Main Window");
    mainWindow->setSize(750, 600);

    int x = 10;
    // Black
    auto child = new RkWidget(mainWindow);
    child->setTitle("Child- - black");
    child->setSize(50, 50);
    child->setPosition(x, 10);
    child->setBackgroundColor(0, 0, 0);
    child->show();

    x += 60;
    // White
    child = new RkWidget(mainWindow);
    child->setTitle("Child- - white");
    child->setSize(50, 50);
    child->setPosition(x, 10);
    child->setBackgroundColor(255, 255, 255);
    child->setBorderWidth(3);
    child->setBorderColor(0, 0, 255);
    child->show();

    x += 60;
    // Red
    child = new RkWidget(mainWindow);
    child->setTitle("Child- - red");
    child->setSize(50, 50);
    child->setPosition(x, 10);
    child->setBackgroundColor(255, 0, 0);
    child->show();

    x += 60;
    // Green
    child = new RkWidget(mainWindow);
    child->setTitle("Child- - green");
    child->setSize(50, 50);
    child->setPosition(x, 10);
    child->setBackgroundColor(0, 255, 0);
    child->show();

    x += 60;
    // Blue
    child = new RkWidget(mainWindow);
    child->setTitle("Child- - blue");
    child->setSize(50, 50);
    child->setPosition(x, 10);
    child->setBackgroundColor(0, 0, 255);
    child->show();

    x += 60;
    // Alloy orange
    child = new RkWidget(mainWindow);
    child->setTitle("Child - alloy orange");
    child->setSize(50, 50);
    child->setPosition(x, 10);
    child->setBackgroundColor(196, 98, 16);
    child->setBorderWidth(3);
    child->setBorderColor(255, 100, 255);
    child->show();

    mainWindow->show();
    int res = app.exec();
    return res;
}


