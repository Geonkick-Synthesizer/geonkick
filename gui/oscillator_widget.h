#ifndef OSCILLATOR_WIDGET_H
#define OSCILLATOR_WIDGET_H

#include "envelope.h"

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class OscillatorWidget : public QWidget
{
   Q_OBJECT
public:
   OscillatorWidget(QWidget *parent);
   ~OscillatorWidget();

 protected:
     void drawAxes(void);
     void drawEnvelope(void);
     void paintEvent(QPaintEvent *event);
     void recalculateOrigin(void);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);

 private:
     QPainter widgetPainter;
     GKickEnvelope kickEnvelope;
     int xPadding;
     int yPadding;
     QPointF originPoint;
     QPointF mousePoint;
};

#endif
