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
	mainWindow->setPosition(180, 180);
        mainWindow->setSize(400, 500);

        auto child = new RkWidget(mainWindow);
        child->setTitle("Child");
        child->setBackgroundColor(80, 255, 80);
        child->setPosition(10, 10);
        child->setSize(50, 50);
        child->show();

        auto modal = new RkWidget(mainWindow, Rk::WindowFlags::Dialog);
        modal->setTitle("Dialog");
        modal->setBackgroundColor(80, 80, 80);
        modal->setSize(170, 110);

        mainWindow->show();
        modal->show();

        int res = app.exec();
        return res;
}


