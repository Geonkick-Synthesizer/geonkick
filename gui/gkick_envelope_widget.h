#ifndef OSCILLATOR_WIDGET_H
#define OSCILLATOR_WIDGET_H

#include "gkick_oscillator.h"
#include "oscillator_envelope.h"

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class EnvelopeDrawingArea;

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

   GKickEnvelopeWidget(QWidget *parent,
                       std::shared_ptr<GKickApi> &api,
                       std::vector<std::shared_ptr<GKickOscillator>> &oscillators);
   ~GKickEnvelopeWidget();

 protected:
     void paintEvent(QPaintEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);

 private:
     std::shared_ptr<GKickEnvelope> currentEnvelope;
     std::vector<std::shared_ptr<GKickEnvelope>> envelopes;
     QLabel *envelopeTitleLabel;
     EnvelopeDrawingArea *drawArea;
     QPushButton *showAmplitudeEnvButton;
     QPushButton *showFrequencyEnvButton;

     int xPadding;
     int yPadding;
     QPointF originPoint;
     QPointF mousePoint;
};

#endif
