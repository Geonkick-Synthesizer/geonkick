#include "oscillator_widget.h"

#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>

#include <QDebug>

OscillatorWidget::OscillatorWidget(QWidget *parent)
	: QWidget(parent),
	  widgetPainter(),
	  kickEnvelope(),
	  xPadding(50),
	  yPadding(50),
	  originPoint(0.0, 0.0),
	  mousePoint(0.0, 0.0)
{
	//	widgetPainter.begin(this);
}

OscillatorWidget::~OscillatorWidget()
{
  
}

void OscillatorWidget::paintEvent(QPaintEvent *event)
{
	recalculateOrigin();
	drawAxes();
	drawEnvelope();
}

void OscillatorWidget::drawEnvelope(void)
{
	QPainter painter(this);
	kickEnvelope.setWidth(width() - 2 * xPadding);
	kickEnvelope.setHeight(height() - 2 * yPadding);
	kickEnvelope.setOrigin(originPoint);
	kickEnvelope.draw(painter);
}

void OscillatorWidget::drawAxes(void)
{
	QPainter painter(this);
	QPen pen(Qt::black);
	pen.setWidth(1);
	painter.setPen(pen);
        
	painter.drawLine(xPadding, yPadding, xPadding, height() - yPadding);
	painter.drawLine(xPadding, height() - yPadding,
			 width() - xPadding, height() - yPadding);
}

void 
OscillatorWidget::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "mousePressEvent";
	mousePoint.setX(event->x());
	mousePoint.setY(event->y());
	kickEnvelope.selectPoint(QPointF(event->x() - xPadding,
					 height() - (event->y() + yPadding)));
	if (kickEnvelope.hasSelected()) {
		update();
	}
}

void 
OscillatorWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (kickEnvelope.hasSelected()) {
		kickEnvelope.unselectPoint();
		update();
	}
}

void 
OscillatorWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!kickEnvelope.hasSelected()) {
		return;
	}

	double left  = kickEnvelope.getLeftPointLimit();
	double right = kickEnvelope.getRightPointLimit();		
	if (event->x() < left + xPadding) {
		kickEnvelope.setOutOfRangeX(GKickEnvelope::OUT_OF_RANGE_LEFT);
	} else if (event->x() > right + xPadding) {
		kickEnvelope.setOutOfRangeX(GKickEnvelope::OUT_OF_RANGE_RIGHT);
	} else {
		kickEnvelope.setOutOfRangeX(GKickEnvelope::OUT_OF_RANGE_NONE);		
	}

	if (event->y() < yPadding) {
		kickEnvelope.setOutOfRangeY(GKickEnvelope::OUT_OF_RANGE_TOP);
	} else if(event->y() > height() - xPadding) {
		kickEnvelope.setOutOfRangeY(GKickEnvelope::OUT_OF_RANGE_BOTTOM);
	} else {
		kickEnvelope.setOutOfRangeY(GKickEnvelope::OUT_OF_RANGE_NONE);
	}

	kickEnvelope.moveSelectedPoint(event->x() - mousePoint.x(),
				       -(event->y() - mousePoint.y()));
	mousePoint.setX(event->x());
	mousePoint.setY(event->y());
	update();
}

void OscillatorWidget::recalculateOrigin(void)
{
	originPoint.setX(xPadding);
	originPoint.setY((height() - yPadding));
}


