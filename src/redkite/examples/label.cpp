/**
 * File name: label.cpp
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
#include "RkLabel.h"
#include "RkLog.h"

#include "RkPainter.h"
#include "RkImage.h"

/**
 * Generated with png2c
 * File name: label.png
 * Image size name: 100x94
 */
const unsigned char rk_image[] = {};

class MyImage : public RkWidget {
public:
        MyImage(RkWidget * parent)
                : RkWidget(parent)
                {
                        setSize(48, 48);
                }
protected:
        void paintEvent(RkPaintEvent *event) override {
                //RK_LOG_DEBUG("called====================================>");
                RkPainter painter(this);
                RK_LOG_DEBUG("called===================================[1]=>");
                RkImage img(48, 48, rk_image);
                painter.drawImage(img, 0, 0);
        } 
};

int main(int arc, char **argv)
{
        RkMain app(arc, argv);

        // Create main window.
        auto widget = new RkWidget(&app);
        widget->setTitle("Label Example");
        widget->setSize(250, 250);

        auto image = new MyImage(widget);
        image->show();

        //auto label = new RkLabel(widget);
        //label->setImage(RkImage(48, 48, rk_image));
//        label->setText("Text");
        //label->setX(10);
        //label->setY(10);
        //label->setSize(48, 48);
        //label->setBackgroundColor(200, 200, 200);
        //label->show();

        //auto label = new RkLabel(widget);
        /*label->setTitle("Label with image");
		label->setText("Text");
        label->setImage(RkImage(100, 94, rk_image));
        label->setX(10 + 100 + 5);
        label->setY(10);
        label->setSize(100, 94);
        label->setBackgroundColor(200, 200, 200);
        label->show();*/
		/*

        label = new RkLabel(widget);
        label->setTitle("Label with text & image");
        label->setText("Text");
        label->setImage(RkImage(100, 94, rk_image));
        label->setX(10);
        label->setY(10 + 94 + 5);
        label->setSize(100, 94);
        label->setBackgroundColor(200, 200, 200);
        label->show();*/

        widget->show();
        int res = app.exec();
        return res;
}
