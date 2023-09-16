/**
 * File name: RkWindowWin.cpp
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

#include "RkLog.h"
#include "RkWindowWin.h"
//#include "RkCanvasInfo.h"

RkWindowWin::RkWindowWin(const RkNativeWindowInfo *parent, Rk::WindowFlags flags, bool isTop)
        : parentWindowInfo{parent ? *parent : RkNativeWindowInfo() }
	, windowInfo{nullptr}
        , windowHandle{0}
        , windowPosition{0, 0}
        , windowSize{250, 250}
        , winBorderWidth{0}
        , winBorderColor{255, 255, 255}
        , backgroundColor{255, 255, 255}
        , eventQueue{nullptr}
        //, canvasInfo{nullptr}
        , windowFlags{flags}
	, isTopWindow{isTop}
{
}

RkWindowWin::RkWindowWin(const RkNativeWindowInfo &parent, Rk::WindowFlags flags, bool isTop)
        : parentWindowInfo{parent}
	, windowInfo{nullptr}
        , windowHandle{0}
        , windowPosition{0, 0}
        , windowSize{250, 250}
        , winBorderWidth{0}
        , winBorderColor{255, 255, 255}
        , backgroundColor{255, 255, 255}
        , eventQueue{nullptr}
        //, canvasInfo{nullptr}
        , windowFlags{flags}
	, isTopWindow{isTop}
{
}

RkWindowWin::~RkWindowWin()
{
	freeCanvasInfo();
}

Rk::WindowFlags RkWindowWin::flags() const
{
        return windowFlags;
}

bool RkWindowWin::hasParent() const
{
  return false;// parentWindowInfo != nullptr;
}

bool RkWindowWin::init()
{
  /*        windowHandle.id = CreateWindowExA(0,
                                          hasParent() ? parentWindowInfo->className.c_str() : rk_win_api_class_name().c_str(),
                                          "RkWidget",
                                          hasParent() ? (WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE) : (WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN),
                                          windowPosition.x(),
                                          windowPosition.y(),
                                          windowSize.width(),
                                          windowSize.height(),
                                          !hasParent() ? nullptr : parentWindowInfo->window,
                                          nullptr,
                                          hasParent() ? parentWindowInfo->instance : rk_win_api_instance(),
                                          nullptr);

        if (!windowHandle.id) {
                RK_LOG_ERROR("can't create window");
                return false;
        }

        if (eventQueue)
                SetWindowLongPtr(windowHandle.id, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(eventQueue));
		createCanvasInfo();*/
        return true;
}

void RkWindowWin::show(bool b)
{
  /*        if (isWindowCreated()) {
                ShowWindow(windowHandle.id, b ? SW_SHOW : SW_HIDE);
                UpdateWindow(windowHandle.id);
		}*/
}

const RkNativeWindowInfo* RkWindowWin::nativeWindowInfo() const
{
  /*        if (isWindowCreated()) {
                windowInfo            = std::make_unique<RkNativeWindowInfo>();
                windowInfo->className = hasParent() ? parentWindowInfo->className : rk_win_api_class_name();
                windowInfo->window    = windowHandle.id;
                windowInfo->instance  = hasParent() ? parentWindowInfo->instance : rk_win_api_instance();
                return info.get();
		}*/

        return nullptr;
}

void RkWindowWin::setTitle(const std::string &title)
{
  //        if (isWindowCreated() && !title.empty())
  //              SetWindowTextA(windowHandle.id, title.c_str());
}

bool RkWindowWin::isWindowCreated() const
{
  return false;//windowHandle.id != nullptr;
}

const RkSize& RkWindowWin::size() const
{
  //    if (isWindowCreated()) {
  //            RECT rect;
  //            GetWindowRect(windowHandle.id, &rect);
  //            windowSize = {rect.right - rect.left, rect.bottom - rect.top};
  //    }
        return windowSize;
}

void RkWindowWin::setSize(const RkSize &size)
{
  /*    if (size.width() > 0 && size.height() > 0) {
                windowSize = size;
                if (isWindowCreated())
                        SetWindowPos(windowHandle.id, 0, 0, 0, size.width(), size.height(), SWP_NOMOVE | SWP_NOZORDER);
                resizeCanvas();
		}*/
}

RkPoint& RkWindowWin::position() const
{
  /*        if (isWindowCreated()) {
                RECT rect;
                GetWindowRect(windowHandle.id, &rect);
                RECT rect0 = rect;
                if (hasParent())
                        MapWindowPoints(windowHandle.id, GetParent(windowHandle.id), reinterpret_cast<LPPOINT>(&rect), 2);
                windowPosition = {rect.left - rect0.left, rect.top - rect0.top};
		}*/
        return windowPosition;
}

void RkWindowWin::setPosition(const RkPoint &position)
{
  /*        windowPosition = position;
        if (isWindowCreated())
	SetWindowPos(windowHandle.id, 0, position.x(), position.y(), 0, 0, SWP_NOSIZE | SWP_NOZORDER);*/
}

void RkWindowWin::setBorderWidth(int width)
{
	// IMPLEMENT
        winBorderWidth = width;
}

int RkWindowWin::borderWidth() const
{
	//IMPLEMENT
	return winBorderWidth;
}

void RkWindowWin::setBorderColor(const RkColor &color)
{
        winBorderColor = color;
}

const RkColor& RkWindowWin::borderColor() const
{
	return winBorderColor;
}

void RkWindowWin::setBackgroundColor(const RkColor &background)
{
        backgroundColor = background;
}

RkWindowId RkWindowWin::id() const
{
        RkWindowId id;
        id.id = windowHandle.id;
        return id;
}

const RkColor& RkWindowWin::background() const
{
	return backgroundColor;
}

/*RkCanvasInfo* RkWindowWin::getCanvasInfo() const
{
	return canvasInfo;
	}*/

void RkWindowWin::update()
{
}

#ifdef RK_GRAPHICS_BACKEND_DIRECT2D
void RkWindowWin::createCanvasInfo()
{
/*        canvasInfo = std::make_shared<RkCanvasInfo>();
        RkDirect2DDeviceInfo* device2DInfo = &canvasInfo->direct2DInfo;
        device2DInfo->window = windowHandle.id;
        D3D_FEATURE_LEVEL featureLevels[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1};

       auto hr =  D3D11CreateDevice(
                                    nullptr,
                                    D3D_DRIVER_TYPE_HARDWARE,
                                    nullptr,
                                    D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
                                    featureLevels,
                                    ARRAYSIZE(featureLevels),
                                    D3D11_SDK_VERSION,
                                    &device2DInfo->device3D,
                                    nullptr,
                                    nullptr);
       if (!SUCCEEDED(hr)) {
               RK_LOG_ERROR("can't create 3D device");
               canvasInfo = nullptr;
       } else {
               device2DInfo->device3D->QueryInterface(&device2DInfo->dxgiDevice);
               hr = rk_direct2d_factory()->CreateDevice(device2DInfo->dxgiDevice, &device2DInfo->device2D);
               if (!SUCCEEDED(hr)) {
                       RK_LOG_ERROR("can't create 2D device");
					   device2DInfo->dxgiDevice->Release();
                       device2DInfo->device3D->Release();
                       canvasInfo = nullptr;
               }
       }*/
}

void RkWindowWin::resizeCanvas()
{
// No need to be implemented for Direct2D.
}

void RkWindowWin::freeCanvasInfo()
{
        /*if (canvasInfo) {
                canvasInfo->direct2DInfo.device2D->Release();
                canvasInfo->direct2DInfo.dxgiDevice->Release();
                canvasInfo->direct2DInfo.device3D->Release();
                canvasInfo = nullptr;
        }*/
}
#else
//#error No graphics backend defined
#endif // RK_GRAPHICS_CAIRO_BACKEND

void RkWindowWin::setFocus(bool b)
{
        // IMPLEMENT
}

bool RkWindowWin::hasFocus()
{
        // IMPLEMENT
        return false;
}

bool RkWindowWin::pointerIsOverWindow() const
{
	// IMPLEMENT
	return false;
}

void RkWindowWin::setPointerShape(Rk::PointerShape shape)
{
        // IMPLEMENT
}

void RkWindowWin::setEventQueue(RkEventQueue *queue)
{
  /*        eventQueue = queue;
        SetWindowLongPtr(windowHandle.id, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(eventQueue));
        InvalidateRect(windowHandle.id, nullptr, FALSE);
        UpdateWindow(windowHandle.id);*/
}

void RkWindowWin::setScaleFactor(double factor)
{
	// IMPLEMENT
}

double RkWindowWin::getScaleFactor() const
{
	// IMPLEMENT
	return 1.0;
}

