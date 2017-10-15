/**
 * File name: envelopes_group_box.cpp
 * Project: GeonKick
 *
 * Copyright (C) 2017 Iurie Nistor <nistor@iurie.org>
 *
 * This file is part of MashaFile.
 *
 * MashaFile is free software; you can redistribute it and/or modify
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

#include "envelopes_group_box.h"

#include <QRadioButton>

#include <QLayout>

EnvelopesGroupBox::EnvelopesGroupBox(QWidget *parent)
        : ControlGroupBox(tr("Envelopes"), parent)
{
        QRadioButton *osc1Rb    = new QRadioButton(tr("OSC 1"), this);
        QRadioButton *osc2Rb    = new QRadioButton(tr("OSC 2"), this);
        QRadioButton *noiseRb   = new QRadioButton(tr("Noise"), this);
        QRadioButton *generalRb = new QRadioButton(tr("General"), this);
        osc1Rb->setChecked(true);
        connect(osc1Rb, SIGNAL(clicked(bool)), this, SIGNAL(viewOsc1(bool)));
        connect(osc2Rb, SIGNAL(clicked(bool)), this, SIGNAL(viewOsc2(bool)));
        connect(noiseRb, SIGNAL(clicked(bool)), this, SIGNAL(viewNoiseOsc(bool)));
        connect(generalRb, SIGNAL(clicked(bool)), this, SIGNAL(viewGeneral(bool)));
        layout()->addWidget(osc1Rb);
        layout()->addWidget(osc2Rb);
        layout()->addWidget(noiseRb);
        layout()->addWidget(generalRb);
}

EnvelopesGroupBox::~EnvelopesGroupBox()
{
}
