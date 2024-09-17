#include "RkMain.h"
#include "RkWidget.h"
#include "RkLog.h"

class MyProperty : public RkWidget {
  public:
        // Suitable also for styling info.
        RK_SET_CLASS_INFO(style_element, "MyProperty")
        RK_SET_CLASS_INFO(style_id, "MyID")

        MyProperty(RkMain *app) : RkWidget(app)
        {
                RK_LOG_INFO("called");
        }

        MyProperty(RkWidget *parent) : RkWidget(parent)
        {
                RK_LOG_INFO("called");
        }

        virtual ~MyProperty()
        {
                RK_LOG_INFO("called");
        }
};


class OverrideMyProperty : public MyProperty {
  public:
        // Suitable also for styling info.
        RK_SET_CLASS_INFO(style_element, "OverrideMyProperty")
        RK_SET_CLASS_INFO(style_class, "MyProperty")
        RK_SET_CLASS_INFO(style_id, "MyNewId")

        OverrideMyProperty(RkWidget *parent) : MyProperty(parent)
        {
                RK_LOG_INFO("called");
        }

        ~OverrideMyProperty()
        {
                RK_LOG_INFO("called");
        }
};

int main(int arc, char **argv)
{
    RkMain app(arc, argv);

    auto widget = new MyProperty(&app);
    widget->setTitle("Property Example");
    widget->show();

    RK_LOG_DEBUG("element: " << widget->rk_property_style_element());
    RK_LOG_DEBUG("class: " << widget->rk_property_style_class());
    RK_LOG_DEBUG("id: " << widget->rk_property_style_id());

    auto widgetOverride = new OverrideMyProperty(widget);
    widgetOverride->setTitle("OverrideMyProperty Example");
    widgetOverride->show();

    RK_LOG_DEBUG("child element: " << widgetOverride->rk_property_style_element());
    RK_LOG_DEBUG("child class: " << widgetOverride->rk_property_style_class());
    RK_LOG_DEBUG("child id: " << widgetOverride->rk_property_style_id());

    int res = app.exec();
    return res;
}


