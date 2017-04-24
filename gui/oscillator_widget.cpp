#include "oscillator_widget.h"

#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <QDebug>

OscillatorWidget::OscillatorWidget(QWidget *parent, GKickOscillator *osc)
	: QWidget(parent),
	  kickOscillator(osc),
	  widgetPainter(),
	  oscEnvelope(),
	  xPadding(50),
	  yPadding(50),
	  originPoint(0.0, 0.0),
	  mousePoint(0.0, 0.0)
{

  this->resize(800, 400);
  calculateRatio();  
  oscEnvelope.addEnvelopePoints(kickOscillator->getEnvelopePoints());
   //  oscEnvelope->setEnvelopeType(kickOscillator->getEnvelopeType());
   connectoToOscillator();
}

void OscillatorWidget::calculateRatio(void)
{
  double w = 1.0;//kickOscillator->getEnvelopeLenth();
  double h = 1.0; //kickOscillator->getEnvelopeHeight();

  qDebug() << "WIDTH: " << width();
  qDebug() << "HEIGHT: " << height(); 
  if (w > 0.0 && h > 0.0) {
    oscEnvelope.setXRatio(w / (width() - 2 * xPadding));
    oscEnvelope.setYRatio(h / (height() - 2 * yPadding));
  } else {
    oscEnvelope.setXRatio(1);
    oscEnvelope.setYRatio(1);
  }
}

void OscillatorWidget::connectoToOscillator(void)
{
  connect(&oscEnvelope, SIGNAL(pointAdded(const QPointF &)),
	  kickOscillator, SLOT(addPoint(const QPointF &)));
  connect(&oscEnvelope, SIGNAL(pointRemoved(int)),
	  kickOscillator, SLOT(removePoint(int)));
  connect(&oscEnvelope, SIGNAL(pointUpdated(int, const QPointF&)),
	  kickOscillator, SLOT(updatePoint(int, const QPointF&)));

}

OscillatorWidget::~OscillatorWidget()
{
  
}

void OscillatorWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

        calculateRatio();
	recalculateOrigin();
	drawAxes();
	drawEnvelope();
}

void OscillatorWidget::drawEnvelope(void)
{
	QPainter painter(this);
	oscEnvelope.setWidth(width() - 2 * xPadding);
	oscEnvelope.setHeight(height() - 2 * yPadding);
	oscEnvelope.setOrigin(originPoint);
	oscEnvelope.draw(painter);
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
	if (event->button() == Qt::RightButton) {
		oscEnvelope.removePoint(QPointF(event->x() - xPadding,
					 height() - (event->y() + yPadding)));
		update();
		return; 
	}
	
	qDebug() << "mousePressEvent";
	mousePoint.setX(event->x());
	mousePoint.setY(event->y());
	oscEnvelope.selectPoint(QPointF(event->x() - xPadding,
				 height() - (event->y() + yPadding)));
	if (oscEnvelope.hasSelected()) {
		update();
	}
}

void 
OscillatorWidget::mouseReleaseEvent(QMouseEvent *event)
{
  Q_UNUSED(event)
    if (oscEnvelope.hasSelected()) {
      oscEnvelope.unselectPoint();
      update();
    }
}

void 
OscillatorWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	oscEnvelope.addPoint(QPointF(event->x() - xPadding, height() - (event->y() + yPadding)));
	update();
}

void 
OscillatorWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!oscEnvelope.hasSelected()) {
		return;
	}

	double left  = oscEnvelope.getLeftPointLimit();
	double right = oscEnvelope.getRightPointLimit();		
	if (event->x() < left + xPadding) {
		oscEnvelope.setOutOfRangeX(OscillatorEnvelope::OUT_OF_RANGE_LEFT);
	} else if (event->x() > right + xPadding) {
		oscEnvelope.setOutOfRangeX(OscillatorEnvelope::OUT_OF_RANGE_RIGHT);
	} else {
		oscEnvelope.setOutOfRangeX(OscillatorEnvelope::OUT_OF_RANGE_NONE);		
	}

	if (event->y() < yPadding) {
		oscEnvelope.setOutOfRangeY(OscillatorEnvelope::OUT_OF_RANGE_TOP);
	} else if(event->y() > height() - xPadding) {
		oscEnvelope.setOutOfRangeY(OscillatorEnvelope::OUT_OF_RANGE_BOTTOM);
	} else {
		oscEnvelope.setOutOfRangeY(OscillatorEnvelope::OUT_OF_RANGE_NONE);
	}

	oscEnvelope.moveSelectedPoint(event->x() - mousePoint.x(),
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

void OscillatorWidget::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);
  calculateRatio();
}
