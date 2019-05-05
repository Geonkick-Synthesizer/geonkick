/**
 * File name: top_bar.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include "top_bar.h"
#include "geonkick_button.h"

#include "RkLabel.h"

extern const unsigned char rk_logo_png[];
extern const unsigned char rk_open_active_png[];
extern const unsigned char rk_save_active_png[];
extern const unsigned char rk_export_active_png[];
extern const unsigned char rk_about_png[];

TopBar::TopBar(GeonkickWidget *parent)
        : GeonkickWidget(parent)
        , openFileButton{nullptr}
        , saveFileButton{nullptr}
        , exportFileButton{nullptr}
        , presetNameLabel{nullptr}
{
        setFixedWidth(parent->width());
        setFixedHeight(40);

        auto logo = new RkLabel(this);
        RkImage image(120, 20, rk_logo_png);
        logo->setSize(image.width(), image.height());
        logo->setBackgroundColor(68, 68, 70);
        logo->setImage(image);
        logo->setY((height() - logo->height()) / 2);
        logo->show();

        openFileButton = new GeonkickButton(this);
        openFileButton->show();
        openFileButton->setSize(90, 30);
        openFileButton->setX(logo->x() + logo->width() + 5);
        openFileButton->setY((height() - openFileButton->height()) / 2);
        openFileButton->setUnpressedImage(RkImage(90, 30, rk_open_active_png));
        openFileButton->setCheckable(true);
        RK_ACT_BIND(openFileButton, toggled, RK_ACT_ARGS(bool b), this, openFile());

        saveFileButton = new GeonkickButton(this);
        saveFileButton->setSize(90, 30);
        saveFileButton->setX(openFileButton->x() + openFileButton->width() + 5);
        saveFileButton->setY(openFileButton->y());
        saveFileButton->setUnpressedImage(RkImage(90, 30, rk_save_active_png));
        saveFileButton->setCheckable(true);
        RK_ACT_BIND(saveFileButton, toggled, RK_ACT_ARGS(bool b), this, saveFile());

        exportFileButton = new GeonkickButton(this);
        exportFileButton->setSize(90, 30);
        exportFileButton->setX(saveFileButton->x() + saveFileButton->width() + 5);
        exportFileButton->setY(saveFileButton->y());
        exportFileButton->setUnpressedImage(RkImage(90, 30, rk_export_active_png));
        exportFileButton->setCheckable(true);
        //        RK_ACT_BIND(exportFileButton, toggled, RK_ACT_ARS(bool b), parent, openExportDialog());

        auto aboutButton = new GeonkickButton(this);
        aboutButton->setSize(90, 30);
        aboutButton->setX(exportFileButton->x() + exportFileButton->width() + 5);
        aboutButton->setY(exportFileButton->y());
        aboutButton->setUnpressedImage(RkImage(90, 30, rk_about_png));
        aboutButton->setCheckable(true);
        RK_ACT_BIND(aboutButton, toggled, RK_ACT_ARGS(bool b), this, openAbout());

        presetNameLabel = new RkLabel(this);
}

TopBar::~TopBar()
{
}

void TopBar::setPresetName(const std::string &name)
{
        /*        if (name.size() > 20) {
                std::string preset = name;
                preset.truncate(20);
                preset += "...";
                presetNameLabel->setText(preset);
        }
        else {
                presetNameLabel->setText(name);
                }*/
}
