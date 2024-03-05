/**
 * File name: RkMain.h
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

#ifndef RK_MAIN_H
#define RK_MAIN_H

#include "Rk.h"

class RkWidget;
class RkEventQueue;
class RkNativeWindowInfo;

class RkMain {
  public:
          RkMain();
          RkMain(int argc, char **argv);
          ~RkMain();
          RkWidget* topLevelWidget(void) const;
          int exec(bool block = true);
          RkEventQueue* eventQueue() const;

 protected:
          RK_DECLARE_O_PTR(RkMain);

 private:
          RK_DISABLE_COPY(RkMain);
          RK_DISABLE_MOVE(RkMain);
};

#endif // RK_MAIN_H
