/**
 * File name: percussion_view.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://geontime.com>
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

#include <RkLineEdit>
#include <RkButton>

KitPercussionView::KitPercussionView(GeonkickWidget *parent,
                                     PercussionModel *model)
        : GeonkickWidget(parent)
        , percussionModel{model}
        , nameWidth{100}
        , keyWidth{30}
{
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

void KitPercussionView::setNameWidth(int width)
{
}

void KitPercussionView::setKeyWidth(int width)
{
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

void mouseButtonPressEvent(RkMouseEvent *event) override;
void mouseDoubleClickEvent(RkMouseEvent *event) override;

void drawPercussions(RkPainter &painter);
        void drawConnections(RkPainter &painter);
        void drawConnection(RkPainter &painter, const RkPoint &point);
        int getKey(int x) const;
	void updatePercussionName();
        void keyPressEvent(RkKeyEvent *event) override;
        void editPercussionName(PercussionIndex index);

 private:
        PercussionModel *percussionModel;
	int keyWidth;
	int channelWidth;
	int percussionHeight;
        int percussionNameWidth;
        int percussionWidth;
	RkLineEdit *editPercussion;
	int editedLineIndex;
        RkButton *addButton;
        RkButton *openKitButton;
        RkButton *saveKitButton;
};
