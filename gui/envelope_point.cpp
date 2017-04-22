#include "envelope_point.h"

#include <QDebug>

GKickEnvelopePoint::GKickEnvelopePoint(void)
	: QPointF(),
	  is_selected(false),
	  pointRadius(7),
	  dotRadius(3)
{
}

GKickEnvelopePoint::GKickEnvelopePoint(const QPointF &point)
	: QPointF(point),
	  is_selected(false),
	  pointRadius(7),
	  dotRadius(3)
{
}

GKickEnvelopePoint::GKickEnvelopePoint(double x, double y)
	: QPointF(x, y),
	  is_selected(false),
	  pointRadius(7),
	  dotRadius(3)
{
}

GKickEnvelopePoint::~GKickEnvelopePoint()
{
	
}

void GKickEnvelopePoint::draw(QPainter &painter, const QPointF &origin)
{
	QPen pen;
	if (isSelected()) {
		pen.setWidth(2);
	} else {
		pen.setWidth(1);
	}

	painter.setPen(pen);
	painter.setBrush(Qt::white);

	QRectF rect;
	double X0 = origin.x();
	double Y0 = origin.y();
	rect.setCoords(X0 + (x() - radius()), Y0 - (y() - radius()),
		       X0 + (x() + radius()), Y0 - (y() + radius()));
	painter.drawEllipse(rect);
	painter.drawText(X0 + (x() + 3/2 * radius()),
			 Y0 - (y() + 3/2 * radius()), "17.890 KHz");

	painter.setBrush(Qt::black);
	rect.setCoords(X0 + (x() - radius()/3), Y0 - (y() - radius()/3),
		       X0 + (x() + radius()/3), Y0 - (y() + radius()/3));
	painter.drawEllipse(rect);
}

double GKickEnvelopePoint::radius(void)
{
	return pointRadius;
}

double GKickEnvelopePoint::getDotRadius(void)
{
	return dotRadius;
}

bool GKickEnvelopePoint::isSelected(void)
{
	return is_selected;
}

void GKickEnvelopePoint::selectPoint(void)
{
	is_selected = true;
}

void GKickEnvelopePoint::unselectPoint(void)
{
	is_selected = false;
}

bool GKickEnvelopePoint::hasPoint(const QPointF &point)
{
	double px = point.x();
	double py = point.y();

	qDebug() << "mpx:" << px << "mpy: " << py;
	qDebug() << "x:"  << x() << "y: " << y();	
	
	if ((px > x() - pointRadius) && (px < x() + pointRadius)
	    && (py > y() - pointRadius) && (y() < y() + pointRadius)
	    && (pow(x() - px, 2) + pow(y() - py, 2) < pow(pointRadius, 2)))
		{
			return true;
		}
	return false;
}

