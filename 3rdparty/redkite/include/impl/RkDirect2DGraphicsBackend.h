/**
 * File name: RkDirect2DGraphicsBackend.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor (http://quamplex.com/redkite)
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#ifndef RK_DIRECT2D_GRAPHICS_BACKEND_H
#define RK_DIRECT2D_GRAPHICS_BACKEND_H

#include "RkGraphicsBackend.h"

struct ID2D1DeviceContext;
struct ID2D1SolidColorBrush;
struct ID2D1StrokeStyle;
struct IDXGISwapChain1;
struct ID2D1Bitmap1;
struct IDXGIAdapter;
struct IDXGIFactory2;
struct ID3D11Texture2D;
struct IDXGISurface;
struct IDWriteTextFormat;

class RkDirect2DGraphicsBackend final : public RkGraphicsBackend {
 public:
        RkDirect2DGraphicsBackend(RkCanvas* canvas);
        ~RkDirect2DGraphicsBackend();
        void drawText(const std::string &text, int x, int y);
        void drawImage(const std::string &file, int x, int y);
        void drawImage(const RkImage &image, int x, int y);
        void drawEllipse(const RkPoint& p, int width, int height);
        void drawLine(const RkPoint &p1, const RkPoint &p2);
        void drawRect(const RkRect &rect);
        void drawPolyLine(const std::vector<RkPoint> &points);
        void fillRect(const RkRect &rect, const RkColor &color);
        void setPen(const RkPen &pen);
        void setFont(const RkFont &font);
        int getTextWidth(const std::string &text) const final;
        void translate(const RkPoint &offset);
        void rotate(rk_real angle);

 protected:
	 void releaseContextResources();
	 bool validateCanvas(RkCanvas *canvas) const;
	 bool prepareContext(RkCanvas *canvas);

 private:
        ID2D1DeviceContext *deviceContext;
        IDXGISwapChain1 *swapChain;
        ID2D1Bitmap1  *d2dTargetBitmap;
        ID2D1SolidColorBrush *targetBrush;
		IDXGIAdapter *dxgiAdapter;
		IDXGIFactory2 *dxgiFactory;
		ID3D11Texture2D *backBuffer;
		IDXGISurface *dxgiBackBuffer;
        float strokeWidth;
        ID2D1StrokeStyle *strokeStyle;
        IDWriteTextFormat *textFormat;
};

#endif // RK_DIRECT2D_GRAPHICS_BACKEND_H
