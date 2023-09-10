/**
 * File name: child.cpp
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
        mainWindow->setBackgroundColor(0, 255, 0);

        int x = 10;
        int y = 10;
        RK_LOG_DEBUG("create childs");
        for (auto i = 0; i < 10; i++) {
                RK_LOG_DEBUG("create child " << i);
                auto child = new RkWidget(mainWindow);
                child->setTitle("Child[" + std::to_string(i) + "] - LEVEL 1");
                child->setSize(60, 60);
                child->setPosition(x, y);
                child->setBorderColor(0, 255, 0);
                child->setBackgroundColor(0, 0, 255);
                child->show();

                auto child_l = new RkWidget(child);
                child_l->setTitle("Child[" + std::to_string(i) + "] - LEVEL 2");
                child_l->setPosition(10, 10);
                child_l->setSize(30, 30);
                child_l->setBorderColor(0, 0, 255);
                child_l->setBackgroundColor(255, 0, 0);
                child_l->show();

                x += 65;
                if (x > 650) {
                        y += 65;
                        x = 10;
                }
        }

        mainWindow->show();
        int res = app.exec();
        return res;
}
