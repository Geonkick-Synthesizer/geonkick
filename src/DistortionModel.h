/**
 * File name: DistortionModel.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#ifndef DISTORTION_MODEL_H
#define DISTORTION_MODEL_H

#include "AbstractModel.h"
#include "geonkick_api.h"
#include "envelope.h"

class DistortionModel: public AbstractModel
{
 public:
        using DistortionType = GeonkickApi::DistortionType;
        explicit DistortionModel(RkObject *parent);
        virtual ~DistortionModel() = default;
        virtual void enable(bool b) = 0;
        virtual bool isEnabled() const = 0;
        virtual void setDistortionType(DistortionType type) = 0;
        virtual DistortionType getDistortionType() const = 0;
        virtual void setInLimiter(double value) = 0;
        virtual double getInLimiter() const = 0;
        virtual void setOutLimiter(double value) = 0;
        virtual double getOutLimiter() const = 0;
        virtual void setDrive(double drive) = 0;
        virtual double getDrive() const = 0;
        virtual Envelope::Category envelopeCategory() const = 0;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(distortionTypeChanged,
                    distortionTypeChanged(DistortionType type),
                    RK_ARG_TYPE(DistortionType),
                    RK_ARG_VAL(type));
        RK_DECL_ACT(inLimiterChanged,
                    inLimiterChanged(double val),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(val));
        RK_DECL_ACT(outLimiterChanged,
                    outLimiterChanged(double val),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(val));
        RK_DECL_ACT(driveChanged,
                    driveChanged(double val),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(val));
};

#endif // DISTORTION_MODEL_H
