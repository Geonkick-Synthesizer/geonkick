/**
 * File name: RkMain.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor 
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

#include "RkMain.h"
#include "RkLog.h"
#include "RkWidget.h"
#include "RkMainImpl.h"

RkMain::RkMain()
        : o_ptr{std::make_unique<RkMainImpl>(this)}
{
}

RkMain::RkMain(int argc, char **argv)
        : o_ptr{std::make_unique<RkMainImpl>(this, argc, argv)}
{
}

RkMain::~RkMain()
{
}

bool RkMain::setTopLevelWidget(RkWidget *widget)
{
        return o_ptr->setTopLevelWidget(widget);
}

RkWidget* RkMain::topLevelWidget(void) const
{
        return o_ptr->topLevelWidget();
}

RkEventQueue* RkMain::eventQueue() const
{
        return o_ptr->getEventQueue();
}

int RkMain::exec(bool block)
{
	return o_ptr->exec(block);
}
