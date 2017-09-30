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

#include "oscillator_group_box.h"

OscillatorGroupBox::OscillatorGroupBox(QWidget *parent, std::shared_prt<GKickOscillator> &osc)
        : ControlGroupBox((osc->name(), parent),
          oscillator(osc)
{
        if (socillator->type() != GKickOscillator::OSC_NOISE) {
                createWaveFunctionGroupBox();
        }
        createEvelopeGroupBox();
        createFilterGroupBox();
}

EnvelopesGroupBox::~EnvelopesGroupBox()
{
}

void EnvelopesGroupBox::createWaveFunctionGroupBox()
{
        QGroupBox *waveFunctionGroupBox = new QGroupBox(tr("Wave function"), this);
        waveFunctionCb = new QComboBox(waveFunctionGroupBox);
        waveFunctionCb->addItem(tr("Sine"));
        waveFunctionCb->addItem(tr("Sqare"));
        waveFunctionCb->addItem(tr("Triangle"));
        waveFunctionCb->addItem(tr("Sawtooth"));
        waveFunctionCb->addItem(tr("Noise"));
        connect(waveFunctionCb, SIGNAL(currentIndexChanged(int)),
                oscillator, SLOT(changeOscFunction(int)));
        layout()->addWidget(waveFunctionGroupBox);
}

void EnvelopesGroupBox::createEvelopeGroupBox()
{
        QGroupBox *envelopeGroupBox = new QGroupBox(tr("Envelope"), this);
        QHLayout *envelopeGroupBoxLayout = new QGridLayout();
        envelopeGroupBox->setLayout(envelopeGroupBoxLayout);
        layout()->addWidget(envelopeGroupBox);

        GKickKnob *envAmplitudeKnob = new GKickKnob(envelope, tr("Amplitute"));
        kickAmplitudeKnob->setMaxValue(1);
        kickAmplitudeKnob->setCurrentValue(oscillator->getOscAmplitudeValue());
        envelopeGroupBoxLayout->addWidget(kickAmplitudeKnob);

        connect(envAmplitudeKnob, SIGNAL(valueUpdated(double)),
                oscillators, SLOT(setOscAmplitudeValue(double)));

        if (oscillator->type() != GKickOscillator::OSC_NOISE) {
                GKickKnob *envFrequencyKnob = new GKickKnob(oscillatorGroupBox, tr("Frequency"));
                envFrequencyKnob->setMaxValue(20000);
                envFrequencyKnob->setCurrentValue(oscillator->getOscFrequencyValue());
                envelopeGroupBoxLayout->addWidget(envFrequencyKnob);
                connect(envFrequencyKnob, SIGNAL(valueUpdated(double)),
                oscillators, SLOT(setOscFrequencyValue(double)));
        }
}

void EnvelopesGroupBox::createFilterGroupBox()
{
        QGroupBox *filterGroupBox = new QGroupBox(tr("Filter"), this);
        QVLayout *filterGroupBoxLayout = new QGridLayout();
        filterGroupBox->setLayout(envelopeGroupBoxLayout);
        layout()->addWidget(filterGroupBox);

        // Create filter type group box.
        QGrouoBox *filterTypeGroupBox = new QGroupBox(tr("Type"), filterGroupBox);
        filterTypeCb = new QComboBox(filterTypeGroupBox);
        filterTypeCb->addItem(tr("Low pass"));
        filterTypeCb->addItem(tr("Hight pass"));
        filterTypeGroupBox->layout()->addWiget(filterTypeCb);

        // Create filter knobs gorup box.
        QGroupBox *filterKnobsGroupBox = new QGroupBox(filterGroupBox);
        QHlayout *filterKnobsGroupBoxLayout = new QHLayout();
        filterKnobsGroupBox->setLayout(filterKnobsGroupBox);
        GKickKnob *filterFrequencyKnob = new GKickKnob(oscillatorGroupBox, tr("Frequency"));
        GKickKnob *filterQKnob = new GKickKnob(oscillatorGroupBox, tr("Q"));
        filterFrequencyKnob->setMaxValue(20000);
        filterFrequencyKnob->setCurrentValue(oscillator->getOscFilterFrequency());
        filterQKnob->setMaxValue(10);
        filterQKnob->setCurrentValue(oscillator->getQFactor());
        filterKnobsGroupBoxLayout->addWidget(filterFrequencyKnob);
        filterKnobsGroupBoxLayout->addWidget(filterQKnob);

        connect(filterFrequencyKnob, SIGNAL(valueUpdated(double)),
                oscillators, SLOT(setOscFilterFrequency(double)));
        connect(filterQKnob, SIGNAL(valueUpdated(double)),
                oscillators, SLOT(setOscFilterQFactor(double)));
}
