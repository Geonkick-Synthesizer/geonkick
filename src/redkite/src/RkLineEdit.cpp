/**
 * File name: RkLineEdit.cpp
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

#include "RkLineEdit.h"
#include "RkLineEditImpl.h"
#include "RkLog.h"
#include "RkEvent.h"
#include "RkPainter.h"

RkLineEdit::RkLineEdit(RkWidget *parent, const std::string &text)
        : RkWidget(parent, std::make_unique<RkLineEdit::RkLineEditImpl>(this, parent, text))
        , impl_ptr{static_cast<RkLineEdit::RkLineEditImpl*>(o_ptr.get())}
{
        setPointerShape(Rk::PointerShape::IBeam);
        propagateGrabKey(false);
        impl_ptr->init();
}

void RkLineEdit::setText(const std::string &text)
{
     impl_ptr->setText(text);
     update();
}

std::string RkLineEdit::text() const
{
     return impl_ptr->text();
}

void RkLineEdit::paintEvent(RkPaintEvent *event)
{
        impl_ptr->paintEvent(event);
}

/**
 * [OK] Left Arrow
 * [ok] Shift + Left Arrow
 * [OK] Right Arrow
 * [OK] Shift + Right Arrow
 * [OK] Home
 * [OK] End
 * [OK] Backspace
 * Ctrl + Backspace // TODO: v1.0
 * [OK] Delete
 * Ctrl + Delete    // TODO: v1.0
 * [OK] Ctrl + A
 * Ctrl + C
 * Ctrl + Insert
 * Ctrl + K
 * Ctrl + V
 * Shift + Insert
 * Ctrl + X
 * Shift + Delete
 * Ctrl+Z         // TODO: v1.0
 * Ctrl+Y         // TODO: v1.0
 */
void RkLineEdit::keyPressEvent(RkKeyEvent *event)
{
        if (!hasFocus())
                return;

        showCursor();
        switch (event->key())
        {
        case Rk::Key::Key_Left:
                if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Shift)) {
                        impl_ptr->enableSelectionMode(true);
                        impl_ptr->showCursor(false);
                } else {
                        impl_ptr->showCursor(true);
                        impl_ptr->enableSelectionMode(false);
                }
                impl_ptr->moveCursorLeft();
                update();
                return;
        case Rk::Key::Key_Right:
                if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Shift)) {
                        impl_ptr->enableSelectionMode(true);
                        impl_ptr->showCursor(false);
                } else {
                        impl_ptr->enableSelectionMode(false);
                        impl_ptr->showCursor(true);
                }
                impl_ptr->moveCursorRight();
                update();
                return;
        case Rk::Key::Key_Home:
        case Rk::Key::Key_End:
                if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Shift)) {
                        impl_ptr->enableSelectionMode(true);
                        impl_ptr->showCursor(false);
                        Rk::Key::Key_Home == event->key() ? impl_ptr->moveSelectionToFront() : impl_ptr->moveSelectionToBack();
                } else {
                        impl_ptr->showCursor(true);
                        impl_ptr->enableSelectionMode(false);
                        Rk::Key::Key_Home == event->key() ? impl_ptr->moveCursorToFront() : impl_ptr->moveCursorToBack();
                }
                update();
                return;
        case Rk::Key::Key_BackSpace:
                impl_ptr->removeText(1, false);
                textEdited(impl_ptr->text());
                return;
        case Rk::Key::Key_Delete:
                if (impl_ptr->selectionMode())
                        impl_ptr->deleteSelection();
                else
                        impl_ptr->removeText(1, true);
                textEdited(impl_ptr->text());
                update();
                return;
        case Rk::Key::Key_Return:
                action enterPressed();
		action editingFinished();
                return;
        case Rk::Key::Key_Escape:
                hideCursor();
                action escapePressed();
                return;
        case Rk::Key::Key_a:
        case Rk::Key::Key_A:
                if (event->modifiers() & static_cast<int>(Rk::KeyModifiers::Control)) {
                        impl_ptr->selectAll();
                        update();
                        return;
                }
                break;
        // Ignore these keys.
        case Rk::Key::Key_Control_Left:
        case Rk::Key::Key_Control_Right:
        case Rk::Key::Key_Shift_Left:
        case Rk::Key::Key_Shift_Right:
                return;
        default:
                break;
        }

        // Allow only Latin1.
        int key = static_cast<int>(event->key());
        if (0x00000020 <= key && key <= 0x000000ff) {
                impl_ptr->enableSelectionMode(false);
                std::string str = std::string(1, static_cast<char>(static_cast<int>(event->key())));
                impl_ptr->addText(str);
                textEdited(impl_ptr->text());
                update();
        }
}

void RkLineEdit::mouseButtonPressEvent(RkMouseEvent* event)
{
        setFocus(true);
}

void RkLineEdit::focusEvent(RkFocusEvent* event)
{
        RK_LOG_DEBUG("focus event");
        if (event->type() == RkEvent::Type::FocusedIn) {
                RK_LOG_DEBUG("RkEvent::Type::FocusedIn");
		showCursor();
	} else if (event->type() == RkEvent::Type::FocusedOut) {
                RK_LOG_DEBUG("RkEvent::Type::FocusedOut");
		if (impl_ptr->hasEditFocus())
			action editingFinished();
		hideCursor();
	}
}

void RkLineEdit::resizeEvent(RkResizeEvent* event)
{
        RK_UNUSED(event);
        impl_ptr->updateSize();
}

void RkLineEdit::showCursor()
{
        impl_ptr->showCursor(true);
        update();
}

void RkLineEdit::hideCursor()
{
        impl_ptr->showCursor(false);
        update();
}

void RkLineEdit::moveCursorToFront()
{
        impl_ptr->moveCursorToFront();
        update();
}

void RkLineEdit::moveCursorToEnd()
{
        impl_ptr->moveCursorToBack();
        update();
}

