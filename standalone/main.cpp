/**
 * File name: main.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#include "mainwindow.h"
#include "geonkick_api.h"
//#include "geonkick_state.h"

#include <RkMain.h>

int main(int argc, char *argv[])
{
        RkMain app(argc, argv);

        // QFontDatabase::addApplicationFont(":/urw_gothic_l_book.ttf");
        // QFont font("URW Gothic L");
        // font.setPixelSize(12);
        // a.setFont(font);

        //        QString preset;
        //if (QCoreApplication::arguments().size() > 1) {
        //        preset = QCoreApplication::arguments().at(1);
        //}

        auto api = std::make_unique<GeonkickApi>();
        api->setStandalone(true);
        if (!api->init()) {
                GEONKICK_LOG_ERROR("can't init API");
                exit(1);
        }

        auto window = new MainWindow(api.get());
        if (!window->init()) {
                GEONKICK_LOG_ERROR("can't init main window");
                exit(1);
        }

        if (!app.setTopLevelWindow(window)) {
                GEONKICK_LOG_ERROR("can't set application main window");
                exit(1);
        }

        window->show();
        return app.exec();
}
