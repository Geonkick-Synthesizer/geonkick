/**
 * File name: channels_widget.h
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

#ifndef CHANNELS_WIDGET_H
#define CHANNELS_WIDGET_H

#include "geonkick_widget.h"
#include "file_dialog.h"

#include "RkPainter.h"

class GeonkickApi;
class RkLineEdit;
class RkButton;

class ChannelsWidget: public GeonkickWidget
{
        struct ChannelKey {
                size_t id;
                std::string name;
                RkRect rect;
        };

        struct Channel {
                size_t id;
                std::string name;
                RkRect rect;
        };

 public:
        ChannelsWidget(GeonkickWidget *parent, GeonkickApi* api);
        ~ChannelsWidget() = default;
        RK_DECL_ACT(updateGui, updateGui(), RK_ARG_TYPE(), RK_ARG_VAL());

 protected:
	void paintWidget(const std::shared_ptr<RkPaintEvent> &event) override;
	void mouseButtonPressEvent(const std::shared_ptr<RkMouseEvent> &event) override;
	void mouseDoubleClickEvent(const std::shared_ptr<RkMouseEvent> &event) override;
        void drawKeys(RkPainter &painter);
        void drawChannels(RkPainter &painter);
        void drawConnections(RkPainter &painter);
        void drawConnection(RkPainter &painter, const RkPoint &point);
        Channel* getChannel(int x, int y);
        const ChannelKey* getKey(int x, int y) const;
        RkPoint getIntersectionPoint(const ChannelKey &key, const Channel &channel) const;
        void createKeys();
        void createChannels();
	void updateChannelName();
        void addChannel();
        void openFileDialog(FileDialog::Type type);
        void openKit(const std::string &file);
        void saveKit(const std::string &file);

 private:
	GeonkickApi* geonkickApi;
        std::vector<ChannelKey> midiKeys;
        std::vector<Channel> channelsList;
        std::vector<std::array<bool, 17>> connectionMatrix;
	int keyWidth;
	int channelHeight;
	int channesNameWidth;
	RkLineEdit *editChannel;
	Channel *editedChannel;
        RkButton *addButton;
        RkButton *openKitButton;
        RkButton *saveKitButton;
};

#endif // CHANNELS_WIDGET_H
