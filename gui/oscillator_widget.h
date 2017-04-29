#ifndef OSCILLATOR_WIDGET_H
#define OSCILLATOR_WIDGET_H

#include "gkickoscillator.h"
#include "oscillator_envelope.h"

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class OscillatorWidget : public QWidget
{
   Q_OBJECT
public:
   OscillatorWidget(QWidget *parent, GKickOscillator *osc);
   ~OscillatorWidget();

   void setOscillator(GKickOscillator *osc);
   void setAmplitudeEnvelope(void);
   void setFrequencyEnvelope(void);

 protected:
     void drawAxes(void);
     void drawEnvelope(void);
     void paintEvent(QPaintEvent *event);
     void recalculateOrigin(void);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void resizeEvent(QResizeEvent *event);
     void connectOscillator(void);
     void disconnectOscillator(void);
     void calculateRatio(void);
     
 private:
     GKickOscillator* kickOscillator;
     QPainter widgetPainter;
     OscillatorEnvelope oscEnvelope;
     int xPadding;
     int yPadding;
     QPointF originPoint;
     QPointF mousePoint;
};

#endif
