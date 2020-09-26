/**
 * File name: envelope_widget.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include <RkContainer.h>
#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(separator);
RK_DECLARE_IMAGE_RC(show_ampl_env_active);
RK_DECLARE_IMAGE_RC(show_ampl_env);
RK_DECLARE_IMAGE_RC(show_ampl_env_hover);
RK_DECLARE_IMAGE_RC(show_freq_env_active);
RK_DECLARE_IMAGE_RC(show_freq_env);
RK_DECLARE_IMAGE_RC(show_freq_env_hover);
RK_DECLARE_IMAGE_RC(show_filter_env);
RK_DECLARE_IMAGE_RC(show_filter_env_active);
RK_DECLARE_IMAGE_RC(show_filter_env_hover);
RK_DECLARE_IMAGE_RC(layer1_env);
RK_DECLARE_IMAGE_RC(layer1_env_active);
RK_DECLARE_IMAGE_RC(layer2_env);
RK_DECLARE_IMAGE_RC(layer2_env_active);
RK_DECLARE_IMAGE_RC(layer3_env);
RK_DECLARE_IMAGE_RC(layer3_env_active);
RK_DECLARE_IMAGE_RC(show_osc1_envelopes_button_active);
RK_DECLARE_IMAGE_RC(show_osc1_envelopes_button);
RK_DECLARE_IMAGE_RC(show_osc2_envelopes_button_active);
RK_DECLARE_IMAGE_RC(show_osc2_envelopes_button);
RK_DECLARE_IMAGE_RC(show_noise_envelopes_button_active);
RK_DECLARE_IMAGE_RC(show_noise_envelopes_button);
RK_DECLARE_IMAGE_RC(show_general_envelopes_button_active);
RK_DECLARE_IMAGE_RC(show_general_envelopes_button);

EnvelopeWidget::EnvelopeWidget(GeonkickWidget *parent,
                               GeonkickApi *api,
                               const std::vector<std::unique_ptr<Oscillator>> &oscillators)
          : GeonkickWidget(parent)
          , drawArea{nullptr}
          , showAmplitudeEnvButton{nullptr}
          , showFrequencyEnvButton{nullptr}
          , frequencySeparator {nullptr}
	  , showFilterEnvButton{nullptr}
          , oscillator1EvelopesButton{nullptr}
          , oscillator2EvelopesButton{nullptr}
          , noiseEvelopesButton{nullptr}
          , generalEvelopesButton{nullptr}
          , layer1Button{nullptr}
          , layer2Button{nullptr}
          , layer3Button{nullptr}
          , geonkickApi{api}
{
        // Create drawing area.
        drawArea = new EnvelopeWidgetDrawingArea(this, geonkickApi);
        drawArea->show();

        auto rect = drawArea->getDrawingArea();

        // Oscillator1 envelope
        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)].get();
        auto envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<OscillatorEnvelope>(oscillator, rect));
        envelopes.insert({static_cast<int>(Envelope::Category::Oscillator1), envelope});
        envelope->setCategory(Envelope::Category::Oscillator1);

        // Oscillator2 envelope
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)].get();
        envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<OscillatorEnvelope>(oscillator, rect));
        envelopes.insert({static_cast<int>(Envelope::Category::Oscillator2), envelope});
        envelope->setCategory(Envelope::Category::Oscillator2);

        // Noise envelope
        oscillator = oscillators[static_cast<int>(Oscillator::Type::Noise)].get();
        envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<OscillatorEnvelope>(oscillator, rect));
        envelopes.insert({static_cast<int>(Envelope::Category::Noise), envelope});
        envelope->setCategory(Envelope::Category::Noise);

        // General nevelope
        envelope = std::dynamic_pointer_cast<Envelope>(std::make_shared<GeneralEnvelope>(geonkickApi, rect));
        envelopes.insert({static_cast<int>(Envelope::Category::General), envelope});
        envelope->setCategory(Envelope::Category::General);
        createButtomMenu();
        showEnvelope(Envelope::Category::General);
}

void EnvelopeWidget::createButtomMenu()
{
        auto buttomAreaWidget = new GeonkickWidget(this);
        buttomAreaWidget->setPosition(drawArea->x(), drawArea->y() + drawArea->height() + 3);
        buttomAreaWidget->setFixedSize(drawArea->width(), 20);
        buttomAreaWidget->show();

        // Amplitude Envelope button
        showAmplitudeEnvButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(showAmplitudeEnvButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showEnvelopeType(Envelope::Type::Amplitude));
        showAmplitudeEnvButton->setImage(RkImage(51, 18, RK_IMAGE_RC(show_ampl_env_active)),
                                         RkButton::ButtonImage::ImagePressed);
        showAmplitudeEnvButton->setImage(RkImage(51, 18, RK_IMAGE_RC(show_ampl_env)),
                                         RkButton::ButtonImage::ImageUnpressed);
        showAmplitudeEnvButton->setImage(RkImage(51, 18, RK_IMAGE_RC(show_ampl_env_active)),
                                         RkButton::ButtonImage::ImageUnpressedHover);
        showAmplitudeEnvButton->show();
        showAmplitudeEnvButton->setPressed(true);

        // Frequency Envelope button
        showFrequencyEnvButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(showFrequencyEnvButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showEnvelopeType(Envelope::Type::Frequency));
        showFrequencyEnvButton->setImage(RkImage(52, 18, RK_IMAGE_RC(show_freq_env_active)),
                                         RkButton::ButtonImage::ImagePressed);
        showFrequencyEnvButton->setImage(RkImage(52, 18, RK_IMAGE_RC(show_freq_env)),
                                         RkButton::ButtonImage::ImageUnpressed);
        showFrequencyEnvButton->setImage(RkImage(52, 18, RK_IMAGE_RC(show_freq_env_active)),
                                         RkButton::ButtonImage::ImageUnpressedHover);
        showFrequencyEnvButton->show();

        // Filter Envelope.
        showFilterEnvButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(showFilterEnvButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showEnvelopeType(Envelope::Type::FilterCutOff));
        showFilterEnvButton->setImage(RkImage(29, 18, RK_IMAGE_RC(show_filter_env_active)),
                                      RkButton::ButtonImage::ImagePressed);
        showFilterEnvButton->setImage(RkImage(29, 18, RK_IMAGE_RC(show_filter_env)),
                                      RkButton::ButtonImage::ImageUnpressed);
        showFilterEnvButton->setImage(RkImage(29, 18, RK_IMAGE_RC(show_filter_env_active)),
                                      RkButton::ButtonImage::ImageUnpressedHover);
        showFilterEnvButton->show();

        // General envelope button
        generalEvelopesButton = new GeonkickButton(buttomAreaWidget);
        generalEvelopesButton->setPressed(true);
        RK_ACT_BIND(generalEvelopesButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showEnvelope(Envelope::Category::General));
        generalEvelopesButton->setImage(RkImage(39, 18, RK_IMAGE_RC(show_general_envelopes_button_active)),
                                        RkButton::ButtonImage::ImagePressed);
        generalEvelopesButton->setImage(RkImage(39, 18, RK_IMAGE_RC(show_general_envelopes_button)),
                                        RkButton::ButtonImage::ImageUnpressed);
        generalEvelopesButton->setImage(RkImage(39, 18, RK_IMAGE_RC(show_general_envelopes_button_active)),
                                        RkButton::ButtonImage::ImageUnpressedHover);

        generalEvelopesButton->show();

        // Noise envelope button
        noiseEvelopesButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(noiseEvelopesButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showEnvelope(Envelope::Category::Noise));
        noiseEvelopesButton->setImage(RkImage(26, 18, RK_IMAGE_RC(show_noise_envelopes_button_active)),
                                      RkButton::ButtonImage::ImagePressed);
        noiseEvelopesButton->setImage(RkImage(26, 18, RK_IMAGE_RC(show_noise_envelopes_button)),
                                      RkButton::ButtonImage::ImageUnpressed);
        noiseEvelopesButton->setImage(RkImage(26, 18, RK_IMAGE_RC(show_noise_envelopes_button_active)),
                                      RkButton::ButtonImage::ImageUnpressedHover);
        noiseEvelopesButton->show();

        // Oscillator2 envelopes button
        oscillator2EvelopesButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(oscillator2EvelopesButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showEnvelope(Envelope::Category::Oscillator2));
        oscillator2EvelopesButton->setImage(RkImage(54, 18, RK_IMAGE_RC(show_osc2_envelopes_button_active)),
                                            RkButton::ButtonImage::ImagePressed);
        oscillator2EvelopesButton->setImage(RkImage(54, 18, RK_IMAGE_RC(show_osc2_envelopes_button)),
                                            RkButton::ButtonImage::ImageUnpressed);
        oscillator2EvelopesButton->setImage(RkImage(54, 18, RK_IMAGE_RC(show_osc2_envelopes_button_active)),
                                            RkButton::ButtonImage::ImageUnpressedHover);
        oscillator2EvelopesButton->show();

        // Oscillator1 envelopes button
        oscillator1EvelopesButton = new GeonkickButton(buttomAreaWidget);
        RK_ACT_BIND(oscillator1EvelopesButton,
                    toggled,
                    RK_ACT_ARGS(bool pressed),
                    this,
                    showEnvelope(Envelope::Category::Oscillator1));
        oscillator1EvelopesButton->setImage(RkImage(53, 18, RK_IMAGE_RC(show_osc1_envelopes_button_active)),
                                            RkButton::ButtonImage::ImagePressed);
        oscillator1EvelopesButton->setImage(RkImage(53, 18, RK_IMAGE_RC(show_osc1_envelopes_button)),
                                            RkButton::ButtonImage::ImageUnpressed);
        oscillator1EvelopesButton->setImage(RkImage(53, 18, RK_IMAGE_RC(show_osc1_envelopes_button_active)),
                                            RkButton::ButtonImage::ImageUnpressedHover);
        oscillator1EvelopesButton->show();
        createLayersButtons(buttomAreaWidget);

	menuContainer = new RkContainer(buttomAreaWidget);
        menuContainer->addSpace(10);
	menuContainer->addWidget(showAmplitudeEnvButton);
        addSeparator(buttomAreaWidget, menuContainer);
	menuContainer->addWidget(showFrequencyEnvButton);
        frequencySeparator = addSeparator(buttomAreaWidget, menuContainer);
	menuContainer->addWidget(showFilterEnvButton);

        menuContainer->addSpace(10, Rk::Alignment::AlignRight);
	menuContainer->addWidget(generalEvelopesButton, Rk::Alignment::AlignRight);
        addSeparator(buttomAreaWidget, menuContainer, Rk::Alignment::AlignRight);
	menuContainer->addWidget(noiseEvelopesButton, Rk::Alignment::AlignRight);
        addSeparator(buttomAreaWidget, menuContainer, Rk::Alignment::AlignRight);
	menuContainer->addWidget(oscillator2EvelopesButton, Rk::Alignment::AlignRight);
        addSeparator(buttomAreaWidget, menuContainer, Rk::Alignment::AlignRight);
	menuContainer->addWidget(oscillator1EvelopesButton, Rk::Alignment::AlignRight);
        addSeparator(buttomAreaWidget, menuContainer, Rk::Alignment::AlignRight, 5);
	menuContainer->addWidget(layer3Button, Rk::Alignment::AlignRight);
        addSeparator(buttomAreaWidget, menuContainer, Rk::Alignment::AlignRight, 5);
        menuContainer->addWidget(layer2Button, Rk::Alignment::AlignRight);
        addSeparator(buttomAreaWidget, menuContainer, Rk::Alignment::AlignRight, 5);
        menuContainer->addWidget(layer1Button, Rk::Alignment::AlignRight);

}

RkWidget* EnvelopeWidget::addSeparator(RkWidget *parent,
                                       RkContainer *layout,
                                       Rk::Alignment alignment,
                                       int margin)
{
        layout->addSpace(margin, alignment);
        auto separator = new RkLabel(parent);
        separator->setBackgroundColor(parent->background());
        separator->setSize(2, 21);
        separator->setImage(RkImage(separator->size(), RK_IMAGE_RC(separator)));
        separator->show();
        layout->addWidget(separator, alignment);
        layout->addSpace(margin, alignment);
        return separator;
}

void EnvelopeWidget::updateKickGraph(std::shared_ptr<RkImage> graphImage)
{
        if (graphImage && !graphImage->isNull())
                drawArea->updateKickGraph(graphImage);
}

Envelope* EnvelopeWidget::getEnvelope(Envelope::Category category)
{
        auto res = envelopes.find(static_cast<int>(category));
        if (res != envelopes.end())
                return res->second.get();
        return nullptr;
}

void EnvelopeWidget::showEnvelope(Envelope::Category category)
{
        generalEvelopesButton->setPressed(category == Envelope::Category::General);
        oscillator1EvelopesButton->setPressed(category == Envelope::Category::Oscillator1);
        oscillator2EvelopesButton->setPressed(category == Envelope::Category::Oscillator2);
        noiseEvelopesButton->setPressed(category == Envelope::Category::Noise);
        auto envelope = getEnvelope(category);
        if (envelope) {
                drawArea->setEnvelope(envelope);
                showEnvelopeType(envelope->type());
        }

}

void EnvelopeWidget::showEnvelopeType(Envelope::Type type)
{
        if (type == Envelope::Type::DistortionVolume
            || type == Envelope::Type::DistortionDrive) {
                auto envelope = getEnvelope(Envelope::Category::General);
                generalEvelopesButton->setPressed(true);
                if (envelope)
                        drawArea->setEnvelope(envelope);
        }

        showAmplitudeEnvButton->setPressed(type == Envelope::Type::Amplitude);
        showFilterEnvButton->setPressed(type == Envelope::Type::FilterCutOff);
        showFrequencyEnvButton->setPressed(type == Envelope::Type::Frequency);
        auto envelope = drawArea->getEnvelope();
	showFrequencyEnvButton->show(envelope->isSupportedType(Envelope::Type::Frequency));
        menuContainer->at(6)->hide(!envelope->isSupportedType(Envelope::Type::Frequency));
        menuContainer->at(7)->hide(!envelope->isSupportedType(Envelope::Type::Frequency));
        menuContainer->at(8)->hide(!envelope->isSupportedType(Envelope::Type::Frequency));
	menuContainer->update();
        if (envelope)
                envelope->setType(type);
        drawArea->update();
        action envelopeTypeSelected(type);
}

void EnvelopeWidget::hideEnvelope(bool b)
{
        drawArea->setHideEnvelope(b);
}

void EnvelopeWidget::createLayersButtons(GeonkickWidget *buttomAreaWidget)
{
        layer1Button = new GeonkickButton(buttomAreaWidget);
        layer1Button->setBackgroundColor(buttomAreaWidget->background());
        layer1Button->setSize(24, 18);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_env)),
                               RkButton::ButtonImage::ImageUnpressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_env_active)),
                               RkButton::ButtonImage::ImagePressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_env_active)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        layer1Button->setCheckable(true);
        layer1Button->setPressed(true);
        RK_ACT_BIND(layer1Button, toggled, RK_ACT_ARGS(bool b),
                    this, setLayer(GeonkickApi::Layer::Layer1));

        layer2Button = new GeonkickButton(buttomAreaWidget);
        layer2Button->setBackgroundColor(buttomAreaWidget->background());
        layer2Button->setSize(24, 18);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_env)),
                               RkButton::ButtonImage::ImageUnpressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_env_active)),
                               RkButton::ButtonImage::ImagePressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_env_active)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        layer2Button->setCheckable(true);
        RK_ACT_BIND(layer2Button, toggled, RK_ACT_ARGS(bool b),
                    this, setLayer(GeonkickApi::Layer::Layer2));

        layer3Button = new GeonkickButton(buttomAreaWidget);
        layer3Button->setBackgroundColor(buttomAreaWidget->background());
        layer3Button->setSize(24, 18);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_env)),
                               RkButton::ButtonImage::ImageUnpressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_env_active)),
                               RkButton::ButtonImage::ImagePressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_env_active)),
                               RkButton::ButtonImage::ImageUnpressedHover);
        layer3Button->setCheckable(true);
        RK_ACT_BIND(layer3Button, toggled, RK_ACT_ARGS(bool b),
                    this, setLayer(GeonkickApi::Layer::Layer3));
}

void EnvelopeWidget::setLayer(GeonkickApi::Layer layer)
{
        layer1Button->setPressed(GeonkickApi::Layer::Layer1 == layer);
        layer2Button->setPressed(GeonkickApi::Layer::Layer2 == layer);
        layer3Button->setPressed(GeonkickApi::Layer::Layer3 == layer);
        geonkickApi->setLayer(layer);
        action requestUpdateGui();
}

void EnvelopeWidget::updateGui()
{
        for (const auto &envelope: envelopes) {
                if (envelope.second->isSupportedType(Envelope::Type::Amplitude))
                        envelope.second->updatePoints();
                if (envelope.second->isSupportedType(Envelope::Type::Frequency))
                        envelope.second->updatePoints();
		if (envelope.second->isSupportedType(Envelope::Type::FilterCutOff))
                        envelope.second->updatePoints();
        }
        drawArea->update();
}

Oscillator* EnvelopeWidget::getCurrentOscillator() const
{
        auto envelope = drawArea->getEnvelope();
        if (envelope)
                return static_cast<OscillatorEnvelope*>(envelope)->getOscillator();
        return nullptr;
}
