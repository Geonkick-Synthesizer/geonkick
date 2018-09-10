/**
 * File name: envelopes_group_box.cpp
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

#include "envelopes_group_box.h"
#include "geonkick_widget.h"

#include <QRadioButton>
#include <QLayout>

EnvelopesGroupBox::EnvelopesGroupBox(GeonkickWidget *parent)
  : GeonkickGroupBox(parent)
{
        /*        QRadioButton *osc1Rb    = new QRadioButton(tr("OSC 1"), this);
        QRadioButton *osc2Rb    = new QRadioButton(tr("OSC 2"), this);
        QRadioButton *noiseRb   = new QRadioButton(tr("Noise"), this);
        QRadioButton *generalRb = new QRadioButton(tr("General"), this);
        osc1Rb->setChecked(true);
        connect(osc1Rb, SIGNAL(clicked(bool)), this, SLOT(viewOsc1(bool)));
        connect(osc2Rb, SIGNAL(clicked(bool)), this, SLOT(viewOsc2(bool)));
        connect(noiseRb, SIGNAL(clicked(bool)), this, SLOT(viewNoiseOsc(bool)));
        connect(generalRb, SIGNAL(clicked(bool)), this, SLOT(viewGeneral(bool)));
        layout()->addWidget(osc1Rb);
        layout()->addWidget(osc2Rb);
        layout()->addWidget(noiseRb);
        layout()->addWidget(generalRb);*/
}

EnvelopesGroupBox::~EnvelopesGroupBox()
{
}

void EnvelopesGroupBox::viewOsc1(bool b)
{
        Q_UNUSED(b);
        emit viewEnvelope(GKickEnvelope::ENV_CATEGORY_OSC_1);
}

void EnvelopesGroupBox::viewOsc2(bool b)
{
        Q_UNUSED(b);
        emit viewEnvelope(GKickEnvelope::ENV_CATEGORY_OSC_2);
}

void EnvelopesGroupBox::viewNoiseOsc(bool b)
{
        Q_UNUSED(b);
        emit viewEnvelope(GKickEnvelope::ENV_CATEGORY_NOISE);
}

void EnvelopesGroupBox::viewGeneral(bool b)
{
        Q_UNUSED(b);
        emit viewEnvelope(GKickEnvelope::ENV_CATEGORY_GENERAL);
}
