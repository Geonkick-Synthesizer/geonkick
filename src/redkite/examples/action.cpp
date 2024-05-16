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
#include "RkButton.h"

static void setRandomColor(RkWidget* widget)
{
        int red = rand() % 151 + 50;
        int green = rand() % 151 + 50;
        int blue = rand() % 151 + 50;

        widget->setBorderColor(red, green, blue);
        widget->setBackgroundColor(red, green, blue);
}

static void drawChildren(RkWidget* parent, int l)
{
        if (l > 1)
                return;
        auto wCh = parent->width() / 10;
        auto hCh = parent->height() / 10;
        for (int y = 0; y < parent->height() -  hCh - 1; y += hCh + 1) {
                for (int x = 0; x < parent->width() - wCh - 1; x += wCh + 1) {
                        auto child = new RkWidget(parent);
                        setRandomColor(child);
                        child->setTitle("Child[" + std::to_string(x) + ":" + std::to_string(y) + "] - " + std::to_string(l));
                        child->setSize(wCh, hCh);
                        RK_LOG_DEV_DEBUG("x:" << x);
                        RK_LOG_DEV_DEBUG("y:" << y);
                        child->setPosition(x, y);
                        child->show();
                        drawChildren(child, l + 1);
                }
        }
}

class  PainterExample: public RkWidget {
  public:
        PainterExample(RkMain& app)
                : RkWidget(app)
                , startDraw{false}
        {
                auto button = new RkButton(this);
                button->setPosition(800, 800);
                button->setSize({30, 30});
                button->setBackgroundColor(255, 0, 100);
                RK_ACT_BIND(button, pressed, RK_ACT_ARGS(), this, drawCircle());
                button->show();
        }

        ~PainterExample() = default;

  protected:
        void paintEvent(RkPaintEvent* event) override
        {
                RK_UNUSED(event);
                RkPainter painter(this);
                painter.fillRect(rect(), RkColor(background()));
                RkPen pen(RkColor(0, 255, 0));
                pen.setWidth(3);
                painter.setPen(pen);
                if (startDraw)
                        painter.drawCircle(700, 850, 100);
        }

        void drawCircle()
        {
                startDraw = !startDraw;
                update();
        }

  private:
        bool startDraw;
};

int main(int arc, char **argv)
{
    RkMain app(arc, argv);

    auto widget = new PainterExample(app);
    widget->setTitle("Painter Example");
    widget->setSize(800, 900);
    drawChildren(widget, 0);
    widget->show();

    return app.exec();
}
