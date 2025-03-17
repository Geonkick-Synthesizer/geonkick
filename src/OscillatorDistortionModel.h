/**
 * File name: OscillatorDistortionModel.h
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

#ifndef OSCILLATOR_DISTORTION_MODEL_H
#define OSCILLATOR_DISTORTION_MODEL_H

#include "DistortionModel.h"
#include "envelope.h"

class OscillatorModel;

class OscillatorDistortionModel: public DistortionModel
{
 public:
        OscillatorDistortionModel(OscillatorModel *parent);
        void enable(bool b) override;
        bool isEnabled() const override;
        void setDistortionType(DistortionType type) override;
        DistortionType getDistortionType() const override;
        void setInLimiter(double value) override;
        double getInLimiter() const override;
        void setOutLimiter(double value) override;
        double getOutLimiter() const override;
        void setDrive(double drive) override;
        double getDrive() const override;
        Envelope::Category envelopeCategory() const override;

 private:
        OscillatorModel *oscillatorModel;
};

#endif // OSCILLATOR_DISTORTION_MODEL_H
