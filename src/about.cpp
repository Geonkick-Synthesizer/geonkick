/**
 * File name: about.cpp
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

#include "about.h"
#include "geonkick_label.h"

#include <QEventLoop>
#include <QVBoxLayout>
#include <QLabel>
#include <QCloseEvent>

AboutDialog::AboutDialog(GeonkickWidget *parent)
        : GeonkickWidget(parent)
{
        setWindowFlags(Qt::Dialog);
        setWindowModality(Qt::ApplicationModal);
        setWindowTitle(tr("About") + std::string(" - ") + std::string(GEOKICK_APP_NAME));
        auto mainLayout = new QVBoxLayout(this);
        auto label = new GeonkickLabel(this);
        label->setImage(QPixmap(":/logo_about.png"));
        mainLayout->addWidget(label, 0, Qt::AlignHCenter);
        std::string aboutTxt = "<h2>Geonkick v" + std::string(GEOKICK_APP_VERION_STRING) + std::string("</h2>")
                + std::string("<p>Geonkick - a free software percussion synthesizer</p> \
                          <p>Copyright (C) 2018 Iurie Nistor</p> \
                          <p>License: GNU General Public License Version 3</p>");
        mainLayout->addWidget(new GeonkickLabel(aboutTxt, this), 0, Qt::AlignHCenter);
        setLayout(mainLayout);
        show();
}

AboutDialog::~AboutDialog()
{
}

int AboutDialog::exec()
{
        QEventLoop eventLoop(this);
        connect(this, SIGNAL(closeDialog()), &eventLoop, SLOT(quit()));
        eventLoop.exec();
        return 0;
}

void AboutDialog::closeEvent(QCloseEvent *event)
{
        event->ignore();
        emit closeDialog();
}
