/**
 * File name: oscillator_group_box.cpp
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

#include "oscillator_group_box.h"
#include "gkick_oscillator.h"
#include "geonkick_widget.h"
#include <gkick_knob.h>

#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

OscillatorGroupBox::OscillatorGroupBox(GeonkickWidget *parent, std::shared_ptr<GKickOscillator> &osc)
          : GeonkickGroupBox(osc->name(), parent),
          oscillator(osc),
          waveFunctionCb(NULL),
          filterTypeCb(NULL)
{
        /*setCheckable(true);
        if (oscillator->getType() != GKickOscillator::OSC_NOISE) {
                createWaveFunctionGroupBox();
        }
        createEvelopeGroupBox();
        createFilterGroupBox();
        layout()->setSpacing(2);*/
        createEvelopeGroupBox();
        createEvelopeGroupBox();
}

OscillatorGroupBox::~OscillatorGroupBox()
{
}

void OscillatorGroupBox::createWaveFunctionGroupBox()
{
        /*        QGroupBox *waveFunctionGroupBox = new QGroupBox(tr("Wave function"), this);
        waveFunctionCb = new QComboBox(waveFunctionGroupBox);
        waveFunctionCb->addItem(tr("Sine"));
        waveFunctionCb->addItem(tr("Sqare"));
        waveFunctionCb->addItem(tr("Triangle"));
        waveFunctionCb->addItem(tr("Sawtooth"));
        waveFunctionCb->addItem(tr("Noise"));
        connect(waveFunctionCb, SIGNAL(currentIndexChanged(int)),
                oscillator.get(), SLOT(changeOscFunction(int)));
        QHBoxLayout *glayout = new QHBoxLayout(waveFunctionGroupBox);
        glayout->setSpacing(0);
        glayout->addWidget(waveFunctionCb);
        waveFunctionGroupBox->setLayout(glayout);
        layout()->addWidget(waveFunctionGroupBox);*/
}

void OscillatorGroupBox::createEvelopeGroupBox()
{

        GeonkickWidget *amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        if (oscillator->getType() == GKickOscillator::OSC_NOISE) {
                amplitudeEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_noise_env.png"));
        } else {
                amplitudeEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_osc_env.png"));
        }
        layout()->addWidget(amplitudeEnvelopeBox);

        GKickKnob *kickLengthKnob = new GKickKnob(amplitudeEnvelopeBox);
        kickLengthKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickLengthKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickLengthKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        //kickLengthKnob->setMaxValue(kickApi->getMaxLength());
        //kickLengthKnob->setCurrentValue(kickApi->getKickLength());

        if (oscillator->getType() == GKickOscillator::OSC_NOISE) {
        } else {
                GKickKnob *kickAmplitudeKnob = new GKickKnob(amplitudeEnvelopeBox);
                kickAmplitudeKnob->setGeometry(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
                kickAmplitudeKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
                kickAmplitudeKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
                kickAmplitudeKnob->setMaxValue(1);
        }
        //kickAmplitudeKnob->setCurrentValue(kickApi->getAmplitude());
        //connect(kickAmplitudeKnob, SIGNAL(valueUpdated(double)),
        //        oscillator.get(), SLOT(setOscAmplitudeValue(double)));

        /*        if (oscillator->getType() != GKickOscillator::OSC_NOISE) {
                GKickKnob *kickFrequencyKnob = new GKickKnob(envelopeGroupBox, tr("Frequency"));
                kickFrequencyKnob->setMaxValue(20000);
                kickFrequencyKnob->setCurrentValue(oscillator->getOscFrequencyValue());
                envelopeGroupBoxLayout->addWidget(kickFrequencyKnob);
                connect(kickFrequencyKnob, SIGNAL(valueUpdated(double)),
                        oscillator.get(), SLOT(setOscFrequencyValue(double)));
                        }*/
}

void OscillatorGroupBox::createFilterGroupBox()
{
        /*        QGroupBox *filterGroupBox = new QGroupBox(tr("Filter"), this);
        QVBoxLayout *filterGroupBoxLayout = new QVBoxLayout(filterGroupBox);
        filterGroupBoxLayout->setSpacing(0);
        filterGroupBoxLayout->setContentsMargins(0,0,0,0);
        filterGroupBox->setLayout(filterGroupBoxLayout);
        layout()->addWidget(filterGroupBox);

        // Create filter type group box.
        QGroupBox *filterTypeGroupBox = new QGroupBox(tr("Type"), filterGroupBox);
        QHBoxLayout *filterTypeGroupBoxLayout = new QHBoxLayout(filterTypeGroupBox);
        filterTypeGroupBoxLayout->setSpacing(0);
        filterTypeGroupBoxLayout->setContentsMargins(0,0,0,0);
        filterTypeGroupBox->setLayout(filterTypeGroupBoxLayout);
        filterTypeCb = new QComboBox(filterTypeGroupBox);
        filterTypeCb->addItem(tr("Low pass"));
        filterTypeCb->addItem(tr("Hight pass"));
        filterTypeGroupBoxLayout->addWidget(filterTypeCb);
        filterGroupBoxLayout->addWidget(filterTypeGroupBox);

        // Create filter knobs gorup box.
        QGroupBox *filterKnobsGroupBox = new QGroupBox(filterGroupBox);
        QHBoxLayout *filterKnobsGroupBoxLayout = new QHBoxLayout(filterKnobsGroupBox);
        filterKnobsGroupBox->setLayout(filterKnobsGroupBoxLayout);
        filterKnobsGroupBoxLayout->setContentsMargins(0,0,0,0);
        GKickKnob *filterFrequencyKnob = new GKickKnob(filterKnobsGroupBox, tr("Frequency"));
        GKickKnob *filterQKnob = new GKickKnob(filterKnobsGroupBox, tr("Q"));
        filterFrequencyKnob->setMaxValue(20000);
        filterFrequencyKnob->setCurrentValue(oscillator->getFilterFrequency());
        filterQKnob->setMaxValue(10);
        filterQKnob->setCurrentValue(oscillator->getFilterQFactor());
        filterKnobsGroupBoxLayout->addWidget(filterFrequencyKnob);
        filterKnobsGroupBoxLayout->addWidget(filterQKnob);
        filterGroupBoxLayout->addWidget(filterKnobsGroupBox);

        connect(filterFrequencyKnob, SIGNAL(valueUpdated(double)),
                oscillator.get(), SLOT(setOscFilterFrequencyValue(double)));
        connect(filterQKnob, SIGNAL(valueUpdated(double)),
        oscillator.get(), SLOT(setOscFilterQFactor(double)));*/
}
