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
#include "RkCanvasInfo.h"

#include <random>

static std::string rk_winApiClassName;
static HINSTANCE rk_winApiInstance = nullptr;

HINSTANCE rk_win_api_instance()
{
        RK_LOG_DEBUG("rk_winApiClassName: " << rk_winApiInstance);
        return rk_winApiInstance;
}

LPCSTR rk_win_api_class_name()
{
        RK_LOG_DEBUG("rk_winApiClassName: " << rk_winApiClassName);
        return rk_winApiClassName.c_str();
}

RkNativeWindowInfo rk_from_native_win(HWND window, HINSTANCE instance, LPCSTR className)
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

static LRESULT CALLBACK RkWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
        auto eventQueue = reinterpret_cast<RkEventQueue*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (!eventQueue)
                return DefWindowProc(hWnd, msg, wParam, lParam);

        switch(msg)
        {
        case WM_DESTROY:
        {
                RK_LOG_DEBUG("WM_DESTROY");
                eventQueue->postEvent(rk_id_from_win(hWnd), std::make_unique<RkCloseEvent>());
		eventQueue->processQueue();
                return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
	{
                auto event = std::make_unique<RkMouseEvent>();
                event->setX(LOWORD(lParam));
                event->setY(HIWORD(lParam));
                if (msg == WM_LBUTTONDOWN) {
                        RK_LOG_DEBUG("WM_LBUTTONDOWN");
                        event->setButton(RkMouseEvent::ButtonType::Left);
                } else if (msg == WM_RBUTTONDOWN) {
                        RK_LOG_DEBUG("WM_RBUTTONDOWN");
                        event->setButton(RkMouseEvent::ButtonType::Right);
                } else {
                        RK_LOG_DEBUG("WM_MBUTTONDOWN");
                        event->setButton(RkMouseEvent::ButtonType::Middle);
                }
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
		eventQueue->processQueue();
                return 0;
        }
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
	{
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseButtonRelease);
                event->setX(LOWORD(lParam));
                event->setY(HIWORD(lParam));
                if (msg == WM_LBUTTONUP) {
                        RK_LOG_DEBUG("WM_LBUTTONUP");
                        event->setButton(RkMouseEvent::ButtonType::Left);
                } else if (msg == WM_RBUTTONUP) {
                        RK_LOG_DEBUG("WM_RBUTTONUP");
                        event->setButton(RkMouseEvent::ButtonType::Right);
                } else {
                        RK_LOG_DEBUG("WM_MBUTTONUP");
                        event->setButton(RkMouseEvent::ButtonType::Middle);
                }
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
		eventQueue->processQueue();
                return 0;
        }
        case WM_MOUSEMOVE:
        {
//                RK_LOG_DEBUG("WM_MOUSEMOVE");
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseMove);
                event->setX(LOWORD(lParam));
                event->setY(HIWORD(lParam));
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        case WM_MOUSEHOVER:
        {
                RK_LOG_DEBUG("WM_MOUSEHOVER");
                auto event = std::make_unique<RkHoverEvent>();
                event->setHover(true);
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        case WM_MOUSELEAVE:
        {
                RK_LOG_DEBUG("WM_MOUSELEAVE");
                auto event = std::make_unique<RkHoverEvent>();
                event->setHover(false);
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        case WM_LBUTTONDBLCLK:
        {
                RK_LOG_DEBUG("WM_LBUTTONDBLCLK");
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseDoubleClick);
                event->setButton(RkMouseEvent::ButtonType::Left);
                event->setX(LOWORD(lParam));
                event->setY(HIWORD(lParam));
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }

        case WM_RBUTTONDBLCLK:
        {
                RK_LOG_DEBUG("WM_RBUTTONDBLCLK");
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseDoubleClick);
                event->setButton(RkMouseEvent::ButtonType::Right);
                event->setX(LOWORD(lParam));
                event->setY(HIWORD(lParam));
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }

        case WM_MBUTTONDBLCLK:
        {
                RK_LOG_DEBUG("WM_MBUTTONDBLCLK");
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseDoubleClick);
                event->setButton(RkMouseEvent::ButtonType::Middle);
                event->setX(LOWORD(lParam));
                event->setY(HIWORD(lParam));
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        case WM_MOUSEWHEEL:
        {
                int delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
                auto event = std::make_unique<RkMouseEvent>();
                auto buttonType = (delta > 0) ? RkMouseEvent::ButtonType::WheelUp : RkMouseEvent::ButtonType::WheelDown;
                event->setX(LOWORD(lParam));
                event->setY(HIWORD(lParam));
                event->setButton(buttonType);
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        case WM_KEYDOWN:
        {
                auto event = std::make_unique<RkKeyEvent>();
                event->setType(RkEvent::Type::KeyPressed);
                event->setKey(static_cast<Rk::Key>(wParam));
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }

        case WM_KEYUP:
        {
                auto event = std::make_unique<RkKeyEvent>();
                event->setType(RkEvent::Type::KeyReleased);
                event->setKey(static_cast<Rk::Key>(wParam));
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        case WM_SIZE:
	{
                eventQueue->postEvent(rk_id_from_win(hWnd), std::make_unique<RkResizeEvent>());
		eventQueue->processQueue();
                return 0;
        }
        case WM_PAINT:
        {
//                RK_LOG_DEBUG("WM_PAINT");
//                RK_LOG_DEBUG("BeginPaint");
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);
                auto id = rk_id_from_win(hWnd);
//                RK_LOG_DEBUG("win id: " << id.id);
                eventQueue->postEvent(id, std::make_unique<RkPaintEvent>());
//                RK_LOG_DEBUG("processQueue()");
                eventQueue->processQueue();
//                RK_LOG_DEBUG("EndPaint...");
                EndPaint(hWnd, &ps);
                return 0;
        }
        case WM_SETFOCUS:
        {
                auto event = std::make_unique<RkFocusEvent>();
                event->setType(RkEvent::Type::FocusedIn);
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        case WM_KILLFOCUS:
        {
                auto event = std::make_unique<RkFocusEvent>();
                event->setType(RkEvent::Type::FocusedOut);
                eventQueue->postEvent(rk_id_from_win(hWnd), std::move(event));
                eventQueue->processQueue();
                return 0;
        }
        
        case WM_ERASEBKGND:
	{
                RK_LOG_DEBUG("WM_ERASEBKGND");
                auto widget = eventQueue->getWidget(rk_id_from_win(hWnd));
                if (widget) {
                        auto color = widget->background();
                        auto background = CreateSolidBrush(static_cast<COLORREF>((color.blue() << 16 )
                                                                                 | (color.green() << 8 )
                                                                                 | (color.red())));
                        auto hdc = reinterpret_cast<HDC>(wParam);
                        RECT rect;
                        GetClientRect(hWnd, &rect);
                        FillRect(hdc, &rect, background);
                        DeleteObject(background);
                        return 1L;
                }
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
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;;
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

        if (!RegisterClassEx(&wc)) {
                RK_LOG_ERROR("can't register window class");
                return FALSE;
        }

        return TRUE;
}
#else
static inline char *wideToMulti(int codePage, const wchar_t *aw)
{
        const int required = WideCharToMultiByte(codePage, 0, aw, -1, NULL, 0, NULL, NULL);
        char *result = new char[required];
        WideCharToMultiByte(codePage, 0, aw, -1, result, required, NULL, NULL);
        return result;
}

#ifdef __cplusplus
extern "C" {
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
        RK_LOG_DEBUG("called");
        rk_winApiInstance = hInstance;
        WNDCLASSEX wc;
        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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
        RK_LOG_DEBUG("rk_winApiClassName : " << rk_winApiClassName);
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

#ifdef __cplusplus
}
#endif

#endif // RK_FOR_SHARED
