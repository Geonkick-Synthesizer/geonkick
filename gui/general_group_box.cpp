/**
 * File name: general_group_box.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor (http://geontime.com)
 *
 * This file is part of Geonkick.
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
#include "geonkick_label.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

GeneralGroupBox::GeneralGroupBox(GeonkickWidget *parent, std::shared_ptr<GKickApi> &api)
        : GeonkickGroupBox(parent),
          kickApi(api)
{

        GeonkickLabel *label = getGroupBoxLabel();
        label->setImage(QPixmap("./themes/geontime/general_groupbox_label.png"));
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
        layout()->setAlignment(amplitudeEnvelopeBox, Qt::AlignTop);

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
        GeonkickWidget *filterEnvelopeBox = new GeonkickWidget(this);
        QPixmap pixmap("./themes/geontime/hboxbk_filter.png");
        filterEnvelopeBox->setFixedSize(pixmap.size().width(), pixmap.size().height());
        filterEnvelopeBox->setBackgroundImage(pixmap);
        layout()->addWidget(filterEnvelopeBox);
        layout()->setAlignment(filterEnvelopeBox, Qt::AlignTop);

        GKickKnob *kickFrequencyKnob = new GKickKnob(filterEnvelopeBox);
        kickFrequencyKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickFrequencyKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickFrequencyKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));

        GKickKnob *kickQFactorKnob = new GKickKnob(filterEnvelopeBox);
        pixmap = QPixmap("./themes/geontime/knob_bk_50x50.png");
        int w = pixmap.size().width();
        int h = pixmap.size().height();
        kickQFactorKnob->setGeometry(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4, w, h);
        kickQFactorKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_50x50.png"));
        kickQFactorKnob->setKnobImage(QPixmap("./themes/geontime/knob_50x50.png"));
        //kickQFactorKnob->setMaxValue(kickApi->getMaxLength());
        //kickQFactorKnob->setCurrentValue(kickApi->getKickLength());

        auto filterType = new GeonkickWidget(filterEnvelopeBox);
        w = 80;
        h = 25;
        filterType->setGeometry(224 / 2 + (224 / 2 - w) / 2, 112 - 20, w, h);
        filterType->setBackgroundImage(QPixmap("./themes/geontime/filter_type_hp.png"));
}
