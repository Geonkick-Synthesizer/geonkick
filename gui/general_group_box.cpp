/**
 * File name: oscillator_group_box.cpp
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of GeonKick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "general_group_box.h"
#include "gkick_knob.h"
#include "gkickapi.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

GeneralGroupBox::GeneralGroupBox(GeonkickWidget *parent, std::shared_ptr<GKickApi> &api)
        : GeonkickGroupBox(tr("General Settings"), parent),
          kickApi(api)
{

        createAplitudeEnvelopeHBox();
        createFilterHBox();
}

GeneralGroupBox::~GeneralGroupBox()
{
}

void GeneralGroupBox::createAplitudeEnvelopeHBox()
{
        GeonkickWidget *amplitudeEnvelopeBox = new GeonkickWidget(this);
        amplitudeEnvelopeBox->setFixedSize(224, 125);
        amplitudeEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_ampl_env.png"));
        layout()->addWidget(amplitudeEnvelopeBox);

        GKickKnob *kickAmplitudeKnob = new GKickKnob(amplitudeEnvelopeBox);
        kickAmplitudeKnob->setGeometry(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickAmplitudeKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickAmplitudeKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickAmplitudeKnob->setMaxValue(1);
        kickAmplitudeKnob->setCurrentValue(kickApi->getAmplitude());
        connect(kickAmplitudeKnob, SIGNAL(valueUpdated(double)),
        kickApi.get(), SLOT(setAmplitude(double)));

        GKickKnob *kickLengthKnob = new GKickKnob(amplitudeEnvelopeBox);
        kickLengthKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickLengthKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickLengthKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickLengthKnob->setMaxValue(kickApi->getMaxLength());
        kickLengthKnob->setCurrentValue(kickApi->getKickLength());
        connect(kickLengthKnob, SIGNAL(valueUpdated(double)),
                kickApi.get(), SLOT(setKickLengthValue(double)));

}

void GeneralGroupBox::createFilterHBox()
{
        /*        GeonkickWidget *filterEnvelopeBox = new GeonkickWidget(this);
        filterEnvelopeBox->setFixedSize(224, 125);
        filterEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_filter.png"));
        layout()->addWidget(filterEnvelopeBox);
        QHBoxLayout *filterEnvelopeBoxLayout  = new QHBoxLayout(filterEnvelopeBox);
        filterEnvelopeBoxLayout->setContentsMargins(0, 0, 0, 0);
        filterEnvelopeBoxLayout->setSpacing(0);
        filterEnvelopeBox->setLayout(filterEnvelopeBoxLayout);

        GKickKnob *kickFrequencyKnob = new GKickKnob(filterEnvelopeBox);
        kickFrequencyKnob->setFixedSize(80, 80);
        kickFrequencyKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickFrequencyKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickFrequencyKnob->setMaxValue(kickApi->getMaxLength());
        kickFrequencyKnob->setCurrentValue(kickApi->getKickLength());

        filterEnvelopeBox->layout()->addWidget(kickFrequencyKnob);
        //       connect(kickFrequencyKnob, SIGNAL(valueUpdated(double)),
        //        kickApi.get(), SLOT(setFilterLengthValue(double)));

        QVBoxLayout *filterLeftSideLayout = new QVBoxLayout(filterEnvelopeBox);
        filterEnvelopeBoxLayout->addLayout(filterLeftSideLayout);

        GKickKnob *kickQFactorKnob = new GKickKnob(filterEnvelopeBox);
        kickQFactorKnob->setFixedSize(67, 67);
        kickQFactorKnob->setBackgroundImage(QPixmap("./themes/geontime/kbob_bk_lter_q.png"));
        kickQFactorKnob->setKnobImage(QPixmap("./themes/geontime/knob_q.png"));
        kickQFactorKnob->setMaxValue(kickApi->getMaxLength());
        kickQFactorKnob->setCurrentValue(kickApi->getKickLength());
        filterLeftSideLayout->addWidget(kickQFactorKnob);

        //       filterTypeCb = new QComboBox(filterEnvelopeBox);
        //        filterTypeCb->setGeometry((225 - 40) / 2 , 125 - 30, 40, 20);
        //filterTypeCb->addItem("Low Pass");
        //filterTypeCb->addItem("Hight Pass");
        //filterLeftSideLayout->addWidget(filterTypeCb);
        //       connect(kickFrequencyKnob, SIGNAL(valueUpdated(double)),
        //        kickApi.get(), SLOT(setFilterLengthValue(double)));

        */
        /*        GeonkickWidget *filterGroupBox = new GeonkickWidget(this);
        filterEnvelopeBox->setFixedSize(224, 125);
        filterEnvelopeBox->setBackgroundImage(QPixmap("./themes/geontime/hboxbk_filter.png"));
        layout()->addWidget(filterEnvelopeBox);

        QHBoxLayout *filterEnvelopeBoxLayout  = new QHBoxLayout(filterEnvelopeBox);
        amplitudeEnvelopeBoxLayout->setContentsMargins(0, 0, 0, 0);
        amplitudeEnvelopeBoxLayout->setSpacing(0);
        amplitudeEnvelopeBox->setLayout(amplitudeEnvelopeBoxLayout);

        GKickKnob *kickLengthKnob = new GKickKnob(amplitudeEnvelopeBox);
        kickLengthKnob->setFixedSize(224 / 2, 125);
        kickLengthKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image_ampl.png"));
        kickLengthKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickLengthKnob->setMaxValue(kickApi->getMaxLength());
        kickLengthKnob->setCurrentValue(kickApi->getKickLength());

        amplitudeEnvelopeBox->layout()->addWidget(kickLengthKnob);
        connect(kickLengthKnob, SIGNAL(valueUpdated(double)),
                kickApi.get(), SLOT(setKickLengthValue(double)));

        GKickKnob *kickAmplitudeKnob = new GKickKnob(this);
        kickLengthKnob->setFixedSize(224 / 2, 125);
        kickAmplitudeKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image_freq.png"));
        kickAmplitudeKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickAmplitudeKnob->setMaxValue(1);
        kickAmplitudeKnob->setCurrentValue(kickApi->getAmplitude());
        amplitudeEnvelopeBox->layout()->addWidget(kickAmplitudeKnob);
        connect(kickAmplitudeKnob, SIGNAL(valueUpdated(double)),
                kickApi.get(), SLOT(setAmplitude(double)));

        /*QGroupBox *filterGroupBox = new QGroupBox(tr("Filter"), this);
        QVBoxLayout *filterGroupBoxLayout = new QVBoxLayout();
        filterGroupBox->setLayout(filterGroupBoxLayout);
        layout()->addWidget(filterGroupBox);*/

        // Create filter type group box.
        /*QGroupBox *filterTypeGroupBox = new QGroupBox(tr("Type"), filterGroupBox);
        QHBoxLayout *filterTypeGroupBoxLayout = new QHBoxLayout(filterTypeGroupBox);
        filterTypeGroupBox->setLayout(filterTypeGroupBoxLayout);
        filterTypeCb = new QComboBox(filterTypeGroupBox);
        filterTypeCb->addItem(tr("Low pass"));
        filterTypeCb->addItem(tr("Hight pass"));
        filterTypeGroupBoxLayout->addWidget(filterTypeCb);
        filterGroupBoxLayout->addWidget(filterTypeGroupBox);

        // Create filter knobs gorup box.
        QGroupBox *filterKnobsGroupBox = new QGroupBox(filterGroupBox);
        QHBoxLayout *filterKnobsGroupBoxLayout = new QHBoxLayout(filterKnobsGroupBox);
        filterKnobsGroupBox->setLayout(filterKnobsGroupBoxLayout);
        GKickKnob *filterFrequencyKnob = new GKickKnob(filterKnobsGroupBox, tr("Frequency"));
        GKickKnob *filterQKnob = new GKickKnob(filterKnobsGroupBox, tr("Q"));
        filterFrequencyKnob->setMaxValue(20000);
        filterFrequencyKnob->setCurrentValue(kickApi->getKickFilterFrequency());
        filterQKnob->setMaxValue(10);
        filterQKnob->setCurrentValue(kickApi->getKickFilterQFactor());
        filterKnobsGroupBoxLayout->addWidget(filterFrequencyKnob);
        filterKnobsGroupBoxLayout->addWidget(filterQKnob);
        filterGroupBoxLayout->addWidget(filterKnobsGroupBox);
        layout()->addWidget(filterGroupBox);

        connect(filterFrequencyKnob, SIGNAL(valueUpdated(double)),
                kickApi.get(), SLOT(setKickFilterFrequency(double)));
        connect(filterQKnob, SIGNAL(valueUpdated(double)),
        kickApi.get(), SLOT(setKickFilterQFactor(double)));*/
}
