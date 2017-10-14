#include "gkick_envelope_widget.h"
#include "general_envelope.h"
#include "envelope_draw_area.h"

#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <QVBoxLayout>

GKickEnvelopeWidget::GKickEnvelopeWidget(QWidget *parent,
                                         std::shared_ptr<GKickApi> &api,
                                         std::vector<std::shared_ptr<GKickOscillator>> &oscillators)
	: QWidget(parent),
	  envelopes(4),
          envelopeTitleLabel(NULL),
          drawArea(NULL),
          showAmplitudeEnvButton(NULL);
          showFrequencyEnvButton(NULL);

{
        envelopes[GKickEnvelopeWidget::ENV_OSC_1] =
                std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_1]);
        envelopes[GKickEnvelopeWidget::ENV_OSC_2] =
                std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_2]);
        envelopes[GKickEnvelopeWidget::ENV_NOISE] =
                std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_NOISE]);
        envelopes[GKickEnvelopeWidget::ENV_GENERAL] =
                std::make_shared<GeneralEnvelope>(api);

        currentEnvelope = envelopes[GKickEnvelopeWidget::ENV_GENERAL];

        setLayout(new QVBoxLayout(this));

        // Create top area.
        envelopeTitleLabel = new QLabel(currentEnvelope->name(), this);
        layout()->addWidget(envelopeTitleLabel);

        // Create drawing area.
        drawArea = new EnvelopeDrawingArea(this, currentEnvelope);
        layout()->addWidget(drawArea);

        // Create bottom area.
        showAmplitudeEnvButton = new QPushButton(tr("AMPL"), this);
        showFrequencyEnvButton = new QPushButton(tr("FREQ"), this);
        connect(showAmplitudeEnvButton, SIGNAL(checked()), this, SLOT(showAmplitudeEnvelope()));
        connect(showFrequencyEnvButton, SIGNAL(checked()), this, SLOT(showFrequencyEnvelope()));
        QHLayout *buttomAreaLayout = new QHLayout(this);
        buttomAreaLayout->addWidget(showAmplitudeEnvButton);
        buttomAreaLayout->addWidget(showFrequencyEnvButton);
        layout()->addLayout(buttomAreaLayout);
        updateButtonArea();
}

GKickEnvelopeWidget::~GKickEnvelopeWidget()
{

}

void GKickEnvelopeWidget::updateButtonArea()
{
        if (currentEnvelope->type() == OSC_1
            || currentEnvelope->type() == OSC_2)  {
                showAmplitudeEnvButton->setVisible(true);
                showFrequencyEnvButton->setVisible(true);
        } else {
                showAmplitudeEnvButton->setVisible(false);
                showFrequencyEnvButton->setVisible(false);
        }

        if (currentEnvelope->envelopeType() == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                showAmplitudeEnvButton->setDown(true);
                showFrequencyEnvButton->setDown(false);
        } else {
                showAmplitudeEnvButton->setDown(false);
                showFrequencyEnvButton->setDown(true);
        }
}

void GKickEnvelopeWidget::viewEnvelope(EnvelopeType type)
{
        currentEnvelope = envelopes[type];
        envelopeTitleLabel->setText(currentEnvelope->name());
        drawArea->setEnvelope(currentEnvelope);
        updateButtonArea();
        drawArea->update();
}

void GKickEnvelopeWidget::showAmplitudeEnvelope()
{
       currentEnvelope->setEnvelopeType(GKickEnvelope::ENV_APLITUDE);
       drawArea->update();
}

void GKickEnvelopeWidget::showFrequencyEnvelope()
{
        currentEnvelope->setEnvelopeType(GKickEnvelope::ENV_FRQUENCY);
        drawArea->update();
}

