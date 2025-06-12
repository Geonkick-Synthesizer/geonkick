/**
 * File name: PathButton.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#include "PathButton.h"
#include "DesktopPaths.h"

#include <RkPainter.h>

PathButton::PathButton(GeonkickWidget *parent,
                       const fs::path& path,
                       const std::string &suffix)
        : GeonkickButton(parent)
        , buttonPath{path}
        , pathSuffix {suffix}
{
        std::string pathString;
        DesktopPaths desktopPaths;
        if (desktopPaths.isRootPath(path)) {
	        pathString = path.root_name().empty() ?
                             path.string() : path.root_name().string();
        } else if (path == desktopPaths.getFactoryPresetsPath()) {
                pathString = "Factory Presets";
        } else if (path == desktopPaths.getUserPresetsPath()) {
                pathString = "User Presets";
        } else {
                pathString = path.filename().string();
        }
        pathString += getSuffix();

        RkPainter painter(this);
        setSize(painter.getTextWidth(pathString) + 6,
                painter.font().size() + 7);

        RkImage buttonImage(size());
        RkPainter imagePainter(&buttonImage);
        auto pen = imagePainter.pen();

        // Unpressed image
        imagePainter.fillRect(rect(), {88, 88, 88});
        pen.setColor({10, 10, 10});
        imagePainter.setPen(pen);
        imagePainter.drawText(rect(), pathString);
        setImage(buttonImage, RkButton::State::Unpressed);

        // Hover image
        imagePainter.fillRect(rect(), {108, 108, 108});
        pen.setColor({200, 200, 200});
        imagePainter.setPen(pen);
        imagePainter.drawText(rect(), pathString);
        setImage(buttonImage, RkButton::State::UnpressedHover);

        // Pressed image
        imagePainter.fillRect(rect(), {150, 150, 150});
        pen.setColor({255, 255, 255});
        imagePainter.setPen(pen);
        imagePainter.drawText(rect(), pathString);
        setImage(buttonImage, RkButton::State::Pressed);
}

const fs::path& PathButton::getPath() const
{
        return buttonPath;
}

void PathButton::setSuffix(const std::string_view& suffix)
{
        pathSuffix = suffix;
}

const std::string& PathButton::getSuffix() const
{
        return pathSuffix;
}
