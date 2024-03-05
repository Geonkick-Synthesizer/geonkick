/**
 * File name: RkObject.cpp
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

#include "RkObjectImpl.h"
#include "RkLog.h"
#include "RkEventQueueImpl.h"

RkObject::RkObject(RkObject *parent)
        : o_ptr{std::make_unique<RkObjectImpl>(this, parent)}
{
        RK_LOG_DEBUG("called: " << this);
        if (parent)
                parent->addChild(this);
}

RkObject::RkObject(RkObject *parent, std::unique_ptr<RkObjectImpl> impl)
        : o_ptr{std::move(impl)}
{
        RK_LOG_DEBUG("called: " << this);
        if (parent)
                parent->addChild(this);
}

RkObject::~RkObject()
{
        RK_LOG_DEBUG("called: " << this);
        if (eventQueue())
                RK_IMPL_PTR(eventQueue())->removeObject(this);
        o_ptr->removeChildrens();
}

Rk::ObjectType RkObject::type() const
{
        return o_ptr->getObjectType();
}

RkObject* RkObject::parent() const
{
        return o_ptr->parent();
}

const std::unordered_set<RkObject*>& RkObject::children() const
{
        return o_ptr->getChildren();
}

void RkObject::setEventQueue(RkEventQueue* queue)
{
        o_ptr->setEventQueue(queue);
}

RkEventQueue* RkObject::eventQueue() const
{
        return o_ptr->getEventQueue();
}

void RkObject::event(RkEvent *event)
{
}

void RkObject::addShortcut(Rk::Key key, Rk::KeyModifiers modifier)
{
        if (eventQueue())
                RK_IMPL_PTR(eventQueue())->addShortcut(this, key, modifier);
}

void RkObject::removeShortcut(Rk::Key key, Rk::KeyModifiers modifier)
{
        if (eventQueue())
                RK_IMPL_PTR(eventQueue())->removeShortcut(this, key, modifier);
}

void RkObject::rk__add_observer(std::unique_ptr<RkObserver> observer)
{
        o_ptr->addObserver(std::move(observer));
}

const std::list<std::unique_ptr<RkObserver>>& RkObject::rk__observers() const
{
        return o_ptr->observers();
}

void RkObject::rk__add_bound_object(RkObject* obj)
{
        o_ptr->addBoundObject(obj);
}

void RkObject::addChild(RkObject* child)
{
        o_ptr->addChild(child);
}

void RkObject::removeChild(RkObject *child)
{
        o_ptr->removeChild(child);
}

void RkObject::removeObservers(RkObject *obj)
{
        o_ptr->removeObservers(obj);
}

void RkObject::removeBoundObject(RkObject *obj)
{
        o_ptr->removeBoundObject(obj);
}

RkObject* RkObject::findObject(const std::string &name) const
{
        //        if (eventQueue())
        //        return RK_IMPL_PTR(eventQueue())->findObjectByName(name);
        return nullptr;
}

void RkObject::setName(const std::string &name)
{
        o_ptr->setName(name);
}

std::string RkObject::name() const
{
        return o_ptr->name();
}
