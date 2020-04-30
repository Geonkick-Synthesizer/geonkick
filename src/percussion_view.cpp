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
{
        setSize(parent->width(), 20);
        auto percussionContiner = new RkWidgetContiner(this);
        percussionContiner->setSize(size());

        // Play button
        auto button = new RkButton(this);
        button->setType(RkButton::PushButton);
        button->setSize(16, 16);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(per_play)),
                         RKButton::ButtonImage::ImageUnpressed);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(per_play_hover)),
                         RKButton::ButtonImage::ImageUnpressedHover);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(per_play_on)),
                         RKButton::ButtonImage::ImagePpressed);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(per_play_on_hover)),
                         RKButton::ButtonImage::ImagePpressedHover);
        RK_ACT_BIND(button, toggled, RK_ACT_ARGS(int toggled), percussionModel, play());
        percussionContiner->addWidget(button, Rk::Alignment::AlignRight);

        // Mute button
        button = new RkButton(this);
        button->setSize(16, 16);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(mute)),
                         RKButton::ButtonImage::ImageUnpressed);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(mute_hover)),
                         RKButton::ButtonImage::ImageUnpressedHover);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(mute_on)),
                         RKButton::ButtonImage::ImagePpressed);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(mute_on_hover)),
                         RKButton::ButtonImage::ImagePpressedHover);
        RK_ACT_BIND(button,
                    toggled,
                    RK_ACT_ARGS(int toggled),
                    percussionModel,
                    setMute(toggled));
        RK_ACT_BIND(percussionModel,
                    muteUpdated,
                    RK_ACT_ARGS(bool b),
                    button,
                    setPressed(b));
        percussionContiner->addWidget(button, Rk::Alignment::AlignRight);

        // Solo button
        button = new RkButton(this);
        button->setSize(16, 16);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(solo)),
                         RKButton::ButtonImage::ImageUnpressed);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(solo_hover)),
                         RKButton::ButtonImage::ImageUnpressedHover);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(solo_on)),
                         RKButton::ButtonImage::ImagePpressed);
        button->setImage(RkImage(button->size(), RK_IMGE_RC(solo_on_hover)),
                         RKButton::ButtonImage::ImagePpressedHover);
        RK_ACT_BIND(button,
                    toggled,
                    RK_ACT_ARGS(int toggled),
                    percussionModel,
                    setSolo(toggled));
        RK_ACT_BIND(percussionModel,
                    soloUpdated,
                    RK_ACT_ARGS(bool b),
                    button,
                    setPressed(b));
        percussionContiner->addWidget(button, Rk::Alignment::AlignRight);

        // Limiter
        auto limiter = new GeonkickSlider(this);
        limiter->setSize(200, 10);
        RK_ACT_BIND(limiter, valueUpdated, RK_ACT_ARGS(int val), percussionModel, setLimiter(val));
        RK_ACT_BIND(percussionModel, limiterUpdated, RK_ACT_ARGS(int val), limiter, setValue(val));
        auto leveler = new RkProgressBar(this);
        leveler->setSize({limiter->width(), limiter->height() / 2});
        leveler->setProgressColor({125, 200, 125});
        leveler->setRange(0, 100);
        RK_ACT_BIND(percussionModel, levelerUpdated, RK_ACT_ARGS(int val), leveler, setValue(val));
        auto limiterBox = new RkWidgetContiner(percussionContiner, Rk::Orientation::Vertical);
        limiterBox->setSize(limiter->width(), percussionContiner->height());
        limiterBox->addWidget(leveler);
        limiterBox->addWidget(limiter);
        percussionContiner->addContiner(limiterBox);
}

void KitPercussionView::setModel(PercussionModel model)
{
        percussionModel = model;
        RK_ACT_BIND(model, nameUpdated, RK_ACT_ARGS(std::string name), this, updateName(name));
        RK_ACT_BIND(model, keyUpdated, RK_ACT_ARGS(KeyIndex index), this, updateName(index));
        RK_ACT_BIND(model, limiterUpdated, RK_ACT_ARGS(int val), this, updateName(val));
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
