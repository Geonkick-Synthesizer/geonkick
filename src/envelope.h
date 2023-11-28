/**
 * File name: envelope.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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

#ifndef GEONKICK_ENVELOPE_H
#define GEONKICK_ENVELOPE_H

#include "geonkick_api.h"

#include <RkPainter.h>
#include <RkRealPoint.h>

#include <memory>
#include <unordered_set>

class Envelope : public RkObject
{
 public:
	using ApplyType = GeonkickApi::EnvelopeApplyType;

        enum class DrawLayer {
                Axies,
                Envelope
        };

        using Type = GeonkickApi::EnvelopeType;
        enum class Category:int {
                Oscillator1 = static_cast<int>(GeonkickApi::OscillatorType::Oscillator1),
                Oscillator2 = static_cast<int>(GeonkickApi::OscillatorType::Oscillator2),
                Noise       = static_cast<int>(GeonkickApi::OscillatorType::Noise),
                General
        };

        explicit Envelope(const RkRect &area = RkRect());
        virtual ~Envelope() = default;
        int W(void) const;
        int H(void) const;
        virtual double envelopeLength(void) const { return 0;}
        virtual double envelopeAmplitude(void) const { return 0;}
        RkPoint getOrigin(void) const;
        void draw(RkPainter &painter, DrawLayer layer);
        void overPoint(const RkPoint &point);
        bool hasSelected() const;
        bool hasOverPoint() const;
        void selectPoint(const RkPoint &point);
        double getSelectedPointValue() const;
        RkRealPoint getSelectedPoint() const;
        void updateSelectedPointValue(double val);
        void setEditCurrentPoint(bool edit = true);
        void unselectPoint(void);
        void moveSelectedPoint(int x, int y);
        void addPoint(const RkPoint &point);
        void removePoint(const RkPoint &point);
        Category category() const;
        Type type() const;
        bool isSupportedType(Type type) const;
        const RkRect& getDrawingArea();
        virtual void setEnvelopeLengh(double len) { RK_UNUSED(len); }
        void setCategory(Category cat);
        void setType(Type type);
        void addSupportedType(Type type);
        void removeSupportedType(Type type);
        void setPoints(const std::vector<RkRealPoint>  &points);
        void removePoints();
        void setDrawingArea(const RkRect &rect);
        virtual void updateEnvelope();
	virtual void updatePoints();
         RK_DECL_ACT(envelopeLengthUpdated,
                     envelopeLengthUpdated(double val),
                     RK_ARG_TYPE(double),
                     RK_ARG_VAL(val));
         RK_DECL_ACT(amplitudeUpdated,
                     amplitudeUpdated(double val),
                     RK_ARG_TYPE(double),
                     RK_ARG_VAL(val));
         RK_DECL_ACT(envelopeUpdated,
                     envelopeUpdated(),
                     RK_ARG_TYPE(),
                     RK_ARG_VAL());
        std::string getCurrentPointInfo() const;
	void setApplyType(ApplyType apply);
	ApplyType getApplyType() const;

 protected:
        virtual void pointAddedEvent(double x, double y) = 0;
        virtual void pointUpdatedEvent(unsigned int index, double x, double y) = 0;
        virtual void pointRemovedEvent(unsigned int index)  = 0;
        void drawAxies(RkPainter &painter);
        void drawScale(RkPainter &painter);
        void drawTimeScale(RkPainter &painter);
        void drawValueScale(RkPainter &painter);
        void drawPoints(RkPainter &painter);
        void drawPoint(RkPainter &painter, const RkPoint &point);
        void drawPointValue(RkPainter &painter, const RkPoint &point, double value);
        void drawLines(RkPainter &painter);
        RkRealPoint scaleDown(const RkPoint &point);
        RkPoint scaleUp(const RkRealPoint &point);
        bool hasPoint(const RkRealPoint &point, const RkPoint &p);
        int getPointRadius() const;
        void setPointRadius(int radius);
        int getDotRadius()  const;
        void setDotRadius(int radius);
        double getLeftPointLimit(void) const;
        double getRightPointLimit(void) const;
        std::string frequencyToNote(rk_real f) const;
        double convertToHumanValue(double val) const;
        double convertFromHumanValue(double val) const;
        bool hasEditingPoint() const;

 private:
        RkRect drawingArea;
        std::vector<RkRealPoint> envelopePoints;
        int pointRadius;
        int dotRadius;
        std::vector<RkRealPoint>::size_type selectedPointIndex;
        std::unordered_set<Type> supportedTypes;
        std::vector<RkRealPoint>::size_type overPointIndex;
        bool isOverPoint;
        bool pointSelected;
        Category envelopeCategory;
        Type envelopeType;
        size_t editedPointIndex;
        bool isEditingPoint;
	ApplyType applyType;
};

#endif // GEONKICK_ENVELOPE_H
