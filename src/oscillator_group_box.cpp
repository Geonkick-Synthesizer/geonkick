/**
 * File name: oscillator_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
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
#include "OscillatorFunctionView.h"

#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(controls_osc1_on);
RK_DECLARE_IMAGE_RC(controls_osc1_hover);
RK_DECLARE_IMAGE_RC(controls_osc1_off);
RK_DECLARE_IMAGE_RC(controls_osc2_on);
RK_DECLARE_IMAGE_RC(controls_osc2_hover);
RK_DECLARE_IMAGE_RC(controls_osc2_off);
RK_DECLARE_IMAGE_RC(controls_osc3_on);
RK_DECLARE_IMAGE_RC(controls_osc3_hover);
RK_DECLARE_IMAGE_RC(controls_osc3_off);
RK_DECLARE_IMAGE_RC(fm_radio);
RK_DECLARE_IMAGE_RC(fm_radio_hover);
RK_DECLARE_IMAGE_RC(fm_radio_active);
RK_DECLARE_IMAGE_RC(wf_bk_hbox);
RK_DECLARE_IMAGE_RC(wave_button_sine);
RK_DECLARE_IMAGE_RC(wave_button_sine_active);
RK_DECLARE_IMAGE_RC(wave_button_sine_hover);
RK_DECLARE_IMAGE_RC(wave_button_square);
RK_DECLARE_IMAGE_RC(wave_button_square_active);
RK_DECLARE_IMAGE_RC(wave_button_square_hover);
RK_DECLARE_IMAGE_RC(wave_button_triangle);
RK_DECLARE_IMAGE_RC(wave_button_triangle_active);
RK_DECLARE_IMAGE_RC(wave_button_triangle_hover);
RK_DECLARE_IMAGE_RC(wave_button_sawtooth);
RK_DECLARE_IMAGE_RC(wave_button_sawtooth_active);
RK_DECLARE_IMAGE_RC(wave_button_sawtooth_hover);
RK_DECLARE_IMAGE_RC(wave_button_noise);
RK_DECLARE_IMAGE_RC(wave_button_noise_active);
RK_DECLARE_IMAGE_RC(wave_button_noise_hover);
RK_DECLARE_IMAGE_RC(wave_button_sample);
RK_DECLARE_IMAGE_RC(wave_button_sample_active);
RK_DECLARE_IMAGE_RC(wave_button_sample_hover);
RK_DECLARE_IMAGE_RC(hboxbk_osc_env);
RK_DECLARE_IMAGE_RC(knob);
RK_DECLARE_IMAGE_RC(knob_bk_image);
RK_DECLARE_IMAGE_RC(osc_ampl_button_off);
RK_DECLARE_IMAGE_RC(osc_ampl_button_on);
RK_DECLARE_IMAGE_RC(osc_ampl_button_hover);
RK_DECLARE_IMAGE_RC(osc_freq_button_off);
RK_DECLARE_IMAGE_RC(osc_freq_button_on);
RK_DECLARE_IMAGE_RC(osc_freq_button_hover);
RK_DECLARE_IMAGE_RC(osc_pitch_button_off);
RK_DECLARE_IMAGE_RC(osc_pitch_button_on);
RK_DECLARE_IMAGE_RC(osc_pitch_button_hover);
RK_DECLARE_IMAGE_RC(density_env_button);
RK_DECLARE_IMAGE_RC(density_env_button_active);
RK_DECLARE_IMAGE_RC(density_env_button_hover);

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
          , noiseButton{nullptr}
          , amplitudeKnob{nullptr}
          , frequencyKnob{nullptr}
          , pitchShiftKnob{nullptr}
          , noiseDensityKnob{nullptr}
          , filterTypeIsChecked{false}
          , amplitudeEnvelopeBox{nullptr}
          , oscFreqEnvelopeButton{nullptr}
          , pitchEnvelopeButton{nullptr}
          , densityEnvelopeButton{nullptr}
          , functionView{nullptr}
{
        setFixedSize(224, 335);
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
        waveFunctionHBox->setBackgroundImage(RkImage(waveFunctionHBox->size(), RK_IMAGE_RC(wf_bk_hbox)));
        waveFunctionHBox->show();

        oscillatorCheckbox = new GeonkickButton(waveFunctionHBox);
        oscillatorCheckbox->setCheckable(true);
        oscillatorCheckbox->setFixedSize(69, 21);
        oscillatorCheckbox->setPosition((waveFunctionHBox->width() - oscillatorCheckbox->width()) / 2, 0);
        if (oscillator->type() == Oscillator::Type::Oscillator1) {
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc1_on)),
                                             RkButton::State::Pressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc1_off)),
                                             RkButton::State::Unpressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc1_hover)),
                                             RkButton::State::UnpressedHover);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc1_hover)),
                                             RkButton::State::PressedHover);
                fmCheckbox = new GeonkickButton(waveFunctionHBox);
                fmCheckbox->setCheckable(true);
                fmCheckbox->setPosition(20, 9);
                fmCheckbox->setFixedSize(26, 18);
                fmCheckbox->setImage(RkImage(fmCheckbox->size(), RK_IMAGE_RC(fm_radio_active)),
                                     RkButton::State::Pressed);
                fmCheckbox->setImage(RkImage(fmCheckbox->size(), RK_IMAGE_RC(fm_radio)),
                                     RkButton::State::Unpressed);
                fmCheckbox->setImage(RkImage(fmCheckbox->size(), RK_IMAGE_RC(fm_radio_hover)),
                                     RkButton::State::UnpressedHover);
                fmCheckbox->setImage(RkImage(fmCheckbox->size(), RK_IMAGE_RC(fm_radio_hover)),
                                     RkButton::State::PressedHover);
                fmCheckbox->show();
                RK_ACT_BIND(fmCheckbox, toggled, RK_ACT_ARGS(bool b), oscillator, setAsFm(b));
        } else if (oscillator->type() == Oscillator::Type::Oscillator2) {
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc2_on)),
                                             RkButton::State::Pressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc2_off)),
                                             RkButton::State::Unpressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc2_hover)),
                                             RkButton::State::UnpressedHover);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc2_hover)),
                                             RkButton::State::PressedHover);
        } else {
                oscillatorCheckbox->setFixedSize(69, 21);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc3_on)),
                                             RkButton::State::Pressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc3_off)),
                                             RkButton::State::Unpressed);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc3_hover)),
                                             RkButton::State::UnpressedHover);
                oscillatorCheckbox->setImage(RkImage(oscillatorCheckbox->size(),
                                                     RK_IMAGE_RC(controls_osc3_hover)),
                                             RkButton::State::PressedHover);
        }
        RK_ACT_BIND(oscillatorCheckbox, toggled, RK_ACT_ARGS(bool b), oscillator, enable(b));

        sineButton = new GeonkickButton(waveFunctionHBox);
        sineButton->setBackgroundColor(waveFunctionHBox->background());
        sineButton->setFixedSize(26, 18);
        sineButton->setPosition(20, 32);
        sineButton->setImage(RkImage(sineButton->size(), RK_IMAGE_RC(wave_button_sine)), RkButton::State::Unpressed);
        sineButton->setImage(RkImage(sineButton->size(), RK_IMAGE_RC(wave_button_sine_active)), RkButton::State::Pressed);
        sineButton->setImage(RkImage(sineButton->size(), RK_IMAGE_RC(wave_button_sine_hover)), RkButton::State::UnpressedHover);
        RK_ACT_BIND(sineButton, toggled, RK_ACT_ARGS(bool b), this, setWaveFunction(Oscillator::FunctionType::Sine));
        sineButton->show();

        squareButton = new GeonkickButton(waveFunctionHBox);
        squareButton->setBackgroundColor(waveFunctionHBox->background());
        squareButton->setFixedSize(26, 18);
        squareButton->setPosition(sineButton->x() + sineButton->width() + 5,  sineButton->y());
        squareButton->setUnpressedImage(RkImage(squareButton->size(), RK_IMAGE_RC(wave_button_square)));
        squareButton->setPressedImage(RkImage(squareButton->size(), RK_IMAGE_RC(wave_button_square_active)));
        squareButton->setImage(RkImage(sineButton->size(), RK_IMAGE_RC(wave_button_square_hover)), RkButton::State::UnpressedHover);
        RK_ACT_BIND(squareButton, toggled, RK_ACT_ARGS(bool b), this, setWaveFunction(Oscillator::FunctionType::Square));
        squareButton->show();

        triangleButton = new GeonkickButton(waveFunctionHBox);
        triangleButton->setBackgroundColor(waveFunctionHBox->background());
        triangleButton->setFixedSize(26, 18);
        triangleButton->setPosition(squareButton->x() + squareButton->width() + 5, squareButton->y());
        triangleButton->setUnpressedImage(RkImage(triangleButton->size(), RK_IMAGE_RC(wave_button_triangle)));
        triangleButton->setPressedImage(RkImage(triangleButton->size(), RK_IMAGE_RC(wave_button_triangle_active)));
        triangleButton->setImage(RkImage(triangleButton->size(), RK_IMAGE_RC(wave_button_triangle_hover)), RkButton::State::UnpressedHover);
        RK_ACT_BIND(triangleButton, toggled, RK_ACT_ARGS(bool b), this, setWaveFunction(Oscillator::FunctionType::Triangle));
        triangleButton->show();

        sawtoothButton = new GeonkickButton(waveFunctionHBox);
        sawtoothButton->setBackgroundColor(waveFunctionHBox->background());
        sawtoothButton->setSize(26, 18);
        sawtoothButton->setPosition(sineButton->x(), sineButton->y() + sineButton->height() + 5);
        sawtoothButton->setUnpressedImage(RkImage(sawtoothButton->size(), RK_IMAGE_RC(wave_button_sawtooth)));
        sawtoothButton->setPressedImage(RkImage(sawtoothButton->size(), RK_IMAGE_RC(wave_button_sawtooth_active)));
        sawtoothButton->setImage(RkImage(sawtoothButton->size(), RK_IMAGE_RC(wave_button_sawtooth_hover)), RkButton::State::UnpressedHover);
        RK_ACT_BIND(sawtoothButton, toggled, RK_ACT_ARGS(bool b), this, setWaveFunction(Oscillator::FunctionType::Sawtooth));

        noiseButton = new GeonkickButton(waveFunctionHBox);
        noiseButton->setBackgroundColor(waveFunctionHBox->background());
        noiseButton->setSize(26, 18);
        noiseButton->setPosition(sawtoothButton->x() + sawtoothButton->width() + 5, sawtoothButton->y());
        noiseButton->setUnpressedImage(RkImage(noiseButton->size(), RK_IMAGE_RC(wave_button_noise)));
        noiseButton->setPressedImage(RkImage(noiseButton->size(), RK_IMAGE_RC(wave_button_noise_active)));
        noiseButton->setImage(RkImage(noiseButton->size(), RK_IMAGE_RC(wave_button_noise_hover)), RkButton::State::UnpressedHover);
        RK_ACT_BIND(noiseButton, toggled, RK_ACT_ARGS(bool b), this, setWaveFunction(Oscillator::FunctionType::NoiseWhite));

        sampleButton = new GeonkickButton(waveFunctionHBox);
        sampleButton->setBackgroundColor(waveFunctionHBox->background());
        sampleButton->setFixedSize(26, 18);
        sampleButton->setPosition(noiseButton->x() + noiseButton->width() + 5, noiseButton->y());
        sampleButton->setUnpressedImage(RkImage(sampleButton->size(), RK_IMAGE_RC(wave_button_sample)));
        sampleButton->setPressedImage(RkImage(sampleButton->size(), RK_IMAGE_RC(wave_button_sample_active)));
        sampleButton->setImage(RkImage(sampleButton->size(), RK_IMAGE_RC(wave_button_sample_hover)), RkButton::State::UnpressedHover);
        RK_ACT_BIND(sampleButton, toggled, RK_ACT_ARGS(bool b), this, setWaveFunction(Oscillator::FunctionType::Sample));

        functionView = new OscillatorFunctionView(waveFunctionHBox, oscillator);
        functionView->setPosition(sampleButton->x() + sampleButton->width() + 5,
                                  sineButton->y() - 10);
}

void OscillatorGroupBox::createEvelopeGroupBox()
{
        amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        amplitudeEnvelopeBox->setBackgroundImage(RkImage(224, 125, RK_IMAGE_RC(hboxbk_osc_env)));
        amplitudeEnvelopeBox->setPosition(0, 85);
        amplitudeEnvelopeBox->show();

        amplitudeKnob = new Knob(amplitudeEnvelopeBox);
        amplitudeKnob->setDefaultValue(0.26);
        amplitudeKnob->setFixedSize(80, 78);
        amplitudeKnob->setPosition((224 / 2 - 80) / 2, (125 - 80) / 2 - 2);
        amplitudeKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        amplitudeKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        amplitudeKnob->setRange(0, 1.0);
        amplitudeKnob->show();
        RK_ACT_BIND(amplitudeKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    oscillator,
                    setAmplitude(val));

        auto oscAmplEnvelopeButton = new GeonkickButton(amplitudeEnvelopeBox);
        oscAmplEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::Amplitude
                                          && static_cast<Oscillator::Type>(viewState()->getEnvelopeCategory())
                                          == oscillator->type());
        oscAmplEnvelopeButton->setFixedSize(63, 21);
        oscAmplEnvelopeButton->setPosition(amplitudeKnob->x() + amplitudeKnob->width() / 2 - oscAmplEnvelopeButton->width() / 2,
                                           amplitudeKnob->y() + amplitudeKnob->height() + 2);
        oscAmplEnvelopeButton->setImage(RkImage(oscAmplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_off)),
                                        RkButton::State::Unpressed);
        oscAmplEnvelopeButton->setImage(RkImage(oscAmplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_on)),
                                        RkButton::State::Pressed);
        oscAmplEnvelopeButton->setImage(RkImage(oscAmplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_hover)),
                                        RkButton::State::PressedHover);
        oscAmplEnvelopeButton->setImage(RkImage(oscAmplEnvelopeButton->size(), RK_IMAGE_RC(osc_ampl_button_hover)),
        RkButton::State::UnpressedHover);
        oscAmplEnvelopeButton->show();
        RK_ACT_BIND(oscAmplEnvelopeButton,
                    pressed,
                    RK_ACT_ARGS(),
                    viewState(), setEnvelope(static_cast<Envelope::Category>(oscillator->type()),
                                             Envelope::Type::Amplitude));
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    oscAmplEnvelopeButton, setPressed(envelope == Envelope::Type::Amplitude
                                                      && static_cast<Oscillator::Type>(category)
                                                      == oscillator->type()));
        pitchShiftKnob = new Knob(amplitudeEnvelopeBox);
        pitchShiftKnob->setDefaultValue(12);
        pitchShiftKnob->setSize(80, 78);
        pitchShiftKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2 - 2);
        pitchShiftKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        pitchShiftKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        pitchShiftKnob->setRange(0, 48);
        RK_ACT_BIND(pitchShiftKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    oscillator,
                    setPitchShift(val));

        noiseDensityKnob = new Knob(amplitudeEnvelopeBox);
        noiseDensityKnob->setDefaultValue(1.0);
        noiseDensityKnob->setSize(80, 78);
        noiseDensityKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2 - 2);
        noiseDensityKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        noiseDensityKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        noiseDensityKnob->setRange(0, 1.0);
        RK_ACT_BIND(noiseDensityKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    oscillator,
                    setNoiseDensity(val));

        frequencyKnob = new Knob(amplitudeEnvelopeBox);
        frequencyKnob->setDefaultValue(800);
        frequencyKnob->setRangeType(Knob::RangeType::Logarithmic);
        frequencyKnob->setSize(80, 78);
        frequencyKnob->setPosition(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2 - 2);
        frequencyKnob->setKnobBackgroundImage(RkImage(80, 80, RK_IMAGE_RC(knob_bk_image)));
        frequencyKnob->setKnobImage(RkImage(70, 70, RK_IMAGE_RC(knob)));
        frequencyKnob->setRange(200, 16000);
        RK_ACT_BIND(frequencyKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double val),
                    oscillator,
                    setFrequency(val));

        oscFreqEnvelopeButton = new GeonkickButton(amplitudeEnvelopeBox);
        oscFreqEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::Frequency
                                          && static_cast<Oscillator::Type>(viewState()->getEnvelopeCategory())
                                          == oscillator->type());
        oscFreqEnvelopeButton->setFixedSize(63, 21);
        oscFreqEnvelopeButton->setPosition(frequencyKnob->x() + frequencyKnob->width() / 2 - oscFreqEnvelopeButton->width() / 2,
                                           frequencyKnob->y() + frequencyKnob->height() + 2);
        oscFreqEnvelopeButton->setImage(RkImage(oscFreqEnvelopeButton->size(), RK_IMAGE_RC(osc_freq_button_off)),
                                        RkButton::State::Unpressed);
        oscFreqEnvelopeButton->setImage(RkImage(oscFreqEnvelopeButton->size(), RK_IMAGE_RC(osc_freq_button_on)),
                                        RkButton::State::Pressed);
        oscFreqEnvelopeButton->setImage(RkImage(oscFreqEnvelopeButton->size(), RK_IMAGE_RC(osc_freq_button_hover)),
                                        RkButton::State::PressedHover);
        oscFreqEnvelopeButton->setImage(RkImage(oscFreqEnvelopeButton->size(), RK_IMAGE_RC(osc_freq_button_hover)),
                                        RkButton::State::UnpressedHover);
        RK_ACT_BIND(oscFreqEnvelopeButton,
                    pressed,
                    RK_ACT_ARGS(),
                    viewState(), setEnvelope(static_cast<Envelope::Category>(oscillator->type()),
                                             Envelope::Type::Frequency));
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    oscFreqEnvelopeButton, setPressed(envelope == Envelope::Type::Frequency
                                                      && static_cast<Oscillator::Type>(category)
                                                      == oscillator->type()));

        pitchEnvelopeButton = new GeonkickButton(amplitudeEnvelopeBox);
        pitchEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::PitchShift
                                        && static_cast<Oscillator::Type>(viewState()->getEnvelopeCategory())
                                        == oscillator->type());
        pitchEnvelopeButton->setFixedSize(63, 21);
        pitchEnvelopeButton->setPosition(frequencyKnob->x() + frequencyKnob->width() / 2 - pitchEnvelopeButton->width() / 2,
                                         frequencyKnob->y() + frequencyKnob->height() + 2);
        pitchEnvelopeButton->setImage(RkImage(pitchEnvelopeButton->size(), RK_IMAGE_RC(osc_pitch_button_off)),
                                      RkButton::State::Unpressed);
        pitchEnvelopeButton->setImage(RkImage(pitchEnvelopeButton->size(), RK_IMAGE_RC(osc_pitch_button_on)),
                                      RkButton::State::Pressed);
        pitchEnvelopeButton->setImage(RkImage(pitchEnvelopeButton->size(), RK_IMAGE_RC(osc_pitch_button_hover)),
                                      RkButton::State::PressedHover);
        pitchEnvelopeButton->setImage(RkImage(pitchEnvelopeButton->size(), RK_IMAGE_RC(osc_pitch_button_hover)),
                                      RkButton::State::UnpressedHover);
        RK_ACT_BIND(pitchEnvelopeButton,
                    pressed,
                    RK_ACT_ARGS(),
                    viewState(), setEnvelope(static_cast<Envelope::Category>(oscillator->type()),
                                             Envelope::Type::PitchShift));
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    pitchEnvelopeButton, setPressed(envelope == Envelope::Type::PitchShift
                                                    && static_cast<Oscillator::Type>(category)
                                                    == oscillator->type()));

        densityEnvelopeButton = new GeonkickButton(amplitudeEnvelopeBox);
        densityEnvelopeButton->setPressed(viewState()->getEnvelopeType() == Envelope::Type::NoiseDensity
                                        && static_cast<Oscillator::Type>(viewState()->getEnvelopeCategory())
                                        == oscillator->type());
        densityEnvelopeButton->setFixedSize(63, 21);
        densityEnvelopeButton->setPosition(frequencyKnob->x() + frequencyKnob->width() / 2 - densityEnvelopeButton->width() / 2,
                                         frequencyKnob->y() + frequencyKnob->height() + 2);
        densityEnvelopeButton->setImage(RkImage(densityEnvelopeButton->size(), RK_IMAGE_RC(density_env_button)),
                                      RkButton::State::Unpressed);
        densityEnvelopeButton->setImage(RkImage(densityEnvelopeButton->size(), RK_IMAGE_RC(density_env_button_active)),
                                      RkButton::State::Pressed);
        densityEnvelopeButton->setImage(RkImage(densityEnvelopeButton->size(), RK_IMAGE_RC(density_env_button_hover)),
                                      RkButton::State::PressedHover);
        densityEnvelopeButton->setImage(RkImage(densityEnvelopeButton->size(), RK_IMAGE_RC(density_env_button_hover)),
                                      RkButton::State::UnpressedHover);
        RK_ACT_BIND(densityEnvelopeButton,
                    pressed,
                    RK_ACT_ARGS(),
                    viewState(), setEnvelope(static_cast<Envelope::Category>(oscillator->type()),
                                             Envelope::Type::NoiseDensity));
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    densityEnvelopeButton, setPressed(envelope == Envelope::Type::NoiseDensity
                                                    && static_cast<Oscillator::Type>(category)
                                                    == oscillator->type()));

        switch(oscillator->function()) {
        case Oscillator::FunctionType::Sample:
                pitchEnvelopeButton->show();
                pitchShiftKnob->show();
                break;
        case Oscillator::FunctionType::NoiseWhite:
        case Oscillator::FunctionType::NoiseBrownian:
                densityEnvelopeButton->show();
                noiseDensityKnob->show();
                break;
        default:
                oscFreqEnvelopeButton->show();
                frequencyKnob->show();
                break;
        }
}

void OscillatorGroupBox::createFilterGroupBox()
{
        filterBox = new Filter(this, static_cast<Envelope::Category>(oscillator->type()));
        filterBox->setPosition(0, 210);
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

void OscillatorGroupBox::setWaveFunction(Oscillator::FunctionType type)
{
        sineButton->setPressed(type == Oscillator::FunctionType::Sine);
        squareButton->setPressed(type == Oscillator::FunctionType::Square);
        triangleButton->setPressed(type == Oscillator::FunctionType::Triangle);
        sawtoothButton->setPressed(type == Oscillator::FunctionType::Sawtooth);
        noiseButton->setPressed(type == Oscillator::FunctionType::NoiseWhite);
        sampleButton->setPressed(type == Oscillator::FunctionType::Sample);
        oscillator->setFunction(type);
        functionView->setModel(oscillator);
        updateAmpltudeEnvelopeBox();
}

void OscillatorGroupBox::setOscillatorPhase(int value)
{
        oscillator->setPhase((static_cast<gkick_real>(value) / 100) * (2 * M_PI));
}

void OscillatorGroupBox::setOscillatorSeed(int value)
{
        oscillator->setSeed(10 * value);
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
        sineButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sine);
        squareButton->setPressed(oscillator->function() == Oscillator::FunctionType::Square);
        triangleButton->setPressed(oscillator->function() == Oscillator::FunctionType::Triangle);
        sawtoothButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sawtooth);
        noiseButton->setPressed(oscillator->function() == Oscillator::FunctionType::NoiseWhite
                                || oscillator->function() == Oscillator::FunctionType::NoiseBrownian);
        sampleButton->setPressed(oscillator->function() == Oscillator::FunctionType::Sample);
        updateAmpltudeEnvelopeBox();

        amplitudeKnob->setCurrentValue(oscillator->amplitude());
        frequencyKnob->setCurrentValue(oscillator->frequency());
        pitchShiftKnob->setCurrentValue(oscillator->pitchShift());
        noiseDensityKnob->setCurrentValue(oscillator->noiseDensity());

        if (oscillator->type() == Oscillator::Type::Oscillator1)
                fmCheckbox->setPressed(oscillator->isFm());

        filterBox->enable(oscillator->isFilterEnabled());
        filterBox->setResonance(oscillator->filterQFactor(), 10);
        filterBox->setCutOff(oscillator->filterFrequency(), 800);
        filterBox->setType(oscillator->filter());
        functionView->setModel(oscillator);
}

void OscillatorGroupBox::browseSample()
{
        viewState()->setSamplesBrowserOscillator(static_cast<ViewState::Oscillator>(oscillator->type()));
        viewState()->setMainView(ViewState::View::Samples);
}

void OscillatorGroupBox::updateAmpltudeEnvelopeBox()
{
        noiseDensityKnob->show(oscillator->function() == Oscillator::FunctionType::NoiseWhite
                               || oscillator->function() == Oscillator::FunctionType::NoiseBrownian);
        densityEnvelopeButton->show(oscillator->function() == Oscillator::FunctionType::NoiseWhite
                                    || oscillator->function() == Oscillator::FunctionType::NoiseBrownian);
        pitchShiftKnob->show(oscillator->function() == Oscillator::FunctionType::Sample);
        pitchEnvelopeButton->show(oscillator->function() == Oscillator::FunctionType::Sample);
        frequencyKnob->show(!noiseDensityKnob->isVisible() && !pitchShiftKnob->isVisible());
        oscFreqEnvelopeButton->show(!noiseDensityKnob->isVisible() && !pitchShiftKnob->isVisible());
        amplitudeEnvelopeBox->update();
}
