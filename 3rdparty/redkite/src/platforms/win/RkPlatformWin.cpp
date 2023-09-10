/**
 * File name: RkPlatformWin.cpp
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

#include "RkPlatform.h"
#include "RkEventQueue.h"
#include "RkEvent.h"
#include "RkWidget.h"

#include <random>

static std::string rk_winApiClassName;
static HINSTANCE rk_winApiInstance = nullptr;
#ifdef RK_GRAPHICS_BACKEND_DIRECT2D
#include <d2d1_1.h>
#include <dwrite.h>
static ID2D1Factory1* rk_d2d1Factory = nullptr;
static IDWriteFactory* rk_dWriteFactory = nullptr;
#endif // RK_GRAPHICS_BACKEND_DIRECT2D


HINSTANCE rk_win_api_instance()
{
        return rk_winApiInstance;
}

std::string rk_win_api_class_name()
{
        return rk_winApiClassName;
}

RkNativeWindowInfo rk_from_native_win(HWND window, HINSTANCE instance = nullptr, LPCSTR className = nullptr)
{
        RkNativeWindowInfo info;
        info.instance = instance ? instance : rk_winApiInstance;
        info.className = className ? className : rk_winApiClassName;
        info.window = window;
        return info;
}

RkWindowId rk_id_from_win(HWND window)
{
        RkWindowId id;
        id.id = window;
        return id;
}

#ifdef RK_GRAPHICS_BACKEND_DIRECT2D
ID2D1Factory1* rk_direct2d_factory()
{
        return rk_d2d1Factory;
}

IDWriteFactory* rk_direct_write_factory()
{
        return rk_dWriteFactory;
}
#endif // RK_GRAPHICS_BACKEND_DIRECT2D

static LRESULT CALLBACK RkWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
        auto eventQueue = (RkEventQueue*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!eventQueue)
                return DefWindowProc(hWnd, msg, wParam, lParam);

        switch(msg)
        {
        case WM_DESTROY:
        {
                eventQueue->processEvent(rk_id_from_win(hWnd), std::make_shared<RkCloseEvent>());
                return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
	{
                eventQueue->processEvent(rk_id_from_win(hWnd), std::make_shared<RkMouseEvent>());
                return 0;
        }
        case WM_SIZE:
	{
                eventQueue->processEvent(rk_id_from_win(hWnd), std::make_shared<RkResizeEvent>());
                return 0;
        }
        case WM_PAINT:
        {
                eventQueue->processEvent(rk_id_from_win(hWnd), std::make_shared<RkPaintEvent>());
                ValidateRect(hWnd, NULL);
                return 0;
        }
        case WM_ERASEBKGND:
	{
                auto color = eventQueue->getWidget(rk_id_from_win(hWnd))->background();
                auto background = CreateSolidBrush(static_cast<COLORREF>((color.blue() << 16 ) | (color.green() << 8 ) | (color.red())));
                auto hdc = (HDC)wParam;
                RECT rect;
                GetClientRect(hWnd, &rect);
                FillRect(hdc, &rect, background);
                DeleteObject(background);
                return 1L;
        }
        default:
                break;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
}

#ifdef RK_FOR_SHARED
BOOL WINAPI DllMain(HINSTANCE hInstance,
                    DWORD fdwReason,
                    LPVOID lpvReserved)
{
        rk_winApiInstance = hInstance;
		RK_LOG_DEBUG("instance:" << rk_winApiInstance);
        WNDCLASSEX wc;
        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = 0;
        wc.lpfnWndProc   = RkWindowProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;
        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName  = NULL;

        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<int> uniform_dist(1, 1000000);
        int mean = uniform_dist(e1);
        rk_winApiClassName = ("Redkite_" + std::to_string(mean)).c_str();
        wc.lpszClassName = rk_winApiClassName.c_str();
        wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

#ifdef RK_GRAPHICS_BACKEND_DIRECT2D
        if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &rk_d2d1Factory) != S_OK) {
                RK_LOG_ERROR("can't create D2D1 factory");
                return FALSE;
        }

        if (DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED,
                                __uuidof(IDWriteFactory),
                                reinterpret_cast<IUnknown**>(&rk_dWriteFactory)) != S_OK) {
                RK_LOG_ERROR("can't create Direct Write factory");
                rk_d2d1Factory->Release();
                return FALSE;
        }
#endif // RK_GRAPHICS_BACKEND_DIRECT2D

        if (!RegisterClassEx(&wc)) {
                RK_LOG_ERROR("can't register window class");
                return FALSE;
        }

        return TRUE;
}
#else // RK_FOR_SHARED
static inline char *wideToMulti(int codePage, const wchar_t *aw)
{
        const int required = WideCharToMultiByte(codePage, 0, aw, -1, NULL, 0, NULL, NULL);
        char *result = new char[required];
        WideCharToMultiByte(codePage, 0, aw, -1, result, required, NULL, NULL);
        return result;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
        rk_winApiInstance = hInstance;
        WNDCLASSEX wc;
        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = 0;
        wc.lpfnWndProc   = RkWindowProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;
        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName  = NULL;

        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<int> uniform_dist(1, 1000000);
        int mean = uniform_dist(e1);
        rk_winApiClassName = "Redkite_" + std::to_string(mean);
        wc.lpszClassName = rk_winApiClassName.c_str();
        wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

#ifdef RK_GRAPHICS_BACKEND_DIRECT2D
        if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &rk_d2d1Factory) != S_OK) {
                RK_LOG_ERROR("can't create D2D1 factory");
                return FALSE;
        }

        if (DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED,
                                __uuidof(IDWriteFactory),
                                reinterpret_cast<IUnknown**>(&rk_dWriteFactory)) != S_OK) {
                RK_LOG_ERROR("can't create Direct Write factory");
                rk_d2d1Factory->Release();
                return FALSE;
        }
#endif // RK_GRAPHICS_BACKEND_DIRECT2D

        if (!RegisterClassEx(&wc)) {
                RK_LOG_ERROR("can't register window class");
                return 1;
        }

        int argc;
        wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
        if (!argvW)
                return 1;

        std::vector<char*> args(argc, nullptr);
        for (decltype(args.size()) i = 0; i < args.size(); i++)
                args[i] = wideToMulti(CP_ACP, argvW[i]);
        LocalFree(argvW);

        const int exitCode = main(args.size(), args.data());
        for (auto arg : args)
                delete [] arg;

        return exitCode;
}
#endif // RK_FOR_SHARED
