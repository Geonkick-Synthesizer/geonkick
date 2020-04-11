/**
 * File name: kit_widget.h
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

#ifndef KIT_WIDGET_H
#define KIT_WIDGET_H

#include "geonkick_widget.h"
#include "file_dialog.h"

#include "RkPainter.h"

class RkLineEdit;
class RkButton;
class KitModel;

class KitWidget: public GeonkickWidget
{
 public:
        KitWidget(GeonkickWidget *parent, KitModel *model);
        void updateGui();

 protected:
	void paintWidget(const std::shared_ptr<RkPaintEvent> &event) override;
	void mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event) override;
	void mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event) override;
        void drawKeys(RkPainter &painter);
        void drawPercussions(RkPainter &painter);
        void drawConnections(RkPainter &painter);
        void drawConnection(RkPainter &painter, const RkPoint &point);
        int getLine(int x, int y) const;
        int getKey(int x) const;
	void updatePercussionName();
        void showFileDialog(FileDialog::Type type);
        void openKit(const std::string &file);
        void saveKit(const std::string &file);
        void addNewPercussion();
        void removePercussion(int id);
        void copyPercussion(int id);
        int selectedPercussion() const;
        void keyPressEvent(const std::shared_ptr<RkKeyEvent> &event) override;
        bool validPercussionIndex(int index) const;
        bool validKeyIndex(int keyIndex) const;
        void editPercussionName(int index);

 private:
        KitModel *kitModel;
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

#endif // KIT_WIDGET_H
