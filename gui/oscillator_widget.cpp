#include "oscillator_widget.h"

#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <QDebug>

OscillatorWidget::OscillatorWidget(QWidget *parent, GKickOscillator *osc)
	: QWidget(parent),
	  kickOscillator(osc),
	  widgetPainter(),
	  oscEnvelope(this),
	  xPadding(50),
	  yPadding(50),
	  originPoint(0.0, 0.0),
	  mousePoint(0.0, 0.0),
	  kickLength(0.0)
{
        setOscillator(kickOscillator);
}

void OscillatorWidget::calculateRatio(void)
{
        double w = oscEnvelope.getEnvelopeLenth();
        double h = oscEnvelope.getEnvelopeHeight();

        if (w > 0.0 && h > 0.0) {
                oscEnvelope.setXRatio((width() - 2 * xPadding) / w);
                oscEnvelope.setYRatio((height() - 2 * yPadding) / h);
        } else {
                oscEnvelope.setXRatio(1);
                oscEnvelope.setYRatio(1);
        }
}

void OscillatorWidget::connectOscillator(void)
{
        connect(&oscEnvelope, SIGNAL(pointAdded(const QPointF &)),
                kickOscillator, SLOT(addPoint(const QPointF &)));
        connect(&oscEnvelope, SIGNAL(pointRemoved(int)),
                kickOscillator, SLOT(removePoint(int)));
        connect(&oscEnvelope, SIGNAL(pointUpdated(int, const QPointF&)),
                kickOscillator, SLOT(updatePoint(int, const QPointF&)));
        connect(kickOscillator, SIGNAL(oscAmplitudeValueUpdated(double)),
                this, SLOT(setEnvelopeAmplitudeValue(double)));
        connect(kickOscillator, SIGNAL(oscFrequencyValueUpdated(double)),
                this, SLOT(setEnvelopeFrequencyValue(double)));
}

void OscillatorWidget::disconnectOscillator(void)
{
        disconnect(&oscEnvelope, SIGNAL(pointAdded(const QPointF &)), 0, 0);
        disconnect(&oscEnvelope, SIGNAL(pointRemoved(int)), 0, 0);
        disconnect(&oscEnvelope, SIGNAL(pointUpdated(int, const QPointF&)), 0, 0);
        disconnect(kickOscillator, SIGNAL(oscAmplitudeValueUpdated(double)), this, 0);
        disconnect(kickOscillator, SIGNAL(oscFrequencyValueUpdated(double)), this, 0);
}

OscillatorWidget::~OscillatorWidget()
{

}

void OscillatorWidget::paintEvent(QPaintEvent *event)
{
        Q_UNUSED(event);
        recalculateOrigin();
        calculateRatio();
        drawAxes();
        drawEnvelope();
}

void OscillatorWidget::drawEnvelope(void)
{
	QPainter painter(this);
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
        Q_UNUSED(event);
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

	oscEnvelope.moveSelectedPoint(event->x() - xPadding,
				      height() - (event->y() + yPadding));
	mousePoint.setX(event->x());
	mousePoint.setY(event->y());
	update();
}

void OscillatorWidget::recalculateOrigin(void)
{
	originPoint.setX(xPadding);
	originPoint.setY((height() - yPadding));
	oscEnvelope.setOrigin(originPoint);
}

void OscillatorWidget::resizeEvent(QResizeEvent *event)
{
        Q_UNUSED(event);
}

void OscillatorWidget::setOscillator(GKickOscillator *osc)
{
        disconnectOscillator();
        kickOscillator = osc;
        kickLength = kickOscillator->getKickLength();
        connectOscillator();
        update();
}

void OscillatorWidget::setAmplitudeEnvelope(void)
{
        kickOscillator->setCurrentEnvelope(GKickOscillator::OSC_ENV_AMPLITUDE);
        oscEnvelope.removePoints();
        oscEnvelope.setType(OscillatorEnvelope::OSC_ENV_AMPLITUDE);
        oscEnvelope.setEnvelopeValue(kickOscillator->getOscAmplitudeValue());
        oscEnvelope.addEnvelopePoints(kickOscillator->getEnvelopePoints());
        update();
}

void OscillatorWidget::setFrequencyEnvelope(void)
{
        kickOscillator->setCurrentEnvelope(GKickOscillator::OSC_ENV_FREQUENCY);
        oscEnvelope.removePoints();
        oscEnvelope.setType(OscillatorEnvelope::OSC_ENV_FREQUENCY);
        oscEnvelope.setEnvelopeValue(kickOscillator->getOscFrequencyValue());
        oscEnvelope.addEnvelopePoints(kickOscillator->getEnvelopePoints());
        update();
}

void OscillatorWidget::updateKickLength(double v)
{
        kickLength = v;
        update();
}

double OscillatorWidget::getKickLength(void)
{
        return kickLength;
}

void OscillatorWidget::setEnvelopeAmplitudeValue(double v)
{
	if (oscEnvelope.getType() == OscillatorEnvelope::OSC_ENV_AMPLITUDE) {
		oscEnvelope.setEnvelopeValue(v);
		update();
	}
}

void OscillatorWidget::setEnvelopeFrequencyValue(double v)
{
	if (oscEnvelope.getType() == OscillatorEnvelope::OSC_ENV_FREQUENCY) {
		oscEnvelope.setEnvelopeValue(v);
		update();
	}
}
