/**
 * File name: kit.h
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

#ifndef GEONKICK_KIT_H
#define GEONKCIK_KIT_H

class GeonkickApi;
class GeonkickState;

class Kit {
        explicit Kit(GeonkickApi *api);
        bool loadKit(const std::string &file);
        bool saveKit(const std::string &file);
        bool loadPercussion(const std::string &file);
        bool savePercussion(int index, const std::string &file);
        void selectPercussion(int index);
        void addPercussion(const std::shared_ptr<PercussionState> &percussion);
        //        void updatePercussion(int index, const std::shared_ptr<PercussionState> &percussion);
        void removePercussion(int index);
        void copyPercussion(int index);
        void movePercussionUp(int index);
        void movePercussionDown(int index);
        int percussionKey(int index) const;
        void setPercussionKey(int index, int key);
        size_t percussionsNumber() const;

 protected:
        int getId(int index);
        bool isIdValid(int id);

 private:
        GeopnkickApi *geonkickApi;
        std::vector<int> percussionIds;
};

#endif // GEONKICK_KIT_H
