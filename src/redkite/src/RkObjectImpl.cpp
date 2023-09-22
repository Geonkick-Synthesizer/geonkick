/**
 * File name: RkObjectImpl.cpp
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
#include "RkEventQueue.h"
#include "RkObserver.h"

RkObject::RkObjectImpl::RkObjectImpl(RkObject* interface,
                                     RkObject* parent,
                                     Rk::ObjectType type)
        : inf_ptr{interface}
        , objectType{type}
        , parentObject{parent}
        , eventQueue{nullptr}
{
        RK_LOG_DEBUG("["<< this << "][inf: " << inf_ptr << "]: parent " << parent);
}

RkObject::RkObjectImpl::~RkObjectImpl()
{
        RK_LOG_DEBUG("[" << this << "]: interface: " << inf_ptr);
        // Remove myself from bound objects.
        for (auto &obj: boundObjects)
                obj->removeObservers(inf_ptr);
        boundObjects.clear();

        // Remove myself from the observers objects.
        for (const auto &o: observersList) {
                if (o->object())
                        o->object()->removeBoundObject(inf_ptr);
        }
        observersList.clear();

        // Remove myself from the paren object.
        if (inf_ptr->parent())
                inf_ptr->parent()->removeChild(inf_ptr);
}

void RkObject::RkObjectImpl::removeChildrens()
{
        RK_LOG_DEBUG("size: " << objectChildren.size());
        auto tmpChidlren = std::move(objectChildren);
        for (auto child: tmpChidlren)
                delete child;
}

RkObject* RkObject::RkObjectImpl::parent() const
{
        return parentObject;
}

const std::unordered_set<RkObject*>&
RkObject::RkObjectImpl::getChildren() const
{
        return objectChildren;
}

void RkObject::RkObjectImpl::setEventQueue(RkEventQueue *queue)
{
        if (!eventQueue && queue) {
                eventQueue = queue;
                for (auto child: objectChildren)
                        child->setEventQueue(eventQueue);
        }
}

RkEventQueue* RkObject::RkObjectImpl::getEventQueue() const
{
        return eventQueue;
}

void RkObject::RkObjectImpl::addObserver(std::unique_ptr<RkObserver> ob)
{
        auto res = std::find(observersList.begin(), observersList.end(), ob);
        if (res == std::end(observersList))
                observersList.push_back(std::move(ob));
}

void RkObject::RkObjectImpl::removeObservers(RkObject *obj)
{
        observersList.erase(std::remove_if(observersList.begin(),
                                           observersList.end(),
                                           [obj](const std::unique_ptr<RkObserver> &o)  {
                                                   return o->object() != nullptr
                                                           && o->object() == obj;
                                               })
                                , observersList.end());
}

const std::vector<std::unique_ptr<RkObserver>>&
RkObject::RkObjectImpl::observers() const
{
        return observersList;
}

void RkObject::RkObjectImpl::addBoundObject(RkObject *obj)
{
        auto res = std::find(std::begin(boundObjects), std::end(boundObjects), obj);
        if (res == std::end(boundObjects))
                boundObjects.push_back(obj);
}

void RkObject::RkObjectImpl::removeBoundObject(RkObject *obj)
{
        boundObjects.erase(std::remove_if(boundObjects.begin(),
                                              boundObjects.end(),
                                              [obj](RkObject *o)  {
                                                      if (o == obj)
                                                              return true;
                                                      return false;
                                              })
                               , boundObjects.end());
}

void RkObject::RkObjectImpl::addChild(RkObject* child)
{
        RK_LOG_DEBUG("add child: " << child);
        objectChildren.insert(child);
        if (eventQueue) {
                RK_LOG_DEBUG("add child to queue: " << child);
                eventQueue->addObject(child);
        }
}

void RkObject::RkObjectImpl::removeChild(RkObject* child)
{
        RK_LOG_DEBUG("size: " << objectChildren.size());
        RK_LOG_DEBUG("remove child: " << child);
        if (!objectChildren.empty()) {
                auto res = objectChildren.find(child);
                if (res != objectChildren.end()) {
                        RK_LOG_DEBUG("erase child" << child);
                        objectChildren.erase(child);
                }
        }
}

Rk::ObjectType RkObject::RkObjectImpl::getObjectType() const
{
        return objectType;
}

void RkObject::RkObjectImpl::setName(const std::string &name)
{
        objectName = name;
}

std::string RkObject::RkObjectImpl::name() const
{
        return objectName;
}
