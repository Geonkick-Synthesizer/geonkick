/**
 * File name: ExportAbstract.h
 * Project: Geonkick (A percussion synthesizer)
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

#ifndef GEONGKICK_EXPORT_ABSTRACT_H
#define GEONGKICK_EXPORT_ABSTRACT_H

#include "globals.h"

#include <RkObject.h>

class ExportAbstract: public RkObject {
 public:
        enum class ExportFormat : int {
                Flac = static_cast<int>(GeonkickTypes::Formats::Flac),
                Wav  = static_cast<int>(GeonkickTypes::Formats::Wav),
                Ogg  = static_cast<int>(GeonkickTypes::Formats::Ogg),
                Sfz  = static_cast<int>(GeonkickTypes::Formats::Sfz)
        };

        RK_DECL_ACT(exportFinished,
                    exportFinished(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(onError,
                    onError(std::string error),
                    RK_ARG_TYPE(std::string),
                    RK_ARG_VAL(error));
        RK_DECL_ACT(currentProgress,
                    currentProgress(int progress),
                    RK_ARG_TYPE(int progress),
                    RK_ARG_VAL(progress));

        ExportAbstract(RkObject *parent,
                       const std::filesystem::path &path = std::filesystem::path(),
                       ExportFormat expFormat = ExportFormat::Flac);
        virtual ~ExportAbstract() = default;
        virtual bool doExport() = 0;
        ExportFormat format() const;
        void setFormat(ExportFormat exFormat);
        std::string errorString() const;
        bool isError() const;
        int progress() const;

 protected:
        void setError(const std::string &error);
        void setProgress(const int val);
        std::filesystem::path getExportPath() const;
        
 private:
        ExportFormat exportFormat;
        std::filesystem::path exportPath;
        std::string exportError;
        int exportProgress;
};

#endif // GEONGKICK_EXPORT_ABSTRACT_H
