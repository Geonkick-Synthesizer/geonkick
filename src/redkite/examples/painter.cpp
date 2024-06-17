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

class  PainterExample: public RkWidget {
  public:
        PainterExample(RkMain &app)
                : RkWidget(app)
                , clickPoint(50, 50)
                , startDraw{true}
        {
                RK_LOG_INFO("called");
                setBackgroundColor(80, 80, 80);
        }

        ~PainterExample() = default;

  protected:
        void paintEvent(RkPaintEvent *event) override
        {
                RK_LOG_INFO("called");
                if (startDraw) {
                        RK_LOG_INFO("start draw");
                        RkPainter painter(this);
                        RkPen pen(RkColor(255, 0, 0));
                        pen.setWidth(1);
                        pen.setStyle(RkPen::PenStyle::DashLine);
                        painter.setPen(pen);
                        painter.drawLine({10, 10}, {100, 100});
                        painter.drawCircle(50, 50, 40);
                        painter.fillRect({50, 50, 20, 20}, background());
                        pen.setStyle(RkPen::PenStyle::DotLine);
                        pen.setColor({0, 55, 123});
                        painter.setPen(pen);
                        painter.drawRect({10, 10, 10, 10});
                }

			 /*
                if (startDraw) {

                        if (image.width() != width() || image.height() != height()) {
                                RkImage im(width(), height());
                                image = im;
                        }

                        RkPainter painter(&image);
                        painter.fillRect(rect(), background());
                        RkPen pen(RkColor(255, 0, 0));
                        pen.setWidth(1);
                        pen.setStyle(RkPen::PenStyle::DashLine);
                        painter.setPen(pen);
                        painter.drawCircle(50, 50, 40);
                        painter.drawLine(50, 50, 100, 100);

                        painter.drawCircle(clickPoint.x() + 40 / 2, clickPoint.y() + 40/2, 40);
                        painter.drawLine(clickPoint, RkPoint(clickPoint.x() + 80, clickPoint.y() + 80));

                        pen.setStyle(RkPen::PenStyle::DotLine);
                        pen.setColor(RkColor(0, 255, 0));
                        painter.setPen(pen);
                        painter.drawCircle(clickPoint.x() + 40 / 2, clickPoint.y() - 40/2, 40);
                        painter.drawLine(clickPoint, RkPoint(clickPoint.x() + 80, clickPoint.y() - 80));

                        pen.setStyle(RkPen::PenStyle::SolidLine);
                        pen.setColor(RkColor(0, 0, 255));
                        painter.setPen(pen);
                        painter.drawCircle(clickPoint.x() - 40 / 2, clickPoint.y() - 40/2, 40);
                        painter.drawLine(clickPoint, RkPoint(clickPoint.x() - 80, clickPoint.y() - 80));

                        int y = 50;
                        painter.fillRect(RkRect(50, y, 100, 25), RkColor(255, 255, 255));
                        RkFont font = painter.font();
                        font.setStyle(RkFont::Style::Italic);
                        painter.setFont(font);
                        painter.drawText({50, y, 100, 25}, "Hello!");

                        y += 55;
                        painter.fillRect(RkRect(50, y, 100, 25), RkColor(255, 255, 255));
                        painter.drawText({50, y, 100, 25}, "Hello!", Rk::Alignment::AlignLeft);

                        y += 55;
                        painter.fillRect(RkRect(50, y, 100, 25), RkColor(255, 255, 255));
                        painter.drawText({50, y, 100, 25}, "Hello!", Rk::Alignment::AlignRight);

                        painter.drawRect(RkRect(50, y, 100, 25));
                        RkPainter paint(this);
                        paint.drawImage(image, 0, 0);
						*/
                }

        void mouseButtonPressEvent(RkMouseEvent *event) override
        {
                clickPoint = RkPoint(event->x(), event->y());
                startDraw = !startDraw;
                update();
        }

        void mouseMoveEvent(RkMouseEvent* event) override
        {
                if (startDraw) {
                        clickPoint = RkPoint(event->x(), event->y());
                        update();
                }
        }


  private:
        RkPoint clickPoint;
        bool startDraw;
        //        RkImage image;
};

int main(int arc, char **argv)
{
    RkMain app(arc, argv);

    auto widget = new PainterExample(app);
    widget->setTitle("Painter Example");
    widget->setSize(350, 350);
    widget->show();

    return app.exec();
}
