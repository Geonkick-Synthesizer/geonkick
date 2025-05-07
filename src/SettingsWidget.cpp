/**
 * File name: SettingsWidget.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor
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

#include "SettingsWidget.h"
#include "geonkick_api.h"
#include "GeonkickConfig.h"

#include "RkContainer.h"
#include "RkLabel.h"
#include "RkSpinBox.h"
#include "RkImage.h"
#include "RkPainter.h"

RK_DECLARE_IMAGE_RC(close_button);
RK_DECLARE_IMAGE_RC(close_button_hover);

SettingsCheckBox::SettingsCheckBox(GeonkickWidget *parent, const RkSize &size)
        : GeonkickButton(parent)
{
        setSize(size);
        setBorderWidth(1);
        setBorderColor(55, 55, 55);
        setBackgroundColor({255, 255, 255});
        setCheckable(true);
        RkImage img(size);
        RkPainter painter(&img);
        painter.fillRect(RkRect(0, 0, img.width(), img.height()), {100, 100, 100});
        setImage(img, RkButton::State::Unpressed);
        painter.fillRect(RkRect(3, 3, img.width() - 6, img.height() - 6), {55, 55, 55});
        setImage(img, RkButton::State::Pressed);
        painter.fillRect(RkRect(0, 0, img.width(), img.height()), {100, 100, 100});
        painter.fillRect(RkRect(3, 3, img.width() - 6, img.height() - 6), {65, 65, 65});
        setImage(img, RkButton::State::PressedHover);
        painter.fillRect(RkRect(0, 0, img.width(), img.height()), {90, 90, 90});
        setImage(img, RkButton::State::UnpressedHover);
        show();
}

SettingsWidget::SettingsWidget(GeonkickWidget *parent, GeonkickApi* api)
        : GeonkickWidget(parent, Rk::WidgetFlags::Popup)
        , geonkickApi{api}
{
        setFixedSize({450, 250});
        setBackgroundColor({68, 68, 70});
        setBorderWidth(2);
        setBorderColor(80, 80, 80);

        auto closeButton = new GeonkickButton(this);
        closeButton->setSize({16, 16});
        closeButton->setPosition(width() - 5 - closeButton->width(), 5);
        closeButton->setImage(RkImage(closeButton->size(), RK_IMAGE_RC(close_button)),
                               RkButton::State::Unpressed);
        closeButton->setImage(RkImage(closeButton->size(), RK_IMAGE_RC(close_button_hover)),
                               RkButton::State::UnpressedHover);
        closeButton->setImage(RkImage(closeButton->size(), RK_IMAGE_RC(close_button)),
                               RkButton::State::Pressed);
        RK_ACT_BINDL(closeButton, pressed, RK_ACT_ARGS(), [=, this]() { close(); });

        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->setPosition({10, 10});
        mainContainer->setSize(size() - RkSize(20, 20));

        auto titleLabel = new RkLabel(this, "Settings");
        titleLabel->setSize({100, 20});
        titleLabel->setBackgroundColor(background());
        titleLabel->setTextColor({200, 200, 200});

        RkFont font;
        font.setSize(12);
        font.setWeight(RkFont::Weight::Bold);
        titleLabel->setFont(font);
        titleLabel->show();
        mainContainer->addWidget(titleLabel);
        mainContainer->addSpace(10);

        createMidiChannelSettings(mainContainer);
        mainContainer->addSpace(5);
        createScaleGUISettings(mainContainer);
        mainContainer->addSpace(5);
        createShowSidebarSettings(mainContainer);
}

void SettingsWidget::createMidiChannelSettings(RkContainer *container)
{
        auto horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({container->size().width(), 30});
        auto forceMidiCheckBox = new SettingsCheckBox(this, {16, 16});
        forceMidiCheckBox->setPressed(GeonkickConfig().isMidiChannelForced());
        RK_ACT_BINDL(forceMidiCheckBox,
                     toggled,
                     RK_ACT_ARGS(bool b),
                     [=, this](bool b){
                             GeonkickConfig cfg;
                             cfg.setMidiChannelForced(b);
                             geonkickApi->forceMidiChannel(cfg.getMidiChannel(),
                                                           cfg.isMidiChannelForced());
                             cfg.save();});
        horizontalContainer->addWidget(forceMidiCheckBox);
        horizontalContainer->addSpace(3);

        auto label = new RkLabel(this, "Force all midi channels to ");
        label->setSize({130, 20});
        label->setTextColor({255, 255, 255});
        label->setBackgroundColor(background());
        label->show();
        horizontalContainer->addWidget(label);

        // MIDI channels spin box.
        auto midiChannelSpinBox = new RkSpinBox(this);
        midiChannelSpinBox->setTextColor({250, 250, 250});
        midiChannelSpinBox->setBackgroundColor({60, 57, 57});
        midiChannelSpinBox->upControl()->setBackgroundColor({50, 47, 47});
        midiChannelSpinBox->upControl()->setTextColor({100, 100, 100});
        midiChannelSpinBox->downControl()->setBackgroundColor({50, 47, 47});
        midiChannelSpinBox->downControl()->setTextColor({100, 100, 100});
        midiChannelSpinBox->setSize({50, 20});
        midiChannelSpinBox->addItem("Any");
        for (size_t i = 0; i < geonkickApi->numberOfMidiChannels(); i++)
                midiChannelSpinBox->addItem(std::to_string(i + 1));
        midiChannelSpinBox->setCurrentIndex(GeonkickConfig().getMidiChannel() + 1);
        RK_ACT_BINDL(midiChannelSpinBox,
                     currentIndexChanged,
                     RK_ACT_ARGS(int index),
                     [=, this](int index){
                             GeonkickConfig cfg;
                             cfg.setMidiChannel(index - 1);
                             geonkickApi->forceMidiChannel(index - 1,
                                                           cfg.isMidiChannelForced());
                             cfg.save(); });
        midiChannelSpinBox->show();
        horizontalContainer->addSpace(3);
        horizontalContainer->addWidget(midiChannelSpinBox);
        container->addContainer(horizontalContainer);
}

void SettingsWidget::createScaleGUISettings(RkContainer *container)
{
        GeonkickConfig config;
        auto buttonsContainer = new RkContainer(this);
        buttonsContainer->setSize({container->size().width(), 16});
        auto label = new RkLabel(this, "Scale UI: ");
        label->setSize(50, 16);
        label->setTextColor({255, 255, 255});
        label->setBackgroundColor(background());
        label->show();
        buttonsContainer->addWidget(label);

        buttonsContainer->addSpace(3);
        auto noScaleCheckBox = new SettingsCheckBox(this, {16, 16});
        noScaleCheckBox->setPressed(config.getScaleFactor() == 1.0);
        buttonsContainer->addWidget(noScaleCheckBox);
        buttonsContainer->addSpace(2);
        auto xNoScale = new RkLabel(this, "x1");
        xNoScale->setTextColor({255, 255, 255});
        xNoScale->setBackgroundColor(background());
        xNoScale->setSize(16, 16);
        buttonsContainer->addWidget(xNoScale);

        buttonsContainer->addSpace(7);
        auto oneHalfCheckBox = new SettingsCheckBox(this, {16, 16});
        oneHalfCheckBox->setPressed(config.getScaleFactor() == 1.5);
        buttonsContainer->addWidget(oneHalfCheckBox);
        buttonsContainer->addSpace(2);
        auto xOneHalfScale = new RkLabel(this, "x1.5");
        xOneHalfScale->setTextColor({255, 255, 255});
        xOneHalfScale->setBackgroundColor(background());
        xOneHalfScale->setSize(22, 16);
        buttonsContainer->addWidget(xOneHalfScale);

        buttonsContainer->addSpace(7);
        auto doubleScaleCheckBox = new SettingsCheckBox(this, {16, 16});
        doubleScaleCheckBox->setPressed(config.getScaleFactor() == 2.0);
        buttonsContainer->addWidget(doubleScaleCheckBox);
        buttonsContainer->addSpace(2);
        auto xDoubleScale = new RkLabel(this, "x2");
        xDoubleScale->setTextColor({255, 255, 255});
        xDoubleScale->setBackgroundColor(background());
        xDoubleScale->setText("x2");
        xDoubleScale->setSize(16, 16);
        buttonsContainer->addWidget(xDoubleScale);

        RK_ACT_BINDL(noScaleCheckBox, toggled,  RK_ACT_ARGS(bool b),
                     [=, this](bool b){
                             oneHalfCheckBox->setPressed(false);
                             doubleScaleCheckBox->setPressed(false);
                             GeonkickConfig cfg;
                             cfg.setScaleFactor(1.0);
                             cfg.save();
                     });
        RK_ACT_BINDL(oneHalfCheckBox, toggled,  RK_ACT_ARGS(bool b),
                     [=, this](bool b){
                             noScaleCheckBox->setPressed(false);
                             doubleScaleCheckBox->setPressed(false);
                             GeonkickConfig cfg;
                             cfg.setScaleFactor(1.5);
                             cfg.save();
                     });
        RK_ACT_BINDL(doubleScaleCheckBox, toggled,  RK_ACT_ARGS(bool b),
                     [=, this](bool b){
                             noScaleCheckBox->setPressed(false);
                             oneHalfCheckBox->setPressed(false);
                             GeonkickConfig cfg;
                             cfg.setScaleFactor(2.0);
                             cfg.save();
                     });

        container->addContainer(buttonsContainer);
}

void SettingsWidget::createShowSidebarSettings(RkContainer *container)
{
        container->addSpace(5);
        GeonkickConfig config;
        auto hoziontalContainer = new RkContainer(this);
        hoziontalContainer->setSize({container->size().width(), 16});
        auto label = new RkLabel(this, "Show sidebar: ");
        label->setSize(70, 16);
        label->setTextColor({255, 255, 255});
        label->setBackgroundColor(background());
        label->show();
        hoziontalContainer->addWidget(label, Rk::Alignment::AlignLeft);

        hoziontalContainer->addSpace(3);
        auto showSidebar = new SettingsCheckBox(this, {16, 16});
        showSidebar->setPressed(config.isShowSidebar());
        hoziontalContainer->addWidget(showSidebar, Rk::Alignment::AlignLeft);
        RK_ACT_BINDL(showSidebar, toggled,  RK_ACT_ARGS(bool b),
                     [=, this](bool b){
                             GeonkickConfig cfg;
                             cfg.setShowSidebar(b);
                             cfg.save();
                     });
        container->addContainer(hoziontalContainer);
}


