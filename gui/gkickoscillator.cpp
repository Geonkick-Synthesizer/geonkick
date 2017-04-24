/**
 * File name: gkickoscillator.cpp
 * Project: GeonKick
 *
 * Copyright (C) 2015 Iurie Nistor
 * Email: nistor@iurie.org
 *
 * This file is part of GeonKick.
 *
 * waveToMp3 is free software; you can redistribute it and/or modify
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

#include "gkickoscillator.h"

#include <QDebug>

GKickOscillator::GKickOscillator(GKickApi *api, int index) :
  kickApi(api),
  oscillatorIndex(index),
  envelopeIndex(0)
{
    qDebug() << "GKickOscillator::GKickOscillator()";
}

GKickOscillator::~GKickOscillator()
{
  qDebug() << "GKickOscillator::~GKickOscillator()";
}

//void GKickOscillator::setApi(GKickApi *api)
//{/
//  kickApi = api;
//}

QPolygonF GKickOscillator::getEnvelopePoints(void)
{
  QPolygonF points;
  
  if (kickApi) {
    points =  kickApi->getOscEvelopePoints(oscillatorIndex, envelopeIndex);
  }

  return points;
}

void GKickOscillator::addPoint(const QPointF &point)
{
  if (kickApi) {
    kickApi->addOscEnvelopePoint(oscillatorIndex, envelopeIndex, point);
  }
}

void GKickOscillator::removePoint(int index)
{
  if (kickApi) {
    kickApi->removeOscEvelopePoint(oscillatorIndex, envelopeIndex, index);
  }
}

void GKickOscillator::updatePoint(int index, const QPointF &point)
{
  if (kickApi) {
    kickApi->updateOscEvelopePoint(oscillatorIndex, envelopeIndex, index, point);
  }
}

void GKickOscillator::setOscillatorIndex(int index)
{
  oscillatorIndex = index;
}

int GKickOscillator::getOscillatorIndex(void)
{
  return oscillatorIndex;
}
