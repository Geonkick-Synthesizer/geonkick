/**
 * File name: instrument_view.cpp
 * Project: Geonkick (A percussive synthesizer)
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

#include "kit_widget.h"
#include "instrument_view.h"
#include "instrument_model.h"
#include "geonkick_slider.h"
#include "MidiKeyWidget.h"
#include "geonkick_button.h"

#include <RkEvent.h>
#include <RkPainter.h>
#include <RkLineEdit.h>
#include <RkButton.h>
#include <RkContainer.h>
#include <RkProgressBar.h>

RK_DECLARE_IMAGE_RC(mute);
RK_DECLARE_IMAGE_RC(mute_hover);
RK_DECLARE_IMAGE_RC(mute_on);
RK_DECLARE_IMAGE_RC(solo);
RK_DECLARE_IMAGE_RC(solo_hover);
RK_DECLARE_IMAGE_RC(solo_on);
RK_DECLARE_IMAGE_RC(per_play);
RK_DECLARE_IMAGE_RC(per_play_hover);
RK_DECLARE_IMAGE_RC(per_play_on);
RK_DECLARE_IMAGE_RC(remove_per_button);
RK_DECLARE_IMAGE_RC(remove_per_button_hover);
RK_DECLARE_IMAGE_RC(remove_per_button_on);
RK_DECLARE_IMAGE_RC(copy_per_button);
RK_DECLARE_IMAGE_RC(copy_per_button_hover);
RK_DECLARE_IMAGE_RC(copy_per_button_on);
RK_DECLARE_IMAGE_RC(kit_midi_on);
RK_DECLARE_IMAGE_RC(kit_midi_off);
RK_DECLARE_IMAGE_RC(kit_midi_hover);

InstrumentLimiter::InstrumentLimiter(GeonkickWidget *parent)
        : GeonkickSlider(parent)
        , levelerValue{0}
{
}

void InstrumentLimiter::setLeveler(int value)
{
        levelerValue = value;
        if (value > 100)
                levelerValue = 100;
        else if (value < 0)
                levelerValue = 0;
        else
                levelerValue = value;
        update();
}

int InstrumentLimiter::getLeveler() const
{
        return levelerValue;
}

void InstrumentLimiter::paintWidget(RkPaintEvent *event)
{
        GeonkickSlider::paintWidget(event);
        RkPainter painter(this);
        double value = (static_cast<double>(levelerValue) / 100) * (width() - 2);
        RkColor color(40, 200, 40);
        if (levelerValue > 0) {
                if (getOrientation() == GeonkickSlider::Orientation::Horizontal)
                        painter.fillRect(RkRect(1, 2,
                                                value, height() - 4), color);
                else
                        painter.fillRect(RkRect(height() - 2 - value, 2,
                                                width() - 4, value), color);
        }
}

KitInstrumentView::KitInstrumentView(KitWidget *parent,
                                     InstrumentModel *model)
        : GeonkickWidget(parent)
        , parentView{parent}
        , instrumentModel{model}
        , nameWidth{100}
        , channelWidth{30}
        , editInstrument{nullptr}
        , keyButton{nullptr}
        , copyButton{nullptr}
        , removeButton{nullptr}
        , playButton{nullptr}
        , muteButton{nullptr}
        , soloButton{nullptr}
        , instrumentLimiter{nullptr}
{
        setSize(parent->width(), 20);
        createView();
        setModel(model);
}

KitInstrumentView::InstrumentIndex KitInstrumentView::index() const
{
        if (instrumentModel)
                return instrumentModel->index();
        return -1;
}

void KitInstrumentView::createView()
{
        auto instrumentContainer = new RkContainer(this);
        instrumentContainer->setSize(size());
        instrumentContainer->setHiddenTakesPlace();
        instrumentContainer->addSpace(nameWidth + instrumentModel->numberOfChannels() * channelWidth + 10);

        // Midi key button
        keyButton = new GeonkickButton(this);
        keyButton->setTextColor({250, 250, 250});
        keyButton->setType(RkButton::ButtonType::ButtonUncheckable);
        keyButton->setSize(30, 20);
        keyButton->setImage(RkImage(keyButton->size(), RK_IMAGE_RC(kit_midi_off)),
                            RkButton::State::Unpressed);
        keyButton->setImage(RkImage(keyButton->size(), RK_IMAGE_RC(kit_midi_on)),
                                 RkButton::State::Pressed);
        keyButton->setImage(RkImage(keyButton->size(), RK_IMAGE_RC(kit_midi_hover)),
                            RkButton::State::UnpressedHover);
        RK_ACT_BIND(keyButton, toggled, RK_ACT_ARGS(bool pressed), this, showMidiPopup());
        instrumentContainer->addWidget(keyButton);
        instrumentContainer->addSpace(10);

        // Remove button
        removeButton = new RkButton(this);
        removeButton->setType(RkButton::ButtonType::ButtonPush);
        removeButton->setSize(16, 16);
        removeButton->setImage(RkImage(removeButton->size(), RK_IMAGE_RC(remove_per_button)),
                             RkButton::State::Unpressed);
        removeButton->setImage(RkImage(removeButton->size(), RK_IMAGE_RC(remove_per_button_hover)),
                             RkButton::State::UnpressedHover);
        removeButton->setImage(RkImage(removeButton->size(), RK_IMAGE_RC(remove_per_button_on)),
                             RkButton::State::Pressed);
        removeButton->setImage(RkImage(removeButton->size(), RK_IMAGE_RC(remove_per_button_hover)),
                             RkButton::State::PressedHover);
        removeButton->show();
        instrumentContainer->addWidget(removeButton);
        instrumentContainer->addSpace(3);

        // Copy button
        copyButton = new RkButton(this);
        copyButton->setType(RkButton::ButtonType::ButtonPush);
        copyButton->setSize(16, 16);
        copyButton->setImage(RkImage(copyButton->size(), RK_IMAGE_RC(copy_per_button)),
                             RkButton::State::Unpressed);
        copyButton->setImage(RkImage(copyButton->size(), RK_IMAGE_RC(copy_per_button_hover)),
                             RkButton::State::UnpressedHover);
        copyButton->setImage(RkImage(copyButton->size(), RK_IMAGE_RC(copy_per_button_on)),
                             RkButton::State::Pressed);
        copyButton->setImage(RkImage(copyButton->size(), RK_IMAGE_RC(copy_per_button_hover)),
                             RkButton::State::PressedHover);
        copyButton->show();
        instrumentContainer->addWidget(copyButton);
        instrumentContainer->addSpace(10);

        // Limiter
        instrumentLimiter = new InstrumentLimiter(this);
        instrumentLimiter->setSize(100, 10);
        auto limiterBox = new RkContainer(this, Rk::Orientation::Vertical);
        limiterBox->setHiddenTakesPlace();
        limiterBox->setSize({instrumentLimiter->width(), instrumentContainer->height()});
        limiterBox->addSpace((height() - instrumentLimiter->height()) / 2);
        limiterBox->addWidget(instrumentLimiter);
        instrumentContainer->addSpace(5);
        instrumentContainer->addContainer(limiterBox);
        instrumentContainer->addSpace(10);

        // Mute button
        muteButton = new RkButton(this);
        muteButton->setType(RkButton::ButtonType::ButtonCheckable);
        muteButton->setSize(16, 16);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute)),
                             RkButton::State::Unpressed);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute_hover)),
                             RkButton::State::UnpressedHover);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute_on)),
                             RkButton::State::Pressed);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute_hover)),
                             RkButton::State::PressedHover);
        muteButton->show();
        instrumentContainer->addWidget(muteButton);
        instrumentContainer->addSpace(3);

        // Solo button
        soloButton = new RkButton(this);
        soloButton->setType(RkButton::ButtonType::ButtonCheckable);
        soloButton->setSize(16, 16);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo)),
                             RkButton::State::Unpressed);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo_hover)),
                             RkButton::State::UnpressedHover);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo_on)),
                             RkButton::State::Pressed);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo_hover)),
                             RkButton::State::PressedHover);
        soloButton->show();
        instrumentContainer->addWidget(soloButton);
        instrumentContainer->addSpace(3);

        // Play button
        playButton = new RkButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(16, 16);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(per_play)),
                         RkButton::State::Unpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(per_play_hover)),
                         RkButton::State::UnpressedHover);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(per_play_on)),
                         RkButton::State::Pressed);
        playButton->show();
        instrumentContainer->addWidget(playButton);
}

void KitInstrumentView::updateView()
{
        instrumentLimiter->onSetValue(instrumentModel->limiter());
        muteButton->setPressed(instrumentModel->isMuted());
        soloButton->setPressed(instrumentModel->isSolo());
        keyButton->setText(MidiKeyWidget::midiKeyToNote(instrumentModel->key()));
        keyButton->setBackgroundColor((index() % 2) ? RkColor(100, 100, 100) : RkColor(50, 50, 50));
        update();
}

void KitInstrumentView::setModel(InstrumentModel *model)
{
        if (!model)
                return;

        instrumentModel = model;
        RK_ACT_BIND(removeButton, released, RK_ACT_ARGS(), this, remove());
        RK_ACT_BIND(copyButton, released, RK_ACT_ARGS(), instrumentModel, copy());
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), instrumentModel, play());
        RK_ACT_BIND(muteButton, toggled, RK_ACT_ARGS(bool toggled), instrumentModel, mute(toggled));
        RK_ACT_BIND(soloButton, toggled, RK_ACT_ARGS(bool toggled), instrumentModel, solo(toggled));
        RK_ACT_BIND(instrumentLimiter, valueUpdated, RK_ACT_ARGS(int val), instrumentModel, setLimiter(val));
        RK_ACT_BIND(instrumentModel, nameUpdated, RK_ACT_ARGS(std::string name), this, update());
        RK_ACT_BIND(instrumentModel, keyUpdated, RK_ACT_ARGS(KeyIndex index), this, updateView());
        RK_ACT_BIND(instrumentModel, channelUpdated, RK_ACT_ARGS(int val), this, update());
        RK_ACT_BIND(instrumentModel, limiterUpdated, RK_ACT_ARGS(int val), instrumentLimiter, onSetValue(val));
        RK_ACT_BIND(instrumentModel, muteUpdated, RK_ACT_ARGS(bool b), muteButton, setPressed(b));
        RK_ACT_BIND(instrumentModel, soloUpdated, RK_ACT_ARGS(bool b), soloButton, setPressed(b));
        RK_ACT_BIND(instrumentModel, selected, RK_ACT_ARGS(), this, update());
        RK_ACT_BIND(instrumentModel, modelUpdated, RK_ACT_ARGS(), this, updateView());
        updateView();
}

InstrumentModel* KitInstrumentView::getModel()
{
        return instrumentModel;
}

void KitInstrumentView::remove()
{
        if (getModel())
                getModel()->remove();
}

void KitInstrumentView::paintWidget(RkPaintEvent *event)
{
        RkImage img(size());
        RkPainter paint(&img);
        paint.fillRect(rect(), background());
        auto pen = paint.pen();
        pen.setColor({200, 200, 200});
        auto font = paint.font();
        font.setSize(12);
        paint.setFont(font);
        RkColor backgroundColor = {160, 160, 160, 80};
        if (index() % 2)
                backgroundColor = {200, 200, 200, 80};
        paint.fillRect(RkRect(0, 0, nameWidth, height()), backgroundColor);
        paint.setPen(pen);
        paint.drawText(RkRect(7, (height() - font.size()) / 2, nameWidth, font.size()),
                       instrumentModel->name(), Rk::Alignment::AlignLeft);

        auto n = instrumentModel->numberOfChannels();
        int x = nameWidth;
        while (n--) {
                if (n % 2)
                        paint.fillRect(RkRect(x, 0, channelWidth, height()),
                                                {backgroundColor.red() + 20,
                                                backgroundColor.green() + 20,
                                                backgroundColor.blue() + 20, 80});
                else
                        paint.fillRect(RkRect(x, 0, channelWidth, height()),
                                                {backgroundColor.red() - 20,
                                                backgroundColor.green() - 20,
                                                backgroundColor.blue() - 20, 80});
                x += channelWidth;
        }

        if (instrumentModel->isSelected())
                paint.fillRect(RkRect(0, 0, 4, height()), {255, 255, 255, 90});

        pen = paint.pen();
        pen.setColor({50, 160, 50});
        pen.setWidth(8);
        paint.setPen(pen);
        paint.drawCircle({nameWidth + instrumentModel->channel() * channelWidth + channelWidth / 2 , height() / 2},  4);
        RkPainter painter(this);
        painter.drawImage(img, 0, 0);
}

void KitInstrumentView::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() != RkMouseEvent::ButtonType::Left
            && event->button() != RkMouseEvent::ButtonType::WheelUp
            && event->button() != RkMouseEvent::ButtonType::WheelDown)
                return;

        updateInstrumentName();
        setFocus(true);
        if (event->button() == RkMouseEvent::ButtonType::Left) {
                int leftLimit  = nameWidth;
                int rightLimit = nameWidth + channelWidth * instrumentModel->numberOfChannels();
                if (event->x() <= leftLimit)
                        instrumentModel->select();
                else if (event->x() > leftLimit && event->x() < rightLimit)
                        instrumentModel->setChannel((event->x() - nameWidth) / channelWidth);
        }
}

void KitInstrumentView::mouseDoubleClickEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::WheelUp
            || event->button() == RkMouseEvent::ButtonType::WheelDown) {
                mouseButtonPressEvent(event);
                return;
        }

        if (event->button() == RkMouseEvent::ButtonType::Left && event->x() < nameWidth) {
                if (editInstrument == nullptr) {
                        editInstrument = new RkLineEdit(this);
                        editInstrument->setSize({nameWidth, height()});
                        RK_ACT_BIND(editInstrument, editingFinished, RK_ACT_ARGS(),
                                    this, updateInstrumentName());
                }
                editInstrument->setText(instrumentModel->name());
                editInstrument->moveCursorToFront();
                editInstrument->show();
                editInstrument->setFocus();
        }
}

void KitInstrumentView::updateInstrumentName()
{
        if (editInstrument) {
		auto name = editInstrument->text();
		if (!name.empty()) {
			instrumentModel->setName(name);
			editInstrument->close();
                        editInstrument = nullptr;
		}
	}
}

void KitInstrumentView::updateLeveler()
{
        if (instrumentModel->leveler() > instrumentLimiter->getLeveler())
                instrumentLimiter->setLeveler(instrumentModel->leveler());
        else if (instrumentLimiter->getLeveler() > 0)
                instrumentLimiter->setLeveler(instrumentLimiter->getLeveler() - 2);
}

void KitInstrumentView::showMidiPopup()
{
        auto midiPopup = new MidiKeyWidget(dynamic_cast<GeonkickWidget*>(getTopWidget()),
                                           instrumentModel);
        midiPopup->setPosition(keyButton->x() - midiPopup->width() - 5,
                               getTopWidget()->height() - 2 * midiPopup->height()
                               + height() * (index() - 3));
        RK_ACT_BIND(midiPopup,
                    isAboutToClose,
                    RK_ACT_ARGS(),
                    keyButton,
                    setPressed(false));
        midiPopup->show();
}
