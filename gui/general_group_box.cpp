/**
 * File name: oscillator_group_box.cpp
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

#include "general_group_box.h"
#include "gkick_knob.h"
#include "gkickapi.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

GeneralGroupBox::GeneralGroupBox(QWidget *parent, std::shared_ptr<GKickApi> &api)
        : ControlGroupBox(tr("General"), parent),
          kickApi(api)
{
        GKickKnob *kickLengthKnob = new GKickKnob(this, tr("Length"));
        kickLengthKnob->setMaxValue(kickApi->getMaxLength());
        kickLengthKnob->setCurrentValue(kickApi->getKickLength());
        layout()->addWidget(kickLengthKnob);
        connect(kickLengthKnob, SIGNAL(valueUpdated(double)),
                kickApi.get(), SLOT(setKickLengthValue(double)));

        GKickKnob *kickAmplitudeKnob = new GKickKnob(this, tr("Amplitute"));
        kickAmplitudeKnob->setMaxValue(1);
        kickAmplitudeKnob->setCurrentValue(kickApi->getAmplitude());
        layout()->addWidget(kickAmplitudeKnob);
        connect(kickAmplitudeKnob, SIGNAL(valueUpdated(double)),
                        kickApi.get(), SLOT(setAmplitude(double)));


        createFilterGroupBox();
}

GeneralGroupBox::~GeneralGroupBox()
{
}

void GeneralGroupBox::createFilterGroupBox()
{
        QGroupBox *filterGroupBox = new QGroupBox(tr("Filter"), this);
        QVBoxLayout *filterGroupBoxLayout = new QVBoxLayout();
        filterGroupBox->setLayout(filterGroupBoxLayout);
        layout()->addWidget(filterGroupBox);

        // Create filter type group box.
/*        QGroupBox *filterTypeGroupBox = new QGroupBox(tr("Type"), filterGroupBox);
        filterTypeCb = new QComboBox(filterTypeGroupBox);
        filterTypeCb->addItem(tr("Low pass"));
        filterTypeCb->addItem(tr("Hight pass"));
        filterTypeGroupBox->layout()->addWiget(filterTypeCb);
        filterGroupBox->layout()->addWiget(filterTypeGroupBox);

        // Create filter knobs gorup box.
        QGroupBox *filterKnobsGroupBox = new QGroupBox(filterGroupBox);
        QHBoxLayout *filterKnobsGroupBoxLayout = new QHLayout();
        filterKnobsGroupBox->setLayout(filterKnobsGroupBox);
        GKickKnob *filterFrequencyKnob = new GKickKnob(oscillatorGroupBox, tr("Frequency"));
        GKickKnob *filterQKnob = new GKickKnob(oscillatorGroupBox, tr("Q"));
        filterFrequencyKnob->setMaxValue(20000);
        filterFrequencyKnob->setCurrentValue(kickApi->getKickFilterFrequency());
        filterQKnob->setMaxValue(10);
        filterQKnob->setCurrentValue(kickApi->getKickFilterQFactor());
        filterKnobsGroupBoxLayout->addWidget(filterFrequencyKnob);
        filterKnobsGroupBoxLayout->addWidget(filterQKnob);

        connect(filterFrequencyKnob, SIGNAL(valueUpdated(double)),
                kickApi.get(), SLOT(setKickFilterFrequency(double)));
        connect(filterQKnob, SIGNAL(valueUpdated(double)),
                kickApi.get(), SLOT(setKickFilterQFactor(double)));
*/
}
