/**
 * File name: main.cpp
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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
#include "geonkick_state.h"

#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QVector>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        QFontDatabase::addApplicationFont("./themes/geontime/urw_gothic_l_book.ttf");
        QFont font("URW Gothic L");
        font.setPixelSize(12);
        a.setFont(font);

        auto api = std::make_unique<GeonkickApi>();
        if (!api->init()) {
                GEONKICK_LOG_ERROR("can't init API");
                exit(1);
        }

        MainWindow window(api.get());
        if (!window.init()) {
                qDebug() << "can't init main window";
                exit(1);
        }

        window.show();
        return a.exec();
}
