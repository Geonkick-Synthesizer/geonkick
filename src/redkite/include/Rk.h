/**
 * File name: Rk.h
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

#ifndef RK_GLOBAL_H
#define RK_GLOBAL_H

#define RK_VERSION 0x010300
#define RK_MAJOR   0x01
#define RK_MINOR   0x03
#define RK_PATCH   0x01

#include <utility>
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <thread>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <array>
#include <iostream>
#include <sstream>

#define RK_UNUSED(expr) (void)expr

#ifdef RK_OS_WIN
#define RK_NO_EXPORT
#ifdef RK_EXPORT_INTERFACE
// Export Redkite interface
#define RK_EXPORT  __declspec(dllexport)
#else
#define RK_EXPORT
#endif
#elif RK_OS_MAC
#error not implemented for Mac
#else // RK_OS_GNU
#define RK_NO_EXPORT __attribute__((visibility("hidden")))
#ifdef RK_EXPORT_INTERFACE
// Export Redkite interface
#define RK_EXPORT __attribute__((visibility("default")))
#else
#define RK_EXPORT
#endif
#endif

#define RK_DECLARE_IMPL(Class) \
  class Class##Impl; \
  std::unique_ptr<Class##Impl> o_ptr

#define RK_DELCATE_IMPL_PTR(Class) \
  class Class##Impl; \
  Class##Impl *impl_ptr

#define RK_DECALRE_INTERFACE_PTR(Class) Class *inf_ptr

#define RK_CLASS_INFO(name, value) virtual std::string rk__property_ ##name () const { return std::string( #value ); }
#define RK_SET_CLASS_INFO(name, value) virtual std::string rk__property_ ##name () const override { return std::string( #value ); }

#define RK_DISABLE_COPY(Class) \
          Class(const Class &other) = delete; \
          Class& operator=(const Class &other) = delete

#define RK_DISABLE_MOVE(Class) \
          Class(Class &&other) = delete; \
          Class& operator=(Class &&other) = delete

#if defined(RK_OS_WIN) && !defined(RK_FOR_SHARED)
int rkMain(int, char **);
#define main rkMain
#endif // RK_OS_WIN && !RK_FOR_SHARED

#if defined(RK_SINGLE_PRECISION)
using rk_real = float;
#else
using rk_real = double;
#endif // RK_SINGLE_PRECISION

namespace Rk {
        enum class Alignment : int {
                AlignLeft   = 1,
                AlignRight  = 2,
		AlignCenter = 3,
		AlignTop    = 4,
		AlignBottom = 5
        };

        enum class Orientation : int {
                Horizontal = 0,
                Vertical = 1
        };

        enum class WindowFlags: int {
                Widget = 0x00000000,
                Dialog = 0x00000001,
                Popup  = 0x00000002
        };

        enum class Modality : int {
                NonModal = 0,

                // Disable input for parent and all its children
                // except the current modal one.
                ModalParent = 1,

                // Disable input for all widgets hierarhy
                // of the top widget, including top widget,
                // except the current modal one.
                ModalTopWidget = 2
        };

        enum class WidgetAttribute : int {
                NoAttributes = 0x00000000,
                KeyInputEnabled = 0x00000001,
                MouseInputEnabled = 0x00000002,
                CloseInputEnabled = 0x00000004
        };

        enum class Key : unsigned int {
                Key_None        = 0x00000000,

                /**
                 * Group: key modifiers, as bit flags.
                 */
                Key_Shift_Left    = 0x00010000,
                Key_Shift_Right   = 0x00020000,
                Key_Control_Left  = 0x00040000,
                Key_Control_Right = 0x00080000,
                Key_Caps_Lock     = 0x00100000,
                Key_Shift_Lock    = 0x00200000,
                Key_Meta_Left     = 0x00400000,
                Key_Meta_Right    = 0x00800000,
                Key_Alt_Left      = 0x01000000,
                Key_Alt_Right     = 0x02000000,
                Key_Super_Left    = 0x04000000,
                Key_Super_Right   = 0x08000000,
                Key_Hyper_Left    = 0x10000000,
                Key_Hyper_Right   = 0x20000000,

                /**
                 * Group: Cursor control keys.
                 * Range: 0x00500000 - 0x005f0000
                 */
                Key_Home      = 0x00500000,
                Key_Left      = 0x00510000,
                Key_Up        = 0x00520000,
                Key_Right     = 0x00530000,
                Key_Down      = 0x00540000,
                Key_Page_Up   = 0x00550000,
                Key_Page_Down = 0x00560000,
                Key_End       = 0x00570000,
                Key_Begin     = 0x00580000,

                /**
                 * Group: Other edit keys.
                 * Range: 0x00590000 - 0x00620000
                 */
                Key_BackSpace   = 0x00590000,
                Key_Tab         = 0x005a0000,
                Key_Linefeed    = 0x005b0000,
                Key_Clear       = 0x005c0000,
                Key_Return      = 0x005d0000,
                Key_Pause       = 0x005e0000,
                Key_Scroll_Lock = 0x005f0000,
                Key_Sys_Req     = 0x00600000,
                Key_Escape      = 0x00610000,
                Key_Delete      = 0x00620000,

                /**
                 * Group: LATIN1
                 * Rnage: 0x00000020 - 0x000000ff
                 */
                Key_Space          = 0x00000020,
                Key_Exclam         = 0x00000021,
                Key_Quotedbl       = 0x00000022,
                Key_Numbersign     = 0x00000023,
                Key_Dollar         = 0x00000024,
                Key_Percent        = 0x00000025,
                Key_Ampersand      = 0x00000026,
                Key_Apostrophe     = 0x00000027,
                Key_Quoteright     = 0x00000027,
                Key_Parenleft      = 0x00000028,
                Key_Parenright     = 0x00000029,
                Key_Asterisk       = 0x0000002a,
                Key_Plus           = 0x0000002b,
                Key_Comma          = 0x0000002c,
                Key_Minus          = 0x0000002d,
                Key_Period         = 0x0000002e,
                Key_Slash          = 0x0000002f,
                Key_0              = 0x00000030,
                Key_1              = 0x00000031,
                Key_2              = 0x00000032,
                Key_3              = 0x00000033,
                Key_4              = 0x00000034,
                Key_5              = 0x00000035,
                Key_6              = 0x00000036,
                Key_7              = 0x00000037,
                Key_8              = 0x00000038,
                Key_9              = 0x00000039,
                Key_Colon          = 0x0000003a,
                Key_Semicolon      = 0x0000003b,
                Key_Less           = 0x0000003c,
                Key_Equal          = 0x0000003d,
                Key_Greater        = 0x0000003e,
                Key_Question       = 0x0000003f,
                Key_At             = 0x00000040,
                Key_A              = 0x00000041,
                Key_B              = 0x00000042,
                Key_C              = 0x00000043,
                Key_D              = 0x00000044,
                Key_E              = 0x00000045,
                Key_F              = 0x00000046,
                Key_G              = 0x00000047,
                Key_H              = 0x00000048,
                Key_I              = 0x00000049,
                Key_J              = 0x0000004a,
                Key_K              = 0x0000004b,
                Key_L              = 0x0000004c,
                Key_M              = 0x0000004d,
                Key_N              = 0x0000004e,
                Key_O              = 0x0000004f,
                Key_P              = 0x00000050,
                Key_Q              = 0x00000051,
                Key_R              = 0x00000052,
                Key_S              = 0x00000053,
                Key_T              = 0x00000054,
                Key_U              = 0x00000055,
                Key_V              = 0x00000056,
                Key_W              = 0x00000057,
                Key_X              = 0x00000058,
                Key_Y              = 0x00000059,
                Key_Z              = 0x0000005a,
                Key_Bracketleft    = 0x0000005b,
                Key_Backslash      = 0x0000005c,
                Key_Bracketright   = 0x0000005d,
                Key_Asciicircum    = 0x0000005e,
                Key_Underscore     = 0x0000005f,
                Key_Grave          = 0x00000060,
                Key_Quoteleft      = 0x00000060,
                Key_a              = 0x00000061,
                Key_b              = 0x00000062,
                Key_c              = 0x00000063,
                Key_d              = 0x00000064,
                Key_e              = 0x00000065,
                Key_f              = 0x00000066,
                Key_g              = 0x00000067,
                Key_h              = 0x00000068,
                Key_i              = 0x00000069,
                Key_j              = 0x0000006a,
                Key_k              = 0x0000006b,
                Key_l              = 0x0000006c,
                Key_m              = 0x0000006d,
                Key_n              = 0x0000006e,
                Key_o              = 0x0000006f,
                Key_p              = 0x00000070,
                Key_q              = 0x00000071,
                Key_r              = 0x00000072,
                Key_s              = 0x00000073,
                Key_t              = 0x00000074,
                Key_u              = 0x00000075,
                Key_v              = 0x00000076,
                Key_w              = 0x00000077,
                Key_x              = 0x00000078,
                Key_y              = 0x00000079,
                Key_z              = 0x0000007a,
                Key_Braceleft      = 0x0000007b,
                Key_Bar            = 0x0000007c,
                Key_Braceright     = 0x0000007d,
                Key_Asciitilde     = 0x0000007e,
                Key_Nobreakspace   = 0x000000a0,
                Key_Exclamdown     = 0x000000a1,
                Key_Cent           = 0x000000a2,
                Key_Sterling       = 0x000000a3,
                Key_Currency       = 0x000000a4,
                Key_Yen            = 0x000000a5,
                Key_Brokenbar      = 0x000000a6,
                Key_Section        = 0x000000a7,
                Key_Diaeresis      = 0x000000a8,
                Key_Copyright      = 0x000000a9,
                Key_Ordfeminine    = 0x000000aa,
                Key_Guillemotleft  = 0x000000ab,
                Key_Notsign        = 0x000000ac,
                Key_Hyphen         = 0x000000ad,
                Key_Registered     = 0x000000ae,
                Key_Macron         = 0x000000af,
                Key_Degree         = 0x000000b0,
                Key_Plusminus      = 0x000000b1,
                Key_Twosuperior    = 0x000000b2,
                Key_Threesuperior  = 0x000000b3,
                Key_Acute          = 0x000000b4,
                Key_Mu             = 0x000000b5,
                Key_Paragraph      = 0x000000b6,
                Key_Periodcentered = 0x000000b7,
                Key_Cedilla        = 0x000000b8,
                Key_Onesuperior    = 0x000000b9,
                Key_Masculine      = 0x000000ba,
                Key_Guillemotright = 0x000000bb,
                Key_Onequarter     = 0x000000bc,
                Key_Onehalf        = 0x000000bd,
                Key_Threequarters  = 0x000000be,
                Key_Questiondown   = 0x000000bf,
                Key_Agrave         = 0x000000c0,
                Key_Aacute         = 0x000000c1,
                Key_Acircumflex    = 0x000000c2,
                Key_Atilde         = 0x000000c3,
                Key_Adiaeresis     = 0x000000c4,
                Key_Aring          = 0x000000c5,
                Key_Ae             = 0x000000c6,
                Key_Ccedilla       = 0x000000c7,
                Key_Egrave         = 0x000000c8,
                Key_Eacute         = 0x000000c9,
                Key_Ecircumflex    = 0x000000ca,
                Key_Ediaeresis     = 0x000000cb,
                Key_Igrave         = 0x000000cc,
                Key_Iacute         = 0x000000cd,
                Key_Icircumflex    = 0x000000ce,
                Key_Idiaeresis     = 0x000000cf,
                Key_Eth            = 0x000000d0,
                Key_Ntilde         = 0x000000d1,
                Key_Ograve         = 0x000000d2,
                Key_Oacute         = 0x000000d3,
                Key_Ocircumflex    = 0x000000d4,
                Key_Otilde         = 0x000000d5,
                Key_Odiaeresis     = 0x000000d6,
                Key_Multiply       = 0x000000d7,
                Key_Oslash         = 0x000000d8,
                Key_Ooblique       = 0x000000d8,
                Key_Ugrave         = 0x000000d9,
                Key_Uacute         = 0x000000da,
                Key_Ucircumflex    = 0x000000db,
                Key_Udiaeresis     = 0x000000dc,
                Key_Yacute         = 0x000000dd,
                Key_Thorn          = 0x000000de,
                Key_Ssharp         = 0x000000df,
                Key_agrave         = 0x000000e0,
                Key_aacute         = 0x000000e1,
                Key_acircumflex    = 0x000000e2,
                Key_atilde         = 0x000000e3,
                Key_adiaeresis     = 0x000000e4,
                Key_aring          = 0x000000e5,
                Key_ae             = 0x000000e6,
                Key_ccedilla       = 0x000000e7,
                Key_egrave         = 0x000000e8,
                Key_eacute         = 0x000000e9,
                Key_ecircumflex    = 0x000000ea,
                Key_ediaeresis     = 0x000000eb,
                Key_igrave         = 0x000000ec,
                Key_iacute         = 0x000000ed,
                Key_icircumflex    = 0x000000ee,
                Key_idiaeresis     = 0x000000ef,
                Key_eth            = 0x000000f0,
                Key_ntilde         = 0x000000f1,
                Key_ograve         = 0x000000f2,
                Key_oacute         = 0x000000f3,
                Key_ocircumflex    = 0x000000f4,
                Key_otilde         = 0x000000f5,
                Key_odiaeresis     = 0x000000f6,
                Key_division       = 0x000000f7,
                Key_oslash         = 0x000000f8,
                Key_ooblique       = 0x000000f8,
                Key_ugrave         = 0x000000f9,
                Key_uacute         = 0x000000fa,
                Key_ucircumflex    = 0x000000fb,
                Key_udiaeresis     = 0x000000fc,
                Key_yacute         = 0x000000fd,
                Key_thorn          = 0x000000fe,
                Key_Ydiaeresis     = 0x000000ff
        };

        enum class KeyModifiers: int {
                NoModifier    = 0x0000,
                Shift_Left    = 0x0001,
                Shift_Right   = 0x0002,
                Shift         = Shift_Left | Shift_Right,
                Control_Left  = 0x0004,
                Control_Right = 0x0008,
                Control       = Control_Left | Control_Right,
                Caps_Lock     = 0x0010,
                Shift_Lock    = 0x0020,
                Meta_Left     = 0x0040,
                Meta_Right    = 0x0080,
                Meta          = Meta_Left | Meta_Right,
                Alt_Left      = 0x0100,
                Alt_Right     = 0x0200,
                Alt           = Alt_Left | Alt_Right,
                Super_Left    = 0x0400,
                Super_Right   = 0x0800,
                Super         = Super_Left | Super_Right,
                Hyper_Left    = 0x1000,
                Hyper_Right   = 0x2000,
                Hyper         = Hyper_Left | Hyper_Right
        };

        enum class PointerShape: int {
                NoShape = 0,
                Arrow = 1,
                UpArrow = 2,
                Cross = 3,
                Wait = 4,
                WhatsThis = 5,
                IBeam = 6,
                SizeVertical = 7,
                SizeHorizontal = 8,
                SizeTLDiagonal = 9,
                SizeTRDiagonal = 10,
                SizeAll = 11,
                SplitVertical = 12,
                SplitHorizontal = 13,
                PointingHand = 14,
                OpenHand = 15,
                ClosedHand = 16,
                Forbidden = 17,
                Busy = 18
        };

        enum class ObjectType : int {
                Object = 0,
                Widget = 1
        };
}

#define RK_ACT_ARGS(arg, ...) arg, ##__VA_ARGS__
#define RK_ARG_TYPE(type, ...) type, ##__VA_ARGS__
#define RK_ARG_VAL(val, ...) val, ##__VA_ARGS__

#define RK_DECL_ACT(ObseverName, prot, type, val) \
        class rk__observer_##ObseverName : public RkObserver { \
        public: \
                rk__observer_ ##ObseverName (RkObject *obj, const std::function<void(type)> &cb) \
                        : RkObserver(obj), \
                          observerCallback{cb} {} \
                rk__observer_##ObseverName () = default; \
                std::function<void(type)> observerCallback; \
        }; \
        \
        void prot \
        { \
                for (const auto& ob: rk__observers()) { \
                        RK_LOG_DEBUG("observerCallback[get]: " << RkObject::name()); \
                        RK_LOG_DEBUG("observerCallback[get]: " << ob.get()); \
                        RK_LOG_DEBUG("observerCallback[get]: " << ob.get()->object()->name()); \
                        auto observer = dynamic_cast<rk__observer_ ##ObseverName *>(ob.get()); \
                        RK_LOG_DEBUG("object[call_0]...");                \
                        if (observer) { \
                                RK_LOG_DEBUG("object[call]..."); \
                                RK_LOG_DEBUG("object[call] : " << ob->object()->name()); \
                                observer->observerCallback(val); \
                                RK_LOG_DEBUG("object[end] : " << ob->object()->name()); \
                        } \
                } \
                RK_LOG_DEBUG("observerCallback[exit]"); \
        } \
        void rk__add_action_cb_##ObseverName (RkObject *obj, const std::function<void(type)> &cb) \
        { \
                if (obj) { \
                RK_LOG_DEBUG("rk__add_action_cb: " << RkObject::name() << ": " << obj->name()); \
                } \
                rk__add_observer(std::make_unique<rk__observer_##ObseverName >(obj, cb)); \
        }

#define RK_ACT_BIND(obj1, act, act_args, obj2, callback) \
        obj1->rk__add_action_cb_##act (obj2, [=](act_args){ obj2->callback; }); \
        obj2->rk__add_bound_object(obj1)

// Bind lamda functions to object actions.
#define RK_ACT_BINDL(obj1, act, act_args, lamda)   \
        obj1->rk__add_action_cb_##act (nullptr, lamda)

#define action

#define RK_DECLARE_IMAGE_RC(name) extern const unsigned char rk__ ## name ## _png[]
#define RK_IMAGE_RC(name) rk__ ## name ## _png

using RkString = std::string;

#endif // RK_GLOBAL_H
