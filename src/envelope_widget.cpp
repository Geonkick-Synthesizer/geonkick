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
#include "ViewState.h"

#include <RkContainer.h>
#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(separator);
RK_DECLARE_IMAGE_RC(layer1_env);
RK_DECLARE_IMAGE_RC(layer1_env_active);
RK_DECLARE_IMAGE_RC(layer2_env);
RK_DECLARE_IMAGE_RC(layer2_env_active);
RK_DECLARE_IMAGE_RC(layer3_env);
RK_DECLARE_IMAGE_RC(layer3_env_active);

EnvelopeWidget::EnvelopeWidget(GeonkickWidget *parent,
                               GeonkickApi *api,
                               const std::vector<std::unique_ptr<Oscillator>> &oscillators)
          : GeonkickWidget(parent)
          , drawArea{nullptr}
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
        showEnvelope(Envelope::Category::General, Envelope::Type::Amplitude);

        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    this, showEnvelope(category, envelope));
}

void EnvelopeWidget::createButtomMenu()
{
        auto buttomAreaWidget = new GeonkickWidget(this);
        buttomAreaWidget->setBackgroundColor(40, 40, 40);
        buttomAreaWidget->setFixedSize(90, 20);
        buttomAreaWidget->setPosition(55 + drawArea->x(),
                                      drawArea->y() + drawArea->height() - buttomAreaWidget->height() - 6);
        buttomAreaWidget->show();

        createLayersButtons(buttomAreaWidget);
	menuContainer = new RkContainer(buttomAreaWidget);
	menuContainer->addWidget(layer1Button);
        menuContainer->addSpace(5);
        menuContainer->addWidget(layer2Button);
        menuContainer->addSpace(5);
        menuContainer->addWidget(layer3Button);

}

void EnvelopeWidget::updateKickGraph(std::shared_ptr<RkImage> graphImage)
{
        if (graphImage && !graphImage->isNull())
                drawArea->updateKickGraph(graphImage);
}

Envelope* EnvelopeWidget::getEnvelope(Envelope::Category category)
{
        if (auto res = envelopes.find(static_cast<int>(category)); res != envelopes.end())
                return res->second.get();
        return nullptr;
}

void EnvelopeWidget::showEnvelope(Envelope::Category category, Envelope::Type type)
{
        if (auto envelope = getEnvelope(category); envelope) {
                envelope->setType(type);
                drawArea->setEnvelope(envelope);
        }
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
                               RkButton::State::Unpressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_env_active)),
                               RkButton::State::Pressed);
        layer1Button->setImage(RkImage(layer1Button->size(), RK_IMAGE_RC(layer1_env_active)),
                               RkButton::State::UnpressedHover);
        layer1Button->setCheckable(true);
        layer1Button->setPressed(true);
        RK_ACT_BIND(layer1Button, toggled, RK_ACT_ARGS(bool b),
                    this, setLayer(GeonkickApi::Layer::Layer1));

        layer2Button = new GeonkickButton(buttomAreaWidget);
        layer2Button->setBackgroundColor(buttomAreaWidget->background());
        layer2Button->setSize(24, 18);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_env)),
                               RkButton::State::Unpressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_env_active)),
                               RkButton::State::Pressed);
        layer2Button->setImage(RkImage(layer2Button->size(), RK_IMAGE_RC(layer2_env_active)),
                               RkButton::State::UnpressedHover);
        layer2Button->setCheckable(true);
        RK_ACT_BIND(layer2Button, toggled, RK_ACT_ARGS(bool b),
                    this, setLayer(GeonkickApi::Layer::Layer2));

        layer3Button = new GeonkickButton(buttomAreaWidget);
        layer3Button->setBackgroundColor(buttomAreaWidget->background());
        layer3Button->setSize(24, 18);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_env)),
                               RkButton::State::Unpressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_env_active)),
                               RkButton::State::Pressed);
        layer3Button->setImage(RkImage(layer3Button->size(), RK_IMAGE_RC(layer3_env_active)),
                               RkButton::State::UnpressedHover);
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
        for (const auto &envelope: envelopes)
                envelope.second->updatePoints();
        drawArea->update();
}

Oscillator* EnvelopeWidget::getCurrentOscillator() const
{
        if (auto envelope = drawArea->getEnvelope(); envelope)
                return static_cast<OscillatorEnvelope*>(envelope)->getOscillator();
        return nullptr;
}
