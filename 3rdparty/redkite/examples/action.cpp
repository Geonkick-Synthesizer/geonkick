/**
 * File name: painter.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "RkMain.h"
#include "RkWidget.h"
#include "RkPainter.h"
#include "RkPoint.h"
#include "RkLog.h"
#include "RkEvent.h"
#include "RkEventQueue.h"

class Button: public RkWidget {
  public:
        Button(RkWidget *parent = nullptr)
                : RkWidget(parent)
                , isToggled{false} {}

        RK_DECL_ACT(toggled, toggled(bool b), RK_ARG_TYPE(bool), RK_ARG_VAL(b));

  protected:
        void mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event) final
        {
                isToggled = !isToggled;
                // Post action to be executed by the GUI main thread.
                eventQueue()->postAction([&](){ toggled(isToggled); });
                // Or just call toggled(isToggled) directly to be
                // executed by the thread executing this method.
                // Anyway, mouseButtonPressEvent is executed only by GUI main thread.
                // eventQueue()->postAction([&](){ toggled(isToggled); });
                // can be called from a defferent thread than GUI main thread;
        }

private:
        bool isToggled;
};

class  PainterExample: public RkWidget {
  public:
        PainterExample(RkMain *app)
                : RkWidget(app)
                , startDraw{false}
        {
                auto button = new Button(this);
                button->setPosition(30, 30);
                button->setSize({30, 30});
                button->setBackgroundColor(255, 30, 100);
                RK_ACT_BIND(button, toggled, RK_ACT_ARGS(bool b), this, drawCircle(b));
                button->show();
        }

        ~PainterExample() = default;

  protected:
        void paintEvent(const std::shared_ptr<RkPaintEvent> &event) final
        {
                RK_UNUSED(event);
                RkPainter painter(this);
                painter.fillRect(rect(), RkColor(background()));
                RkPen pen(RkColor(255, 0, 0));
                pen.setWidth(1);
                painter.setPen(pen);
                if (startDraw)
                        painter.drawCircle(100, 100, 20);
        }

        void drawCircle(bool b)
        {
                startDraw = b;
                update();
        }

  private:
        bool startDraw;
};

int main(int arc, char **argv)
{
    RkMain app(arc, argv);

    auto widget = new PainterExample(&app);
    widget->setTitle("Painter Example");
    widget->setSize(350, 350);
    widget->show();

    return app.exec();
}
