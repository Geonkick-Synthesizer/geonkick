/**
 * File name: RkObject.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2020 Iurie Nistor
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

#ifndef RK_OBJECT_H
#define RK_OBJECT_H

#include "Rk.h"
#include "RkObserver.h"

class RkEvent;
class RkEventQueue;

class RkObject {
 public:
        explicit RkObject(RkObject *parent = nullptr);
        virtual ~RkObject();
        RK_DECL_ACT(aboutToBeDeleted,
                    aboutToBeDeleted(RkObject *obj),
                    RK_ARG_TYPE(RkObject*), RK_ARG_VAL(obj));
        Rk::ObjectType type() const;
        RkObject* parent() const;
        const std::unordered_set<RkObject*>& children() const;
        void setEventQueue(RkEventQueue* queue);
        RkEventQueue* eventQueue() const;
        virtual void event(RkEvent *event);
        void addShortcut(Rk::Key key, Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void removeShortcut(Rk::Key key, Rk::KeyModifiers modifier = Rk::KeyModifiers::NoModifier);
        void setName(const std::string &name);
        std::string name() const;
        void rk__add_bound_object(RkObject* obj);
        void unbindObject(RkObject* obj);

 protected:
        RK_DECLARE_O_PTR(RkObject);
        explicit RkObject(RkObject *parent, std::unique_ptr<RkObjectImpl> impl);
        void rk__add_observer(std::unique_ptr<RkObserver> observer);
        const std::list<std::unique_ptr<RkObserver>>& rk__observers() const;

 private:
        void addChild(RkObject *child);
        void removeChild(RkObject *child);
        void removeObservers(RkObject *ob);
        void removeBoundObject(RkObject *obj);
        friend class RkEventQueue;
        RK_DISABLE_COPY(RkObject);
        RK_DISABLE_MOVE(RkObject);
};

#endif // RK_OBJECT_H
