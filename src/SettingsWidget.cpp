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
#include "geonkick_button.h"
#include "RkImage.h"
#include "RkPainter.h"

SettingsWidget::SettingsWidget(GeonkickWidget *parent, GeonkickApi* api)
        : GeonkickWidget(parent)
        , geonkickApi{api}
{
        setFixedSize(parent->size());
        auto mainContianer = new RkContainer(this);
        mainContianer->setSize({width(), 30});

        createMidiChannelSettings(mainContianer);
        
        auto label = new RkLabel(this, "Force all midi channels to ");
        label->setSize({130, 20});
        label->setTextColor({255, 255, 255});
        label->setBackgroundColor(background());
        label->show();
        mainContianer->addWidget(label);

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
        mainContianer->addSpace(5);
        mainContianer->addWidget(midiChannelSpinBox);
}

void SettingsWidget::createMidiChannelSettings(RkContainer *container)
{
        auto forceMidiCheckBox = new GeonkickButton(this);
        forceMidiCheckBox->setBorderWidth(1);
        forceMidiCheckBox->setPressed(GeonkickConfig().isMidiChannelForced());
        forceMidiCheckBox->setBorderColor({55, 55, 55});
        forceMidiCheckBox->setBackgroundColor({255, 255, 255});
        forceMidiCheckBox->setCheckable(true);
        forceMidiCheckBox->setFixedSize(16, 16);
        RkImage img(forceMidiCheckBox->size());
        RkPainter painter(&img);
        painter.fillRect(RkRect(0, 0, img.width(), img.height()), {100, 100, 100});
        forceMidiCheckBox->setImage(img, RkButton::State::Unpressed);
        painter.fillRect(RkRect(3, 3, img.width() - 6, img.height() - 6), {55, 55, 55});
        forceMidiCheckBox->setImage(img, RkButton::State::Pressed);
        painter.fillRect(RkRect(0, 0, img.width(), img.height()), {100, 100, 100});
        painter.fillRect(RkRect(3, 3, img.width() - 6, img.height() - 6), {65, 65, 65});
        forceMidiCheckBox->setImage(img, RkButton::State::PressedHover);
        painter.fillRect(RkRect(0, 0, img.width(), img.height()), {90, 90, 90});
        forceMidiCheckBox->setImage(img, RkButton::State::UnpressedHover);
        forceMidiCheckBox->show();
        RK_ACT_BINDL(forceMidiCheckBox,
                     toggled,
                     RK_ACT_ARGS(bool b),
                     [=, this](bool b){
                             GeonkickConfig cfg;
                             cfg.setMidiChannelForced(b);
                             geonkickApi->forceMidiChannel(cfg.getMidiChannel(),
                                                           cfg.isMidiChannelForced());
                             cfg.save();});
        container->addWidget(forceMidiCheckBox);
        container->addSpace(3);
}
