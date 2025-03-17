/**
 * File name: envelope_widget.cpp
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

#include "envelope_widget.h"
#include "general_envelope.h"
#include "oscillator_envelope.h"
#include "envelope_draw_area.h"
#include "geonkick_button.h"
#include "ViewState.h"
#include "GeonkickModel.h"
#include "OscillatorModel.h"

#include <RkContainer.h>
#include <RkLabel.h>

RK_DECLARE_IMAGE_RC(separator);
RK_DECLARE_IMAGE_RC(layer1_env);
RK_DECLARE_IMAGE_RC(layer1_env_active);
RK_DECLARE_IMAGE_RC(layer2_env);
RK_DECLARE_IMAGE_RC(layer2_env_active);
RK_DECLARE_IMAGE_RC(layer3_env);
RK_DECLARE_IMAGE_RC(layer3_env_active);

EnvelopeWidget::EnvelopeWidget(GeonkickWidget *parent, GeonkickModel *model)
        : GeonkickWidget(parent)
        , drawArea{nullptr}
#ifndef GEONKICK_LIMITED_VERSION
        , layer1Button{nullptr}
        , layer2Button{nullptr}
        , layer3Button{nullptr}
#endif // GEONKICK_LIMITED_VERSION
        , geonkickModel{model}
        , geonkickApi{geonkickModel->api()}
        , oscillators{geonkickModel->getOscillatorModels()}
{
        // Create drawing area.
        drawArea = new EnvelopeWidgetDrawingArea(this, geonkickApi);
        auto rect = drawArea->getDrawingArea();

        // Oscillator1 envelope
        auto oscillator = oscillators[static_cast<int>(OscillatorModel::Type::Oscillator1)];
        auto envelope = std::make_unique<OscillatorEnvelope>(oscillator, rect);
        envelope->setCategory(Envelope::Category::Oscillator1);
        envelopes.insert({static_cast<int>(Envelope::Category::Oscillator1),
                        std::move(envelope)});

        // Oscillator2 envelope
        oscillator = oscillators[static_cast<int>(OscillatorModel::Type::Oscillator2)];
        envelope = std::make_unique<OscillatorEnvelope>(oscillator, rect);
        envelope->setCategory(Envelope::Category::Oscillator2);
        envelopes.insert({static_cast<int>(Envelope::Category::Oscillator2),
                        std::move(envelope)});

        // Oscillator3 envelope
        oscillator = oscillators[static_cast<int>(OscillatorModel::Type::Oscillator3)];
        envelope = std::make_unique<OscillatorEnvelope>(oscillator, rect);
        envelope->setCategory(Envelope::Category::Oscillator3);
        envelopes.insert({static_cast<int>(Envelope::Category::Oscillator3),
                        std::move(envelope)});

        // General envelope
        auto generalEnvelope = std::make_unique<GeneralEnvelope>(geonkickApi, rect);
        generalEnvelope->setCategory(Envelope::Category::InstrumentGlobal);
        envelopes.insert({static_cast<int>(Envelope::Category::InstrumentGlobal),
                        std::move(generalEnvelope)});
#ifndef GEONKICK_LIMITED_VERSION
        createButtomMenu();
#endif // GEONKICK_LIMITED_VERSION
        showEnvelope(Envelope::Category::InstrumentGlobal, Envelope::Type::Amplitude);
        RK_ACT_BIND(viewState(), envelopeChanged,
                    RK_ACT_ARGS(Envelope::Category category, Envelope::Type envelope),
                    this, showEnvelope(category, envelope));

        createZoomInfoLabel();
        createPointInfoLabel();
        drawArea->show();
}

#ifndef GEONKICK_LIMITED_VERSION
void EnvelopeWidget::createButtomMenu()
{
        auto buttomAreaWidget = new GeonkickWidget(drawArea);
        buttomAreaWidget->setBackgroundColor(40, 40, 40);
        buttomAreaWidget->setFixedSize(90, 20);
        buttomAreaWidget->setPosition(55 + drawArea->x(),
                                      drawArea->y() + drawArea->height() - buttomAreaWidget->height() - 6);

        auto menuContainer = new RkContainer(buttomAreaWidget);
        createLayersButtons(buttomAreaWidget);
        menuContainer->addWidget(layer1Button);
        menuContainer->addSpace(5);
        menuContainer->addWidget(layer2Button);
        menuContainer->addSpace(5);
        menuContainer->addWidget(layer3Button);
        buttomAreaWidget->show();
}
#endif // GEONKICK_LIMITED_VERSION

void EnvelopeWidget::createPointInfoLabel()
{
        auto pointInfoLabel = new RkLabel(drawArea);
        pointInfoLabel->setBackgroundColor(drawArea->background());
        pointInfoLabel->setTextColor({200, 200, 200});
        pointInfoLabel->setFixedSize(150, 16);
        pointInfoLabel->setPosition(drawArea->width() - 160, drawArea->height() - pointInfoLabel->height() - 7);
        pointInfoLabel->show();
        RK_ACT_BIND(drawArea, isOverPoint,
                    RK_ACT_ARGS(const std::string &info),
                    pointInfoLabel, setText(info));
}

void EnvelopeWidget::createZoomInfoLabel()
{
        auto zoomInfo = "1:" + Geonkick::doubleToStr(drawArea->getEnvelope()->getZoom(), 0);
        auto zoomInfoLabel = new RkLabel(drawArea, zoomInfo);
        zoomInfoLabel->setBackgroundColor(drawArea->background());
        zoomInfoLabel->setTextColor({180, 180, 180});
        zoomInfoLabel->setFixedSize(30, 16);
        zoomInfoLabel->setPosition(10, drawArea->height() - zoomInfoLabel->height() - 8);
        RK_ACT_BIND(drawArea, zoomUpdated,
                    RK_ACT_ARGS(const std::string &info),
                    zoomInfoLabel, setText("1:" + info));
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

#ifndef GEONKICK_LIMITED_VERSION
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
#endif // GEONKICK_LIMITED_VERSION

void EnvelopeWidget::setLayer(GeonkickApi::Layer layer)
{
#ifndef GEONKICK_LIMITED_VERSION
        layer1Button->setPressed(GeonkickApi::Layer::Layer1 == layer);
        layer2Button->setPressed(GeonkickApi::Layer::Layer2 == layer);
        layer3Button->setPressed(GeonkickApi::Layer::Layer3 == layer);
#endif // GEONKICK_LIMITED_VERSION
        geonkickApi->setLayer(layer);
        action requestUpdateGui();
}

void EnvelopeWidget::updateGui()
{
        for (const auto &envelope: envelopes)
                envelope.second->updateEnvelope();
        drawArea->update();
}

OscillatorModel* EnvelopeWidget::getCurrentOscillator() const
{
        if (auto envelope = drawArea->getEnvelope(); envelope)
                return static_cast<OscillatorEnvelope*>(envelope)->getOscillator();
        return nullptr;
}

void EnvelopeWidget::setPointEditingMode(bool b)
{
        drawArea->setPointEditingMode(b);
}
