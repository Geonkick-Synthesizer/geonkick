/**
 * File name: RkShortcut.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#ifndef RK_SHORTCUT_H
#define RK_SHORTCUT_H

#include "Rk.h"

class RkObject;

class RkShortcut {
 public:
        explicit RkShortcut(Rk::Key key, Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier)
                : shortcutKey{key}
                , shortcutModifier{modifier} {}

        virtual ~RkShortcut() = default;
        void addObject(RkObject *obj)
        {
                shortcutObjects.insert(obj);
        }

        void removeObject(RkObject *obj)
        {
                auto res = shortcutObjects.find(obj);
                if (res != shortcutObjects.end())
                        shortcutObjects.erase(obj);
        }

        bool hasObject(RkObject *obj) const { return shortcutObjects.find(obj) != shortcutObjects.end(); }
        Rk::Key key() const { return shortcutKey; }
        Rk::KeyModifiers modifiers() const { return shortcutModifier; }
        const std::unordered_set<RkObject*>& objects() const { return shortcutObjects; };

  private:
        Rk::Key shortcutKey;
        Rk::KeyModifiers shortcutModifier;
        std::unordered_set<RkObject*> shortcutObjects;
};

#endif // RK_SHORTCUT_H

