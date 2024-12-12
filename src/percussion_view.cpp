/**
 * File name: percussion_view.cpp
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

#include "kit_widget.h"
#include "percussion_view.h"
#include "percussion_model.h"
#include "geonkick_slider.h"
#include "MidiKeyWidget.h"
#include "geonkick_button.h"

#include <RkEvent.h>
#include <RkPainter.h>
#include <RkLineEdit.h>
#include <RkButton.h>
#include <RkContainer.h>
#include <RkProgressBar.h>
#include <RkSpinBox.h>

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
RK_DECLARE_IMAGE_RC(note_off_unpressed);
RK_DECLARE_IMAGE_RC(note_off_hover);
RK_DECLARE_IMAGE_RC(note_off_pressed);

PercussionLimiter::PercussionLimiter(GeonkickWidget *parent)
        : GeonkickSlider(parent)
        , levelerValue{0}
{
}

void PercussionLimiter::setLeveler(int value)
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

int PercussionLimiter::getLeveler() const
{
        return levelerValue;
}

void PercussionLimiter::paintWidget(RkPaintEvent *event)
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

KitPercussionView::KitPercussionView(KitWidget *parent,
                                     PercussionModel *model)
        : GeonkickWidget(parent)
        , parentView{parent}
        , percussionModel{model}
        , nameWidth{100}
        , channelWidth{30}
        , editPercussion{nullptr}
        , midiChannelSpinBox{nullptr}
        , keyButton{nullptr}
        , copyButton{nullptr}
        , removeButton{nullptr}
        , playButton{nullptr}
        , muteButton{nullptr}
        , soloButton{nullptr}
        , noteOffButton{nullptr}
        , percussionLimiter{nullptr}
{
        setSize(parent->width(), 21);
        createView();
        setModel(model);
}

KitPercussionView::PercussionIndex KitPercussionView::index() const
{
        if (percussionModel)
                return percussionModel->index();
        return -1;
}

void KitPercussionView::createView()
{
        auto percussionContainer = new RkContainer(this);
        percussionContainer->setSize(size());
        percussionContainer->setHiddenTakesPlace();
        percussionContainer->addSpace(nameWidth + percussionModel->numberOfChannels() * channelWidth + 5);

        // Midi channel spinbox.
        midiChannelSpinBox = new RkSpinBox(this);
        midiChannelSpinBox->setTextColor({250, 250, 250});
        midiChannelSpinBox->setBackgroundColor({60, 57, 57});
        midiChannelSpinBox->upControl()->setBackgroundColor({50, 47, 47});
        midiChannelSpinBox->upControl()->setTextColor({100, 100, 100});
        midiChannelSpinBox->downControl()->setBackgroundColor({50, 47, 47});
        midiChannelSpinBox->downControl()->setTextColor({100, 100, 100});
        midiChannelSpinBox->setSize(50, 20);
        midiChannelSpinBox->show();
        RK_ACT_BIND(midiChannelSpinBox,
                    currentIndexChanged,
                    RK_ACT_ARGS(int index),
                    percussionModel,
                    setMidiChannel(index - 1));
        RK_ACT_BIND(percussionModel,
                    midiChannelUpdated,
                    RK_ACT_ARGS(int index),
                    midiChannelSpinBox,
                    setCurrentIndex(index + 1));
        percussionContainer->addWidget(midiChannelSpinBox);
        percussionContainer->addSpace(5);

        // Midi key button.
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
        percussionContainer->addWidget(keyButton);
        percussionContainer->addSpace(5);

        // Note off button
        noteOffButton = new RkButton(this);
        noteOffButton->setType(RkButton::ButtonType::ButtonCheckable);
        noteOffButton->setSize(23, 16);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_unpressed)),
                                RkButton::State::Unpressed);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_hover)),
                                RkButton::State::UnpressedHover);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_pressed)),
                                RkButton::State::Pressed);
        noteOffButton->setImage(RkImage(noteOffButton->size(), RK_IMAGE_RC(note_off_hover)),
                                RkButton::State::PressedHover);
        noteOffButton->show();
        percussionContainer->addWidget(noteOffButton);
        percussionContainer->addSpace(5);

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
        percussionContainer->addWidget(removeButton);
        percussionContainer->addSpace(3);

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
        percussionContainer->addWidget(copyButton);
        percussionContainer->addSpace(5);

        // Limiter
        percussionLimiter = new PercussionLimiter(this);
        percussionLimiter->setSize(100, 10);
        auto limiterBox = new RkContainer(this, Rk::Orientation::Vertical);
        limiterBox->setHiddenTakesPlace();
        limiterBox->setSize({percussionLimiter->width(), percussionContainer->height()});
        limiterBox->addSpace((height() - percussionLimiter->height()) / 2);
        limiterBox->addWidget(percussionLimiter);
        percussionContainer->addSpace(5);
        percussionContainer->addContainer(limiterBox);
        percussionContainer->addSpace(10);

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
        percussionContainer->addWidget(muteButton);
        percussionContainer->addSpace(3);

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
        percussionContainer->addWidget(soloButton);
        percussionContainer->addSpace(3);

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
        percussionContainer->addWidget(playButton);
}

void KitPercussionView::updateView()
{
        percussionLimiter->onSetValue(percussionModel->limiter(), 55.0 * 100.0 / 75);
        muteButton->setPressed(percussionModel->isMuted());
        soloButton->setPressed(percussionModel->isSolo());
        noteOffButton->setPressed(percussionModel->isNoteOffEnabled());
        size_t nMidiChannels = percussionModel->numberOfMidiChannels();
        midiChannelSpinBox->clear();
        midiChannelSpinBox->addItem("Any");
        for (size_t i = 0; i < nMidiChannels; i++)
                midiChannelSpinBox->addItem(std::to_string(i + 1));
        midiChannelSpinBox->setCurrentIndex(percussionModel->midiChannel() + 1);
        keyButton->setText(MidiKeyWidget::midiKeyToNote(percussionModel->key()));
        keyButton->setBackgroundColor((index() % 2) ? RkColor(100, 100, 100) : RkColor(50, 50, 50));
        update();
}

void KitPercussionView::setModel(PercussionModel *model)
{
        if (!model)
                return;

        percussionModel = model;
        RK_ACT_BIND(removeButton, released, RK_ACT_ARGS(), this, remove());
        RK_ACT_BIND(copyButton, released, RK_ACT_ARGS(), percussionModel, copy());
        RK_ACT_BIND(playButton, pressed, RK_ACT_ARGS(), percussionModel, play());
        RK_ACT_BIND(noteOffButton, toggled, RK_ACT_ARGS(bool toggled), percussionModel, enableNoteOff(toggled));
        RK_ACT_BIND(muteButton, toggled, RK_ACT_ARGS(bool toggled), percussionModel, mute(toggled));
        RK_ACT_BIND(soloButton, toggled, RK_ACT_ARGS(bool toggled), percussionModel, solo(toggled));
        RK_ACT_BIND(percussionLimiter, valueUpdated, RK_ACT_ARGS(int val), percussionModel, setLimiter(val));
        RK_ACT_BIND(percussionModel, nameUpdated, RK_ACT_ARGS(std::string name), this, update());
        RK_ACT_BIND(percussionModel, keyUpdated, RK_ACT_ARGS(KeyIndex index), this, updateView());
        RK_ACT_BIND(percussionModel, channelUpdated, RK_ACT_ARGS(int val), this, update());
        RK_ACT_BIND(percussionModel, limiterUpdated, RK_ACT_ARGS(int val), percussionLimiter, onSetValue(val, 55.0 * 100.0 / 75));
        RK_ACT_BIND(percussionModel, muteUpdated, RK_ACT_ARGS(bool b), muteButton, setPressed(b));
        RK_ACT_BIND(percussionModel, soloUpdated, RK_ACT_ARGS(bool b), soloButton, setPressed(b));
        RK_ACT_BIND(percussionModel, selected, RK_ACT_ARGS(), this, update());
        RK_ACT_BIND(percussionModel, modelUpdated, RK_ACT_ARGS(), this, updateView());
        RK_ACT_BIND(percussionModel, midiChannelUpdated, RK_ACT_ARGS(int val), this, update());
        RK_ACT_BIND(percussionModel, noteOffUpdated, RK_ACT_ARGS(bool b), this, update());
        updateView();
}

PercussionModel* KitPercussionView::getModel()
{
        return percussionModel;
}

void KitPercussionView::remove()
{
        if (getModel())
                getModel()->remove();
}

void KitPercussionView::paintWidget(RkPaintEvent *event)
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
                       percussionModel->name(), Rk::Alignment::AlignLeft);

        auto n = percussionModel->numberOfChannels();
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

        if (percussionModel->isSelected())
                paint.fillRect(RkRect(0, 0, 4, height()), {255, 255, 255, 90});

        pen = paint.pen();
        pen.setColor({50, 160, 50});
        pen.setWidth(8);
        paint.setPen(pen);
        paint.drawCircle({nameWidth + percussionModel->channel() * channelWidth + channelWidth / 2 , height() / 2},  4);
        RkPainter painter(this);
        painter.drawImage(img, 0, 0);
}

void KitPercussionView::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() != RkMouseEvent::ButtonType::Left
            && event->button() != RkMouseEvent::ButtonType::WheelUp
            && event->button() != RkMouseEvent::ButtonType::WheelDown)
                return;

        updatePercussionName();
        setFocus(true);
        if (event->button() == RkMouseEvent::ButtonType::Left) {
                int leftLimit  = nameWidth;
                int rightLimit = nameWidth + channelWidth * percussionModel->numberOfChannels();
                if (event->x() <= leftLimit)
                        percussionModel->select();
                else if (event->x() > leftLimit && event->x() < rightLimit)
                        percussionModel->setChannel((event->x() - nameWidth) / channelWidth);
        }
}

void KitPercussionView::mouseDoubleClickEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::WheelUp
            || event->button() == RkMouseEvent::ButtonType::WheelDown) {
                mouseButtonPressEvent(event);
                return;
        }

        if (event->button() == RkMouseEvent::ButtonType::Left && event->x() < nameWidth) {
                if (editPercussion == nullptr) {
                        editPercussion = new RkLineEdit(this);
                        editPercussion->setSize({nameWidth, height()});
                        RK_ACT_BIND(editPercussion, editingFinished, RK_ACT_ARGS(),
                                    this, updatePercussionName());
                }
                editPercussion->setText(percussionModel->name());
                editPercussion->moveCursorToFront();
                editPercussion->show();
                editPercussion->setFocus();
        }
}

void KitPercussionView::updatePercussionName()
{
        if (editPercussion) {
		auto name = editPercussion->text();
		if (!name.empty()) {
			percussionModel->setName(name);
			editPercussion->close();
                        editPercussion = nullptr;
		}
	}
}

void KitPercussionView::updateLeveler()
{
        if (percussionModel->leveler() > percussionLimiter->getLeveler())
                percussionLimiter->setLeveler(percussionModel->leveler());
        else if (percussionLimiter->getLeveler() > 0)
                percussionLimiter->setLeveler(percussionLimiter->getLeveler() - 2);
}

void KitPercussionView::showMidiPopup()
{
        auto midiPopup = new MidiKeyWidget(dynamic_cast<GeonkickWidget*>(getTopWidget()),
                                           percussionModel);
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
