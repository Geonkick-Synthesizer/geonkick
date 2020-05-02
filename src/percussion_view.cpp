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

#include <RkLineEdit.h>
#include <RkButton.h>
#include <RkWidgetContiner.h>

RK_DECLARE_IMGE_RC(mute);
RK_DECLARE_IMGE_RC(mute_hover);
RK_DECLARE_IMGE_RC(mute_on);
RK_DECLARE_IMGE_RC(mute_on_hover);
RK_DECLARE_IMGE_RC(solo);
RK_DECLARE_IMGE_RC(solo_hover);
RK_DECLARE_IMGE_RC(solo_on);
RK_DECLARE_IMGE_RC(solo_on_hover);
RK_DECLARE_IMGE_RC(per_play);
RK_DECLARE_IMGE_RC(per_play_hover);
RK_DECLARE_IMGE_RC(per_play_on);
RK_DECLARE_IMGE_RC(per_play_on_hover);

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
        auto percussionContiner = new RkWidgetContiner(this);
        percussionContiner->setSize(size());

        // Play button
        playButton = new RkButton(this);
        playButton->setType(RkButton::PushButton);
        playButton->setSize(16, 16);
        playButton->setImage(RkImage(button->size(), RK_IMGE_RC(per_play)),
                         RKButton::ButtonImage::ImageUnpressed);
        playButton->setImage(RkImage(button->size(), RK_IMGE_RC(per_play_hover)),
                         RKButton::ButtonImage::ImageUnpressedHover);
        playButton->setImage(RkImage(button->size(), RK_IMGE_RC(per_play_on)),
                         RKButton::ButtonImage::ImagePpressed);
        playButton->setImage(RkImage(button->size(), RK_IMGE_RC(per_play_on_hover)),
                         RKButton::ButtonImage::ImagePpressedHover);
        percussionContiner->addWidget(playButton, Rk::Alignment::AlignRight);

        // Mute button
        muteButton = new RkButton(this);
        muteButton->setSize(16, 16);
        muteButton->setImage(RkImage(button->size(), RK_IMGE_RC(mute)),
                             RkButton::ButtonImage::ImageUnpressed);
        muteButton->setImage(RkImage(button->size(), RK_IMGE_RC(mute_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        muteButton->setImage(RkImage(button->size(), RK_IMGE_RC(mute_on)),
                             RkButton::ButtonImage::ImagePpressed);
        muteButton->setImage(RkImage(button->size(), RK_IMGE_RC(mute_on_hover)),
                             RkButton::ButtonImage::ImagePpressedHover);
        percussionContiner->addWidget(button, Rk::Alignment::AlignRight);

        // Solo button
        soloButton = new RkButton(this);
        soloButton->setSize(16, 16);
        soloButton->setImage(RkImage(button->size(), RK_IMGE_RC(solo)),
                             RkButton::ButtonImage::ImageUnpressed);
        soloButton->setImage(RkImage(button->size(), RK_IMGE_RC(solo_hover)),
                             RkButton::ButtonImage::ImageUnpressedHover);
        soloButton->setImage(RkImage(button->size(), RK_IMGE_RC(solo_on)),
                             RkButton::ButtonImage::ImagePpressed);
        soloButton->setImage(RkImage(button->size(), RK_IMGE_RC(solo_on_hover)),
                             RkButton::ButtonImage::ImagePpressedHover);
        percussionContiner->addWidget(soloButton, Rk::Alignment::AlignRight);

        // Limiter
        limiterSlider = new GeonkickSlider(this);
        limiterSlider->setSize(200, 10);
        levelerProgress = new RkProgressBar(this);
        levelerProgress->setSize({limiter->width(), limiter->height() / 2});
        levelerProgress->setProgressColor({125, 200, 125});
        levelerProgress->setRange(0, 100);
        auto limiterBox = new RkWidgetContiner(percussionContiner, Rk::Orientation::Vertical);
        limiterBox->setSize(limiterSlider->width(), percussionContiner->height());
        limiterBox->addWidget(levelerProgress);
        limiterBox->addWidget(limiterSlider);
        percussionContiner->addContiner(limiterBox);
}

void KitPercussionView::setModel(PercussionModel model)
{
        percussionModel = model;
        RK_ACT_BIND(playButton, toggled, RK_ACT_ARGS(int toggled), percussionModel, play());
        RK_ACT_BIND(muteButton, toggled, RK_ACT_ARGS(int toggled), percussionModel, setMute(toggled));
        RK_ACT_BIND(soloButton, toggled, RK_ACT_ARGS(bool toggled), percussionModel, setSolo(toggled));
        RK_ACT_BIND(limiterSlider, valueUpdated, RK_ACT_ARGS(int val), percussionModel, setLimiter(val));
        RK_ACT_BIND(percussionModel, nameUpdated, RK_ACT_ARGS(std::string name), this, update());
        RK_ACT_BIND(percussionModel, keyUpdated, RK_ACT_ARGS(KeyIndex index), this, update());
        RK_ACT_BIND(percussionModel, limiterUpdated, RK_ACT_ARGS(int val), limiterSlider, onSetValue(val));
        RK_ACT_BIND(percussionModel, levelerUpdated, RK_ACT_ARGS(int val), levelerProgress, setValue(val));
        RK_ACT_BIND(percussionModel, muteUpdated, RK_ACT_ARGS(bool b), muteButton, setPressed(b));
        RK_ACT_BIND(percussionModel, soloUpdated, RK_ACT_ARGS(bool b), soloButton, setPressed(b));
}

PercussionModel* KitPercussionView::getModel()
{
        return percussionModel;
}

void KitPercussionView::paintWidget(RkPaintEvent *event)
{
        auto pen = painter.pen();
        pen.setColor({200, 200, 200});
        auto font = painter.font();
        font.setSize(12);
        painter.setFont(font);
        painter.drawText(RkRect(0, (height() - font->size()) / 2, nameWidth, height()),
                         percussionModel->name());

        auto n = percussionModel->keysNumber();
        int x = nameWidth;
        for (n--) {
                if (n % 2)
                        painter.fillRect(RkRect(x, 0, keyWidth, height()), {60, 60, 60});
                else
                        painter.fillRect(RkRect(x, 0, keyWidth, height()), {{50, 50, 50}});
                x += keyWidth;
        }

        auto pen = painter.pen();
        pen.setColor({50, 160, 50});
        pen.setWidth(8);
        painter.setPen(pen);
        painter.drawCircle({percussionModel->key() * keyWidth + keyWidth / 2 , height() / 2},  4);
}

void KitPercussionView::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Left && event->x() < nameWidth)
                percussionModel->select();
}

void KitPercussionView::mouseDoubleClickEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Left && event->x() < nameWidth) {
                if (editPercussion == nullptr) {
                        editPercussion = new RkLineEdit(this);
                        editPercussion->setSize({nameWidth, percussionHeight});
                        RK_ACT_BIND(editPercussion, editingFinished, RK_ACT_ARGS(),
                                    this, updatePercussionName());
                }
                editPercussion->setText(percussionModel->percussionName());
                editPercussion->moveCursorToEnd();
                editPercussion->show();
        }
}

void KitPercussionView::updatePercussionName()
{
        if (editPercussion) {
		auto name = editPercussion->text();
		if (!name.empty()) {
			percussionModel->setName(name);
			delete editPercussion;
                        editPercussion = nullptr;
		}
	}
}
