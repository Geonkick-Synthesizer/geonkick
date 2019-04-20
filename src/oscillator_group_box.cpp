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
//#include "geonkick_checkbox.h"

extern const unsigned char rk_osc1_groupbox_label_png[];
extern const unsigned char rk_wf_bk_hbox_png[];
extern const unsigned char rk_osc2_groupbox_label_png[];
extern const unsigned char rk_noise_groupbox_label_png[];
extern const unsigned char rk_checkbox_checked_png[];
extern const unsigned char rk_checkbox_unchecked_png[];
extern const unsigned char rk_wave_button_sine_png[];
extern const unsigned char rk_wave_button_sine_active_png[];
extern const unsigned char rk_wave_button_square_png[];
extern const unsigned char rk_wave_button_square_active_png[];
extern const unsigned char rk_wave_button_triangle_png[];
extern const unsigned char rk_wave_button_triangle_active_png[];
extern const unsigned char rk_wave_button_sawtooth_png[];
extern const unsigned char rk_wave_button_sawtooth_active_png[];
extern const unsigned char rk_hboxbk_noise_env_png[];
extern const unsigned char rk_hboxbk_osc_env_png[];
extern const unsigned char rk_knob_bk_image_png[];
extern const unsigned char rk_knob_png[];
extern const unsigned char rk_noise_type_white_png[];
extern const unsigned char rk_noise_type_white_active_png[];
extern const unsigned char rk_noise_type_brownian_png[];
extern const unsigned char rk_noise_type_brownian_active_png[];
extern const unsigned char rk_knob_bk_image_png[];
extern const unsigned char rk_knob_png[];
extern const unsigned char rk_checkbox_checked_png[];
extern const unsigned char rk_checkbox_unchecked_png[];
extern const unsigned char rk_knob_bk_image_png[];
extern const unsigned char rk_knob_png[];
extern const unsigned char rk_knob_bk_50x50_png[];
extern const unsigned char rk_knob_bk_50x50_png[];
extern const unsigned char rk_knob_50x50_png[];
extern const unsigned char rk_filter_type_hp_png[];
extern const unsigned char rk_filter_type_lp_png[];
extern const unsigned char rk_hboxbk_filter_png[];

OscillatorGroupBox::OscillatorGroupBox(GeonkickWidget *parent, Oscillator *osc)
          : GeonkickGroupBox{parent}
           , oscillator{osc}
           , oscillatorCheckbox{nullptr}
           , filterCheckbox{nullptr}
           , sineButton{nullptr}
           , squareButton{nullptr}
           , triangleButton{nullptr}
           , sawtoothButton{nullptr}
           , noiseWhiteButton{nullptr}
           , noiseBrownianButton{nullptr}
           , filterType{nullptr}
           , kickQFactorKnob{nullptr}
           , kickFrequencyKnob{nullptr}
           , envelopeAmplitudeKnob{nullptr}
           , frequencyAmplitudeKnob{nullptr}
           , filterTypeIsChecked{false}
{
        setFixedSize(224, 380);
        //        oscillatorCheckbox = new GeonkickCheckbox(this);
        //        oscillatorCheckbox->setPosition(0, 0);
        //        RK_ACT_BIND(oscillatorCheckbox, stateUpdated, RK_ACT_ARGS(bool b), oscillator, enable(b));
        //        if (oscillator->type() == Oscillator::Type::Oscillator1)
        //        oscillatorCheckbox->setCheckboxLabelImage(RkImage(74, 11, rk_osc1_groupbox_label_png));
        //else if (oscillator->type() == Oscillator::Type::Oscillator2)
        //        oscillatorCheckbox->setCheckboxLabelImage(RkImage(76, 11, rk_osc2_groupbox_label_png));
        //else
        //        oscillatorCheckbox->setCheckboxLabelImage(RkImage(37, 11, rk_noise_groupbox_label_png));
        //oscillatorCheckbox->setCheckedImage(RkImage(12, 12, rk_checkbox_checked_png));
        //oscillatorCheckbox->setUncheckedImage(RkImage(12, 12, rk_checkbox_unchecked_png));
        //setGroupBoxLabel(oscillatorCheckbox);
        //oscillatorCheckbox->show();

        if (oscillator->type() != Oscillator::Type::Noise)
                createWaveFunctionGroupBox();
        createEvelopeGroupBox();
        createFilterGroupBox();
        update();
}

OscillatorGroupBox::~OscillatorGroupBox()
{
}

void OscillatorGroupBox::createWaveFunctionGroupBox()
{
        auto waveFunctionHBox = new GeonkickWidget(this);
        waveFunctionHBox->setFixedSize(224, 85);
        waveFunctionHBox->setPosition(0, 11);
        waveFunctionHBox->setBackgroundImage(RkImage(224, 85, rk_wf_bk_hbox_png));
        waveFunctionHBox->show();

        sineButton = new GeonkickButton(waveFunctionHBox);
        sineButton->setSize(90, 30);
        sineButton->setPosition((waveFunctionHBox->width() / 2 - sineButton->width()) / 2, 22);
        sineButton->setUnpressedImage(RkImage(90, 30, rk_wave_button_sine_png));
        sineButton->setPressedImage(RkImage(90, 30, rk_wave_button_sine_active_png));
        RK_ACT_BIND(sineButton, toggled, RK_ACT_ARGS(bool b), this, setSineWave(b));
        sineButton->show();

        squareButton = new GeonkickButton(waveFunctionHBox);
        squareButton->setFixedSize(90, 30);
        squareButton->setPosition((waveFunctionHBox->width() / 2 - squareButton->width()) / 2,
                                  21 + squareButton->height());
        squareButton->setUnpressedImage(RkImage(90, 30, rk_wave_button_square_png));
        squareButton->setPressedImage(RkImage(90, 30, rk_wave_button_square_active_png));
        RK_ACT_BIND(squareButton, toggled, RK_ACT_ARGS(bool b), this, setSquareWave(b));
        squareButton->show();

        triangleButton = new GeonkickButton(waveFunctionHBox);
        triangleButton->setSize(90, 30);
        triangleButton->setPosition(waveFunctionHBox->width() / 2 + (waveFunctionHBox->width() / 2 - triangleButton->width()) / 2, 22);
        triangleButton->setUnpressedImage(RkImage(90, 30, rk_wave_button_triangle_png));
        triangleButton->setPressedImage(RkImage(90, 30, rk_wave_button_triangle_active_png));
        RK_ACT_BIND(triangleButton, toggled, RK_ACT_ARGS(bool b), this, setTriangleWave(b));
        triangleButton->show();

        sawtoothButton = new GeonkickButton(waveFunctionHBox);
        sawtoothButton->setSize(90, 30);
        sawtoothButton->setPosition(waveFunctionHBox->width() / 2 + (waveFunctionHBox->width() / 2 - sawtoothButton->width()) / 2,
                                    21 + sawtoothButton->height());
        sawtoothButton->setUnpressedImage(RkImage(90, 30, rk_wave_button_sawtooth_png));
        sawtoothButton->setPressedImage(RkImage(90, 30, rk_wave_button_sawtooth_active_png));
        RK_ACT_BIND(sawtoothButton, toggled, RK_ACT_ARGS(bool b), this, setSawtoothWave(b));
        sawtoothButton->show();
}

void OscillatorGroupBox::createEvelopeGroupBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setPosition(0, 12);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        if (oscillator->type() == Oscillator::Type::Noise)
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, rk_hboxbk_noise_env_png));
        else
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, rk_hboxbk_osc_env_png));

        envelopeAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        envelopeAmplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2);
        envelopeAmplitudeKnob->setFixedSize(80, 80);
        envelopeAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
        envelopeAmplitudeKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
        envelopeAmplitudeKnob->setRange(0.01, 1.0);
        envelopeAmplitudeKnob->show();
        RK_ACT_BIND(envelopeAmplitudeKnob, valueUpdated, RK_ACT_ARGS(double val), oscillator, setAmplitude(val));

        if (oscillator->type() == Oscillator::Type::Noise) {
                noiseWhiteButton = new GeonkickButton(this);
                noiseWhiteButton->setPosition((224 / 2 - 80) / 2 + 80, 0);
                noiseWhiteButton->setFixedSize(90, 30);
                noiseWhiteButton->setUnpressedImage(RkImage(90, 30, rk_noise_type_white_png));
                noiseWhiteButton->setPressedImage(RkImage(90, 30, rk_noise_type_white_active_png));
                RK_ACT_BIND(noiseWhiteButton, toggled, RK_ACT_ARGS(bool b), this, setNoiseWhite(b));
                noiseWhiteButton->show();
                noiseBrownianButton = new GeonkickButton(this);
                noiseWhiteButton->setPosition((224 / 2 - 80) / 2 + 80, 40);
                noiseBrownianButton->setFixedSize(90, 30);
                RK_ACT_BIND(noiseBrownianButton, toggled, RK_ACT_ARGS(bool b), this, setNoiseBrownian(b));
                noiseBrownianButton->setUnpressedImage(RkImage(90, 30, rk_noise_type_brownian_png));
                noiseBrownianButton->setPressedImage(RkImage(90, 30, rk_noise_type_brownian_active_png));
                noiseBrownianButton->show();
        } else {
                frequencyAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
                frequencyAmplitudeKnob->setRangeType(Knob::RangeType::Logarithmic);
                frequencyAmplitudeKnob->setSize(80, 80);
                frequencyAmplitudeKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2);
                frequencyAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
                frequencyAmplitudeKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
                frequencyAmplitudeKnob->setRange(200, 20000);
                RK_ACT_BIND(frequencyAmplitudeKnob, valueUpdated, RK_ACT_ARGS(double val),
                            oscillator, setFrequency(val));
        }
}

void OscillatorGroupBox::createFilterGroupBox()
{
        auto filterEnvelopeBox = new GeonkickWidget(this);
        filterEnvelopeBox->setPosition(0, 130);
        filterEnvelopeBox->setBackgroundImage(RkImage(224, 125, rk_hboxbk_filter_png));
        filterEnvelopeBox->setFixedSize(224, 125);
        filterEnvelopeBox->show();

        //        filterCheckbox = new GeonkickCheckbox(filterEnvelopeBox);
        //        filterCheckbox->setPosition(10, 10);
        //        filterCheckbox->setCheckedImage(12, 12, rk_checkbox_checked_png);
        //        filterCheckbox->setUncheckedImage(12, 12, rk_checkbox_unchecked_png);
        //        filterCheckbox->show();
        //        RK_ACT_BIND(filterCheckbox, stateUpdated, RK_ACT_ARGS(bool b), geonkickApi, enableKickFilter(b));

        kickFrequencyKnob = new Knob(filterEnvelopeBox);
        kickFrequencyKnob->setRangeType(Knob::RangeType::Logarithmic);
        kickFrequencyKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2);
        kickFrequencyKnob->setFixedSize(80, 80);
        kickFrequencyKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
        kickFrequencyKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
        kickFrequencyKnob->setRange(200, 20000);
        kickFrequencyKnob->show();
        //        RK_ACT_BIND(kickFrequencyKnob, valueUpdated, RK_ACT_ARGS(double val), geonkickApi, setKickFilterFrequency(val));

        kickQFactorKnob = new Knob(filterEnvelopeBox);
        int w = 50;
        int h = 50;
        kickQFactorKnob->setPosition(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4);
        kickQFactorKnob->setFixedSize(w, h);
        kickQFactorKnob->setKnobBackgroundImage(RkImage(50, 50, rk_knob_bk_50x50_png));
        kickQFactorKnob->setKnobImage(RkImage(50, 50, rk_knob_50x50_png));
        kickQFactorKnob->setRange(0.01, 10);
        kickQFactorKnob->show();
        //        RK_ACT_BIND(kickQFactorKnob, valueUpdated, RK_ACT_ARGS(double val), this, setOscillatorFilterQFactor(val));

        filterType = new GeonkickButton(filterEnvelopeBox);
        filterType->setCheckable(true);
        RK_ACT_BIND(filterType, toggled, RK_ACT_ARGS(bool state), this, setFilterType(state));
        w = 80;
        h = 25;
        filterType->setPosition(224 / 2 + (224 / 2 - w) / 2, 112 - 200);
        filterType->setFixedSize(w, h);
        filterType->setPressedImage(RkImage(80, 25, rk_filter_type_hp_png));
        filterType->setUnpressedImage(RkImage(80, 25, rk_filter_type_lp_png));
        filterType->show();
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
        //        if (oscillator->isEnabled())
                //                oscillatorCheckbox->setChecked(true);
        //        else
                //                oscillatorCheckbox->setChecked(false);


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

        //        filterCheckbox->setChecked(oscillator->isFilterEnabled());
        kickQFactorKnob->setCurrentValue(oscillator->filterQFactor());
        kickFrequencyKnob->setCurrentValue(oscillator->filterFrequency());
        if (oscillator->filter() == Oscillator::FilterType::LowPass)
                filterType->setPressed(false);
        else
                filterType->setPressed(true);
}
