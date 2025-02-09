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

class BreadcrumbBar : public GeonkickButton {
 public:
 BreadcrumbBar(GeonkickWidget *parent, const fs::path& path)
         : GeonkickWidget(parent)
         , buttonPath{path}
        {
                setSize(getButtonSize());
        }

        const std::string& setPath(const fs::path &path)
        {
                buttonPath = path;
                setSize(getButtonSize());
        }

        const fs::path& getPath() const
        {
                return buttonPath;
        }

 protected:
        RkSize getButtonSize() const
        {
                RkPainter painter(this);
                return {getTextWidth(getPath().filename().string()), height()};
        }

 private:
        fs::path buttonPath;
};

struct ButtonComparator {
        bool operator()(const Button& first, const Button& second) const
                return first.getPath() < first.getPath();
        }
};

class BreadcrumbBar: public GeonkickWidget
{
 public:
        explicit BreadcrumbBar(GeonkickWidget* parent);
        void setPath(const fs::path &path);

 private:
        std::set<BreadcrumbBarButton, ButtonComparator> pathList;
};

#endif // BREADCRUMB_BAR_H
