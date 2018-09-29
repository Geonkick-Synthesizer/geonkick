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

Envelope::Envelope(QObject *object, const QRect &area)
        : QObject(object),
          drawingArea(area),
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

QPoint Envelope::origin(void) const
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
        QPoint point = origin();
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
        QPoint point = origin();
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

        painter.translate(origin().x() - 30, origin().y() - H() / 2 + 35);
        painter.rotate(-90);

        painter.drawText(-5, -5, text);
        painter.rotate(90);
        painter.translate(-(origin().x() - 30), -(origin().y() - H() / 2 + 35));

        QFont font = painter.font();
        font.setPixelSize(10);
        painter.setFont(font);
        painter.setPen(QPen(QColor(110, 110, 110)));

        if (type() == Type::Amplitude) {
                double step = 1.0 / 10;
                double val = step;
                for (int i = 1; i <= 10; i++) {
                        int x = origin().x();
                        int y = origin().y() - H() * val;
                        QPen pen(QColor(80, 80, 80));
                        pen.setStyle(Qt::DotLine);
                        painter.setPen(pen);
                        painter.drawLine(x + 1, y, x + W(), y);
                        QRect rect(x - 28,  y - font.pixelSize() / 2, 20, font.pixelSize());
                        painter.setPen(QPen(QColor(110, 110, 110)));
                        painter.drawText(rect,  Qt::AlignRight, QString::number(val, 'f', 1));
                        val += step;
                }
        } else if (type() == Type::Frequency) {
                std::vector<int> values {20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};
                for (auto value : values) {
                        int x = origin().x();
                        int y = origin().y() - H() * (log10(value) - log10(20)) / (log10(envelopeAmplitude()) - log10(20));
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
	for (const auto &point : envelopePoints) {
		point->draw(painter);
        }
}

void Envelope::drawLines(QPainter &painter)
{
        QPolygon points;
	for (const auto& point : envelopePoints) {
	        points << QPoint(origin().x() + point->x(),
                                  origin().y() - (point->y()));
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
        std::vector<std::shared_ptr<EnvelopePoint>>::size_type index = 0;
	for (const auto& p : envelopePoints) {
		if (p->hasPoint(point)) {
			p->selectPoint();
                        selectedPointIndex = index;
                        pointSelected = true;
			break;
		}
                index++;
	}
}

void Envelope::unselectPoint(void)
{
        if (pointSelected) {
		envelopePoints[selectedPointIndex]->unselectPoint();
                pointSelected = false;
        }
}

int Envelope::getLeftPointLimit(void) const
{
        int x;
	if (!pointSelected || envelopePoints.empty() || selectedPointIndex < 1) {
		x = 0;
	} else {
		x = envelopePoints[selectedPointIndex - 1]->x();
	}

	return x;
}

int Envelope::getRightPointLimit(void) const
{
	int x;
	if (!pointSelected || envelopePoints.empty()) {
		x = 0;
	} else if (selectedPointIndex >= envelopePoints.size() - 1) {
		x = W();
	} else {
		x = envelopePoints[selectedPointIndex + 1]->x();
        }

	return x;
}

void Envelope::moveSelectedPoint(int x, int y)
{
        if (!pointSelected || envelopePoints.empty()) {
		return;
	}

        auto selectedPoint = envelopePoints[selectedPointIndex];
	if (x < getLeftPointLimit()) {
                selectedPoint->setX(getLeftPointLimit());
	} else if (x > getRightPointLimit()) {
                selectedPoint->setX(getRightPointLimit());
	} else {
                selectedPoint->setX(x);
	}

	if (y < 0) {
                selectedPoint->setY(0);
	} else if (y >= H()) {
                selectedPoint->setY(H());
	} else {
                selectedPoint->setY(y);
	}

        auto scaledPoint = scaleDown(QPoint(selectedPoint->x(), selectedPoint->y()));
        pointUpdatedEvent(selectedPointIndex, scaledPoint.x(), scaledPoint.y());
}

void Envelope::setPoints(const QPolygonF &points)
{
        removePoints();
        for (const auto &point : points) {
                QPoint scaledPoint;
                if (type() == Type::Amplitude) {
                        scaledPoint.setX(point.x() * W());
                        scaledPoint.setY(point.y() * H());
                } else {
                        scaledPoint.setX(point.x() * W());
                        double logVal = log10(point.y() * envelopeAmplitude());
                        double k = logVal / log10(envelopeAmplitude());
                        scaledPoint.setY(H() * k);
                }
                envelopePoints.push_back(std::make_shared<EnvelopePoint>(this, scaledPoint));
        }
}

void Envelope::addPoint(QPoint point)
{
        if (point.y() < 0) {
                point.setY(0);
        }  else if (point.y() > H()) {
                point.setY(H());
        }
  	if (point.x() > W()) {
	        point.setX(W());
		envelopePoints.push_back(std::make_shared<EnvelopePoint>(this, point));
	} else if (point.x() < 0) {
                point.setX(0);
                envelopePoints.insert(envelopePoints.begin(),
                                      std::make_shared<EnvelopePoint>(this, point));
        } else if (envelopePoints.empty()) {
                envelopePoints.push_back(std::make_shared<EnvelopePoint>(this, point));
	} else if (point.x() < envelopePoints[0]->x()) {
                envelopePoints.insert(envelopePoints.begin(),
                                      std::make_shared<EnvelopePoint>(this, point));
	} else if (point.x() > envelopePoints.back()->x()) {
                envelopePoints.push_back(std::make_shared<EnvelopePoint>(this, point));
	} else {
		for (auto it = envelopePoints.begin() ; it != envelopePoints.end(); ++it) {
			if (point.x() < (*it)->x()) {
                                envelopePoints.insert(it, std::make_shared<EnvelopePoint>(this, point));
                                break;
			}
		}
	}

        auto scaledPoint = scaleDown(point);
	pointAddedEvent(scaledPoint.x(), scaledPoint.y());
}

void Envelope::removePoint(const QPoint &point)
{
        std::vector<std::shared_ptr<EnvelopePoint>>::size_type index = 0;
        for (const auto p: envelopePoints) {
		if (p->hasPoint(point)) {
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
        updatePoints();
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
