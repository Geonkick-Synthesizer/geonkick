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
#include "geonkick_slider.h"
#include "filter.h"
#include "file_dialog.h"

#include <RkLabel.h>

extern const unsigned char rk_osc1_radio_button_png[];
extern const unsigned char rk_osc1_radio_button_active_png[];
extern const unsigned char rk_osc2_radio_button_png[];
extern const unsigned char rk_osc2_radio_button_active_png[];
extern const unsigned char rk_noise_radio_button_png[];
extern const unsigned char rk_noise_radio_button_active_png[];
extern const unsigned char rk_fm_radio_button_png[];
extern const unsigned char rk_fm_radio_button_active_png[];
extern const unsigned char rk_wf_bk_hbox_png[];
extern const unsigned char rk_wave_button_sine_png[];
extern const unsigned char rk_wave_button_sine_active_png[];
extern const unsigned char rk_wave_button_square_png[];
extern const unsigned char rk_wave_button_square_active_png[];
extern const unsigned char rk_wave_button_triangle_png[];
extern const unsigned char rk_wave_button_triangle_active_png[];
extern const unsigned char rk_wave_button_sawtooth_png[];
extern const unsigned char rk_phase_label_png[];
extern const unsigned char rk_wave_button_sample_png[];
extern const unsigned char rk_wave_button_sample_active_png[];
extern const unsigned char rk_wave_button_sawtooth_active_png[];
extern const unsigned char rk_button_browse_sample_png[];
extern const unsigned char rk_hboxbk_noise_env_png[];
extern const unsigned char rk_hboxbk_osc_env_png[];
extern const unsigned char rk_knob_png[];
extern const unsigned char rk_noise_type_white_png[];
extern const unsigned char rk_noise_type_white_active_png[];
extern const unsigned char rk_noise_type_brownian_png[];
extern const unsigned char rk_noise_type_brownian_active_png[];
extern const unsigned char rk_knob_bk_image_png[];

OscillatorGroupBox::OscillatorGroupBox(GeonkickWidget *parent, Oscillator *osc)
          : GeonkickGroupBox{parent}
           , oscillator{osc}
           , oscillatorCheckbox{nullptr}
           , fmCheckbox{nullptr}
           , filterBox{nullptr}
           , sineButton{nullptr}
           , squareButton{nullptr}
           , triangleButton{nullptr}
           , sawtoothButton{nullptr}
           , sampleButton{nullptr}
           , sampleBrowseButton{nullptr}
           , phaseSlider{nullptr}
           , noiseWhiteButton{nullptr}
           , noiseBrownianButton{nullptr}
           , envelopeAmplitudeKnob{nullptr}
           , frequencyAmplitudeKnob{nullptr}
           , filterTypeIsChecked{false}
{
        setFixedSize(224, 380);
        oscillatorCheckbox = new GeonkickButton(this);
        oscillatorCheckbox->setCheckable(true);
        oscillatorCheckbox->setPosition(5, 0);
        RK_ACT_BIND(oscillatorCheckbox, toggled, RK_ACT_ARGS(bool b), oscillator, enable(b));

        if (oscillator->type() == Oscillator::Type::Oscillator1) {
                oscillatorCheckbox->setFixedSize(85, 11);
                oscillatorCheckbox->setPressedImage(RkImage(oscillatorCheckbox->size(), rk_osc1_radio_button_active_png));
                oscillatorCheckbox->setUnpressedImage(RkImage(oscillatorCheckbox->size(), rk_osc1_radio_button_png));

                fmCheckbox = new GeonkickButton(this);
                fmCheckbox->setCheckable(true);
                fmCheckbox->setPosition(oscillatorCheckbox->x() + oscillatorCheckbox->width() + 20, 0);
                fmCheckbox->setFixedSize(87, 11);
                fmCheckbox->setPressedImage(RkImage(87, 11, rk_fm_radio_button_active_png));
                fmCheckbox->setUnpressedImage(RkImage(87, 11, rk_fm_radio_button_png));
                RK_ACT_BIND(fmCheckbox, toggled, RK_ACT_ARGS(bool b), oscillator, setAsFm(b));
        } else if (oscillator->type() == Oscillator::Type::Oscillator2) {
                oscillatorCheckbox->setFixedSize(87, 11);
                oscillatorCheckbox->setPressedImage(RkImage(oscillatorCheckbox->size(), rk_osc2_radio_button_active_png));
                oscillatorCheckbox->setUnpressedImage(RkImage(oscillatorCheckbox->size(), rk_osc2_radio_button_png));
        } else {
                oscillatorCheckbox->setFixedSize(48, 11);
                oscillatorCheckbox->setPressedImage(RkImage(oscillatorCheckbox->size(), rk_noise_radio_button_active_png));
                oscillatorCheckbox->setUnpressedImage(RkImage(oscillatorCheckbox->size(), rk_noise_radio_button_png));
        }

        if (oscillator->type() != Oscillator::Type::Noise)
                createWaveFunctionGroupBox();
        createEvelopeGroupBox();
        createFilterGroupBox();
        updateGui();
}

OscillatorGroupBox::~OscillatorGroupBox()
{
}

void OscillatorGroupBox::createWaveFunctionGroupBox()
{
        auto waveFunctionHBox = new GeonkickWidget(this);
        waveFunctionHBox->setBackgroundColor(67, 68, 68);
        waveFunctionHBox->setFixedSize(224, 85);
        waveFunctionHBox->setPosition(0, 18);
        waveFunctionHBox->setBackgroundImage(RkImage(waveFunctionHBox->size(), rk_wf_bk_hbox_png));
        waveFunctionHBox->show();

        sineButton = new GeonkickButton(waveFunctionHBox);
        sineButton->setBackgroundColor(waveFunctionHBox->background());
        sineButton->setFixedSize(67, 14);
        sineButton->setPosition(20, 25);
        sineButton->setUnpressedImage(RkImage(sineButton->size(), rk_wave_button_sine_png));
        sineButton->setPressedImage(RkImage(sineButton->size(), rk_wave_button_sine_active_png));
        RK_ACT_BIND(sineButton, toggled, RK_ACT_ARGS(bool b), this, setSineWave(b));
        sineButton->show();

        squareButton = new GeonkickButton(waveFunctionHBox);
        squareButton->setBackgroundColor(waveFunctionHBox->background());
        squareButton->setFixedSize(67, 14);
        squareButton->setPosition(sineButton->x(),  sineButton->y() + sineButton->height() + 5);
        squareButton->setUnpressedImage(RkImage(squareButton->size(), rk_wave_button_square_png));
        squareButton->setPressedImage(RkImage(squareButton->size(), rk_wave_button_square_active_png));
        RK_ACT_BIND(squareButton, toggled, RK_ACT_ARGS(bool b), this, setSquareWave(b));
        squareButton->show();

        triangleButton = new GeonkickButton(waveFunctionHBox);
        triangleButton->setBackgroundColor(waveFunctionHBox->background());
        triangleButton->setFixedSize(67, 14);
        triangleButton->setPosition(sineButton->x() + 60, sineButton->y());
        triangleButton->setUnpressedImage(RkImage(triangleButton->size(), rk_wave_button_triangle_png));
        triangleButton->setPressedImage(RkImage(triangleButton->size(), rk_wave_button_triangle_active_png));
        RK_ACT_BIND(triangleButton, toggled, RK_ACT_ARGS(bool b), this, setTriangleWave(b));
        triangleButton->show();

        sawtoothButton = new GeonkickButton(waveFunctionHBox);
        sawtoothButton->setBackgroundColor(waveFunctionHBox->background());
        sawtoothButton->setSize(67, 14);
        sawtoothButton->setPosition(sineButton->x() + 60, sineButton->y() + sineButton->height() + 5);
        sawtoothButton->setUnpressedImage(RkImage(sawtoothButton->size(), rk_wave_button_sawtooth_png));
        sawtoothButton->setPressedImage(RkImage(sawtoothButton->size(), rk_wave_button_sawtooth_active_png));
        RK_ACT_BIND(sawtoothButton, toggled, RK_ACT_ARGS(bool b), this, setSawtoothWave(b));
        sawtoothButton->show();

        sampleButton = new GeonkickButton(waveFunctionHBox);
        sampleButton->setBackgroundColor(waveFunctionHBox->background());
        sampleButton->setFixedSize(67, 14);
        sampleButton->setPosition(triangleButton->x() + 73, triangleButton->y());
        sampleButton->setUnpressedImage(RkImage(sampleButton->size(), rk_wave_button_sample_png));
        sampleButton->setPressedImage(RkImage(sampleButton->size(), rk_wave_button_sample_active_png));
        RK_ACT_BIND(sampleButton, toggled, RK_ACT_ARGS(bool b), this, setSampleFunction(b));

        sampleBrowseButton = new GeonkickButton(waveFunctionHBox);
        sampleBrowseButton->setCheckable(true);
        sampleBrowseButton->setBackgroundColor(waveFunctionHBox->background());
        sampleBrowseButton->setFixedSize(67, 14);
        sampleBrowseButton->setPosition(triangleButton->x() + 73, sampleButton->y() + sampleButton->height() + 5);
        sampleBrowseButton->setUnpressedImage(RkImage(sampleBrowseButton->size(), rk_button_browse_sample_png));
        RK_ACT_BIND(sampleBrowseButton, toggled, RK_ACT_ARGS(bool b), this, browseSample());

        auto phaseLabel = new RkLabel(waveFunctionHBox);
        phaseLabel->setFixedSize(30, 8);
        phaseLabel->setPosition(sineButton->x(), sawtoothButton->y() + sawtoothButton->height() + 8);
        phaseLabel->setBackgroundColor(waveFunctionHBox->background());
        phaseLabel->setImage(RkImage(phaseLabel->size(), rk_phase_label_png));
        phaseLabel->show();

        phaseSlider = new GeonkickSlider(waveFunctionHBox);
        phaseSlider->setFixedSize(150, 8);
        phaseSlider->onSetValue(50);
        phaseSlider->setPosition(phaseLabel->x() + phaseLabel->width() + 5, phaseLabel->y() + 1);
        phaseSlider->show();
        RK_ACT_BIND(phaseSlider, valueUpdated, RK_ACT_ARGS(int value), this, setOscillatorPhase(value));
}

void OscillatorGroupBox::createEvelopeGroupBox()
{
        auto amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        if (oscillator->type() == Oscillator::Type::Noise) {
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, rk_hboxbk_noise_env_png));
                amplitudeEnvelopeBox->setPosition(0, 18);
        } else {
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, rk_hboxbk_osc_env_png));
                amplitudeEnvelopeBox->setPosition(0, 111);
        }
        amplitudeEnvelopeBox->show();

        envelopeAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        envelopeAmplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2 - 1);
        envelopeAmplitudeKnob->setFixedSize(80, 80);
        envelopeAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
        envelopeAmplitudeKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
        envelopeAmplitudeKnob->setRange(0, 1.0);
        envelopeAmplitudeKnob->show();
        RK_ACT_BIND(envelopeAmplitudeKnob, valueUpdated, RK_ACT_ARGS(double val), oscillator, setAmplitude(val));

        if (oscillator->type() == Oscillator::Type::Noise) {
                noiseWhiteButton = new GeonkickButton(amplitudeEnvelopeBox);
                noiseWhiteButton->setPosition(224 / 2 + (224 / 2 - 90) / 2 - 10, 27);
                noiseWhiteButton->setFixedSize(90, 30);
                noiseWhiteButton->setUnpressedImage(RkImage(90, 30, rk_noise_type_white_png));
                noiseWhiteButton->setPressedImage(RkImage(90, 30, rk_noise_type_white_active_png));
                RK_ACT_BIND(noiseWhiteButton, toggled, RK_ACT_ARGS(bool b), this, setNoiseWhite(b));
                noiseWhiteButton->show();
                noiseBrownianButton = new GeonkickButton(amplitudeEnvelopeBox);
                noiseBrownianButton->setPosition(224 / 2 + (224 / 2 - 90) / 2 - 10,
                                                 noiseWhiteButton->y() + noiseWhiteButton->height() + 10);
                noiseBrownianButton->setFixedSize(90, 30);
                RK_ACT_BIND(noiseBrownianButton, toggled, RK_ACT_ARGS(bool b), this, setNoiseBrownian(b));
                noiseBrownianButton->setUnpressedImage(RkImage(90, 30, rk_noise_type_brownian_png));
                noiseBrownianButton->setPressedImage(RkImage(90, 30, rk_noise_type_brownian_active_png));
                noiseBrownianButton->show();
        } else {
                frequencyAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
                frequencyAmplitudeKnob->setRangeType(Knob::RangeType::Logarithmic);
                frequencyAmplitudeKnob->setSize(80, 80);
                frequencyAmplitudeKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2 - 1);
                frequencyAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, rk_knob_bk_image_png));
                frequencyAmplitudeKnob->setKnobImage(RkImage(70, 70, rk_knob_png));
                frequencyAmplitudeKnob->setRange(200, 16000);
                RK_ACT_BIND(frequencyAmplitudeKnob, valueUpdated, RK_ACT_ARGS(double val),
                            oscillator, setFrequency(val));
                frequencyAmplitudeKnob->show();
        }
}

void OscillatorGroupBox::createFilterGroupBox()
{
        filterBox = new Filter(this);
        if (oscillator->type() == Oscillator::Type::Noise)
                filterBox->setPosition(0, 151);
        else
                filterBox->setPosition(0, 243);
        filterBox->setCutOffRange(20, 20000);
        filterBox->setResonanceRange(0.01, 10);
        RK_ACT_BIND(filterBox, enabled, RK_ACT_ARGS(bool b), oscillator, enableFilter(b));
        RK_ACT_BIND(filterBox, cutOffChanged, RK_ACT_ARGS(double val), oscillator, setFilterFrequency(val));
        RK_ACT_BIND(filterBox, resonanceChanged, RK_ACT_ARGS(double val), oscillator, setFilterQFactor(val));
        RK_ACT_BIND(filterBox, typeChanged, RK_ACT_ARGS(GeonkickApi::FilterType type),
                    oscillator, setFilterType(type));
}

void OscillatorGroupBox::setSineWave(bool pressed)
{
        if (pressed) {
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
                sampleButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Sine);
        }
}

void OscillatorGroupBox::setSquareWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
                sampleButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Square);
        }
}

void OscillatorGroupBox::setTriangleWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                sawtoothButton->setPressed(false);
                sampleButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Triangle);
        }
}

void OscillatorGroupBox::setSawtoothWave(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
                sampleButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Sawtooth);
        }
}

void OscillatorGroupBox::setSampleFunction(bool pressed)
{
        if (pressed) {
                sineButton->setPressed(false);
                squareButton->setPressed(false);
                triangleButton->setPressed(false);
                sawtoothButton->setPressed(false);
                oscillator->setFunction(Oscillator::FunctionType::Sample);
        }
}

void OscillatorGroupBox::setOscillatorPhase(int value)
{
        oscillator->setPhase((static_cast<gkick_real>(value) / 100) * (2 * M_PI));
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
                filterBox->enable(true);
        oscillator->enable(state);
}

void OscillatorGroupBox::updateGui()
{
        oscillatorCheckbox->setPressed(oscillator->isEnabled());
        if (oscillator->type() == Oscillator::Type::Noise) {
                noiseWhiteButton->setPressed(false);
                noiseBrownianButton->setPressed(false);
                if (oscillator->function() == Oscillator::FunctionType::NoiseWhite)
                        noiseWhiteButton->setPressed(true);
                else
                        noiseBrownianButton->setPressed(true);
        } else {
                sineButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sine);
                squareButton->setPressed(oscillator->function() == Oscillator::FunctionType::Square);
                triangleButton->setPressed(oscillator->function() == Oscillator::FunctionType::Triangle);
                sawtoothButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sawtooth);
                sampleButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sample);
                phaseSlider->onSetValue(100 * oscillator->getPhase() / (2 * M_PI));
        }

        envelopeAmplitudeKnob->setCurrentValue(oscillator->amplitude());
        if (oscillator->type() != Oscillator::Type::Noise)
                frequencyAmplitudeKnob->setCurrentValue(oscillator->frequency());

        if (oscillator->type() == Oscillator::Type::Oscillator1)
                fmCheckbox->setPressed(oscillator->isFm());

        filterBox->enable(oscillator->isFilterEnabled());
        filterBox->setResonance(oscillator->filterQFactor());
        filterBox->setCutOff(oscillator->filterFrequency());
        filterBox->setType(oscillator->filter());
}

void OscillatorGroupBox::browseSample()
{
        auto fileDialog = new FileDialog(this, FileDialog::Type::Open, "Select sample");
        fileDialog->setFilters({".wav", ".WAV", ".flac", ".FLAC", ".ogg", ".OGG"});
        fileDialog->setCurrentDirectoy(oscillator->samplesPath());
        RK_ACT_BIND(fileDialog, selectedFile, RK_ACT_ARGS(const std::string &file), oscillator, setSample(file));
}
