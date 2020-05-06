/**
 * File name: percussion_view.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "percussion_view.h"
#include "percussion_model.h"
#include "geonkick_slider.h"

#include <RkEvent.h>
#include <RkPainter.h>
#include <RkLineEdit.h>
#include <RkButton.h>
#include <RkContainer.h>
#include <RkProgressBar.h>

RK_DECLARE_IMAGE_RC(mute);
RK_DECLARE_IMAGE_RC(mute_hover);
RK_DECLARE_IMAGE_RC(mute_on);
RK_DECLARE_IMAGE_RC(mute_on_hover);
RK_DECLARE_IMAGE_RC(solo);
RK_DECLARE_IMAGE_RC(solo_hover);
RK_DECLARE_IMAGE_RC(solo_on);
RK_DECLARE_IMAGE_RC(solo_on_hover);
RK_DECLARE_IMAGE_RC(per_play);
RK_DECLARE_IMAGE_RC(per_play_hover);
RK_DECLARE_IMAGE_RC(per_play_on);
RK_DECLARE_IMAGE_RC(per_play_on_hover);

KitPercussionView::KitPercussionView(GeonkickWidget *parent,
                                     PercussionModel *model)
        : GeonkickWidget(parent)
        , percussionModel{model}
        , nameWidth{100}
        , keyWidth{30}
        , editPercussion{nullptr}
        , playButton{nullptr}
        , muteButton{nullptr}
        , soloButton{nullptr}
        , limiterSlider{nullptr}
        , levelerProgress{nullptr}
{
        setSize(parent->width(), 20);
        createView();
        setModel(model);
}

void KitPercussionView::createView()
{
        auto percussionContainer = new RkContainer(this);
        percussionContainer->setSize(size());
        percussionContainer->setHiddenTakesPlace();

        // Play button
        playButton = new RkButton(this);
        playButton->setType(RkButton::ButtonType::ButtonPush);
        playButton->setSize(16, 16);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(per_play)),
                         RkButton::ButtonImage::ImageUnpressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(per_play_hover)),
                         RkButton::ButtonImage::ImageUnpressedHover);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(per_play_on)),
                         RkButton::ButtonImage::ImagePressed);
        playButton->setImage(RkImage(playButton->size(), RK_IMAGE_RC(per_play_on_hover)),
                         RkButton::ButtonImage::ImagePressedHover);
        percussionContainer->addWidget(playButton, Rk::Alignment::AlignRight);

        // Mute button
        muteButton = new RkButton(this);
        muteButton->setType(RkButton::ButtonType::ButtonCheckable);
        muteButton->setSize(16, 16);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute)),
                             RkButton::ButtonImage::ImageUnpressed);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute_on)),
                             RkButton::ButtonImage::ImagePressed);
        muteButton->setImage(RkImage(muteButton->size(), RK_IMAGE_RC(mute_on_hover)),
                             RkButton::ButtonImage::ImagePressedHover);
        percussionContainer->addWidget(muteButton, Rk::Alignment::AlignRight);

        // Solo button
        soloButton = new RkButton(this);
        soloButton->setType(RkButton::ButtonType::ButtonCheckable);
        soloButton->setSize(16, 16);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo)),
                             RkButton::ButtonImage::ImageUnpressed);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo_on)),
                             RkButton::ButtonImage::ImagePressed);
        soloButton->setImage(RkImage(soloButton->size(), RK_IMAGE_RC(solo_on_hover)),
                             RkButton::ButtonImage::ImagePressedHover);
        percussionContainer->addWidget(soloButton, Rk::Alignment::AlignRight);

        // Limiter
        limiterSlider = new GeonkickSlider(this);
        limiterSlider->setSize(200, 10);
        //        levelerProgress = new RkProgressBar(this);
        //        levelerProgress->setSize({limiterSlider->width() - 2, limiterSlider->height() / 2});
        //        levelerProgress->setProgressColor({125, 200, 125});
        //        levelerProgress->setRange(0, 100);
        auto limiterBox = new RkContainer(this, Rk::Orientation::Vertical);
        limiterBox->setHiddenTakesPlace();
        limiterBox->setSize({limiterSlider->width(), percussionContainer->height()});
        //        limiterBox->addWidget(levelerProgress);
        //        limiterBox->addSpace(3);
        limiterBox->addWidget(limiterSlider);
        percussionContainer->addContainer(limiterBox, Rk::Alignment::AlignRight);
}

void KitPercussionView::updateView()
{
        limiterSlider->onSetValue(percussionModel->limiter());
}

void KitPercussionView::setModel(PercussionModel *model)
{
        if (!model)
                return;

        percussionModel = model;
        RK_ACT_BIND(playButton, toggled, RK_ACT_ARGS(bool toggled), percussionModel, play());
        RK_ACT_BIND(muteButton, toggled, RK_ACT_ARGS(bool toggled), percussionModel, mute(toggled));
        RK_ACT_BIND(soloButton, toggled, RK_ACT_ARGS(bool toggled), percussionModel, solo(toggled));
        RK_ACT_BIND(limiterSlider, valueUpdated, RK_ACT_ARGS(int val), percussionModel, setLimiter(val));
        RK_ACT_BIND(percussionModel, nameUpdated, RK_ACT_ARGS(std::string name), this, update());
        RK_ACT_BIND(percussionModel, keyUpdated, RK_ACT_ARGS(KeyIndex index), this, update());
        RK_ACT_BIND(percussionModel, limiterUpdated, RK_ACT_ARGS(int val), limiterSlider, onSetValue(val));
        //        RK_ACT_BIND(percussionModel, levelerUpdated, RK_ACT_ARGS(int val), levelerProgress, setValue(val));
        RK_ACT_BIND(percussionModel, muteUpdated, RK_ACT_ARGS(bool b), muteButton, setPressed(b));
        RK_ACT_BIND(percussionModel, soloUpdated, RK_ACT_ARGS(bool b), soloButton, setPressed(b));
        RK_ACT_BIND(percussionModel, modelUpdated, RK_ACT_ARGS(), this, updateView());
        updateView();
}

PercussionModel* KitPercussionView::getModel()
{
        return percussionModel;
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
        paint.fillRect(RkRect(0, 0, nameWidth, height()), backgroundColor);
        paint.setPen(pen);
        paint.drawText(RkRect(7, (height() - font.size()) / 2, nameWidth, font.size()),
                       percussionModel->name(), Rk::Alignment::AlignLeft);

        auto n = percussionModel->keysNumber();
        int x = nameWidth;
        while (n--) {
                if (n % 2)
                        paint.fillRect(RkRect(x, 0, keyWidth, height()),
                                                {backgroundColor.red() + 20,
                                                backgroundColor.green() + 20,
                                                backgroundColor.blue() + 20, 80});
                else
                        paint.fillRect(RkRect(x, 0, keyWidth, height()),
                                                {backgroundColor.red() - 20,
                                                backgroundColor.green() - 20,
                                                backgroundColor.blue() - 20, 80});
                x += keyWidth;
        }

        pen = paint.pen();
        pen.setColor({50, 160, 50});
        pen.setWidth(8);
        paint.setPen(pen);
        paint.drawCircle({nameWidth + percussionModel->key() * keyWidth + keyWidth / 2 , height() / 2},  4);
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
                int rightLimit = nameWidth + keyWidth * percussionModel->keysNumber();
                if (event->x() <= leftLimit)
                        percussionModel->select();
                else if (event->x() > leftLimit && event->x() < rightLimit)
                        percussionModel->setKey((event->x() - nameWidth) / keyWidth);
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
