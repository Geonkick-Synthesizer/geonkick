/**
 * File name: file_dialog.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor (http://geontime.com)
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

#include "file_dialog.h"
#include "geonkick_button.h"

extern const unsigned char rk_open_png[];
extern const unsigned char rk_save_png[];
extern const unsigned char rk_cancel_png[];

FileDialog::FileDialog(GeonkickWidget *parent, FileDialog::Type type, const std::string& title)
        : GeonkickWidget(parent, Rk::WindowFlags::Dialog)
        , dialogType{type}
{
        setTitle(title);
        setFixedSize(600, 400);

        auto acceptButton = new GeonkickButton(this);
        acceptButton->setFixedSize(90, 30);
        acceptButton->setPosition(width() - acceptButton->width() - 10,
                                  height() - acceptButton->height() - 5);
        if (dialogType == Type::Save)
                acceptButton->setUnpressedImage(RkImage(90, 30, rk_save_png));
        else
                acceptButton->setUnpressedImage(RkImage(90, 30, rk_open_png));
        RK_ACT_BIND(acceptButton, toggled, RK_ACT_ARGS(bool pressed), this, onAccept());
        acceptButton->show();

        auto cancelButton = new GeonkickButton(this);
        cancelButton->setFixedSize(90, 30);
        cancelButton->setPosition(acceptButton->x() - cancelButton->width() - 5, acceptButton->y());
        cancelButton->setUnpressedImage(RkImage(90, 30, rk_cancel_png));
        RK_ACT_BIND(cancelButton, toggled, RK_ACT_ARGS(bool pressed), this, onCancel());
        cancelButton->show();
        show();
}

void FileDialog::onAccept()
{
        selectedFile("testfile.gkick");
        close();
}

void FileDialog::onCancel()
{
        close();
}

