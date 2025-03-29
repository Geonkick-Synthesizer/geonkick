/**
 * File name: BrowserPathHistory.h
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

#ifndef PATH_HISTORY_H
#define PATH_HISTORY_H

#include "globals.h"

#include <RkObject.h>

class PathHistory: public RkObject {
 public:
        PathHistory(RkObject *parent);
        void goTo(const fs::path& newPath);
        void goBack();
        void goForward();
        [[nodiscard]] bool hasBackwardHistory() const;
        [[nodiscard]] bool hasForwardHistory() const;
        [[nodiscard]] std::optional<fs::path> getCurrentPath() const;
        RK_DECL_ACT(pathChanged,
                    pathChanged(const fs::path& path),
                    RK_ARG_TYPE(const fs::path&),
                    RK_ARG_VAL(path));
        RK_DECL_ACT(backwardHistoryUpdated,
                    backwardHistoryUpdated(bool hasHistory),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(hasHistory));
        RK_DECL_ACT(forwardHistoryUpdated,
                    forwardHistoryUpdated(bool hasHistory),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(hasHistory));

 private:
        std::vector<std::filesystem::path> pathHistory;
        std::size_t currentIndex;
};

#endif // BROWSER_PATH_HISTORY_H
