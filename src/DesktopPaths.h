/**
 * File name: DesktopPaths.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2023 Iurie Nistor 
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

#ifndef GEONGKICK_DESKTOP_PATHS_H
#define GEONGKICK_DESKTOP_PATHS_H

#include "globals.h"

class DesktopPaths {
 public:
	DesktopPaths();
	std::filesystem::path getHomePath() const;
	std::filesystem::path getDesktopPath() const;
	std::filesystem::path getDownloadsPath() const;
	std::filesystem::path getConfigPath() const;
	std::filesystem::path getDataPath() const;
	std::filesystem::path getUserPresetsPath() const;
        std::filesystem::path getFactoryPresetsPath() const;
        std::vector<std::filesystem::path> getDrivesList() const;
        static bool isRootPath(const fs::path &path);

 protected:
        std::filesystem::path findFactoryPresetsPath(const fs::path &presetsPath) const;
	void loadPaths();
#ifdef GEONKICK_OS_WINDOWS
        void loadDrivesList();
#endif // GEONKICK_OS_WINDOWS

 private:
	std::filesystem::path homePath;
	std::filesystem::path desktopPath;
	std::filesystem::path downloadsPath;
	std::filesystem::path configPath;
	std::filesystem::path dataPath;
	std::filesystem::path factoryPresetsPath;
        std::filesystem::path userPresetsPath;
        std::vector<std::filesystem::path> drivesList;
};

#endif // GEONGKICK_DESKTOP_PATHS_H
