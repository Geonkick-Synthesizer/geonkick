/**
 * File name: PathButton.h
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

#ifndef PATH_BUTTON_H
#define PATH_BUTTON_H

#include "geonkick_button.h"

class PathButton : public GeonkickButton {
 public:
        PathButton(GeonkickWidget *parent,
                   const fs::path& path,
                   const std::string &suffix = std::string());
        const fs::path& getPath() const;
        void setSuffix(const std::string_view& suffix);
        const std::string& getSuffix() const;

 private:
        fs::path buttonPath;
        std::string pathSuffix;
};

#endif // PATH_BUTTON_H
