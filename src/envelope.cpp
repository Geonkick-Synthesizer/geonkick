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

#include "envelope.h"
#include "globals.h"

#include <iomanip>

Envelope::Envelope(const RkRect &area)
        : drawingArea{area}
        , pointRadius{10}
        , dotRadius{3}
        , selectedPointIndex{0}
        , supportedTypes({Type::Amplitude,
			Type::Frequency,
			Type::FilterCutOff,
			Type::DistortionDrive,
			Type::DistortionVolume,
			Type::PitchShift,
                        Type::NoiseDensity,
			Type::FilterQFactor})
        , overPointIndex{0}
        , isOverPoint{false}
        , pointSelected{false}
        , envelopeCategory{Category::Oscillator1}
        , envelopeType{Type::Amplitude}
        , editedPointIndex{0}
        , isEditingPoint{false}
	, applyType{ApplyType::Linear}
        , scrollState{false}
        , zoomInfoMap{{Type::Amplitude, {1.0, 0.0, 0.0}},
                      {Type::Frequency, {1.0, 0.0, 0.0}},
                      {Type::FilterCutOff, {1.0, 0.0, 0.0}},
                      {Type::FilterQFactor, {1.0, 0.0, 0.0}},
                      {Type::DistortionDrive, {1.0, 0.0, 0.0}},
                      {Type::DistortionVolume, {1.0, 0.0, 0.0}},
                      {Type::PitchShift, {1.0, 0.0, 0.0}},
                      {Type::NoiseDensity, {1.0, 0.0, 0.0}}}
{
}

int Envelope::W(void) const
{
        return drawingArea.width();
}

int Envelope::H(void) const
{
        return drawingArea.height();
}

void Envelope::zoomOut()
{
        auto res = zoomInfoMap.find(type());
        if (res == zoomInfoMap.end())
                return;

        res->second.zoomCoefficient = std::clamp(res->second.zoomCoefficient / 2, 1.0, 32.0);
        res->second.timeOrigin = std::clamp(res->second.timeOrigin,
                                           0.0,
                                           envelopeLength() - envelopeLength()
                                           / res->second.zoomCoefficient);
        // res->second.valueOrigin = std::clamp(res->second.valueOrigin,
        //                                      0.0,
        //                                      envelopeAmplitude() - envelopeAmplitude()
        //                                      / res->second.zoomCoefficient);
}

void Envelope::zoomIn()
{
        auto res = zoomInfoMap.find(type());
        if (res == zoomInfoMap.end())
                return;

        res->second.zoomCoefficient = std::clamp(res->second.zoomCoefficient * 2, 1.0, 32.0);
        res->second.timeOrigin = std::clamp(res->second.timeOrigin,
                                           0.0,
                                           envelopeLength() - envelopeLength()
                                           / res->second.zoomCoefficient);
        // res->second.valueOrigin = std::clamp(res->second.valueOrigin,
        //                                      0.0,
        //                                      envelopeAmplitude() - envelopeAmplitude()
        //                                      / res->second.zoomCoefficient);
}

double Envelope::getZoom() const
{
        auto res = zoomInfoMap.find(type());
        if (res == zoomInfoMap.end())
                return 1.0;

        return res->second.zoomCoefficient;
}

RkPoint Envelope::getOrigin(void) const
{
        return drawingArea.bottomLeft();
}

void Envelope::draw(RkPainter &painter, DrawLayer layer)
{
        if (layer == DrawLayer::Axies) {
                drawAxies(painter);
                drawScale(painter);
        } else if (layer == DrawLayer::Envelope) {
                RkImage envelopeImg(W() + 2 * getPointRadius() + 10,
                                    H() + 3 * getPointRadius() + 6);
                RkPainter imgPainter(&envelopeImg);
                imgPainter.fillRect(RkRect({0, 0}, envelopeImg.size()), {0, 0, 0, 0});
                imgPainter.translate({getPointRadius(),
                                envelopeImg.size().height() - getPointRadius()});
                drawPoints(imgPainter);
                drawLines(imgPainter);
                painter.drawImage(envelopeImg, getOrigin().x() - getPointRadius(),
                                  getOrigin().y() + getPointRadius()
                                  - envelopeImg.size().height());
        }
}

void Envelope::drawAxies(RkPainter & painter)
{
        auto pen = painter.pen();
        pen.setColor(RkColor(125, 125, 125));
        pen.setWidth(1);
        painter.setPen(pen);
        RkPoint point = getOrigin();
        painter.drawLine(point.x(), point.y(), point.x() + W() + 10, point.y());
        painter.drawLine(point.x(), point.y(), point.x(), point.y() - H() - 10);
}

void Envelope::drawScale(RkPainter &painter)
{
        drawTimeScale(painter);
        drawValueScale(painter);
}

void Envelope::drawTimeScale(RkPainter &painter)
{
        RkFont font = painter.font();
        font.setSize(10);
        painter.setFont(font);

        auto val = (envelopeLength() / getZoom()) / 10;
        int dx = W() / 10;
        RkPoint point = getOrigin();
        int x  = point.x() + dx;
        for (auto i = 1; i <= 10; i++) {
                RkPen pen(RkColor(80, 80, 80));
                pen.setStyle(RkPen::PenStyle::DotLine);
                painter.setPen(pen);
                painter.drawLine(x, point.y() - font.size() - 4, x, point.y() - H());

                RkRect rect(x - 12, point.y() - 12, 25, font.size());
                painter.setPen(RkColor(110, 110, 110));
                painter.drawText(rect, Geonkick::doubleToStr(getTimeOrigin() + i * val, 3));
                x += dx;
        }

        font.setSize(12);
        painter.setFont(font);
        painter.setPen(RkPen(RkColor(180, 180, 180, 200)));
        painter.drawText(point.x() + W() / 2 - 35, point.y() +  font.size() + 10,
                         "Length, " + std::to_string(std::llround(envelopeLength())) + " ms");
}

void Envelope::drawValueScale(RkPainter &painter)
{
        std::string text;
        switch (type()) {
        case Type::Amplitude:
                text = "Amplitude";
                break;
        case Type::DistortionDrive:
		text = "Distortion Drive";
                break;
        case Type::DistortionVolume:
                text = "Distortion Volume";
                break;
        case Type::Frequency:
        case Type::FilterCutOff:
                text = "Frequency, Hz";
                break;
	case Type::FilterQFactor:
                text = "Resonance";
                break;
        case Type::PitchShift:
                text = "Semitones";
                break;
        case Type::NoiseDensity:
                text = "Density";
                break;
        default:
                text = "Unknown";
        }

        painter.translate(RkPoint(getOrigin().x() - 30, getOrigin().y() - H() / 2 + 35));
        painter.rotate(-M_PI / 2);

        painter.drawText(-5, -5, text);
        painter.rotate(M_PI / 2);
        painter.translate(RkPoint(-(getOrigin().x() - 30), -(getOrigin().y() - H() / 2 + 35)));

        RkFont font = painter.font();
        font.setSize(10);
        painter.setFont(font);
        int rectH = font.size() + 2;
        painter.setPen(RkPen(RkColor(110, 110, 110)));

        if (type() == Type::Amplitude
	    || type() == Type::FilterQFactor
            || type() == Type::DistortionDrive
            || type() == Type::DistortionVolume
            || type() == Type::NoiseDensity) {
                double step = envelopeAmplitude() / 10;
                double amplitude = envelopeAmplitude();
                for (int i = 1; i <= 10; i++) {
                        int x = getOrigin().x();
                        int y = 0;
                        if (amplitude > std::numeric_limits<double>::min())
                                y = getOrigin().y() - H() * (i * step / amplitude);
                        RkPen pen(RkColor(80, 80, 80));
                        pen.setStyle(RkPen::PenStyle::DotLine);
                        painter.setPen(pen);
                        painter.drawLine(x + 1, y, x + W(), y);
                        RkRect rect(x - 28,  y -  rectH / 2, 22, rectH);
                        painter.setPen(RkPen(RkColor(110, 110, 110)));
                        std::ostringstream ss;
			if ( type() == Type::DistortionDrive || type() == Type::DistortionVolume)
				ss << std::fixed << std::setprecision(2) << i * step * pow(10, 36.0/20);
			else
				ss << std::fixed << std::setprecision(2) << i * step;
                        painter.drawText(rect, ss.str(), Rk::Alignment::AlignRight);
                }
        } else if (type() == Type::PitchShift && envelopeAmplitude() > std::numeric_limits<double>::min()) {
                double step = 2 * std::abs(envelopeAmplitude() / 10);
                double amplitude = 2 * std::abs(envelopeAmplitude());
                for (int i = 1; i <= 10; i++) {
                        int x = getOrigin().x();
                        int y = 0;
                        y = getOrigin().y() - H() * (i * step / amplitude);
                        RkPen pen(RkColor(80, 80, 80));
                        pen.setStyle(RkPen::PenStyle::DotLine);
                        painter.setPen(pen);
                        painter.drawLine(x + 1, y, x + W(), y);
                        RkRect rect(x - 28,  y -  rectH / 2, 22, rectH);
                        painter.setPen(RkPen(RkColor(110, 110, 110)));
                        std::ostringstream ss;
                        ss << std::fixed << std::setprecision(1) << i * step - amplitude / 2;
                        painter.drawText(rect, ss.str(), Rk::Alignment::AlignRight);
                }
        } else if (type() == Type::Frequency || type() == Type::FilterCutOff) {
                std::vector<int> values {20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 16000};
                for (auto value : values) {
                        int x = getOrigin().x();
                        int y = getOrigin().y() - H() * (log10(value) - log10(20)) / (log10(envelopeAmplitude()) - log10(20));
                        if (y < 0)
                                break;

                        RkPen pen(RkColor(80, 80, 80));
                        pen.setStyle(RkPen::PenStyle::DotLine);
                        painter.setPen(pen);
                        if (value != 20)
                                painter.drawLine(x, y, x + W(), y);

                        RkRect rect;
                        if (value == 20)
                                rect = RkRect(x - 28, y - rectH / 2, 22, rectH);
                        else
                                rect = RkRect(x - 28, y - rectH / 2, 22, rectH);
                        painter.setPen(RkPen(RkColor(110, 110, 110)));
                        std::string text;
                        if (value >= 1000)
                                text = std::to_string(value / 1000) + "k";
                        else
                                text = std::to_string(value);
                        painter.drawText(rect, text, Rk::Alignment::AlignRight);
                }
        }

}

void Envelope::drawPoints(RkPainter &painter)
{
        RkPen pen;
        pen.setWidth(2);
        pen.setColor(RkColor(200, 200, 200, 150));
	for (decltype(envelopePoints.size()) i = 0; i < envelopePoints.size(); i++) {
                if (pointSelected && i == selectedPointIndex) {
                        RkPen penSeleted;
                        penSeleted.setWidth(2);
                        penSeleted.setColor(RkColor(255, 255, 255, 255));
                        painter.setPen(penSeleted);
                } else if (!pointSelected && isOverPoint && i == overPointIndex) {
                        RkPen penSeleted;
                        penSeleted.setWidth(2);
                        penSeleted.setColor(RkColor(230, 230, 230, 200));
                        painter.setPen(penSeleted);
                } else {
                        painter.setPen(pen);
                }
                RkPoint scaledPoint = scaleUp(envelopePoints[i]);
                scaledPoint = RkPoint(scaledPoint.x(), -scaledPoint.y());
		drawPoint(painter, scaledPoint);
                scaledPoint = RkPoint(scaledPoint.x(), scaledPoint.y() - 1.4 * getPointRadius());
                drawPointValue(painter, scaledPoint, envelopePoints[i].y());
        }
}

void Envelope::drawPoint(RkPainter &painter, const RkPoint &point)
{
        painter.drawCircle(point, getPointRadius());
        painter.drawCircle(point, getDotRadius());
}

void Envelope::drawPointValue(RkPainter &painter, const RkPoint &point, double value)
{
        if (type() == Envelope::Type::Amplitude
	    || type() == Type::FilterQFactor
            || type() == Type::DistortionDrive
            || type() == Type::DistortionVolume
            || type() == Type::NoiseDensity) {
                value *= envelopeAmplitude();
		if (type() == Type::DistortionDrive || type() == Type::DistortionVolume)
			value *= pow(10, 36.0 / 20);
                painter.drawText(point.x(), point.y(), Geonkick::doubleToStr(value, 2));
        } else if (type() == Type::PitchShift) {
                value *= envelopeAmplitude();
                value = 2 * value - envelopeAmplitude();
                painter.drawText(point.x(), point.y(), Geonkick::doubleToStr(value, 1));
        } else if (type() == Envelope::Type::Frequency || type() == Type::FilterCutOff) {
                if (getApplyType() == ApplyType::Linear) {
			value *= envelopeAmplitude();
			if (value < 20)
				painter.drawText(point.x(), point.y(), "20Hz " + frequencyToNote(20));
			if (value >= 20 && value < 1000) {
				painter.drawText(point.x(), point.y(),
						 Geonkick::doubleToStr(value, 0)
						 + "Hz " + frequencyToNote(value));
			} else if (value >= 1000 && value <= 20000) {
				painter.drawText(point.x(), point.y(),
						 Geonkick::doubleToStr(value / 1000, 0)
						 + std::string("kHz ")
						 + frequencyToNote(value));
			}
                } else {
			auto frequency =  pow(10, (log10(envelopeAmplitude()) - log10(20)) * value + log10(20));
			if (frequency >= 20 && frequency < 1000) {
				double roundedValue = std::round(frequency * 10.0) / 10.0;
				painter.drawText(point.x(), point.y(),
						 Geonkick::doubleToStr(roundedValue, 1)
						 + "Hz " + frequencyToNote(frequency));
			} else if (frequency >= 1000 && frequency <= 20000) {
				frequency /= 1000;
				double roundedValue = std::round(frequency * 10.0) / 10.0;
				painter.drawText(point.x(), point.y(),
						 Geonkick::doubleToStr(roundedValue, 1)
						 + std::string("kHz ")
						 + frequencyToNote(1000 * frequency));
			}
		}
        }
}

void Envelope::drawLines(RkPainter &painter)
{
        if (envelopePoints.size() < 2)
                return;

        auto pen = painter.pen();
        pen.setWidth(2);
        pen.setColor(RkColor(200, 200, 200, 150));
        painter.setPen(pen);

        size_t i = 0;
        while (i < envelopePoints.size() - 1) {
                if (i + 1 < envelopePoints.size() && !envelopePoints[i + 1].isControlPoint()) {
                        std::vector<RkPoint> points;
                        auto scaledPoint1 = scaleUp(envelopePoints[i]);
                        points.push_back(RkPoint(scaledPoint1.x(), -scaledPoint1.y()));

                        auto scaledPoint2 = scaleUp(envelopePoints[i + 1]);
                        points.push_back(RkPoint(scaledPoint2.x(), -scaledPoint2.y()));

                        painter.drawPolyline(points);
                        i += 1;
                } else if (i + 2 < envelopePoints.size()) {
                        std::vector<RkPoint> points;
                        auto scaledPoint1 = scaleUp(envelopePoints[i]);
                        points.push_back(RkPoint(scaledPoint1.x(), -scaledPoint1.y()));

                        auto scaledPoint2 = scaleUp(envelopePoints[i + 1]);
                        points.push_back(RkPoint(scaledPoint2.x(), -scaledPoint2.y()));

                        auto scaledPoint3 = scaleUp(envelopePoints[i + 2]);
                        points.push_back(RkPoint(scaledPoint3.x(), -scaledPoint3.y()));

                        painter.drawCurve(points);
                        i += 2;
                } else {
                        break;
                }
        }
}

void Envelope::overPoint(const RkPoint &point)
{
        overPointIndex = 0;
        isOverPoint = false;
	for (decltype(envelopePoints.size()) i = 0; i < envelopePoints.size(); i++) {
		if (hasPoint(envelopePoints[i], point)) {
                        overPointIndex = i;
                        isOverPoint = true;
			break;
		}
        }
}

bool Envelope::hasOverPoint() const
{
        return isOverPoint;
}

bool Envelope::hasSelected(void) const
{
	return pointSelected;
}

void Envelope::selectPoint(const RkPoint &point)
{
        std::vector<RkRealPoint>::size_type index = 0;
	for (const auto& p : envelopePoints) {
		if (hasPoint(p, point)) {
                        selectedPointIndex = overPointIndex = index;
                        pointSelected = isOverPoint = true;
			break;
		}
                index++;
        }
}

void Envelope::updateSelectedPointValue(double val)
{
        if (hasEditingPoint() && editedPointIndex < envelopePoints.size()) {
                EnvelopePoint p = envelopePoints[editedPointIndex];
                p.setY(convertFromHumanValue(val));
                pointUpdatedEvent(editedPointIndex, p);
                updatePoints();
        }
}

void Envelope::setEditCurrentPoint(bool edit)
{
        if (hasOverPoint() && overPointIndex < envelopePoints.size()) {
                editedPointIndex = overPointIndex;
                isEditingPoint = edit;
                return;
        }
        editedPointIndex = 0;
        isEditingPoint = false;
}

double Envelope::getSelectedPointValue() const
{
        return convertToHumanValue(getSelectedPoint().y());
}

EnvelopePoint Envelope::getSelectedPoint() const
{
        if (hasSelected()) {
                if (overPointIndex < envelopePoints.size())
                        return envelopePoints[selectedPointIndex];
        } else if (hasOverPoint()) {
                if (overPointIndex < envelopePoints.size())
                        return envelopePoints[overPointIndex];
        }
        return {};
}

void Envelope::unselectPoint(void)
{
        pointSelected = false;
}

double Envelope::getLeftPointLimit(void) const
{
        double x;
	if (!pointSelected || envelopePoints.empty() || selectedPointIndex < 1)
		x = 0;
	else
		x = envelopePoints[selectedPointIndex - 1].x();

	return x;
}

double Envelope::getRightPointLimit(void) const
{
	double x;
	if (!pointSelected || envelopePoints.empty())
		x = 0;
	else if (selectedPointIndex >= envelopePoints.size() - 1)
		x = 1;
	else
		x = envelopePoints[selectedPointIndex + 1].x();

	return x;
}

void Envelope::moveSelectedPoint(int x, int y)
{
        if (!pointSelected || envelopePoints.empty())
		return;
	y = std::clamp(y, 0, H());
        auto scaledPoint = scaleDown(RkPoint(x, y));
        auto &selectedPoint = envelopePoints[selectedPointIndex];
	if (scaledPoint.x() < getLeftPointLimit())
                selectedPoint.setX(getLeftPointLimit());
	else if (scaledPoint.x() > getRightPointLimit())
                selectedPoint.setX(getRightPointLimit());
	else
                selectedPoint.setX(scaledPoint.x());
	selectedPoint.setY(scaledPoint.y());
        pointUpdatedEvent(selectedPointIndex, selectedPoint);
}

void Envelope::setPoints(const std::vector<EnvelopePoint> &points)
{
        removePoints();
        for (const auto &point : points)
                envelopePoints.push_back(point);
}

void Envelope::addPoint(const RkPoint &point, bool isControlPoint)
{
        EnvelopePoint scaledPoint = scaleDown(point);
        scaledPoint.setAsControlPoint(isControlPoint);

        scaledPoint.setY(std::clamp(scaledPoint.y(), 0.0, 1.0));
        scaledPoint.setX(std::clamp(scaledPoint.x(), 0.0, 1.0));

        auto it = std::lower_bound(
                                   envelopePoints.begin(), envelopePoints.end(), scaledPoint.x(),
                                   [](const EnvelopePoint &ep, float xVal) {
                                           return ep.x() < xVal;
                                   });

        if (isControlPoint) {
                if (it == envelopePoints.begin() || it == envelopePoints.end())
                        return;
                auto prev = std::prev(it);
                auto next = it;
                if (prev->isControlPoint() || next->isControlPoint())
                        return;
        }

        envelopePoints.insert(it, scaledPoint);
        pointAddedEvent(scaledPoint);
}

void Envelope::removePoint(const RkPoint &point)
{
        auto it = std::find_if(envelopePoints.begin(), envelopePoints.end(),
                               [&](const EnvelopePoint &ep) {
                                       return hasPoint(ep, point);
                               });

        if (it == envelopePoints.end()
            || it == envelopePoints.begin()
            || it == envelopePoints.end() - 1)
                return;

        auto index = std::distance(envelopePoints.begin(), it);
        it = envelopePoints.erase(it);
        pointRemovedEvent(index);

        if (it != envelopePoints.begin() && it != envelopePoints.end() - 1) {
                auto prev = std::prev(it);
                auto next = it;
                if (prev->isControlPoint() && next->isControlPoint()) {
                        size_t nextIndex = std::distance(envelopePoints.begin(), next);
                        size_t prevIndex = nextIndex - 1;

                        envelopePoints.erase(next);
                        pointRemovedEvent(nextIndex);

                        envelopePoints.erase(prev);
                        pointRemovedEvent(prevIndex);
                }
        }
}

void Envelope::setCategory(Envelope::Category cat)
{
        envelopeCategory = cat;
}

void Envelope::setType(Type type)
{
        if (isSupportedType(type)) {
                envelopeType = type;
                updateEnvelope();
        }
}

Envelope::Category Envelope::category(void) const
{
	return envelopeCategory;
}

Envelope::Type Envelope::type(void) const
{
	return envelopeType;
}

bool Envelope::isSupportedType(Type type) const
{
        if (supportedTypes.find(type) != supportedTypes.end())
                return true;
        return false;
}

void Envelope::addSupportedType(Type type)
{
        if (!isSupportedType(type))
                supportedTypes.insert(type);
}

void Envelope::removeSupportedType(Type type)
{
        if (isSupportedType(type))
                supportedTypes.erase(type);
}

void Envelope::removePoints()
{
        envelopePoints.clear();
}

const RkRect& Envelope::getDrawingArea()
{
        return drawingArea;
}

void Envelope::setDrawingArea(const RkRect &rect)
{
        drawingArea = rect;
}

void Envelope::updateEnvelope()
{
}

void Envelope::updatePoints()
{
}

RkRealPoint Envelope::scaleDown(const RkPoint &point)
{
        auto calculateX = [&](double x) {
                return (static_cast<double>(x) / W()) / getZoom()
                        + getTimeOrigin() / envelopeLength();
        };

        auto calculateY = [&](double y) {
                if (std::abs(envelopeAmplitude())
                    < std::numeric_limits<double>::epsilon()) {
                        return 0.0;
                }
                // return (static_cast<double>(y) / H()) / getZoom()
                //         + getValueOrigin() / envelopeAmplitude();
                return static_cast<double>(y) / H();
        };

	if (applyType == ApplyType::Logarithmic)
		return {calculateX(point.x()), calculateY(point.y())};

        return {calculateX(point.x()), calculateY(point.y())};
}

RkPoint Envelope::scaleUp(const RkRealPoint &point)
{
        auto calculateX = [&](double x) {
                return getZoom() * (x - getTimeOrigin() / envelopeLength()) * W();
        };

        auto calculateY = [&](double y) {
                if (std::abs(envelopeAmplitude())
                    < std::numeric_limits<double>::epsilon()) {
                        return 0.0;
                }
                // return getZoom() * (y - getValueOrigin() / envelopeAmplitude()) * H();
                return y * H();
        };

	if (getApplyType() == ApplyType::Logarithmic)
		return RkPoint(calculateX(point.x()),
                               calculateY(point.y()));

        if (type() == Type::Amplitude
	    || type() == Type::FilterQFactor
            || type() == Type::DistortionDrive
            || type() == Type::DistortionVolume
            || type() == Type::PitchShift) {
        } else {
                /*double logRange = log10(envelopeAmplitude()) - log10(20);
                double k = 0;
                if (point.y() > 0) {
                        double logValue = log10(envelopeAmplitude() * point.y()) - log10(20);
                        if (logValue > 0)
                                k = logValue / logRange;
                }
                y = k * H();*/
        }
        return RkPoint(calculateX(point.x()), calculateY(point.y()));
}

bool Envelope::hasPoint(const RkRealPoint &point, const RkPoint &p)
{
        RkPoint scaledPoint = scaleUp(point);
        int r = getPointRadius();
	if (pow(p.x() - scaledPoint.x(), 2) + pow(p.y() - scaledPoint.y(), 2) < pow(r, 2))
                return true;

	return false;
}

int Envelope::getPointRadius() const
{
        return pointRadius;
}

void Envelope::setPointRadius(int radius)
{
        pointRadius = radius;
}

int Envelope::getDotRadius() const
{
        return dotRadius;
}

void Envelope::setDotRadius(int radius)
{
        dotRadius = radius;
}

/**
 * This conversion should comply with this table:
 * https://homes.luddy.indiana.edu/donbyrd/Teach/MusicalPitchesTable.htm
 */
std::string Envelope::frequencyToNote(rk_real f) const
{
        if (f < 27.500 || f > 13289.752)
                return "";
        constexpr std::array<const char*, 12> noteNames = {"C",
                                                           "C#",
                                                           "D",
                                                           "D#",
                                                           "E",
                                                           "F",
                                                           "F#",
                                                           "G",
                                                           "G#",
                                                           "A",
                                                           "A#",
                                                           "B"};
        int midiNote  = std::round(12.0 * (log2(f / 27.5)) + 21.0);
        int octave    = midiNote / 12 - 1;
        int noteIndex = static_cast<int>(midiNote) % 12;
        return noteNames[noteIndex] + std::to_string(octave);
}

double Envelope::convertToHumanValue(double val) const
{
        if (type() == Envelope::Type::Amplitude
	    || type() == Type::FilterQFactor
            || type() == Type::DistortionDrive
            || type() == Type::DistortionVolume
            || type() == Type::NoiseDensity) {
                val *= envelopeAmplitude();
		if (type() == Type::DistortionDrive || type() == Type::DistortionVolume)
			return val * pow(10, 36.0 / 20);
		else
			return val;
        } else if (type() == Type::PitchShift) {
                val *= envelopeAmplitude();
                return 2 * val - envelopeAmplitude();
        } else if (type() == Envelope::Type::Frequency || type() == Type::FilterCutOff) {
                if (getApplyType() == ApplyType::Linear)
			val *= envelopeAmplitude();
                else
	                return pow(10, (log10(envelopeAmplitude()) - log10(20)) * val + log10(20));
        }
        return val;
}

double Envelope::convertFromHumanValue(double val) const
{
        if ( envelopeAmplitude() == 0.0 )
                return val;

        if (type() == Envelope::Type::Amplitude
	    || type() == Type::FilterQFactor
            || type() == Type::DistortionDrive
            || type() == Type::DistortionVolume
            || type() == Type::NoiseDensity) {
                val /= envelopeAmplitude();
		if (type() == Type::DistortionDrive || type() == Type::DistortionVolume)
			val /= pow(10, 36.0 / 20);
        } else if (type() == Type::PitchShift) {
                val = (val / envelopeAmplitude() + 1.0) / 2.0;
        } else if (type() == Envelope::Type::Frequency || type() == Type::FilterCutOff) {
                if (getApplyType() == ApplyType::Linear) {
                        val /= envelopeAmplitude();
                } else {
			if (val >= 20 && envelopeAmplitude() >= 20) {
				val = log10(val / 20) / log10(envelopeAmplitude() / 20);
				return std::clamp(val, 0.0, 1.0);
			}
                }
                return 0;
        }

        return std::clamp(val, 0.0, 1.0);
}

bool Envelope::hasEditingPoint() const
{
        return isEditingPoint;
}

std::string Envelope::getCurrentPointInfo() const
{
        if (!hasSelected() && !hasOverPoint() )
                return std::string();

        auto point = getSelectedPoint();
        double roundedValue = std::round(convertToHumanValue(point.y()) * 10000.0) / 10000.0;
        std::string info = Geonkick::doubleToStr(roundedValue, 4);
        switch (type()) {
        case Type::Frequency:
        case Type::FilterCutOff:
                info += "Hz " + frequencyToNote(point.y());
                break;
        default:
                break;
        }

        info += ", " + Geonkick::doubleToStr(point.x() * envelopeLength(), 0) + "ms";
        return info;
}

void Envelope::setApplyType(Envelope::ApplyType apply)
{
	applyType = apply;
}

Envelope::ApplyType Envelope::getApplyType() const
{
	return applyType;
}

void Envelope::setScrollState(bool b)
{
        scrollState = b;
}

bool Envelope::isScrollState() const
{
        return scrollState;
}

void Envelope::setTimeOrigin(double timeOrg)
{
        auto res = zoomInfoMap.find(type());
        if (res == zoomInfoMap.end())
                return;

        res->second.timeOrigin = std::clamp(timeOrg,
                                            0.0,
                                            envelopeLength() - envelopeLength()
                                            / res->second.zoomCoefficient);
}

double Envelope::getTimeOrigin() const
{
        auto res = zoomInfoMap.find(type());
        if (res == zoomInfoMap.end())
                return 0.0;
        return res->second.timeOrigin;
}

void Envelope::setValueOrigin(double valueOrg)
{
        auto res = zoomInfoMap.find(type());
        if (res == zoomInfoMap.end())
                return;
        // TODO:
        //        res->second.valueOrigin = std::clamp(valueOrg,
        //                                     0.0,
        //                                     envelopeAmplitude() - envelopeAmplitude()
        //                                     / res->second.zoomCoefficient);
}

double Envelope::getValueOrigin() const
{
        auto res = zoomInfoMap.find(type());
        if (res == zoomInfoMap.end())
                return 0.0;
        return res->second.valueOrigin;
}

