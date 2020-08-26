/**
 * File name: ViewState.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#include "ViewState.h"

ViewState::ViewState(RkObject *parent)
        : RkObject(parent)
{
}

ViewState::View ViewState::mainView() const
{
        return viewState;
}

void ViewState::setMainView(ViewState::View view)
{
        if (mainView != view) {
                mainView = view;
                action mainViewChanged(mainView);
        }
}

std::string ViewState::sampleBrowserPath() const
{
        return sampleBrowser.currentDirectory;
}

void ViewState::setSamplesBrowserPath(const std::string &path)
{
        if (path != samplesBrowser.currentDirectory) {
                samplesBrowser.currentDirectory = path;
                action sampleBrowserPathChanged(samplesBrowser.currentDirectory);
        }
}

ViewState::Oscillator ViewState::samplesBrowserOscillator() const
{
        return sampleBrowser.oscillator;
}

void ViewState::setSamplesBrowserOscillator(ViewState::Oscillator osc)
{
        if (osc != samplesBrowser.oscillator) {
                samplesBrowser.oscillator = osc;
                action samplesBrowserOscChanged(samplesBrowser.oscillator);
        }
}


