/**
 * File name: RkLineEditImpl.cpp
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

#include "RkLineEditImpl.h"
#include "RkPainter.h"
#include "RkLog.h"
#include "RkTimer.h"
#include "RkEvent.h"

RkLineEdit::RkLineEditImpl::RkLineEditImpl(RkLineEdit *interface,
                                           RkWidget *parent,
                                           const std::string &text)
    : RkWidgetImpl(static_cast<RkWidget*>(interface), parent)
    , inf_ptr{interface}
    , editedText{text}
    , cursorIndex{0}
    , selectionIndex{0}
    , isSelectionMode{false}
    , cursorTimer{nullptr}
    , isShowCursor{false}
    , lastCahnges{std::chrono::system_clock::now()}
    , contentsRect{0, 0, 0, 0}
    , beginX{0}
    , endX{0}
{
}

RkLineEdit::RkLineEditImpl::~RkLineEditImpl()
{
        cursorTimer->stop();
}

void RkLineEdit::RkLineEditImpl::init()
{
        cursorTimer = new RkTimer(inf_ptr, 800);
        cursorTimer->setName("RkLineEdit");
        hasFocus() ? showCursor(true) : showCursor(false);
        RK_ACT_BIND(cursorTimer, timeout, RK_ACT_ARGS(), this, onCursorTimeout());
}

void RkLineEdit::RkLineEditImpl::setText(const std::string &text)
{
	selectionIndex = 0;
	enableSelectionMode(false);
        editedText = text;
        if (editedText.size() < 1)
                cursorIndex = 0;
        else if (cursorIndex > static_cast<decltype(cursorIndex)>(editedText.size()) - 1)
                cursorIndex = editedText.size() - 1;
        lastCahnges = std::chrono::system_clock::now();
}

void RkLineEdit::RkLineEditImpl::moveCursorLeft(int n)
{
        if (editedText.empty()) {
                cursorIndex = 0;
        } else if (selectionMode()) {
                selectionIndex--;
                if (selectionIndex < 0)
                        selectionIndex = 0;
                if (selectionIndex == cursorIndex) {
                        enableSelectionMode(false);
                        showCursor(true);
                }
        } else {
                cursorIndex -= n;
                if (cursorIndex < 0)
                        cursorIndex = 0;
        }
        lastCahnges = std::chrono::system_clock::now();
}

void RkLineEdit::RkLineEditImpl::moveCursorRight(int n)
{
        if (editedText.empty()) {
                cursorIndex = 0;
        } else if (selectionMode()) {
                selectionIndex++;
                if (selectionIndex > static_cast<decltype(selectionIndex)>(editedText.size()))
                        selectionIndex = editedText.size();
                if (selectionIndex == cursorIndex) {
                        enableSelectionMode(false);
                        showCursor(true);
                }
        } else {
                cursorIndex += n;
                if (cursorIndex > static_cast<decltype(cursorIndex)>(editedText.size() - 1))
                        cursorIndex = editedText.size();
        }
        lastCahnges = std::chrono::system_clock::now();
}

void RkLineEdit::RkLineEditImpl::moveCursorToFront()
{
        cursorIndex = 0;
}

void RkLineEdit::RkLineEditImpl::moveCursorToBack()
{
        cursorIndex = editedText.size();
}

void RkLineEdit::RkLineEditImpl::moveSelectionToFront()
{
        if (selectionMode())
                selectionIndex = 0;
}

void RkLineEdit::RkLineEditImpl::moveSelectionToBack()
{
        if (selectionMode())
                selectionIndex = editedText.size();
}

void RkLineEdit::RkLineEditImpl::addText(const std::string& text)
{
        if (isSelectionMode) {
        } else {
                if (cursorIndex == static_cast<decltype(cursorIndex)>(editedText.size()))
                        editedText += text;
                else
                        editedText.insert(cursorIndex, text);
                cursorIndex += text.size();
        }
        lastCahnges = std::chrono::system_clock::now();
}

void RkLineEdit::RkLineEditImpl::removeText(int n, bool after)
{
        if (editedText.size() < 1)
                return;

        if (after) {
                if (static_cast<decltype(editedText.size())>(cursorIndex + n) > editedText.size())
                        editedText.erase(cursorIndex, editedText.size() - 1);
                else
                        editedText.erase(cursorIndex, n);
        } else if (cursorIndex > 0) {
                if (cursorIndex - n < 0) {
                        editedText.erase(editedText.begin(), editedText.begin() + cursorIndex);
                        cursorIndex = 0;
                } else {
                        editedText.erase(cursorIndex - n, n);
                        cursorIndex -= n;
                }
        }

        if (isSelectionMode)
                selectionIndex = cursorIndex;
        lastCahnges = std::chrono::system_clock::now();
}

std::string RkLineEdit::RkLineEditImpl::text() const
{
        return editedText;
}

std::string RkLineEdit::RkLineEditImpl::textTo(int index) const
{
        if (editedText.empty())
                return std::string();
        else
                return editedText.substr(0, index);
}

void RkLineEdit::RkLineEditImpl::enableSelectionMode(bool b)
{
        if (isSelectionMode != b) {
                isSelectionMode = b;
                selectionIndex = cursorIndex;
        }
}

bool RkLineEdit::RkLineEditImpl::selectionMode() const
{
        return isSelectionMode;
}

bool RkLineEdit::RkLineEditImpl::isCursorHidden() const
{
        return !isShowCursor;
}

void RkLineEdit::RkLineEditImpl::onCursorTimeout()
{
        if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastCahnges).count() > 1000) {
                isShowCursor = !isShowCursor;
                inf_ptr->update();
        }
}

void RkLineEdit::RkLineEditImpl::showCursor(bool b)
{
        isShowCursor = b;
        isShowCursor ? cursorTimer->start() : cursorTimer->stop();
}

void RkLineEdit::RkLineEditImpl::selectAll()
{
        if (!editedText.empty()) {
                enableSelectionMode(true);
                showCursor(false);
                selectionIndex = editedText.size();
                cursorIndex = 0;
        }
}

int RkLineEdit::RkLineEditImpl::getCursorIndex() const
{
        return cursorIndex;
}

int RkLineEdit::RkLineEditImpl::selectionStart() const
{
        return selectionIndex > cursorIndex ? cursorIndex : selectionIndex;
}

int RkLineEdit::RkLineEditImpl::selectionEnd() const
{
        return selectionIndex < cursorIndex ? cursorIndex : selectionIndex;
}

std::string RkLineEdit::RkLineEditImpl::getText(int pos, size_t n) const
{
        if (editedText.empty())
                return std::string();

        if (pos > static_cast<decltype(pos)>(editedText.size()))
                return std::string();
        else if (pos < 0)
                pos = 0;
        else if (n > editedText.npos)
                n = editedText.npos;

        return editedText.substr(pos, n);
}

void RkLineEdit::RkLineEditImpl::deleteSelection()
{
        if (selectionMode() && selectionStart() != selectionEnd()) {
                editedText.erase(selectionStart(), selectionEnd() - selectionStart());
                selectionIndex = cursorIndex = selectionStart();
                enableSelectionMode(false);
                showCursor(true);
        }
}

void RkLineEdit::RkLineEditImpl::paintEvent(RkPaintEvent *event)
{
        RK_UNUSED(event);
        if (contentsRect.height() * contentsRect.width() == 0)
                updateSize();

        RkImage img(size());
        {
                RkPainter painter(&img);
                painter.fillRect(rect(), background());
                painter.setFont(font());

                int cursorX = 0;
                if (selectionMode())
                        cursorX = painter.getTextWidth(textTo(selectionIndex));
                else
                        cursorX = painter.getTextWidth(textTo(cursorIndex));

                if (cursorX > endX) {
                        endX += cursorX - endX;
                        beginX = endX - contentsRect.width();
                        cursorX = contentsRect.width();
                } else if (cursorX < beginX) {
                        beginX -= beginX - cursorX;
                        endX = beginX + contentsRect.width();
                        cursorX = 1;
                } else {
                        cursorX = cursorX - beginX;
                        endX = beginX + contentsRect.width();
                }

                // Draw selection background.
                if (selectionMode()) {
                        auto text = getText(0, selectionStart());
                        int xpos = painter.getTextWidth(text);
                        int nSelectedChars = selectionEnd() - selectionStart();
                        text = getText(selectionStart(), nSelectedChars);
                        int w = painter.getTextWidth(text);
                        painter.fillRect(RkRect(contentsRect.left() + xpos - beginX,
                                                contentsRect.top(), w,
                                                contentsRect.height()) , {52, 116, 209});
                }

                // Draw edited text.
                auto pen = painter.pen();
                pen.setColor(textColor());
                painter.setPen(pen);
                painter.drawText(contentsRect.left() - beginX,
                                 contentsRect.top() + (contentsRect.height()
                                 - font().size()) / 2 + font().size(), text());

                // Draw cursor.
                if (!isCursorHidden()) {
                        pen = painter.pen();
                        pen.setColor(color());
                        painter.setPen(pen);
                        painter.drawLine(contentsRect.left() + cursorX + 1, contentsRect.top(),
                                         contentsRect.left() + cursorX + 1,
                                         contentsRect.top() + contentsRect.height());
                }
        }
        RkPainter paint(inf_ptr);
        paint.drawImage(img, 0, 0);
}

void RkLineEdit::RkLineEditImpl::updateSize()
{
        contentsRect = RkRect(2, 2, size().width() - 6, size().height() - 4);
        beginX = 0;
        endX = contentsRect.width();
}

bool RkLineEdit::RkLineEditImpl::hasEditFocus() const
{
	return cursorTimer->started();
}
