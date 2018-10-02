/**
 * File name: envelope.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
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

Envelope::Envelope(QObject *object, const QRect &area)
        : QObject(object),
          drawingArea(area),
          pointRadius(10),
          dotRadius(3),
          selectedPointIndex(0),
          pointSelected(false),
          envelopeType(Type::Amplitude),
          supportedTypes({Type::Amplitude, Type::Frequency})
{

}

Envelope::~Envelope()
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

QPoint Envelope::getOrigin(void) const
{
        return drawingArea.bottomLeft();
}

void Envelope::draw(QPainter &painter)
{
        drawAxies(painter);
        drawScale(painter);
        drawPoints(painter);
        drawLines(painter);
}

void Envelope::drawAxies(QPainter & painter)
{
        painter.setPen(QColor(125, 125, 125));
        QPoint point = getOrigin();
        painter.drawLine(point.x(), point.y(), point.x() + W() + 10, point.y());
        painter.drawLine(point.x(), point.y(), point.x(), point.y() - H() - 10);
}

void Envelope::drawScale(QPainter &painter)
{
        drawTimeScale(painter);
        drawValueScale(painter);
}

void Envelope::drawTimeScale(QPainter &painter)
{
        QFont font = painter.font();
        font.setPixelSize(10);
        painter.setFont(font);

        auto val = envelopeLengh() / 10;
        int dx = W() / 10;
        QPoint point = getOrigin();
        int x  = point.x() + dx;
        for (auto i = 1; i <= 10; i++) {
                QPen pen(QColor(80, 80, 80));
                pen.setStyle(Qt::DotLine);
                painter.setPen(pen);
                painter.drawLine(x, point.y() - font.pixelSize() - 4, x, point.y() - H());

                QRect rect(x - 12, point.y() - 12, 25, font.pixelSize());
                painter.setPen(QPen(QColor(110, 110, 110)));
                painter.drawText(rect, Qt::AlignCenter, QString::number(std::round(i * val)));
                x += dx;
        }

        font.setPixelSize(12);
        painter.setFont(font);
        painter.setPen(QPen(QColor(180, 180, 180, 200)));
        painter.drawText(point.x() + W() / 2 - 35, point.y() +  font.pixelSize() + 10,
                         tr("Length, ") + QString::number(std::round(envelopeLengh())) + " ms");
}

void Envelope::drawValueScale(QPainter &painter)
{
        QString text;
        if (type() == Type::Amplitude) {
                text = tr("Amplitude");
        } else if (type() == Type::Frequency) {
                text = tr("Frequency, Hz");
        }

        painter.translate(getOrigin().x() - 30, getOrigin().y() - H() / 2 + 35);
        painter.rotate(-90);

        painter.drawText(-5, -5, text);
        painter.rotate(90);
        painter.translate(-(getOrigin().x() - 30), -(getOrigin().y() - H() / 2 + 35));

        QFont font = painter.font();
        font.setPixelSize(10);
        painter.setFont(font);
        painter.setPen(QPen(QColor(110, 110, 110)));

        if (type() == Type::Amplitude) {
                double step = envelopeAmplitude() / 10;
                double amplitude = envelopeAmplitude();
                for (int i = 1; i <= 10; i++) {
                        int x = getOrigin().x();
                        int y = 0;
                        if (amplitude > std::numeric_limits<double>::min()) {
                                y = getOrigin().y() - H() * (i * step / amplitude);
                        }
                        QPen pen(QColor(80, 80, 80));
                        pen.setStyle(Qt::DotLine);
                        painter.setPen(pen);
                        painter.drawLine(x + 1, y, x + W(), y);
                        QRect rect(x - 28,  y - font.pixelSize() / 2, 20, font.pixelSize());
                        painter.setPen(QPen(QColor(110, 110, 110)));
                        painter.drawText(rect,  Qt::AlignRight, QString::number(i * step, 'f', 2));
                }
        } else if (type() == Type::Frequency) {
                std::vector<int> values {20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};
                for (auto value : values) {
                        int x = getOrigin().x();
                        int y = getOrigin().y() - H() * (log10(value) - log10(20)) / (log10(envelopeAmplitude()) - log10(20));
                        if (y < 0) {
                                break;
                        }

                        QPen pen(QColor(80, 80, 80));
                        pen.setStyle(Qt::DotLine);
                        painter.setPen(pen);
                        if (value != 20) {
                                painter.drawLine(x, y, x + W(), y);
                        }

                        QRect rect;
                        if (value == 20) {
                                rect = QRect(x - 24, y - font.pixelSize() / 2, 20, font.pixelSize());
                        } else {
                                rect = QRect(x - 24, y - font.pixelSize() / 2, 20, font.pixelSize());
                        }
                        painter.setPen(QPen(QColor(110, 110, 110)));
                        QString text;
                        if (value >= 1000) {
                                text = QString::number(value / 1000) + "k";
                        } else {
                                text = QString::number(value);
                        }
                        painter.drawText(rect, Qt::AlignRight, text);
                }
        }

}

void Envelope::drawPoints(QPainter &painter)
{
        QPoint origin = getOrigin();
	for (const auto &point : envelopePoints) {
                QPoint scaledPoint = scaleUp(point);
                scaledPoint = QPoint(scaledPoint.x() + origin.x(), origin.y() - scaledPoint.y());
		drawPoint(painter, scaledPoint);
                scaledPoint = QPoint(scaledPoint.x(), scaledPoint.y() - 1.4 * getPointRadius());
                drawPointValue(painter, scaledPoint, point.y() * envelopeAmplitude());
        }
}

void Envelope::drawPoint(QPainter &painter, const QPoint &point)
{
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(200, 200, 200, 200));
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
	painter.setPen(pen);

        int r = getPointRadius();
	QRect rect(point.x() - r, point.y() - r, 2 * r, 2 * r);
	painter.drawEllipse(rect);

        QBrush brush = painter.brush();
        painter.setBrush(QColor(200, 200, 200, 200));
        r = getDotRadius();
        rect = QRect(point.x() - r, point.y() - r, 2 * r, 2 * r);
        painter.drawEllipse(rect);
        painter.setBrush(brush);
}

void Envelope::drawPointValue(QPainter &painter, const QPoint &point, double value)
{
        if (type() == Envelope::Type::Amplitude) {
                painter.drawText(point, QString::number(value, 'f', 2));
        } else if (type() == Envelope::Type::Frequency) {
                if (value < 20) {
                        painter.drawText(point, "20Hz " + frequencyToNote(20));
                }
                if (value >= 20 && value < 1000) {
                        painter.drawText(point, QString::number(value, 'f', 0)
                                         + "Hz " + frequencyToNote(value));
                } else if (value >= 1000 && value <= 20000) {
                        painter.drawText(point, QString::number(value / 1000, 'f', 1) + "kHz "
                                         + frequencyToNote(value));
                }
        }
}

void Envelope::drawLines(QPainter &painter)
{
        QPolygon points;
        QPoint origin = getOrigin();
	for (const auto& point : envelopePoints) {
                auto scaledPoint = scaleUp(point);
	        points << QPoint(origin.x() + scaledPoint.x(),
                                 origin.y() - scaledPoint.y());
	}

	auto pen = painter.pen();
	pen.setWidth(2);
        pen.setColor(QColor(200, 200, 200, 200));
	painter.setPen(pen);
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
	painter.drawPolyline(points);
}

bool Envelope::hasSelected(void) const
{
	return pointSelected;
}

void Envelope::selectPoint(const QPoint &point)
{
        GEONKICK_DEBUG_POINT(point);
        std::vector<QPointF>::size_type index = 0;
	for (const auto& p : envelopePoints) {
		if (hasPoint(p, point)) {
                        selectedPointIndex = index;
                        GEONKICK_LOG_DEBUG("selected : " << selectedPointIndex);
                        pointSelected = true;
			break;
		}
                index++;
	}
}

void Envelope::unselectPoint(void)
{
        if (pointSelected) {
                pointSelected = false;
        }
}

double Envelope::getLeftPointLimit(void) const
{
        double x;
	if (!pointSelected || envelopePoints.empty() || selectedPointIndex < 1) {
		x = 0;
	} else {
		x = envelopePoints[selectedPointIndex - 1].x();
	}

	return x;
}

double Envelope::getRightPointLimit(void) const
{
	double x;
	if (!pointSelected || envelopePoints.empty()) {
		x = 0;
	} else if (selectedPointIndex >= envelopePoints.size() - 1) {
		x = 1;
	} else {
		x = envelopePoints[selectedPointIndex + 1].x();
        }

	return x;
}

void Envelope::moveSelectedPoint(int x, int y)
{
        if (!pointSelected || envelopePoints.empty()) {
		return;
	}

        auto scaledPoint = scaleDown(QPoint(x, y));
        auto &selectedPoint = envelopePoints[selectedPointIndex];
	if (scaledPoint.x() < getLeftPointLimit()) {
                selectedPoint.setX(getLeftPointLimit());
	} else if (scaledPoint.x() > getRightPointLimit()) {
                selectedPoint.setX(getRightPointLimit());
	} else {
                selectedPoint.setX(scaledPoint.x());
	}

	if (y < 0) {
                selectedPoint.setY(0);
	} else if (y > H()) {
                selectedPoint.setY(1);
	} else {
                selectedPoint.setY(scaledPoint.y());
	}

        pointUpdatedEvent(selectedPointIndex, selectedPoint.x(), selectedPoint.y());
}

void Envelope::setPoints(const QPolygonF &points)
{
        removePoints();
        for (const auto &point : points) {
                envelopePoints.push_back(point);
        }
}

void Envelope::addPoint(const QPoint &point)
{
        GEONKICK_DEBUG_POINT(point);
        auto scaledPoint = scaleDown(point);
        GEONKICK_DEBUG_POINT(scaledPoint);
        if (scaledPoint.y() < 0) {
                scaledPoint.setY(0);
        }  else if (scaledPoint.y() > 1) {
                scaledPoint.setY(1);
        }
  	if (scaledPoint.x() > 1) {
	        scaledPoint.setX(1);
		envelopePoints.push_back(scaledPoint);
	} else if (scaledPoint.x() < 0) {
                scaledPoint.setX(0);
                envelopePoints.insert(envelopePoints.begin(), scaledPoint);
        } else if (envelopePoints.empty()) {
                envelopePoints.push_back(scaledPoint);
	} else if (scaledPoint.x() < envelopePoints[0].x()) {
                envelopePoints.insert(envelopePoints.begin(), scaledPoint);
	} else if (scaledPoint.x() > envelopePoints.back().x()) {
                envelopePoints.push_back(scaledPoint);
	} else {
		for (auto it = envelopePoints.begin() ; it != envelopePoints.end(); ++it) {
			if (scaledPoint.x() < (*it).x()) {
                                envelopePoints.insert(it, scaledPoint);
                                break;
			}
		}
	}

	pointAddedEvent(scaledPoint.x(), scaledPoint.y());
}

void Envelope::removePoint(const QPoint &point)
{
        std::vector<QPointF>::size_type index = 0;
        for (const auto p: envelopePoints) {
		if (hasPoint(p, point)) {
			if (p != envelopePoints.front() && p != envelopePoints.back()) {
				envelopePoints.erase(envelopePoints.begin() + index);
				pointRemovedEvent(index);
			}
			break;
		}
                index++;
        }
}

bool Envelope::setType(Type type)
{
        if (isSupportedType(type)) {
                envelopeType = type;
                updatePoints();
                return true;
        }
        return false;
}

Envelope::Type Envelope::type(void) const
{
	return envelopeType;
}

bool Envelope::isSupportedType(Type type) const
{
        if (supportedTypes.find(type) != supportedTypes.end()) {
                return true;
        }
        return false;
}

void Envelope::addSupportedType(Type type)
{
        if (!isSupportedType(type)) {
                supportedTypes.insert(type);
        }
}

void Envelope::removeSupportedType(Type type)
{
        if (isSupportedType(type)) {
                supportedTypes.erase(type);
        }
}

void Envelope::removePoints()
{
        envelopePoints.clear();
}

const QRect& Envelope::getDrawingArea()
{
        return drawingArea;
}

void Envelope::setDrawingArea(const QRect &rect)
{
        drawingArea = rect;
}

QPointF Envelope::scaleDown(const QPoint &point)
{
        QPointF scaledPoint;
        if (type() == Type::Amplitude) {
                scaledPoint = QPointF(static_cast<double>(point.x()) / W(),
                                      static_cast<double>(point.y()) / H());
        } else {
                scaledPoint.setX(static_cast<double>(point.x()) / W());
                double logVal = (static_cast<double>(point.y()) / H()) * (log10(envelopeAmplitude()) - log10(20));
                double val = pow(10, logVal + log10(20));
                scaledPoint.setY(val / envelopeAmplitude());
        }

        return scaledPoint;
}

QPoint Envelope::scaleUp(const QPointF &point)
{
        int x = 0;
        int y = 0;
        if (type() == Type::Amplitude) {
                x = point.x() * W();
                y = point.y() * H();
        } else {
                x = static_cast<int>(point.x() * W());
                double logRange = log10(envelopeAmplitude()) - log10(20);
                double k = 0;
                if (point.y() > 0) {
                        double logValue = log10(envelopeAmplitude() * point.y()) - log10(20);
                        if (logValue > 0) {
                                k = logValue / logRange;
                        }
                }
                y = k * H();
        }

        return QPoint(x, y);
}

bool Envelope::hasPoint(const QPointF &point, const QPoint &p)
{
        QPoint scaledPoint = scaleUp(point);
        int r = getPointRadius();
	if (pow(p.x() - scaledPoint.x(), 2) + pow(p.y() - scaledPoint.y(), 2) < pow(r, 2)) {
                return true;
        }

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

QString Envelope::frequencyToNote(double f)
{
        if (f < 16.35160 || f > 7902.133) {
                return QString();
        }

        int n = 0;
        while (f > 32.70320) {
                f /=2;
                n++;
        }

        int octave = n;
        std::vector<double> pitches {
                        16.35160,
                        17.32391,
                        18.35405,
                        19.44544,
                        20.60172,
                        21.82676,
                        23.12465,
                        24.49971,
                        25.95654,
                        27.50000,
                        29.13524,
                        30.86771};

        std::vector<QString> notes{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

        n = 12;
        while (--n && pitches[n] > f);
        if (n < 11 && f > (pitches[n + 1] - pitches[n]) / 2) {
                n++;
        }

        return "(" + notes[n] + QString::number(octave) + ")";
}

