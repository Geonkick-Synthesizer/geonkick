#ifndef OSCILLATOR_WIDGET_H
#define OSCILLATOR_WIDGET_H

#include "gkick_oscillator.h"
#include "oscillator_envelope.h"

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class GKickEnvelopeWidget : public QWidget
{
   Q_OBJECT
public:

   enum EnvelopeType {
           ENV_OSC_1,
           ENV_OSC_2,
           ENV_NOISE,
           ENV_GENERAL
   };

   GKickEnvelopeWidget::GKickEnvelopeWidget(QWidget *parent,
                                            std::shared_ptr<GKickApi> &api,
                                            std::vector<std::shared_ptr<GKickOscillator>> &oscillators);
   ~GKickEnvelopeWidget();

   void setOscillator(GKickOscillator *osc);
   double getKickLength(void);

   public slots:
           void viewEnvelope(EnvelopeType type);
	   /*void setAmplitudeEnvelope(void);
	   void setFrequencyEnvelope(void);
	   void updateKickLength(double v);
	   void setEnvelopeAmplitudeValue(double v);
	   void setEnvelopeFrequencyValue(double v);*/

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
     double kickLength;
};

#endif
