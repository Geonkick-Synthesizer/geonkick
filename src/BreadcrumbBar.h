/**
 * File name: BreadcrumbBar.h
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

#ifndef BREADCRUMB_BAR_H
#define BREADCRUMB_BAR_H

#include "geonkick_widget.h"
#include "geonkick_button.h"

class PathButton;

class BreadcrumbBar: public GeonkickWidget
{
 public:
        explicit BreadcrumbBar(GeonkickWidget* parent);
        void setPath(const fs::path &path);
        RK_DECL_ACT(pathChanged,
                    pathChanged(const fs::path &path),
                    RK_ARG_TYPE(const fs::path),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(sizeUpdated,
                    sizeUpdated(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());

 protected:
        void updateButtonView();
        void pathPressed(size_t index);

 private:
        fs::path currentPath;
        std::vector<PathButton*> pathButtons;
};

#endif // BREADCRUMB_BAR_H
