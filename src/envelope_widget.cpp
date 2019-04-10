/**
 * File name: envelope_widget.cpp
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

#include "envelope_widget.h"
#include "general_envelope.h"
#include "oscillator_envelope.h"
#include "envelope_draw_area.h"
#include "geonkick_button.h"
#include "kick_graph.h"

extern const unsigned char rk_show_ampl_env_active_png[];
extern const unsigned char rk_show_ampl_env_png[];
extern const unsigned char rk_show_freq_env_active_png[];
extern const unsigned char rk_show_freq_env_png[];
extern const unsigned char rk_show_osc1_envelopes_button_active_png[];
extern const unsigned char rk_show_osc1_envelopes_button_png[];
extern const unsigned char rk_show_osc2_envelopes_button_active_png[];
extern const unsigned char rk_show_osc2_envelopes_button_png[];
extern const unsigned char rk_show_noise_envelopes_button_active_png[];
extern const unsigned char rk_show_noise_envelopes_button_png[];
extern const unsigned char rk_show_general_envelopes_button_active_png[];
extern const unsigned char rk_show_general_envelopes_button_png[];

EnvelopeWidget::EnvelopeWidget(GeonkickWidget *parent,
                               GeonkickApi *api,
                               const std::vector<std::unique_ptr<Oscillator>> &oscillators)
          : GeonkickWidget(parent)
          , drawArea{nullptr}
          , showAmplitudeEnvButton{nullptr}
          , showFrequencyEnvButton{nullptr}

{
        // Create drawing area.
        drawArea = new EnvelopeWidgetDrawingArea(this);
        drawArea->show();

        auto rect = drawArea->getDrawingArea();

        // Oscillator1 envelope
        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)].get();
        auto envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<OscillatorEnvelope>(oscillator, rect));
        //        connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.insert({static_cast<int>(EnvelopeType::Oscillator1), envelope});

        // Oscillator2 envelope
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)].get();
        envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<OscillatorEnvelope>(oscillator, rect));
        //connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.insert({static_cast<int>(EnvelopeType::Oscillator2), envelope});

        // Noise envelope
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Noise)].get();
        envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<OscillatorEnvelope>(oscillator, rect));
        //connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.insert({static_cast<int>(EnvelopeType::Noise), envelope});

        // General nevelope
        envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<GeneralEnvelope>(api, rect));
        //        connect(this, SIGNAL(update()), envelope.get(), SLOT(updatePoints()));
        envelopes.insert({static_cast<int>(EnvelopeType::General), envelope});

        auto graph = std::make_unique<KickGraph>(api);
        drawArea->setKickGraph(graph);

        createButtomMenu();
        showGeneralEnvelope();
}

EnvelopeWidget::~EnvelopeWidget()
{

}

void EnvelopeWidget::createButtomMenu()
{
        auto buttomAreaWidget = new GeonkickWidget(this);
        buttomAreaWidget->setPosition(drawArea->x(), drawArea->y() + drawArea->height());
        buttomAreaWidget->setFixedSize(drawArea->width(), 34);
        buttomAreaWidget->show();

        // Amplitude Envelope button
        showAmplitudeEnvButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(showAmplitudeEnvButton, toggled, RK_ACT_ARGS(bool pressed), this, showAmplitudeEnvelope());
        showAmplitudeEnvButton->setPressedImage(RkImage(90, 30, rk_show_ampl_env_active_png));
        showAmplitudeEnvButton->setUnpressedImage(RkImage(90, 30, rk_show_ampl_env_png));
        showAmplitudeEnvButton->setPosition(0, (buttomAreaWidget->height() - showAmplitudeEnvButton->height()) / 2);
        showAmplitudeEnvButton->show();

        // Frequency Envelope button
        showFrequencyEnvButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(showFrequencyEnvButton, toggled, RK_ACT_ARGS(bool pressed), this, showFrequencyEnvelope());
        showFrequencyEnvButton->setPressedImage(RkImage(90, 30, rk_show_freq_env_active_png));
        showFrequencyEnvButton->setUnpressedImage(RkImage(90, 30, rk_show_freq_env_png));
        showFrequencyEnvButton->setPosition(showAmplitudeEnvButton->width(),
                                            (buttomAreaWidget->height() - showFrequencyEnvButton->height()) / 2);
        showFrequencyEnvButton->show();

        // General envelope button
        generalEvelopesButton = new GeonkickButton(buttomAreaWidget);
        generalEvelopesButton->setPressed(true);
        RK_ACT_BIND(generalEvelopesButton, toggled, RK_ACT_ARGS(bool pressed), this, showGeneralEnvelope());
        generalEvelopesButton->setPressedImage(RkImage(90, 30, rk_show_general_envelopes_button_active_png));
        generalEvelopesButton->setUnpressedImage(RkImage(90, 30, rk_show_general_envelopes_button_png));
        generalEvelopesButton->setPosition(buttomAreaWidget->width() - generalEvelopesButton->width(),
                                           (buttomAreaWidget->height() - generalEvelopesButton->height()) / 2);
        generalEvelopesButton->show();

        // Noise envelope button
        noiseEvelopesButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(noiseEvelopesButton, toggled, RK_ACT_ARGS(bool pressed), this, showNoiseEnvelope());
        noiseEvelopesButton->setPressedImage(RkImage(90, 30, rk_show_noise_envelopes_button_active_png));
        noiseEvelopesButton->setUnpressedImage(RkImage(90, 30, rk_show_noise_envelopes_button_png));
        noiseEvelopesButton->setPosition(generalEvelopesButton->x() - noiseEvelopesButton->width(),
                                         (buttomAreaWidget->height() - noiseEvelopesButton->height()) / 2);
        noiseEvelopesButton->show();

        // Oscillator2 envelopes button
        osccillator2EvelopesButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(osccillator2EvelopesButton, toggled, RK_ACT_ARGS(bool pressed), this, showOsc2Envelope());
        osccillator2EvelopesButton->setPressedImage(RkImage(90, 30, rk_show_osc2_envelopes_button_active_png));
        osccillator2EvelopesButton->setUnpressedImage(RkImage(90, 30, rk_show_osc2_envelopes_button_png));
        osccillator2EvelopesButton->setPosition(noiseEvelopesButton->x() - osccillator2EvelopesButton->width(),
                                                (buttomAreaWidget->height() - osccillator2EvelopesButton->height()) / 2);
        osccillator2EvelopesButton->show();

        // Oscillator1 envelopes button
        osccillator1EvelopesButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(osccillator1EvelopesButton, toggled, RK_ACT_ARGS(bool pressed), this, showOsc1Envelope());
        osccillator1EvelopesButton->setPressedImage(RkImage(90, 30, rk_show_osc1_envelopes_button_active_png));
        osccillator1EvelopesButton->setUnpressedImage(RkImage(90, 30, rk_show_osc1_envelopes_button_png));
        osccillator1EvelopesButton->setPosition(osccillator2EvelopesButton->x() - osccillator1EvelopesButton->width(),
                                                (buttomAreaWidget->height() - osccillator1EvelopesButton->height()) / 2);
        osccillator1EvelopesButton->show();
}

Envelope* EnvelopeWidget::getEnvelope(EnvelopeType type)
{
        auto res = envelopes.find(static_cast<int>(type));
        if (res != envelopes.end())
                return res->second.get();
        return nullptr;
}

void EnvelopeWidget::showGeneralEnvelope()
{
        osccillator1EvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showAmplitudeEnvButton->setPressed(false);
        showAmplitudeEnvButton->hide();
        showFrequencyEnvButton->setPressed(false);
        showFrequencyEnvButton->hide();
        drawArea->setEnvelope(getEnvelope(EnvelopeType::General));
}

void EnvelopeWidget::showOsc1Envelope()
{
        generalEvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showFrequencyEnvButton->show();
        showAmplitudeEnvButton->show();
        auto envelope = getEnvelope(EnvelopeType::Oscillator1);
        if (envelope && envelope->type() == Envelope::Type::Amplitude) {
                showAmplitudeEnvButton->setPressed(true);
                showFrequencyEnvButton->setPressed(false);
        } else {
                showAmplitudeEnvButton->setPressed(false);
                showFrequencyEnvButton->setPressed(true);
        }
        drawArea->setEnvelope(envelope);
}

void EnvelopeWidget::showOsc2Envelope()
{
        generalEvelopesButton->setPressed(false);
        osccillator1EvelopesButton->setPressed(false);
        noiseEvelopesButton->setPressed(false);
        showFrequencyEnvButton->show();
        showAmplitudeEnvButton->show();
        auto envelope = getEnvelope(EnvelopeType::Oscillator2);
        if (envelope->type() == Envelope::Type::Amplitude) {
                showAmplitudeEnvButton->setPressed(true);
                showFrequencyEnvButton->setPressed(false);
        } else {
                showAmplitudeEnvButton->setPressed(false);
                showFrequencyEnvButton->setPressed(true);
        }
        drawArea->setEnvelope(envelope);
}

void EnvelopeWidget::showNoiseEnvelope()
{
        generalEvelopesButton->setPressed(false);
        osccillator1EvelopesButton->setPressed(false);
        osccillator2EvelopesButton->setPressed(false);
        showAmplitudeEnvButton->setPressed(false);
        showAmplitudeEnvButton->hide();
        showFrequencyEnvButton->setPressed(false);
        showFrequencyEnvButton->hide();
        drawArea->setEnvelope(getEnvelope(EnvelopeType::Noise));
}

void EnvelopeWidget::showAmplitudeEnvelope()
{
        auto envelope = drawArea->getEnvelope();
        showFrequencyEnvButton->setPressed(false);
        if (envelope && envelope->isSupportedType(Envelope::Type::Amplitude)) {
                envelope->setType(Envelope::Type::Amplitude);
                drawArea->updateArea();
        }
}

void EnvelopeWidget::showFrequencyEnvelope()
{
        auto envelope = drawArea->getEnvelope();
        showAmplitudeEnvButton->setPressed(false);
        if (envelope && envelope->isSupportedType(Envelope::Type::Frequency)) {
                envelope->setType(Envelope::Type::Frequency);
                drawArea->updateArea();
        }
}

void EnvelopeWidget::hideEnvelope(bool b)
{
        drawArea->setHideEnvelope(b);
}
