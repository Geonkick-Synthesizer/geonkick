/**
 * File name: envelope_draw_area.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
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

#include "envelope_draw_area.h"
#include "envelope.h"
#include "kick_graph.h"
#include "EnvelopePointContextWidget.h"

#include <RkPainter.h>
#include <RkEvent.h>

EnvelopeWidgetDrawingArea::EnvelopeWidgetDrawingArea(GeonkickWidget *parent, GeonkickApi *api)
          : GeonkickWidget(parent)
          , geonkickApi{api}
          , currentEnvelope{nullptr}
          , hideEnvelope{false}
          , kickGraphImage{nullptr}
          , kickGraphics{nullptr}
          , pointEditingMode{false}
{
        setFixedSize(850, 300);
        int padding = 50;
        drawingArea = RkRect(1.1 * padding, padding / 2, width() - 1.5 * padding, height() - 1.2 * padding);
        setBackgroundColor(40, 40, 40);
        kickGraphics = new KickGraph(this, geonkickApi, drawingArea.size());
        RK_ACT_BIND(kickGraphics,
                    graphUpdated,
                    RK_ACT_ARGS(std::shared_ptr<RkImage> graphImage),
                    this, updateKickGraph(graphImage));
}

void EnvelopeWidgetDrawingArea::setEnvelope(Envelope* envelope)
{
        if (envelope) {
                currentEnvelope = envelope;
                envelopeUpdated();
        }
}

void EnvelopeWidgetDrawingArea::paintWidget([[maybe_unused]] RkPaintEvent *event)
{
        if (width() != envelopeImage.width() || height() != envelopeImage.height()) {
                RkImage im(size());
                envelopeImage = im;
        }

        RkPainter painter(&envelopeImage);
        painter.fillRect(rect(), background());

        if (kickGraphImage && !kickGraphImage->isNull())
                painter.drawImage(*kickGraphImage.get(), drawingArea.topLeft().x(), drawingArea.topLeft().y());
        else
                kickGraphics->updateGraphBuffer();

        if (currentEnvelope)
                currentEnvelope->draw(painter, Envelope::DrawLayer::Axies);

        if (currentEnvelope && !isHideEnvelope())
                currentEnvelope->draw(painter, Envelope::DrawLayer::Envelope);

        painter.drawText(180, height() - 12, getEnvStateText());
        auto pen = painter.pen();
        pen.setWidth(1);
        pen.setColor({20, 20, 20, 255});
        painter.setPen(pen);
        painter.drawRect({0, 0, width() - 1, height() - 1});

        RkPainter paint(this);
        paint.drawImage(envelopeImage, 0, 0);
}

std::string EnvelopeWidgetDrawingArea::getEnvStateText() const
{
        std::string str = "L" + std::to_string(static_cast<int>(geonkickApi->layer()) + 1) + " / ";
        if (currentEnvelope->category() == Envelope::Category::Oscillator1)
                str += "OSC1";
        else if (currentEnvelope->category() == Envelope::Category::Oscillator2)
                str += "OSC2";
        else if (currentEnvelope->category() == Envelope::Category::Noise)
                str += "NOISE";
        else
                str += "GEN";

        str += " / ";
        if (currentEnvelope->type() == Envelope::Type::Amplitude)
                str += "AENV";
        else if (currentEnvelope->type() == Envelope::Type::Frequency)
                str += "FENV";
        else if (currentEnvelope->type() == Envelope::Type::DistortionDrive)
                str += "DIST / DRIVE";
        else if (currentEnvelope->type() == Envelope::Type::DistortionVolume)
                str += "DIST / VOL";
        else
                str += "CFENV";

        return str;
}

void EnvelopeWidgetDrawingArea::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() != RkMouseEvent::ButtonType::Right
            && event->button() != RkMouseEvent::ButtonType::Left)
                return;

        RkPoint point(event->x() - drawingArea.left(),
                      drawingArea.bottom() - event->y());
        if (event->button() == RkMouseEvent::ButtonType::Right) {
                if (currentEnvelope) {
                        currentEnvelope->removePoint(point);
                        envelopeUpdated();
                }
        } else {
                mousePoint.setX(event->x());
                mousePoint.setY(event->y());
                if (currentEnvelope) {
                        currentEnvelope->selectPoint(point);
                        if (currentEnvelope->hasSelected())
                                envelopeUpdated();
                }
        }
        setFocus(true);
}

void EnvelopeWidgetDrawingArea::mouseButtonReleaseEvent(RkMouseEvent *event)
{
        if (!currentEnvelope)
                return;

        auto toUpdate = false;
        if (currentEnvelope->hasSelected()) {
                currentEnvelope->unselectPoint();
                toUpdate = true;
        }

        if (currentEnvelope->hasOverPoint())
                toUpdate = true;

        if (toUpdate)
                envelopeUpdated();
}

void EnvelopeWidgetDrawingArea::mouseDoubleClickEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::Left) {
                if (!currentEnvelope)
                        return;
                RkPoint point(event->x() - drawingArea.left(), drawingArea.bottom() - event->y());
                if (pointEditingMode && currentEnvelope->hasOverPoint()) {
                        auto act = std::make_unique<RkAction>();
                        int x = event->x();
                        int y = event->y();
                        auto topWidget = dynamic_cast<GeonkickWidget*>(RkWidget::getTopWidget());
                        act->setCallback([&, x, y, topWidget](void){
                                auto widget = new EnvelopePointContextWidget(currentEnvelope,
                                                                             topWidget);
                                widget->setPosition({x, y + 40});
                                widget->show();
                        });
                        eventQueue()->postAction(std::move(act));
                } else {
                        currentEnvelope->addPoint(point);
                        currentEnvelope->selectPoint(point);
                        envelopeUpdated();
                }
        }
}

void EnvelopeWidgetDrawingArea::mouseMoveEvent(RkMouseEvent *event)
{
        if (!currentEnvelope)
                return;

        RkPoint point(event->x() - drawingArea.left(), drawingArea.bottom() - event->y());
        if (currentEnvelope->hasSelected()) {
                currentEnvelope->moveSelectedPoint(point.x(), point.y());
                mousePoint.setX(event->x());
                mousePoint.setY(event->y());
                envelopeUpdated();
		return;
        }

	auto overPoint = currentEnvelope->hasOverPoint();
        currentEnvelope->overPoint(point);
	if (overPoint != currentEnvelope->hasOverPoint())
                envelopeUpdated();
}

void EnvelopeWidgetDrawingArea::envelopeUpdated()
{
        action isOverPoint(currentEnvelope->getCurrentPointInfo());
        update();
}

Envelope* EnvelopeWidgetDrawingArea::getEnvelope() const
{
        return currentEnvelope;
}

const RkRect EnvelopeWidgetDrawingArea::getDrawingArea()
{
        return drawingArea;
}

void EnvelopeWidgetDrawingArea::updateKickGraph(const std::shared_ptr<RkImage> &graphImage)
{
        kickGraphImage = graphImage;
        update();
}

bool EnvelopeWidgetDrawingArea::isHideEnvelope() const
{
        return hideEnvelope;
}

void EnvelopeWidgetDrawingArea::setHideEnvelope(bool b)
{
        if (hideEnvelope != b) {
                hideEnvelope = b;
                update();
        }
}

void EnvelopeWidgetDrawingArea::setPointEditingMode(bool b)
{
        pointEditingMode = b;
}

