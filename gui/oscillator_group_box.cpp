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
#include "geonkick_button.h"
#include "gkick_knob.h"
#include "geonkick_label.h"

#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

OscillatorGroupBox::OscillatorGroupBox(GeonkickWidget *parent, std::shared_ptr<GKickOscillator> &osc)
          : GeonkickGroupBox(parent),
            oscillator(osc),
            waveFunctionCb(nullptr),
            filterTypeCb(nullptr),
            sineButton(nullptr),
            squareButton(nullptr),
            triangleButton(nullptr),
            sawtoothButton(nullptr)
{
        GeonkickLabel *label = getGroupBoxLabel();
        if (oscillator->getType() == GKickOscillator::OSC_1) {
                label->setImage(QPixmap("./themes/geontime/osc1_groupbox_label.png"));
        } else if (oscillator->getType() == GKickOscillator::OSC_2) {
                label->setImage(QPixmap("./themes/geontime/osc2_groupbox_label.png"));
        } else {
                label->setImage(QPixmap("./themes/geontime/noise_groupbox_label.png"));
        }

        if (oscillator->getType() != GKickOscillator::OSC_NOISE) {
                createWaveFunctionGroupBox();
        }
        createEvelopeGroupBox();
        createFilterGroupBox();
}

OscillatorGroupBox::~OscillatorGroupBox()
{
}

void OscillatorGroupBox::createWaveFunctionGroupBox()
{
        auto waveFunctionHBox = new GeonkickWidget(this);
        QPixmap pixmap("./themes/geontime/wf_bk_hbox.png");
        waveFunctionHBox->setFixedSize(pixmap.size().width(), pixmap.size().height());
        waveFunctionHBox->setBackgroundImage(pixmap);
        layout()->addWidget(waveFunctionHBox);

        sineButton = new GeonkickButton(waveFunctionHBox);
        sineButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_triangle.png"));
        sineButton->setPressedImage(QPixmap("./themes/geontime/wave_button_triangle_active.png"));
        sineButton->move((waveFunctionHBox->width() / 2 - sineButton->width()) / 2, 22);
        connect(sineButton, SIGNAL(toggled(bool)), this, SLOT(setSineWave(bool)));

        squareButton = new GeonkickButton(waveFunctionHBox);
        squareButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_triangle.png"));
        squareButton->setPressedImage(QPixmap("./themes/geontime/wave_button_triangle_active.png"));
        squareButton->move((waveFunctionHBox->width() / 2 - squareButton->width()) / 2, 21 + squareButton->height());
        connect(squareButton, SIGNAL(toggled(bool)), this, SLOT(setSquareWave(bool)));

        triangleButton = new GeonkickButton(waveFunctionHBox);
        triangleButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_triangle.png"));
        triangleButton->setPressedImage(QPixmap("./themes/geontime/wave_button_triangle_active.png"));
        triangleButton->move(waveFunctionHBox->width() / 2 + (waveFunctionHBox->width() / 2 - triangleButton->width()) / 2, 22);
        connect(triangleButton, SIGNAL(toggled(bool)), this, SLOT(setTriangleWave(bool)));

        sawtoothButton = new GeonkickButton(waveFunctionHBox);
        sawtoothButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_triangle.png"));
        sawtoothButton->move(waveFunctionHBox->width() / 2 +  (waveFunctionHBox->width() / 2 - sawtoothButton->width()) / 2, 21 + sawtoothButton->height());
        sawtoothButton->setPressedImage(QPixmap("./themes/geontime/wave_button_triangle_active.png"));
        connect(sawtoothButton, SIGNAL(toggled(bool)), this, SLOT(setSawtoothWave(bool)));
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
        GeonkickWidget *filterEnvelopeBox = new GeonkickWidget(this);
        filterEnvelopeBox->setFixedSize(224, 125);
        filterEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_filter.png"));
        layout()->addWidget(filterEnvelopeBox);

        GKickKnob *kickFrequencyKnob = new GKickKnob(filterEnvelopeBox);
        kickFrequencyKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickFrequencyKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickFrequencyKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));

        GKickKnob *kickQFactorKnob = new GKickKnob(filterEnvelopeBox);
        QPixmap pixmap("./themes/geontime/knob_bk_50x50.png");
        int w = pixmap.size().width();
        int h = pixmap.size().height();
        kickQFactorKnob->setGeometry(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4, w, h);
        kickQFactorKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_50x50.png"));
        kickQFactorKnob->setKnobImage(QPixmap("./themes/geontime/knob_50x50.png"));
        //        kickQFactorKnob->setMaxValue(kickApi->getMaxLength());
        //kickQFactorKnob->setCurrentValue(kickApi->getKickLength());

        auto filterType = new GeonkickWidget(filterEnvelopeBox);
        w = 80;
        h = 25;
        filterType->setGeometry(224 / 2 + (224 / 2 - w) / 2, 112 - 20, w, h);
        filterType->setBackgroundImage(QPixmap("./themes/geontime/filter_type_hp.png"));
}

void OscillatorGroupBox::setSineWave(bool pressed)
{
        if (pressed) {
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
        }
}

void OscillatorGroupBox::setSquareWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
        }
}

void OscillatorGroupBox::setTriangleWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                sawtoothButton->setPressed(false);
        }
}

void OscillatorGroupBox::setSawtoothWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
        }
}
