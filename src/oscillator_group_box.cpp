/**
 * File name: oscillator_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

#include "oscillator_group_box.h"
#include "oscillator.h"
#include "geonkick_widget.h"
#include "geonkick_button.h"
#include "knob.h"
#include "geonkick_checkbox.h"

extern const unsigend char *rk_osc1_groupbox_label_png[];
extern const unsigend char *rk_osc2_groupbox_label_png[];
extern const unsigend char *rk_noise_groupbox_label_png[];
extern const unsigend char *rk_checkbox_checked_png[];
extern const unsigend char *rk_checkbox_unchecked_png[];
extern const unsigned char *rk_wave_button_sine_png[];
extern const unsigned char *rk_wave_button_sine_active_png[];
extern const unsigned char *rk_wave_button_square_png[];
extern const unsigned char *rk_wave_button_square_active_png[];
extern const unsigned char *rk_wave_button_triangle_png[];
extern const unsigned char *rk_wave_button_triangle_active_png[];
extern const unsigned char *rk_wave_button_sawtooth_png[];
extern const unsigned char *rk_wave_button_sawtooth_active_png[];
extern const unsigned char *rk_hboxbk_noise_env_png[];
extern const unsigned char *rk_hboxbk_osc_env_png[];
extern const unsigned char *rk_knob_bk_image_png[];
extern const unsigned char *rk_knob_png;
extern const unsigned char *rk_noise_type_white_png[];
extern const unsigned char *rk_noise_type_white_active_png[];
extern const unsigned char *rk_noise_type_brownian_png[];
extern const unsigned char *rk_noise_type_brownian_active_png[];
extern const unsigned char *rk_knob_bk_image_png[];
extern const unsigned char *rk_knob_png[];

void OscillatorGroupBox::createFilterGroupBox()
{
        auto pixmap = RkImage(rk_hboxbk_filter_png);
        filterCheckbox->setCheckedImage(":/checkbox_checked_png);
        filterCheckbox->setUncheckedImage(":/checkbox_unchecked_png);
        kickFrequencyKnob->setBackgroundImage(RkImage(rk_knob_bk_image_png));
        kickFrequencyKnob->setKnobImage(RkImage(rk_knob_png));
        pixmap = RkImage(rk_knob_bk_50x50_png);
        kickQFactorKnob->setBackgroundImage(RkImage(rk_knob_bk_50x50_png));
        kickQFactorKnob->setKnobImage(RkImage(rk_knob_50x50_png));
        filterType->setPressedImage(RkImage(rk_filter_type_hp_png));
        filterType->setUnpressedImage(RkImage(rk_filter_type_lp_png));


OscillatorGroupBox::OscillatorGroupBox(GeonkickWidget *parent, Oscillator *osc)
          : GeonkickGroupBox{parent},
            oscillator{osc},
            oscillatorCheckbox{nullptr},
            filterCheckbox{nullptr},
            sineButton{nullptr},
            squareButton{nullptr},
            triangleButton{nullptr},
            sawtoothButton{nullptr},
            noiseWhiteButton{nullptr},
            noiseBrownianButton{nullptr},
            filterType{nullptr},
            kickQFactorKnob{nullptr},
            kickFrequencyKnob{nullptr},
            envelopeAmplitudeKnob{nullptr},
            frequencyAmplitudeKnob{nullptr},
            filterTypeIsChecked{false}
{
        oscillatorCheckbox = new GeonkickCheckbox(this);
        RK_ACT_BIND(oscillatorCheckbox, stateUpdated, RK_ARGS(bool b), oscillator, enable(b));
        if (oscillator->type() == Oscillator::Type::Oscillator1)
                oscillatorCheckbox->setCheckboxLabelImage(RkImage(74, 11, rk_osc1_groupbox_label_png));
        else if (oscillator->type() == Oscillator::Type::Oscillator2)
                oscillatorCheckbox->setCheckboxLabelImage(RkImage(76, 11, rk_osc2_groupbox_label_png));
        else
                oscillatorCheckbox->setCheckboxLabelImage(RkImage(37, 11, rk_noise_groupbox_label_png));
        oscillatorCheckbox->setCheckedImage(RkImage(12, 12, rk_checkbox_checked_png));
        oscillatorCheckbox->setUncheckedImage(RkImage(12, 12, rk_checkbox_unchecked_png));
        oscillatorCheckbox->setPadding(10, 0, 0, 0);
        setGroupBoxLabel(oscillatorCheckbox, Qt::AlignLeft);

        if (oscillator->type() != Oscillator::Type::Noise)
                createWaveFunctionGroupBox();
        createEvelopeGroupBox();
        createFilterGroupBox();
        setPadding(0, 0, 8, 0);
        update();
}

OscillatorGroupBox::~OscillatorGroupBox()
{
}

void OscillatorGroupBox::createWaveFunctionGroupBox()
{
        auto waveFunctionHBox = new GeonkickWidget(this);
        RkImage img(224, 85, rk_wf_bk_hbox_png);
        waveFunctionHBox->setFixedSize(img.width(), img.height());
        waveFunctionHBox->setBackgroundImage(img);
        addWidget(waveFunctionHBox);
        setWidgetAlignment(waveFunctionHBox, Qt::AlignTop);

        sineButton = new GeonkickButton(waveFunctionHBox);
        sineButton->setUnpressedImage(RkImage(rk_wave_button_sine_png));
        sineButton->setPressedImage(RkImage(rk_wave_button_sine_active_png));
        sineButton->move((waveFunctionHBox->width() / 2 - sineButton->width()) / 2, 22);
        RK_ACT_BIND(sineButton, SIGNAL(toggled(bool)), this, SLOT(setSineWave(bool)));

        squareButton = new GeonkickButton(waveFunctionHBox);
        squareButton->setUnpressedImage(RkImage(rk_wave_button_square_png));
        squareButton->setPressedImage(RkImage(rk_wave_button_square_active_png));
        squareButton->move((waveFunctionHBox->width() / 2 - squareButton->width()) / 2, 21 + squareButton->height());
        RK_ACT_BIND(squareButton, SIGNAL(toggled(bool)), this, SLOT(setSquareWave(bool)));

        triangleButton = new GeonkickButton(waveFunctionHBox);
        triangleButton->setUnpressedImage(RkImage(rk_wave_button_triangle_png));
        triangleButton->setPressedImage(RkImage(rk_wave_button_triangle_active_png));
        triangleButton->move(waveFunctionHBox->width() / 2 + (waveFunctionHBox->width() / 2 - triangleButton->width()) / 2, 22);
        RK_ACT_BIND(triangleButton, SIGNAL(toggled(bool)), this, SLOT(setTriangleWave(bool)));

        sawtoothButton = new GeonkickButton(waveFunctionHBox);
        sawtoothButton->setUnpressedImage(RkImage(rk_wave_button_sawtooth_png));
        sawtoothButton->move(waveFunctionHBox->width() / 2 +  (waveFunctionHBox->width() / 2 - sawtoothButton->width()) / 2, 21 + sawtoothButton->height());
        sawtoothButton->setPressedImage(RkImage(rk_wave_button_sawtooth_active_png));
        RK_ACT_BIND(sawtoothButton, SIGNAL(toggled(bool)), this, SLOT(setSawtoothWave(bool)));
}

void OscillatorGroupBox::createEvelopeGroupBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        if (oscillator->type() == Oscillator::Type::Noise)
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(rk_hboxbk_noise_env_png));
        else
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(rk_hboxbk_osc_env_png));
        addWidget(amplitudeEnvelopeBox);
        setWidgetAlignment(amplitudeEnvelopeBox, Qt::AlignTop);

        envelopeAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        envelopeAmplitudeKnob->setRange(0.001, 0.26);
        envelopeAmplitudeKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        envelopeAmplitudeKnob->setBackgroundImage(RkImage(rk_knob_bk_image_png));
        envelopeAmplitudeKnob->setKnobImage(RkImage(rk_knob_png));
        RK_ACT_BIND(envelopeAmplitudeKnob, SIGNAL(valueUpdated(double)),
                oscillator, SLOT(setAmplitude(double)));

        if (oscillator->type() == Oscillator::Type::Noise) {
                auto vLayout = new QVBoxLayout(amplitudeEnvelopeBox);
                noiseWhiteButton = new GeonkickButton(this);
                RK_ACT_BIND(noiseWhiteButton, SIGNAL(toggled(bool)), this, SLOT(setNoiseWhite(bool)));
                vLayout->addWidget(noiseWhiteButton);
                noiseWhiteButton->setUnpressedImage(RkImage(rk_noise_type_white_png));
                noiseWhiteButton->setPressedImage(RkImage(rk_noise_type_white_active_png));
                noiseBrownianButton = new GeonkickButton(this);
                RK_ACT_BIND(noiseBrownianButton, SIGNAL(toggled(bool)), this, SLOT(setNoiseBrownian(bool)));
                vLayout->addWidget(noiseBrownianButton);
                noiseBrownianButton->setUnpressedImage(RkImage(rk_noise_type_brownian_png));
                noiseBrownianButton->setPressedImage(RkImage(rk_noise_type_brownian_active_png));
                vLayout->setContentsMargins(224/2, 20, 0, 12);
                vLayout->setSpacing(0);
        } else {
                frequencyAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
                frequencyAmplitudeKnob->setRangeType(Knob::RangeType::Logarithmic);
                frequencyAmplitudeKnob->setGeometry(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
                frequencyAmplitudeKnob->setBackgroundImage(RkImage(rk_knob_bk_image_png));
                frequencyAmplitudeKnob->setKnobImage(RkImage(rk_knob_png));
                frequencyAmplitudeKnob->setRange(200, 20000);
                RK_ACT_BIND(frequencyAmplitudeKnob, SIGNAL(valueUpdated(double)),
                oscillator, SLOT(setFrequency(double)));
        }
}

void OscillatorGroupBox::createFilterGroupBox()
{
        GeonkickWidget *filterEnvelopeBox = new GeonkickWidget(this);
        auto pixmap = RkImage(rk_hboxbk_filter_png);
        filterEnvelopeBox->setBackgroundImage(pixmap);
        filterEnvelopeBox->setFixedSize(pixmap.size().width(), pixmap.size().height());
        addWidget(filterEnvelopeBox);
        setWidgetAlignment(filterEnvelopeBox, Qt::AlignTop);

        filterCheckbox = new GeonkickCheckbox(filterEnvelopeBox);
        filterCheckbox->setCheckedImage(rk_checkbox_checked_png);
        filterCheckbox->setUncheckedImage(rk_checkbox_unchecked_png);
        filterCheckbox->move(10, 10);
        RK_ACT_BIND(filterCheckbox, SIGNAL(stateUpdated(bool)), oscillator, SLOT(enableFilter(bool)));

        kickFrequencyKnob = new Knob(filterEnvelopeBox);
        kickFrequencyKnob->setRangeType(Knob::RangeType::Logarithmic);
        kickFrequencyKnob->setRange(20, 20000);
        kickFrequencyKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickFrequencyKnob->setBackgroundImage(RkImage(rk_knob_bk_image_png));
        kickFrequencyKnob->setKnobImage(RkImage(rk_knob_png));
        RK_ACT_BIND(kickFrequencyKnob, SIGNAL(valueUpdated(double)), oscillator,  SLOT(setFilterFrequency(double)));

        kickQFactorKnob = new Knob(filterEnvelopeBox);
        kickQFactorKnob->setRange(0.01, 10);
        pixmap = RkImage(50, 50, rk_knob_bk_50x50_png);
        int w = pixmap.size().width();
        int h = pixmap.size().height();
        kickQFactorKnob->setGeometry(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4, w, h);
        kickQFactorKnob->setBackgroundImage(RkImage(50, 50, rk_knob_bk_50x50_png));
        kickQFactorKnob->setKnobImage(RkImage(50, 50, rk_knob_50x50_png));
        RK_ACT_BIND(kickQFactorKnob, SIGNAL(valueUpdated(double)), oscillator, SLOT(setFilterQFactor(double)));

        filterType = new GeonkickButton(filterEnvelopeBox);
        filterType->setCheckable(true);
        RK_ACT_BIND(filterType, SIGNAL(toggled(bool)), this, SLOT(setFilterType(bool)));
        w = 80;
        h = 25;
        filterType->setGeometry(224 / 2 + (224 / 2 - w) / 2, 112 - 20, w, h);
        filterType->setPressedImage(RkImage(rk_filter_type_hp_png));
        filterType->setUnpressedImage(RkImage(rk_filter_type_lp_png));
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

void OscillatorGroupBox::setNoiseWhite(bool pressed)
{
        if (pressed) {
                noiseBrownianButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::NoiseWhite);
        }
}

void OscillatorGroupBox::setNoiseBrownian(bool pressed)
{
        if (pressed) {
                noiseWhiteButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::NoiseBrownian);
        }
}

void OscillatorGroupBox::groupBoxLabelUpdated(bool state)
{
        if (filterTypeIsChecked && state == true)
                filterType->setPressed(true);
        oscillator->enable(state);
}

void OscillatorGroupBox::setFilterType(bool state)
{
        if (state)
                oscillator->setFilterType(Oscillator::FilterType::HighPass);
        else
                oscillator->setFilterType(Oscillator::FilterType::LowPass);
}

void OscillatorGroupBox::update()
{
        if (oscillator->isEnabled())
                oscillatorCheckbox->setChecked(true);
        else
                oscillatorCheckbox->setChecked(false);


        if (oscillator->type() == Oscillator::Type::Noise) {
                noiseWhiteButton->setPressed(false);
                noiseBrownianButton->setPressed(false);
                if (oscillator->function() == Oscillator::FunctionType::NoiseWhite)
                        noiseWhiteButton->setPressed(true);
                else
                        noiseBrownianButton->setPressed(true);
        } else {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
                if (oscillator->function() == Oscillator::FunctionType::Sine)
                        sineButton->setPressed(true);
                else if (oscillator->function() == Oscillator::FunctionType::Square)
                        squareButton->setPressed(true);
                else if (oscillator->function() == Oscillator::FunctionType::Triangle)
                        triangleButton->setPressed(true);
                else if (oscillator->function() == Oscillator::FunctionType::Sawtooth)
                        sawtoothButton->setPressed(true);
        }

        envelopeAmplitudeKnob->setCurrentValue(oscillator->amplitude());
        if (oscillator->type() != Oscillator::Type::Noise)
                frequencyAmplitudeKnob->setCurrentValue(oscillator->frequency());

        filterCheckbox->setChecked(oscillator->isFilterEnabled());
        kickQFactorKnob->setCurrentValue(oscillator->filterQFactor());
        kickFrequencyKnob->setCurrentValue(oscillator->filterFrequency());
        if (oscillator->filter() == Oscillator::FilterType::LowPass)
                filterType->setPressed(false);
        else
                filterType->setPressed(true);
}
