/**
 * File name: gkickapi.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
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

#ifndef GKICK_API_H
#define GKICK_API_H

#include <memory>
#include <vector>
#include <QPolygonF>
#include <QObject>

#include "geonkick.h"

class GKickOscillator;

class GKickApi: public QObject {
 Q_OBJECT

 public:
        GKickApi(void);
        ~GKickApi();
        bool init();
        std::vector<std::shared_ptr<GKickOscillator>> getOscillators(void);
        bool isOscillatorEnabled(unsigned int index);
        QPolygonF getOscEvelopePoints(int osc, int envelope) const;
        void addOscEnvelopePoint(int osc,
                                 int envelope,
                                 const QPointF &point);
        void removeOscEvelopePoint(int osc,
                                   int envelope,
                                   int index);
        void updateOscEvelopePoint(int osc,
                                   int envelope,
                                   int index,
                                   const QPointF &point);
        void setOscFunction(int oscillatorIndex, enum geonkick_osc_func_type type);
        enum geonkick_osc_func_type getOscFunction(int oscillatorIndex) const;
        double getKickLength(void) const;
        void setMaxLength(double len);
        double getMaxLength() const;
        bool setOscAmplitudeValue(int oscillatorIndex, double v);
        bool setOscFrequencyValue(int oscillatorIndex, double v);
        double getOscAmplitudeValue(int oscillatorIndex) const;
        double getOscFrequencyValue(int oscillatorIndex) const;
        void addEnvelopePoint(double x, double y);
        void updateEnvelopePoint(unsigned int index, double x, double y);
        void removeEnvelopePoint(unsigned int index);
        double getAmplitude() const;
        void setKickFilterFrequency(double f);
        double getKickFilterFrequency(void) const;
        void setKickFilterQFactor(double f);
        double getKickFilterQFactor() const;

 public slots:
        void setAmplitude(double val);
        void setKickLength(double len);
        void enableOscillator(unsigned int index, bool enable);
        void setOscillatorFilterType(int index, enum geonkick_filter_type type);

 signals:
        void kickLengthUpdated(double len);

 private:
      	struct geonkick *gKickApi;
};

#endif
