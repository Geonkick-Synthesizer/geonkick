/**
 * File name: main.cpp
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

#include "mainwindow.h"
#include "geonkick_state.h"

#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QVector>
#include <QFontDatabase>

static std::shared_ptr<GeonkickState> getDefaultState()
{
        std::shared_ptr<GeonkickState> state = std::make_shared<GeonkickState>();
        state->setLimiterValue(1.0);
        state->setKickLength(300);
        state->setKickAmplitude(1.0);
        state->enableKickFilter(false);
        state->setKickFilterFrequency(500);
        state->setKickFilterQFactor(1.0);
        state->setKickFilterType(GeonkickApi::FilterType::LowPass);
        QPolygonF envelope;
        envelope << QPointF(0, 1);
        envelope << QPointF(1, 1);
        state->setKickEnvelopePoints(envelope);

        std::vector<GeonkickApi::OscillatorType> oscillators = {
                GeonkickApi::OscillatorType::Oscillator1,
                GeonkickApi::OscillatorType::Oscillator2,
                GeonkickApi::OscillatorType::Noise,
        };

        for (auto const &osc: oscillators) {
                int index = static_cast<int>(osc);
                if (osc == GeonkickApi::OscillatorType::Oscillator1) {
                        state->setOscillatorEnabled(index, true);
                } else {
                        state->setOscillatorEnabled(index, false);
                }
                state->setOscillatorFunction(index, GeonkickApi::FunctionType::Sine);
                state->setOscillatorAmplitue(index, 1);
                state->setOscillatorFrequency(index, 5000);
                state->setOscillatorFilterEnabled(index, false);
                state->setOscillatorFilterType(index, GeonkickApi::FilterType::LowPass);
                state->setOscillatorFilterCutOffFreq(index, 5000);
                state->setOscillatorFilterFactor(index, 1);
                state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Amplitude);
                if (osc != GeonkickApi::OscillatorType::Noise) {
                        state->setOscillatorEnvelopePoints(index, envelope, GeonkickApi::EnvelopeType::Frequency);
                }
        }

        return state;
}

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

        QFontDatabase::addApplicationFont("./themes/geontime/urw_gothic_l_book.ttf");
        QFont font("URW Gothic L");
        font.setPixelSize(12);
        a.setFont(font);

        auto api = std::make_unique<GeonkickApi>();
        if (!api->init()) {
                GEONKICK_LOG_ERROR("can't init API");
                exit(1);
        } else {
                api->setState(getDefaultState());
        }

        MainWindow window(api.get());
        if (!window.init()) {
                qDebug() << "can't init main window";
                exit(1);
        }

        window.show();
        return a.exec();
}
