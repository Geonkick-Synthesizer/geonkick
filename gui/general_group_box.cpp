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

#include "geonkick_api.h"
#include "general_group_box.h"
#include "knob.h"
#include "geonkick_label.h"
#include "geonkick_checkbox.h"
#include "geonkick_button.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

GeneralGroupBox::GeneralGroupBox(GeonkickWidget *parent, GeonkickApi *api)
        : GeonkickGroupBox(parent),
          geonkickApi(api),
          filterCheckbox(nullptr)
{

        auto label = new GeonkickLabel(this);
        label->setImage(QPixmap("./themes/geontime/general_groupbox_label.png"));
        setGroupBoxLabel(label, Qt::AlignLeft);
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
        addWidget(amplitudeEnvelopeBox);

        Knob *kickAmplitudeKnob = new Knob(amplitudeEnvelopeBox);
        kickAmplitudeKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickAmplitudeKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickAmplitudeKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickAmplitudeKnob->setRange(0.01, 1.0);
        kickAmplitudeKnob->setCurrentValue(geonkickApi->kickAmplitude());
        connect(kickAmplitudeKnob, SIGNAL(valueUpdated(double)),
        geonkickApi, SLOT(setKickAmplitude(double)));

        Knob *kickLengthKnob = new Knob(amplitudeEnvelopeBox);
        kickLengthKnob->setGeometry(224 / 2 + (224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickLengthKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickLengthKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickLengthKnob->setRange(50, geonkickApi->kickMaxLength());
        kickLengthKnob->setCurrentValue(geonkickApi->kickLength());
        connect(kickLengthKnob, SIGNAL(valueUpdated(double)), geonkickApi, SLOT(setKickLength(double)));
}

void GeneralGroupBox::createFilterHBox()
{
        auto filterEnvelopeBox = new GeonkickWidget(this);
        auto pixmap = QPixmap("./themes/geontime/hboxbk_filter.png");
        filterEnvelopeBox->setBackgroundImage(pixmap);
        filterEnvelopeBox->setFixedSize(pixmap.size().width(), pixmap.size().height());
        addWidget(filterEnvelopeBox);
        setWidgetAlignment(filterEnvelopeBox, Qt::AlignTop);

        filterCheckbox = new GeonkickCheckbox(filterEnvelopeBox);
        filterCheckbox->setCheckedImage("./themes/geontime/checkbox_checked.png");
        filterCheckbox->setUncheckedImage("./themes/geontime/checkbox_unchecked.png");
        filterCheckbox->move(10, 10);
        connect(filterCheckbox, SIGNAL(stateUpdated(bool)), geonkickApi, SLOT(enableKickFilter(bool)));

        Knob *kickFrequencyKnob = new Knob(filterEnvelopeBox);
        kickFrequencyKnob->setGeometry((224 / 2 - 80) / 2, (125 - 80) / 2,  80, 80);
        kickFrequencyKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_image.png"));
        kickFrequencyKnob->setKnobImage(QPixmap("./themes/geontime/knob.png"));
        kickFrequencyKnob->setRange(200, 20000);
        kickFrequencyKnob->setCurrentValue(geonkickApi->kickFilterFrequency());
        connect(kickFrequencyKnob, SIGNAL(valueUpdated(double)), geonkickApi, SLOT(setKickFilterFrequency(double)));

        Knob *kickQFactorKnob = new Knob(filterEnvelopeBox);
        pixmap = QPixmap("./themes/geontime/knob_bk_50x50.png");
        int w = pixmap.size().width();
        int h = pixmap.size().height();
        kickQFactorKnob->setGeometry(224 / 2  + (224 / 2 - w) / 2, (125 - h) / 4, w, h);
        kickQFactorKnob->setBackgroundImage(QPixmap("./themes/geontime/knob_bk_50x50.png"));
        kickQFactorKnob->setKnobImage(QPixmap("./themes/geontime/knob_50x50.png"));
        kickQFactorKnob->setRange(0.01, 10);
        connect(kickQFactorKnob, SIGNAL(valueUpdated(double)), geonkickApi, SLOT(setKickFilterQFactor(double)));

        auto filterType = new GeonkickButton(filterEnvelopeBox);
        connect(filterType, SIGNAL(toggled(bool)), this, SLOT(setFilterType(bool)));
        w = 80;
        h = 25;
        filterType->setGeometry(224 / 2 + (224 / 2 - w) / 2, 112 - 20, w, h);
        filterType->setPressedImage(QPixmap("./themes/geontime/filter_type_hp.png"));
        filterType->setUnpressedImage(QPixmap("./themes/geontime/filter_type_lp.png"));
}

void GeneralGroupBox::setFilterType(bool state)
{
        if (state) {
                geonkickApi->setKickFilterType(Oscillator::FilterType::LowPass);
        } else {
                geonkickApi->setKickFilterType(Oscillator::FilterType::HighPass);
        }
}

