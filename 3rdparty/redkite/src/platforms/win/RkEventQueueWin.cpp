/**
 * File name: RkQueueWin.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/redkite)
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

#include "RkEventQueueWin.h"
#include "RkLog.h"
#include "RkWidget.h"
#include "RkEvent.h"

RkEventQueueWin::RkEventQueueWin()
{
        RK_LOG_INFO("called");
}

RkEventQueueWin::~RkEventQueueWin()
{
}

void RkEventQueueWin::getEvents(std::vector<std::pair<RkWindowId, std::shared_ptr<RkEvent>>> &eventsQueue)
{
        MSG msg;
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE) > 0) {
                /*std::shared_ptr<RkEvent> event = nullptr;
                switch (msg.message)
                {
                case RK_WIN_MESSAGE_PAINT:
					    event = std::make_shared<RkPaintEvent>();
                        break;
                //case WM_PAINT:
                //        OutputDebugString("getEvents [REDKITE]Event:paint");
                //        event = RkEvent::paintEvent();
                //        break;
                case WM_KEYDOWN:
                        //event = RkEvent::keyPressEvent();
                        break;
                case WM_KEYUP:
                        //event = RkEvent::keyReleaseEvent();
                        break;
                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN:
                case WM_MBUTTONDOWN:
					     event = std::make_shared<RkMouseEvent>();
                        break;
                case WM_LBUTTONUP:
                case WM_RBUTTONUP:
                case WM_MBUTTONUP:
						event = std::make_shared<RkMouseEvent>();
                        break;
                case WM_SIZE:
                        //event = RkEvent::resizeEvent();
                        break;
                case WM_QUIT:
                case WM_CLOSE:
                case WM_DESTROY:
                        event = std::make_shared<RkCloseEvent>();
                        break;
                default:
                        break;
                        }*/

                TranslateMessage (&msg);
                DispatchMessage (&msg);

                //                if (event)
                //        eventsQueue.push_back({rk_id_from_win(msg.hwnd), event});
        }
}
