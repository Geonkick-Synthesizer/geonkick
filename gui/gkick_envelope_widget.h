#ifndef OSCILLATOR_WIDGET_H
#define OSCILLATOR_WIDGET_H

#include "gkick_oscillator.h"
#include "oscillator_envelope.h"

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

class EnvelopeDrawingArea;
class QLabel;
class QPushButton;

class GKickEnvelopeWidget : public QWidget
{
   Q_OBJECT
public:

   GKickEnvelopeWidget(QWidget *parent,
                       std::shared_ptr<GKickApi> &api,
                       std::vector<std::shared_ptr<GKickOscillator>> &oscillators);
   ~GKickEnvelopeWidget();

 protected:
     void updateButtonArea();
     void paintEvent(QPaintEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseDoubleClickEvent(QMouseEvent *event);
     void viewEnvelope(GKickEnvelope::EnvelopeType type);
     void showAmplitudeEnvelope();
     void showFrequencyEnvelope();

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
