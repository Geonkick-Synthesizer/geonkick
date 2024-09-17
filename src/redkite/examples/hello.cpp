#include "RkMain.h"
#include "RkWidget.h"
#include "RkLog.h"

int main(int arc, char **argv)
{
    RK_LOG_INFO("called");
    RkMain app(arc, argv);

    auto widget = new RkWidget(app);
    widget->setTitle("Hello!");
    widget->setBackgroundColor({80, 122, 10});
    widget->setSize(400, 500);
    widget->show();

    int res = app.exec();
    return res;
}
