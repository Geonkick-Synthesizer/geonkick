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
#include "RkEventQueueImpl.h"
#include "RkEventQueueWin.h"
#include "RkEvent.h"
#include "RkWidget.h"
#include "RkCanvasInfo.h"
#include "RkWindowWin.h"
#include "RkSystemWindow.h"

#include <random>

#include <windowsx.h>

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

#include <Windows.h>

static void processSystemEvent(auto& eventQueue, std::unique_ptr<RkEvent> event)
{
    eventQueue->getPlatformEventQueue()->addEvent(std::move(event));
    eventQueue->processEvents();
}

static Rk::Key convertToRkKey(unsigned int winKey)
{
        if (winKey >= 0x30 && winKey <= 0x39)
                return static_cast<Rk::Key>(winKey);

        if (winKey >= 0x41 && winKey <= 0x5A) {
                if (!(GetKeyState(VK_SHIFT) & 0x8000))
                        return static_cast<Rk::Key>(tolower(winKey));
                return static_cast<Rk::Key>(winKey);
        }

        Rk::Key rkKey;
        switch(winKey) {
        case VK_BACK:
                rkKey = Rk::Key::Key_BackSpace;
                break;
        case VK_TAB:
                rkKey = Rk::Key::Key_Tab;
                break;
        case VK_CLEAR:
                rkKey = Rk::Key::Key_Linefeed;
                break;
        case VK_RETURN:
                rkKey = Rk::Key::Key_Return;
                break;
        case VK_PAUSE:
                rkKey = Rk::Key::Key_Pause;
                break;
        case VK_SCROLL:
                rkKey = Rk::Key::Key_Scroll_Lock;
                break;
        case VK_PRINT:
                rkKey = Rk::Key::Key_Sys_Req;
                break;
        case VK_ESCAPE:
                rkKey = Rk::Key::Key_Escape;
                break;
        case VK_DELETE:
                rkKey = Rk::Key::Key_Delete;
                break;
        case VK_SHIFT:
        case VK_LSHIFT:
                rkKey = Rk::Key::Key_Shift_Left;
                break;
        case VK_RSHIFT:
                rkKey = Rk::Key::Key_Shift_Right;
                break;
        case VK_CONTROL:
        case VK_LCONTROL:
                rkKey = Rk::Key::Key_Control_Left;
                break;
        case VK_RCONTROL:
                rkKey = Rk::Key::Key_Control_Right;
                break;
        case VK_MENU:
        case VK_LMENU:
                rkKey = Rk::Key::Key_Alt_Left;
                break;
        case VK_RMENU:
                rkKey = Rk::Key::Key_Alt_Right;
                break;
        case VK_CAPITAL:
                rkKey = Rk::Key::Key_Caps_Lock;
                break;
        case VK_NUMLOCK:
                rkKey = Rk::Key::Key_Shift_Lock;
                break;
        case VK_LWIN:
                rkKey = Rk::Key::Key_Meta_Left;
                break;
        case VK_RWIN:
                rkKey = Rk::Key::Key_Meta_Right;
                break;
        case VK_HOME:
                rkKey = Rk::Key::Key_Home;
                break;
        case VK_LEFT:
                rkKey = Rk::Key::Key_Left;
                break;
        case VK_RIGHT:
                rkKey = Rk::Key::Key_Right;
                break;
        case VK_UP:
                rkKey = Rk::Key::Key_Up;
                break;
        case VK_DOWN:
                rkKey = Rk::Key::Key_Down;
                break;
        case VK_PRIOR:
                rkKey = Rk::Key::Key_Page_Up;
                break;
        case VK_NEXT:
                rkKey = Rk::Key::Key_Page_Down;
                break;
        case VK_END:
                rkKey = Rk::Key::Key_End;
                break;
        case VK_OEM_PLUS:
                rkKey = Rk::Key::Key_Plus;
                break;
        case VK_OEM_MINUS:
                rkKey = Rk::Key::Key_Minus;
                break;
        default:
                rkKey = Rk::Key::Key_None;
        }
        return rkKey;
}

int rkKeyModifiers = 0;

static void rkUpdateModifiers(Rk::Key key, RkEvent::Type type)
{
        if (key == Rk::Key::Key_Shift_Left
            || key == Rk::Key::Key_Shift_Right
            || key == Rk::Key::Key_Control_Left
            || key == Rk::Key::Key_Control_Right)
        {
                if (type == RkEvent::Type::KeyPressed)
                        rkKeyModifiers |= static_cast<int>(key) >> 16;
                else
                        rkKeyModifiers &= ~(static_cast<int>(key) >> 16);
        }
}

HWND rkCurrnetWind{};

static LRESULT CALLBACK RkWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
        auto userDataPtr = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!userDataPtr)
              return DefWindowProc(hWnd, msg, wParam, lParam);

        auto eventQueue = RK_IMPL_PTR(reinterpret_cast<RkEventQueue*>(userDataPtr));
        if (!eventQueue)
                return DefWindowProc(hWnd, msg, wParam, lParam);

        switch(msg)
        {
        case WM_TIMER:
        {
                if (wParam == RK_SYSTEM_WINDOW_TIMER_ID)
                        eventQueue->processQueue();
                return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
	{
                SetFocus(hWnd);
                auto event = std::make_unique<RkMouseEvent>();
                auto x = static_cast<short int>(LOWORD(lParam));
                auto y = static_cast<short int>(HIWORD(lParam));
                event->setX(static_cast<double>(x) / eventQueue->scaleFactor());
                event->setY(static_cast<double>(y) / eventQueue->scaleFactor());
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
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
	{
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseButtonRelease);
                auto x = static_cast<short int>(LOWORD(lParam));
                auto y = static_cast<short int>(HIWORD(lParam));
		event->setX(static_cast<double>(x) / eventQueue->scaleFactor());
                event->setY(static_cast<double>(y) / eventQueue->scaleFactor());
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
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        case WM_MOUSEMOVE:
        {
                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_HOVER | TME_LEAVE;
                tme.hwndTrack = hWnd;
                tme.dwHoverTime = HOVER_DEFAULT;
                TrackMouseEvent(&tme);
                
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseMove);
                auto x = static_cast<short int>(LOWORD(lParam));
                auto y = static_cast<short int>(HIWORD(lParam));
		event->setX(static_cast<double>(x) / eventQueue->scaleFactor());
                event->setY(static_cast<double>(y) / eventQueue->scaleFactor());
                processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        case WM_LBUTTONDBLCLK:
        {
                SetFocus(hWnd);
                RK_LOG_DEBUG("WM_LBUTTONDBLCLK");
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseDoubleClick);
                event->setButton(RkMouseEvent::ButtonType::Left);
                auto x = static_cast<short int>(LOWORD(lParam));
                auto y = static_cast<short int>(HIWORD(lParam));
		event->setX(static_cast<double>(x) / eventQueue->scaleFactor());
                event->setY(static_cast<double>(y) / eventQueue->scaleFactor());
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }

        case WM_RBUTTONDBLCLK:
        {
                SetFocus(hWnd);
                RK_LOG_DEBUG("WM_RBUTTONDBLCLK");
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseDoubleClick);
                event->setButton(RkMouseEvent::ButtonType::Right);
                auto x = static_cast<short int>(LOWORD(lParam));
                auto y = static_cast<short int>(HIWORD(lParam));
		event->setX(static_cast<double>(x) / eventQueue->scaleFactor());
                event->setY(static_cast<double>(y) / eventQueue->scaleFactor());
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }

        case WM_MBUTTONDBLCLK:
        {
                RK_LOG_DEBUG("WM_MBUTTONDBLCLK");
                auto event = std::make_unique<RkMouseEvent>();
                event->setType(RkEvent::Type::MouseDoubleClick);
                event->setButton(RkMouseEvent::ButtonType::Middle);
                auto x = static_cast<short int>(LOWORD(lParam));
                auto y = static_cast<short int>(HIWORD(lParam));
		event->setX(static_cast<double>(x) / eventQueue->scaleFactor());
                event->setY(static_cast<double>(y) / eventQueue->scaleFactor());
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        case WM_MOUSEWHEEL:
        {
                SetFocus(hWnd);
                int delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
                auto event = std::make_unique<RkMouseEvent>();
                auto buttonType = (delta > 0) ? RkMouseEvent::ButtonType::WheelUp : RkMouseEvent::ButtonType::WheelDown;
		int screenX = GET_X_LPARAM(lParam);
		int screenY = GET_Y_LPARAM(lParam);
		POINT screenPoint;
                screenPoint.x = screenX;
                screenPoint.y = screenY;
		ScreenToClient(hWnd, &screenPoint);
		event->setX(static_cast<double>(screenPoint.x) / eventQueue->scaleFactor());
                event->setY(static_cast<double>(screenPoint.y) / eventQueue->scaleFactor());
		event->setButton(buttonType);
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        case WM_KEYDOWN:
        {
                RK_LOG_DEBUG("WM_KEYDOWN");
                auto event = std::make_unique<RkKeyEvent>();
                event->setType(RkEvent::Type::KeyPressed);
                event->setKey(convertToRkKey(static_cast<unsigned int>(wParam)));
                rkUpdateModifiers(event->key(), event->type());
                if (rkKeyModifiers != static_cast<int>(Rk::KeyModifiers::NoModifier))
                        event->setModifiers(rkKeyModifiers);
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }

        case WM_KEYUP:
        {
                RK_LOG_DEBUG("WM_KEYUP");
                auto event = std::make_unique<RkKeyEvent>();
                event->setType(RkEvent::Type::KeyReleased);
                event->setKey(convertToRkKey(static_cast<unsigned int>(wParam)));
                rkUpdateModifiers(event->key(), event->type());
                if (rkKeyModifiers != static_cast<int>(Rk::KeyModifiers::NoModifier))
                        event->setModifiers(rkKeyModifiers);
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        case WM_SIZE:
	{
                processSystemEvent(eventQueue, std::make_unique<RkResizeEvent>());
                return 0;
        }
        case WM_PAINT:
        {
                PAINTSTRUCT ps;
                BeginPaint(hWnd, &ps);
		processSystemEvent(eventQueue, std::make_unique<RkPaintEvent>());
                EndPaint(hWnd, &ps);
                return 0;
        }
        case WM_SETFOCUS:
        {
                auto event = std::make_unique<RkFocusEvent>();
                event->setType(RkEvent::Type::FocusedIn);
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        case WM_KILLFOCUS:
        {
                auto event = std::make_unique<RkFocusEvent>();
                event->setType(RkEvent::Type::FocusedOut);
		processSystemEvent(eventQueue, std::move(event));
                return 0;
        }
        
        case WM_ERASEBKGND:
	{
                auto color = eventQueue->getSystemWindow()->background();
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
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wc.lpfnWndProc   = RkWindowProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;
        wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
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
#endif // __cplusplus

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
#endif // __cplusplus
#endif // RK_FOR_SHARED
