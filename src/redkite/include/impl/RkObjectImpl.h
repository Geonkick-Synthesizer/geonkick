/**
 * File name: RkObjectImpl.h
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

#ifndef RK_OBJECT_IMPL_H
#define RK_OBJECT_IMPL_H

#include "RkObject.h"

class RkEventQueue;

class RkObject::RkObjectImpl {
 public:
        explicit RkObjectImpl(RkObject* interface,
                              RkObject* parent,
                              Rk::ObjectType type = Rk::ObjectType::Object);
        virtual ~RkObjectImpl();
        void removeChildrens();
        RkObject* parent() const;
        const std::unordered_set<RkObject*>& getChildren() const;
        virtual void setEventQueue(RkEventQueue *queue);
        RkEventQueue* getEventQueue() const;
        void addObserver(std::unique_ptr<RkObserver> ob);
        void removeObservers(RkObject *obj);
        const std::vector<std::unique_ptr<RkObserver>>& observers() const;
        void addBoundObject(RkObject *obj);
        void removeBoundObject(RkObject *obj);
        void addChild(RkObject* child);
        void removeChild(RkObject* child);
        Rk::ObjectType getObjectType() const;
        void setName(const std::string &name);
        std::string name() const;

 private:
        RK_DECALRE_INTERFACE_PTR(RkObject);
        Rk::ObjectType objectType;
        RkObject *parentObject;
        RkEventQueue *eventQueue;
        std::unordered_set<RkObject*> objectChildren;
        std::vector<std::unique_ptr<RkObserver>> observersList;
        std::vector<RkObject*> boundObjects;
        std::string objectName;
};

#endif // RK_OBJECT_IMPL_H
