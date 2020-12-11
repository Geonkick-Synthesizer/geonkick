/**
 * File name: main.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include <RkMain.h>

int main(int argc, char *argv[])
{
        RkMain app(argc, argv);

        std::string preset;
        if (argc == 2)
                preset = argv[1];

        auto api = new GeonkickApi;
        api->setEventQueue(app.eventQueue());
        api->setStandalone(true);
        if (!api->init()) {
                GEONKICK_LOG_ERROR("can't init API");
                delete api;
                exit(1);
        }

        auto window = new MainWindow(&app, api, preset);
        if (!window->init()) {
                GEONKICK_LOG_ERROR("can't init main window");
                exit(1);
        }

        return app.exec();
}
