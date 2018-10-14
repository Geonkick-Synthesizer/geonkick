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
#include "oscillator.h"
#include "geonkick_widget.h"
#include "geonkick_button.h"
#include "knob.h"
#include "geonkick_checkbox.h"

#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

OscillatorGroupBox::OscillatorGroupBox(GeonkickWidget *parent, Oscillator *osc)
          : GeonkickGroupBox(parent),
            oscillator(osc),
            filterCheckbox(nullptr),
            sineButton(nullptr),
            squareButton(nullptr),
            triangleButton(nullptr),
            sawtoothButton(nullptr),
            filterTypeIsChecked(false),
            filterType(nullptr)
{
        auto checkbox = new GeonkickCheckbox(this);
        connect(checkbox, SIGNAL(stateUpdated(bool)), osc, SLOT(enable(bool)));
        if (oscillator->type() == Oscillator::Type::Oscillator1) {
                checkbox->setCheckboxLabelImage("./themes/geontime/osc1_groupbox_label.png");
        } else if (oscillator->type() == Oscillator::Type::Oscillator2) {
                checkbox->setCheckboxLabelImage("./themes/geontime/osc2_groupbox_label.png");
        } else {
                checkbox->setCheckboxLabelImage("./themes/geontime/noise_groupbox_label.png");
        }
        checkbox->setCheckedImage("./themes/geontime/checkbox_checked.png");
        checkbox->setUncheckedImage("./themes/geontime/checkbox_unchecked.png");
        checkbox->setPadding(10, 0, 0, 0);
        if (oscillator->isEnabled()) {
                checkbox->setChecked(true);
        }
        setGroupBoxLabel(checkbox, Qt::AlignLeft);

        if (oscillator->type() != Oscillator::Type::Noise) {
                createWaveFunctionGroupBox();
        }
        createEvelopeGroupBox();
        createFilterGroupBox();
        setPadding(0, 0, 8, 0);
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
        addWidget(waveFunctionHBox);
        setWidgetAlignment(waveFunctionHBox, Qt::AlignTop);

        sineButton = new GeonkickButton(waveFunctionHBox);
        if (oscillator->function() == Oscillator::FunctionType::Sine) {
                sineButton->setPressed(true);
        }
        sineButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_sine.png"));
        sineButton->setPressedImage(QPixmap("./themes/geontime/wave_button_sine_active.png"));
        sineButton->move((waveFunctionHBox->width() / 2 - sineButton->width()) / 2, 22);
        connect(sineButton, SIGNAL(toggled(bool)), this, SLOT(setSineWave(bool)));

        squareButton = new GeonkickButton(waveFunctionHBox);
        if (oscillator->function() == Oscillator::FunctionType::Square) {
                squareButton->setPressed(true);
        }

        squareButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_square.png"));
        squareButton->setPressedImage(QPixmap("./themes/geontime/wave_button_square_active.png"));
        squareButton->move((waveFunctionHBox->width() / 2 - squareButton->width()) / 2, 21 + squareButton->height());
        connect(squareButton, SIGNAL(toggled(bool)), this, SLOT(setSquareWave(bool)));

        triangleButton = new GeonkickButton(waveFunctionHBox);
        if (oscillator->function() == Oscillator::FunctionType::Triangle) {
                triangleButton->setPressed(true);
        }
        triangleButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_triangle.png"));
        triangleButton->setPressedImage(QPixmap("./themes/geontime/wave_button_triangle_active.png"));
        triangleButton->move(waveFunctionHBox->width() / 2 + (waveFunctionHBox->width() / 2 - triangleButton->width()) / 2, 22);
        connect(triangleButton, SIGNAL(toggled(bool)), this, SLOT(setTriangleWave(bool)));

        sawtoothButton = new GeonkickButton(waveFunctionHBox);
        if (oscillator->function() == Oscillator::FunctionType::Sawtooth) {
                sawtoothButton->setPressed(true);
        }
        sawtoothButton->setUnpressedImage(QPixmap("./themes/geontime/wave_button_sawtooth.png"));
        sawtoothButton->move(waveFunctionHBox->width() / 2 +  (waveFunctionHBox->width() / 2 - sawtoothButton->width()) / 2, 21 + sawtoothButton->height());
        sawtoothButton->setPressedImage(QPixmap("./themes/geontime/wave_button_sawtooth_active.png"));
        connect(sawtoothButton, SIGNAL(toggled(bool)), this, SLOT(setSawtoothWave(bool)));
}

void OscillatorGroupBox::createEvelopeGroupBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        if (oscillator->type() == Oscillator::Type::Noise) {
                amplitudeEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_noise_env.png"));
        } else {
                amplitudeEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_osc_env.png"));
        }
        addWidget(amplitudeEnvelopeBox);
        setWidgetAlignment(amplitudeEnvelopeBox, Qt::AlignTop);

        auto envelopeAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        envelopeAmplitudeKnob->setRange(0.001, 0.1);
        envelopeAmplitudeKnob->setCurrentValue(oscillator->amplitude());
        envelopeAmplitudeKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        envelopeAmplitudeKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        envelopeAmplitudeKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        connect(envelopeAmplitudeKnob, SIGNAL(valueUpdated(double)),
                oscillator, SLOT(setAmplitude(double)));

        if (oscillator->type() == Oscillator::Type::Noise) {
                auto vLayout = new QVBoxLayout(amplitudeEnvelopeBox);
                auto noiseWhiteButton = new GeonkickButton(this);
                vLayout->addWidget(noiseWhiteButton);
                noiseWhiteButton->setUnpressedImage(QPixmap("./themes/geontime/noise_type_white.png"));
                noiseWhiteButton->setPressedImage(QPixmap("./themes/geontime/noise_type_white_active.png"));
                auto noisePinkButton = new GeonkickButton(this);
                vLayout->addWidget(noisePinkButton);
                noisePinkButton->setUnpressedImage(QPixmap("./themes/geontime/noise_type_pink.png"));
                noisePinkButton->setPressedImage(QPixmap("./themes/geontime/noise_type_pink_active.png"));
                auto noiseBrownianButton = new GeonkickButton(this);
                vLayout->addWidget(noiseBrownianButton);
                noiseBrownianButton->setUnpressedImage(QPixmap("./themes/geontime/noise_type_brownian.png"));
                noiseBrownianButton->setPressedImage(QPixmap("./themes/geontime/noise_type_brownian_active.png"));
                vLayout->setContentsMargins(224/2, 10, 0, 12);
                vLayout->setSpacing(0);
        } else {
                Knob *frequencyAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
                frequencyAmplitudeKnob->setGeometry(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
                frequencyAmplitudeKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
                frequencyAmplitudeKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
                frequencyAmplitudeKnob->setRange(200, 20000);
                frequencyAmplitudeKnob->setCurrentValue(oscillator->frequency());
                connect(frequencyAmplitudeKnob, SIGNAL(valueUpdated(double)),
                oscillator, SLOT(setFrequency(double)));
        }
}

void OscillatorGroupBox::createFilterGroupBox()
{
        GeonkickWidget *filterEnvelopeBox = new GeonkickWidget(this);
        auto pixmap = QPixmap("./themes/geontime/hboxbk_filter.png");
        filterEnvelopeBox->setBackgroundImage(pixmap);
        filterEnvelopeBox->setFixedSize(pixmap.size().width(), pixmap.size().height());
        addWidget(filterEnvelopeBox);
        setWidgetAlignment(filterEnvelopeBox, Qt::AlignTop);

        filterCheckbox = new GeonkickCheckbox(filterEnvelopeBox);
        filterCheckbox->setCheckedImage("./themes/geontime/checkbox_checked.png");
        filterCheckbox->setUncheckedImage("./themes/geontime/checkbox_unchecked.png");
        filterCheckbox->move(10, 10);
        connect(filterCheckbox, SIGNAL(stateUpdated(bool)), oscillator, SLOT(enableFilter(bool)));

        Knob *kickFrequencyKnob = new Knob(filterEnvelopeBox);
        kickFrequencyKnob->setRange(20, 20000);
        kickFrequencyKnob->setCurrentValue(oscillator->filterFrequency());
        kickFrequencyKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickFrequencyKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickFrequencyKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        connect(kickFrequencyKnob, SIGNAL(valueUpdated(double)), oscillator,  SLOT(setFilterFrequency(double)));

        Knob *kickQFactorKnob = new Knob(filterEnvelopeBox);
        kickQFactorKnob->setRange(0.01, 10);
        kickQFactorKnob->setCurrentValue(oscillator->filterQFactor());
        pixmap = QPixmap("./themes/geontime/knob_bk_50x50.png");
        int w = pixmap.size().width();
        int h = pixmap.size().height();
        kickQFactorKnob->setGeometry(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4, w, h);
        kickQFactorKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_50x50.png"));
        kickQFactorKnob->setKnobImage(QPixmap("./themes/geontime/knob_50x50.png"));
        connect(kickQFactorKnob, SIGNAL(valueUpdated(double)), oscillator, SLOT(setFilterQFactor(double)));

        filterType = new GeonkickButton(filterEnvelopeBox);
        filterType->setCheckable(true);
        if (oscillator->filter() == Oscillator::FilterType::LowPass) {
                filterType->setPressed(false);
        } else {
                filterType->setPressed(true);
        }
        connect(filterType, SIGNAL(toggled(bool)), this, SLOT(setFilterType(bool)));
        w = 80;
        h = 25;
        filterType->setGeometry(224 / 2 + (224 / 2 - w) / 2, 112 - 20, w, h);
        filterType->setPressedImage(QPixmap("./themes/geontime/filter_type_hp.png"));
        filterType->setUnpressedImage(QPixmap("./themes/geontime/filter_type_lp.png"));
}

void OscillatorGroupBox::setSineWave(bool pressed)
{
        if (pressed) {
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Sine);
        }
}

void OscillatorGroupBox::setSquareWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Square);
        }
}

void OscillatorGroupBox::setTriangleWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                sawtoothButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Triangle);
        }
}

void OscillatorGroupBox::setSawtoothWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Sawtooth);
        }
}


void OscillatorGroupBox::groupBoxLabelUpdated(bool state)
{
        if (filterTypeIsChecked && state == true) {
                filterType->setPressed(true);
        }
        oscillator->enable(state);
}

void OscillatorGroupBox::setFilterType(bool state)
{
        if (state) {
                oscillator->setFilterType(Oscillator::FilterType::LowPass);
        } else {
                oscillator->setFilterType(Oscillator::FilterType::HighPass);
        }
}
