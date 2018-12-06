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
          openFileButton(nullptr),
          saveFileButton(nullptr),
          exportFileButton(nullptr)
{
        if (parent) {
                setFixedSize(parent->width() , 40);
        }

        /*QPalette pal;
        pal.setColor(QPalette::Background, QColor(200, 200, 70));
        pal.setColor(QPalette::WindowText, Qt::white);
        setAutoFillBackground(true);
        setPalette(pal);*/


        auto buttonsLayout = new QHBoxLayout(this);
        buttonsLayout->setContentsMargins(15, 0, 0, 0);
        setLayout(buttonsLayout);

        auto logo = new GeonkickLabel(this);
        logo->setImage(QPixmap(":/logo.png"));
        buttonsLayout->addWidget(logo);
        buttonsLayout->setAlignment(logo, Qt::AlignLeft);
        buttonsLayout->addSpacing(20);

        openFileButton = new GeonkickButton(this);
        openFileButton->setUnpressedImage(QPixmap(":/open_active.png"));
        openFileButton->setCheckable(true);
        buttonsLayout->addWidget(openFileButton);
        buttonsLayout->setAlignment(openFileButton, Qt::AlignLeft);
        connect(openFileButton, SIGNAL(toggled(bool)), parent, SLOT(openPreset()));

        saveFileButton = new GeonkickButton(this);
        saveFileButton->setUnpressedImage(QPixmap(":/save_active.png"));
        saveFileButton->setCheckable(true);
        buttonsLayout->addWidget(saveFileButton);
        buttonsLayout->setAlignment(saveFileButton, Qt::AlignLeft);
        connect(saveFileButton, SIGNAL(toggled(bool)), parent, SLOT(savePreset()));

        exportFileButton = new GeonkickButton(this);
        exportFileButton->setUnpressedImage(QPixmap(":/export_active.png"));
        exportFileButton->setCheckable(true);
        buttonsLayout->addWidget(exportFileButton);
        buttonsLayout->setAlignment(exportFileButton, Qt::AlignLeft);
        connect(exportFileButton, SIGNAL(toggled(bool)), parent, SLOT(openExportDialog()));

        settingsButton = new GeonkickButton(this);
        settingsButton->setUnpressedImage(QPixmap(":/settings_active.png"));
        settingsButton->setCheckable(true);
        buttonsLayout->addWidget(settingsButton);
        buttonsLayout->setAlignment(settingsButton, Qt::AlignLeft);
        buttonsLayout->addStretch(1);
}

TopBar::~TopBar()
{
}
