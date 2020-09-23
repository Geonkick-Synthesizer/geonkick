/**
 * File name: oscillator_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://iuriepage.wordpress.com)
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
#include "ViewState.h"

#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(controls_osc1_on);
RK_DECLARE_IMAGE_RC(controls_osc1_off);
RK_DECLARE_IMAGE_RC(controls_osc2_on);
RK_DECLARE_IMAGE_RC(controls_osc2_off);
RK_DECLARE_IMAGE_RC(controls_noise_on);
RK_DECLARE_IMAGE_RC(controls_noise_off);
RK_DECLARE_IMAGE_RC(fm_radio);
RK_DECLARE_IMAGE_RC(fm_radio_active);
RK_DECLARE_IMAGE_RC(wf_bk_hbox);
RK_DECLARE_IMAGE_RC(wave_button_sine);
RK_DECLARE_IMAGE_RC(wave_button_sine_active);
RK_DECLARE_IMAGE_RC(wave_button_square);
RK_DECLARE_IMAGE_RC(wave_button_square_active);
RK_DECLARE_IMAGE_RC(wave_button_triangle);
RK_DECLARE_IMAGE_RC(wave_button_triangle_active);
RK_DECLARE_IMAGE_RC(wave_button_sawtooth);
RK_DECLARE_IMAGE_RC(phase_label);
RK_DECLARE_IMAGE_RC(wave_button_sample);
RK_DECLARE_IMAGE_RC(wave_button_sample_active);
RK_DECLARE_IMAGE_RC(wave_button_sawtooth_active);
RK_DECLARE_IMAGE_RC(button_browse_sample);
RK_DECLARE_IMAGE_RC(hboxbk_noise_env);
RK_DECLARE_IMAGE_RC(hboxbk_osc_env);
RK_DECLARE_IMAGE_RC(hboxbk_sample_env);
RK_DECLARE_IMAGE_RC(knob);
RK_DECLARE_IMAGE_RC(noise_type_white);
RK_DECLARE_IMAGE_RC(noise_type_white_active);
RK_DECLARE_IMAGE_RC(noise_type_brownian);
RK_DECLARE_IMAGE_RC(noise_type_brownian_active);
RK_DECLARE_IMAGE_RC(knob_bk_image);

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
           , amplitudeEnvelopeBox{nullptr}
{
        setFixedSize(224, 380);
        oscillatorCheckbox = new GeonkickButton(this);
        oscillatorCheckbox->setCheckable(true);
        oscillatorCheckbox->setX(10);
        RK_ACT_BIND(oscillatorCheckbox, toggled,
                    RK_ACT_ARGS(bool b), oscillator, enable(b));

        if (oscillator->type() == Oscillator::Type::Oscillator1) {
                oscillatorCheckbox->setFixedSize(80, 21);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc1_on)),
                                             RkButton::ButtonImage::ImagePressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc1_off)),
                                             RkButton::ButtonImage::ImageUnpressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc1_on)),
                                             RkButton::ButtonImage::ImageUnpressedHover);

                fmCheckbox = new GeonkickButton(this);
                fmCheckbox->setCheckable(true);
                fmCheckbox->setPosition(oscillatorCheckbox->x() + oscillatorCheckbox->width() + 20, 0);
                fmCheckbox->setFixedSize(92, 21);
                fmCheckbox->setImage(RkImage(fmCheckbox->size(), RK_IMAGE_RC(fm_radio_active)),
                                     RkButton::ButtonImage::ImagePressed);
                fmCheckbox->setImage(RkImage(fmCheckbox->size(), RK_IMAGE_RC(fm_radio)),
                                     RkButton::ButtonImage::ImageUnpressed);
                fmCheckbox->setImage(RkImage(fmCheckbox->size(), RK_IMAGE_RC(fm_radio_active)),
                                     RkButton::ButtonImage::ImageUnpressedHover);
                RK_ACT_BIND(fmCheckbox, toggled, RK_ACT_ARGS(bool b), oscillator, setAsFm(b));
        } else if (oscillator->type() == Oscillator::Type::Oscillator2) {
                oscillatorCheckbox->setFixedSize(82, 21);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc2_on)),
                                             RkButton::ButtonImage::ImagePressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc2_off)),
                                             RkButton::ButtonImage::ImageUnpressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc2_on)),
                                             RkButton::ButtonImage::ImageUnpressedHover);
        } else {
                oscillatorCheckbox->setFixedSize(51, 21);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_noise_on)),
                                             RkButton::ButtonImage::ImagePressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_noise_off)),
                                             RkButton::ButtonImage::ImageUnpressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_noise_on)),
                                             RkButton::ButtonImage::ImageUnpressedHover);
        }

        if (oscillator->type() != Oscillator::Type::Noise)
                createWaveFunctionGroupBox();
        createEvelopeGroupBox();
        createFilterGroupBox();
        updateGui();
}

void OscillatorGroupBox::createWaveFunctionGroupBox()
{
        auto waveFunctionHBox = new GeonkickWidget(this);
        waveFunctionHBox->setBackgroundColor(67, 68, 68);
        waveFunctionHBox->setFixedSize(224, 85);
        waveFunctionHBox->setPosition(0, 25);
        waveFunctionHBox->setBackgroundImage(RkImage(waveFunctionHBox->size(), RK_IMAGE_RC(wf_bk_hbox)));
        waveFunctionHBox->show();

        sineButton = new GeonkickButton(waveFunctionHBox);
        sineButton->setBackgroundColor(waveFunctionHBox->background());
        sineButton->setFixedSize(67, 14);
        sineButton->setPosition(20, 25);
        sineButton->setUnpressedImage(RkImage(sineButton->size(), RK_IMAGE_RC(wave_button_sine)));
        sineButton->setPressedImage(RkImage(sineButton->size(), RK_IMAGE_RC(wave_button_sine_active)));
        RK_ACT_BIND(sineButton, toggled, RK_ACT_ARGS(bool b), this, setSineWave(b));
        sineButton->show();

        squareButton = new GeonkickButton(waveFunctionHBox);
        squareButton->setBackgroundColor(waveFunctionHBox->background());
        squareButton->setFixedSize(67, 14);
        squareButton->setPosition(sineButton->x(),  sineButton->y() + sineButton->height() + 5);
        squareButton->setUnpressedImage(RkImage(squareButton->size(), RK_IMAGE_RC(wave_button_square)));
        squareButton->setPressedImage(RkImage(squareButton->size(), RK_IMAGE_RC(wave_button_square_active)));
        RK_ACT_BIND(squareButton, toggled, RK_ACT_ARGS(bool b), this, setSquareWave(b));
        squareButton->show();

        triangleButton = new GeonkickButton(waveFunctionHBox);
        triangleButton->setBackgroundColor(waveFunctionHBox->background());
        triangleButton->setFixedSize(67, 14);
        triangleButton->setPosition(sineButton->x() + 60, sineButton->y());
        triangleButton->setUnpressedImage(RkImage(triangleButton->size(), RK_IMAGE_RC(wave_button_triangle)));
        triangleButton->setPressedImage(RkImage(triangleButton->size(), RK_IMAGE_RC(wave_button_triangle_active)));
        RK_ACT_BIND(triangleButton, toggled, RK_ACT_ARGS(bool b), this, setTriangleWave(b));
        triangleButton->show();

        sawtoothButton = new GeonkickButton(waveFunctionHBox);
        sawtoothButton->setBackgroundColor(waveFunctionHBox->background());
        sawtoothButton->setSize(67, 14);
        sawtoothButton->setPosition(sineButton->x() + 60, sineButton->y() + sineButton->height() + 5);
        sawtoothButton->setUnpressedImage(RkImage(sawtoothButton->size(), RK_IMAGE_RC(wave_button_sawtooth)));
        sawtoothButton->setPressedImage(RkImage(sawtoothButton->size(), RK_IMAGE_RC(wave_button_sawtooth_active)));
        RK_ACT_BIND(sawtoothButton, toggled, RK_ACT_ARGS(bool b), this, setSawtoothWave(b));
        sawtoothButton->show();

        sampleButton = new GeonkickButton(waveFunctionHBox);
        sampleButton->setBackgroundColor(waveFunctionHBox->background());
        sampleButton->setFixedSize(67, 14);
        sampleButton->setPosition(triangleButton->x() + 73, triangleButton->y());
        sampleButton->setUnpressedImage(RkImage(sampleButton->size(), RK_IMAGE_RC(wave_button_sample)));
        sampleButton->setPressedImage(RkImage(sampleButton->size(), RK_IMAGE_RC(wave_button_sample_active)));
        RK_ACT_BIND(sampleButton, toggled, RK_ACT_ARGS(bool b), this, setSampleFunction(b));

        sampleBrowseButton = new GeonkickButton(waveFunctionHBox);
        sampleBrowseButton->setCheckable(true);
        sampleBrowseButton->setBackgroundColor(waveFunctionHBox->background());
        sampleBrowseButton->setFixedSize(67, 14);
        sampleBrowseButton->setPosition(triangleButton->x() + 73, sampleButton->y() + sampleButton->height() + 5);
        sampleBrowseButton->setUnpressedImage(RkImage(sampleBrowseButton->size(), RK_IMAGE_RC(button_browse_sample)));
        RK_ACT_BIND(sampleBrowseButton, pressed, RK_ACT_ARGS(), this, browseSample());

        auto phaseLabel = new RkLabel(waveFunctionHBox);
        phaseLabel->setFixedSize(30, 8);
        phaseLabel->setPosition(sineButton->x(), sawtoothButton->y() + sawtoothButton->height() + 8);
        phaseLabel->setBackgroundColor(waveFunctionHBox->background());
        phaseLabel->setImage(RkImage(phaseLabel->size(), RK_IMAGE_RC(phase_label)));
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
        amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        if (oscillator->type() == Oscillator::Type::Noise) {
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_noise_env)));
                amplitudeEnvelopeBox->setPosition(0, 25);
        } else {
                if (oscillator->function() == Oscillator::FunctionType::Sample)
                        amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_sample_env)));
                else
                        amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_osc_env)));
                amplitudeEnvelopeBox->setPosition(0, 115);
        }
        amplitudeEnvelopeBox->show();

        envelopeAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        envelopeAmplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2 - 1);
        envelopeAmplitudeKnob->setFixedSize(80, 80);
        envelopeAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        envelopeAmplitudeKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        envelopeAmplitudeKnob->setRange(0, 1.0);
        envelopeAmplitudeKnob->show();
        RK_ACT_BIND(envelopeAmplitudeKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    oscillator,
                    setAmplitude(val));

        if (oscillator->type() == Oscillator::Type::Noise) {
                noiseWhiteButton = new GeonkickButton(amplitudeEnvelopeBox);
                noiseWhiteButton->setPosition(224 / 2 + (224 / 2 - 90) / 2 - 10, 10);
                noiseWhiteButton->setFixedSize(90, 30);
                noiseWhiteButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(noise_type_white)));
                noiseWhiteButton->setPressedImage(RkImage(90, 30, RK_IMAGE_RC(noise_type_white_active)));
                RK_ACT_BIND(noiseWhiteButton, toggled, RK_ACT_ARGS(bool b), this, setNoiseWhite(b));
                noiseWhiteButton->show();
                noiseBrownianButton = new GeonkickButton(amplitudeEnvelopeBox);
                noiseBrownianButton->setPosition(224 / 2 + (224 / 2 - 90) / 2 - 10,
                                                 noiseWhiteButton->y() + noiseWhiteButton->height());
                noiseBrownianButton->setFixedSize(90, 30);
                RK_ACT_BIND(noiseBrownianButton, toggled, RK_ACT_ARGS(bool b), this, setNoiseBrownian(b));
                noiseBrownianButton->setUnpressedImage(RkImage(90, 30, RK_IMAGE_RC(noise_type_brownian)));
                noiseBrownianButton->setPressedImage(RkImage(90, 30, RK_IMAGE_RC(noise_type_brownian_active)));
                noiseBrownianButton->show();

                auto seedLabel = new RkLabel(amplitudeEnvelopeBox, "Seed");
                seedLabel->setFixedSize(30, 10);
                seedLabel->setTextColor({210, 226, 226, 160});
                seedLabel->setPosition(noiseBrownianButton->x()
                                       + (noiseBrownianButton->width() - seedLabel->width()) / 2,
                                       noiseBrownianButton->y() + noiseBrownianButton->height() + 7);
                seedLabel->setBackgroundColor(amplitudeEnvelopeBox->background());
                seedLabel->show();

                seedSlider = new GeonkickSlider(amplitudeEnvelopeBox);
                seedSlider->setFixedSize(115, 8);
                seedSlider->onSetValue(40);
                seedSlider->setPosition(noiseBrownianButton->x() - 13, noiseBrownianButton->y() + 55);
                seedSlider->show();
                RK_ACT_BIND(seedSlider, valueUpdated, RK_ACT_ARGS(int value), this, setOscillatorSeed(value));

        } else {
                frequencyAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
                frequencyAmplitudeKnob->setRangeType(Knob::RangeType::Logarithmic);
                frequencyAmplitudeKnob->setSize(80, 80);
                frequencyAmplitudeKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2 - 1);
                frequencyAmplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
                frequencyAmplitudeKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
                frequencyAmplitudeKnob->setRange(200, 16000);
                RK_ACT_BIND(frequencyAmplitudeKnob,
                            valueUpdated,
                            RK_ACT_ARGS(double val),
                            oscillator,
                            setFrequency(val));
                frequencyAmplitudeKnob->show();
        }
}

void OscillatorGroupBox::createFilterGroupBox()
{
        filterBox = new Filter(this);
        if (oscillator->type() == Oscillator::Type::Noise)
                filterBox->setPosition(0, 154);
        else
                filterBox->setPosition(0, 245);
        filterBox->setCutOffRange(20, 20000);
        filterBox->setResonanceRange(1, 1000);
        RK_ACT_BIND(filterBox, enabled, RK_ACT_ARGS(bool b),
                    oscillator, enableFilter(b));
        RK_ACT_BIND(filterBox, cutOffChanged, RK_ACT_ARGS(double val),
                    oscillator, setFilterFrequency(val));
        RK_ACT_BIND(filterBox, resonanceChanged, RK_ACT_ARGS(double val),
                    oscillator, setFilterQFactor(val));
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
                updateAmpltudeEnvelopeBox();
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
                updateAmpltudeEnvelopeBox();
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
                updateAmpltudeEnvelopeBox();
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
                updateAmpltudeEnvelopeBox();
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
                updateAmpltudeEnvelopeBox();
        }
}

void OscillatorGroupBox::setOscillatorPhase(int value)
{
        oscillator->setPhase((static_cast<gkick_real>(value) / 100) * (2 * M_PI));
}

void OscillatorGroupBox::setOscillatorSeed(int value)
{
        oscillator->setSeed(10 * value);
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
                seedSlider->onSetValue(oscillator->getSeed() / 10);
        } else {
                sineButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sine);
                squareButton->setPressed(oscillator->function() == Oscillator::FunctionType::Square);
                triangleButton->setPressed(oscillator->function() == Oscillator::FunctionType::Triangle);
                sawtoothButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sawtooth);
                sampleButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sample);
                phaseSlider->onSetValue(oscillator->getPhase());
                updateAmpltudeEnvelopeBox();
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
        auto visualState = dynamic_cast<ViewState*>(findObject("ViewState"));
        if (visualState) {
                visualState->setSamplesBrowserOscillator(static_cast<ViewState::Oscillator>(oscillator->type()));
                visualState->setMainView(ViewState::View::Samples);
        }
}

void OscillatorGroupBox::updateAmpltudeEnvelopeBox()
{
        if (oscillator->function() == Oscillator::FunctionType::Sample)
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_sample_env)));
        else
                amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_osc_env)));
        amplitudeEnvelopeBox->update();
}
