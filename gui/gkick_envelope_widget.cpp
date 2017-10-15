#include "gkick_envelope_widget.h"
#include "general_envelope.h"
#include "envelope_draw_area.h"

#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

GKickEnvelopeWidget::GKickEnvelopeWidget(QWidget *parent,
                                         std::shared_ptr<GKickApi> &api,
                                         std::vector<std::shared_ptr<GKickOscillator>> &oscillators)
	: QWidget(parent),
	  envelopes(5),
          envelopeTitleLabel(NULL),
          drawArea(NULL),
          showAmplitudeEnvButton(NULL),
          showFrequencyEnvButton(NULL)

{
        envelopes[GKickEnvelope::ENV_CATEGORY_OSC_1] =
                std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_1]);
        envelopes[GKickEnvelope::ENV_CATEGORY_OSC_2] =
                std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_2]);
        envelopes[GKickEnvelope::ENV_CATEGORY_NOISE] =
                std::make_shared<OscillatorEnvelope>(oscillators[GKickOscillator::OSC_NOISE]);
        envelopes[GKickEnvelope::ENV_CATEGORY_GENERAL] =
                std::make_shared<GeneralEnvelope>(api);

        currentEnvelope = envelopes[GKickEnvelope::ENV_CATEGORY_GENERAL];

        //        QVBoxLayout* vBoxLayout = ;
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
        QHBoxLayout *buttomAreaLayout = new QHBoxLayout(this);
        buttomAreaLayout->addWidget(showAmplitudeEnvButton);
        buttomAreaLayout->addWidget(showFrequencyEnvButton);
        static_cast<QVBoxLayout*>(layout())->addLayout(buttomAreaLayout);
        updateButtonArea();
}

GKickEnvelopeWidget::~GKickEnvelopeWidget()
{

}

void GKickEnvelopeWidget::updateButtonArea()
{
        if (currentEnvelope->category() == GKickEnvelope::ENV_CATEGORY_OSC_1
            || currentEnvelope->category() == GKickEnvelope::ENV_CATEGORY_OSC_2)  {
                showAmplitudeEnvButton->setVisible(true);
                showFrequencyEnvButton->setVisible(true);
        } else {
                showAmplitudeEnvButton->setVisible(false);
                showFrequencyEnvButton->setVisible(false);
        }

        if (currentEnvelope->type() == GKickEnvelope::ENV_TYPE_AMPLITUDE) {
                showAmplitudeEnvButton->setDown(true);
                showFrequencyEnvButton->setDown(false);
        } else {
                showAmplitudeEnvButton->setDown(false);
                showFrequencyEnvButton->setDown(true);
        }
}

void GKickEnvelopeWidget::viewEnvelope(GKickEnvelope::EnvelopeType type)
{
        currentEnvelope = envelopes[type];
        envelopeTitleLabel->setText(currentEnvelope->name());
        drawArea->setEnvelope(currentEnvelope);
        updateButtonArea();
        drawArea->update();
}

void GKickEnvelopeWidget::showAmplitudeEnvelope()
{
       currentEnvelope->setType(GKickEnvelope::ENV_TYPE_AMPLITUDE);
       drawArea->update();
}

void GKickEnvelopeWidget::showFrequencyEnvelope()
{
        currentEnvelope->setType(GKickEnvelope::ENV_TYPE_FREQUENCY);
        drawArea->update();
}

