#include "gkick_knob.h"

#include <QPainter>
#include <QDebug>

GKickKnob::GKickKnob(QWidget *parent)
	: QWidget(parent),
	  knobRadius(GKICK_UI_DEFAULT_KNOB_RADIOUS),
	  lastPositionPoint(),
	  knobValueDegree(0),
	  realValue(0.0),
          isSelected(false)

{
        setFixedSize(2 * knobRadius, 2 * knobRadius);
        setContentsMargins(2, 2, 2, 2);
}

GKickKnob::~GKickKnob()
{

}

void
GKickKnob::paintEvent(QPaintEvent *event)
{
        Q_UNUSED(event);
        QPainter painter(this);

        QRect rect;
        painter.setBrush(Qt::white);
        rect.setCoords(2, 2, width() - 2, height() - 2);
        painter.drawEllipse(rect);

        painter.setBrush(Qt::gray);

        rect.setCoords(2, 2, width() - 2, height() - 2);
        painter.drawPie(rect, (90) * 16, - knobValueDegree * 16);

        painter.setBrush(Qt::white);
        rect.setCoords(8, 8, width() - 8, height() - 8);
        painter.drawEllipse(rect);
}

int
GKickKnob::getRadius(void)
{
        return knobRadius;
}

int
GKickKnob::getWidth(void)
{
        return 2 * knobRadius;
}

int
GKickKnob::getHeight(void)
{
        return 2 * knobRadius;
}

void
GKickKnob::mousePressEvent(QMouseEvent *event)
{
        isSelected = true;
        lastPositionPoint.setX(event->x());
        lastPositionPoint.setY(event->y());
}

void
GKickKnob::mouseReleaseEvent(QMouseEvent *event)
{
        isSelected = false;
}

void
GKickKnob::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void
GKickKnob::mouseMoveEvent(QMouseEvent *event)
{
        if (isSelected) {
                int dy = event->y() - lastPositionPoint.y();
                knobValueDegree += dy;
                if (knobValueDegree < 0) {
                        knobValueDegree = 0;
                } else if (knobValueDegree > 360) {
                        knobValueDegree = 360;
                }
                lastPositionPoint.setX(event->x());
                lastPositionPoint.setY(event->y());
                emit valueUpdated(realValue * (double)knobValueDegree / 360.0);
                update();
        }
}

void GKickKnob::setPosition(double v)
{
        knobValueDegree = v * 360.0;
        if (knobValueDegree > 360.0) {
                knobValueDegree = 360.0;
        } else if(knobValueDegree < 0.0) {
                knobValueDegree = 0.0;
        }
        emit valueUpdated(realValue * (double)knobValueDegree / 360.0);
}

double GKickKnob::getPosition(void)
{
	return (double)knobValueDegree / 360.0;
}

void GKickKnob::setValue(double v)
{
	realValue = v;
}

double GKickKnob::getValue(void)
{
	return realValue * ((double)knobValueDegree / 360.0);
}

void GKickKnob::resizeEvent(QResizeEvent *event)
{
        Q_UNUSED(event);
}
