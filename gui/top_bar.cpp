/**
 * File name: top_bar.cpp
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor (http://geontime.com)
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

#include "top_bar.h"
#include "geonkick_button.h"
#include "geonkick_label.h"

#include <QMouseEvent>
#include <QHBoxLayout>

TopBar::TopBar(GeonkickWidget *parent)
        : GeonkickWidget(parent),
          is_pressed(false),
          dX(0),
          dY(0),
          openFileButton(nullptr),
          saveFileButton(nullptr),
          exportFileButton(nullptr)
{
        if (parent) {
                setFixedSize(parent->width(), 35);
        }

        auto buttonsLayout = new QHBoxLayout(this);
        buttonsLayout->setContentsMargins(15, 0, 0, 0);
        setLayout(buttonsLayout);

        auto logo = new GeonkickLabel(this);
        logo->setImage(QPixmap("./themes/geontime/logo.png"));
        buttonsLayout->addWidget(logo);
        buttonsLayout->setAlignment(logo, Qt::AlignLeft);
        buttonsLayout->addSpacing(20);

        openFileButton = new GeonkickButton(this);
        openFileButton->setUnpressedImage(QPixmap("./themes/geontime/open.png"));
        openFileButton->setPressedImage(QPixmap("./themes/geontime/open_active.png"));
        buttonsLayout->addWidget(openFileButton);
        buttonsLayout->setAlignment(openFileButton, Qt::AlignLeft);

        saveFileButton = new GeonkickButton(this);
        saveFileButton->setUnpressedImage(QPixmap("./themes/geontime/save.png"));
        saveFileButton->setPressedImage(QPixmap("./themes/geontime/save_active.png"));
        buttonsLayout->addWidget(saveFileButton);
        buttonsLayout->setAlignment(saveFileButton, Qt::AlignLeft);

        exportFileButton = new GeonkickButton(this);
        exportFileButton->setUnpressedImage(QPixmap("./themes/geontime/export_active.png"));
        exportFileButton->setCheckable(true);
        buttonsLayout->addWidget(exportFileButton);
        buttonsLayout->setAlignment(exportFileButton, Qt::AlignLeft);
        connect(exportFileButton, SIGNAL(toggled(bool)), parent, SLOT(openExportDialog()));

        settingsButton = new GeonkickButton(this);
        settingsButton->setUnpressedImage(QPixmap("./themes/geontime/settings.png"));
        settingsButton->setPressedImage(QPixmap("./themes/geontime/settings_active.png"));
        buttonsLayout->addWidget(settingsButton);
        buttonsLayout->setAlignment(settingsButton, Qt::AlignLeft);
        buttonsLayout->addStretch(1);

        auto closeButton = new GeonkickButton(this);
        closeButton->setPressedImage(QPixmap("./themes/geontime/close_button.png"));
        closeButton->setUnpressedImage(QPixmap("./themes/geontime/close_button.png"));
        buttonsLayout->addWidget(closeButton);
        buttonsLayout->setAlignment(closeButton, Qt::AlignRight);
        if (parent) {
                connect(closeButton, SIGNAL(toggled(bool)), parent, SLOT(close()));
        }

}

TopBar::~TopBar()
{
}

void TopBar::paintWidget(QPaintEvent *event)
{
        Q_UNUSED(event)
        QPainter painter(this);
        painter.setPen(QPen(QColor(80, 80, 80)));
        painter.drawLine(0, height() - 1, width() - 1, height() - 1);
}

void TopBar::mousePressEvent(QMouseEvent *event)
{
        Q_UNUSED(event)
        is_pressed = true;
        setCursor(Qt::ClosedHandCursor);
        dX = event->x();
        dY = event->y();
}

void TopBar::mouseReleaseEvent(QMouseEvent *event)
{
        Q_UNUSED(event)
        setCursor(Qt::ArrowCursor);
        is_pressed = false;
}

void TopBar::mouseMoveEvent(QMouseEvent *event)
{
        if (is_pressed) {
                auto parent = parentWidget();
                parent->move(event->globalX() - dX, event->globalY() - dY);
        }
}
