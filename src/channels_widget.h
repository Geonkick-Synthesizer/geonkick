/**
 * File name: kit_widget.h
 * Project: Geonkick (A kick synthesizer)
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

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "RkPainter.h"

class GeonkickApi;
class RkLineEdit;
class RkButton;

class KitWidget: public GeonkickWidget
{
        struct KeyInfo {
                std::string name;
                char key;
        };

        struct Percussion {
                size_t id;
                std::string name;
                std::string file;
                char key;
                bool enabled;
                double limiter;
        };

        struct Kit {
                std::string name;
                std::string author;
                std::string url;
                std::vector<Percussion> list;
        };

 public:
        KitWidget(GeonkickWidget *parent, GeonkickApi* api);
        ~KitWidget() = default;
        RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());

 protected:
	void paintWidget(const std::shared_ptr<RkPaintEvent> &event) override;
	void mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event) override;
	void mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event) override;
        void drawKeys(RkPainter &painter);
        void drawPercussions(RkPainter &painter);
        void drawConnections(RkPainter &painter);
        void drawConnection(RkPainter &painter, const RkPoint &point);
        Percussion* getPercussion(int x, int y) const;
        const KeyInfo* getKey(int x, int y) const;
        RkPoint getIntersectionPoint(const PercussionKey &key, const Percussion &percussion) const;
        void createKeys();
	void updatePercussionName();
        void addPercussion(const Percussion &per);
        void openFileDialog(FileDialog::Type type);
        void openKit(const std::string &file);
        void saveKit(const std::string &file);
        Kit parseKit(std::string &fileData, const std::filesystem::path &path);
        std::vector<Percussion> parsePercussions(const rapidjson::Value &envelopeArray,
                                                 const std::filesystem::path &path);
        void addNewPercussion();
        void removePercussion(int id);
        void copyPercussion(int id);

 private:
	GeonkickApi* geonkickApi;
        std::vector<KeyInfo> midiKeys;
        std::vector<Percussion> kitList;
	int keyWidth;
	int percussionHeight;
	int channesNameWidth;
	RkLineEdit *editPercussion;
	Percussion *editedPercussion;
        RkButton *addButton;
        RkButton *openKitButton;
        RkButton *saveKitButton;
};

#endif // KIT_WIDGET_H
