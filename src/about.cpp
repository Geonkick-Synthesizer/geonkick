/**
 * File name: about.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor <http://geontime.com>
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

#include "about.h"

#include <RkLabel.h>

extern const unsigned char rk_logo_about_png[];

AboutDialog::AboutDialog(GeonkickWidget *parent)
        : GeonkickWidget(parent, Rk::WindowFlags::Dialog)
{
        setFixedSize(340, 400);
        setTitle(std::string("About - ") + std::string(GEOKICK_APP_NAME));
        auto logo = new RkLabel(this);
        logo->setImage(RkImage(300, 356, rk_logo_about_png));
        logo->setFixedSize(300, 356);
        logo->setBackgroundColor(background());
        logo->setPosition((width() - logo->width()) / 2, (height() - logo->height()) / 2);
        logo->show();
        show();
}
