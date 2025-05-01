/**
 * File name: ExportAbstract.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor 
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

#ifndef GEONGKICK_EXPORT_ABSTRACT_H
#define GEONGKICK_EXPORT_ABSTRACT_H

#include "globals.h"

class ExportAbstract {
 public:
        enum class ExportFormat : int {
                Flac = static_cast<int>(GeonkickTypes::Formats::Flac),
                Wav  = static_cast<int>(GeonkickTypes::Formats::Wav),
                Ogg  = static_cast<int>(GeonkickTypes::Formats::Ogg),
                Sfz  = static_cast<int>(GeonkickTypes::Formats::Sfz)
        };

        ExportAbstract(const std::filesystem::path &path = std::filesystem::path(),
                       const ExportFormat format = ExportFormat::Flac);
        virtual ~ExportAbstract() = default;
        virtual bool doExport() = 0;
        const ExportFormat& format() const;
        void setFormat(const ExportFormat format);
        void setError(const std::string_view &error);
        const std::string& getError() const;
        bool isError() const;
        const std::filesystem::path& getExportPath() const;

 private:
        std::string errorMessage;
        ExportFormat exportFormat;
        std::filesystem::path exportPath;
};

#endif // GEONGKICK_EXPORT_ABSTRACT_H
