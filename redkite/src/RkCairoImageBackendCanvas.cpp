
#include "RkCairoImageBackendCanvas.h"
#include "RkCanvasInfo.h"
#include "RkLog.h"

RkCairoImageBackendCanvas::RkCairoImageBackendCanvas(const RkSize &size,
                                                     RkImage::Format format,
                                                     const unsigned char *data)
        : canvasInfo{nullptr}
        , imageSize{size}
{
#ifdef RK_OS_WIN
#elif RK_OS_MAC
#else // X11
        auto cairoFormat = toCairoFormat(format);
        if (cairoFormat != CAIRO_FORMAT_INVALID && imageSize.width() > 0 && imageSize.height() > 0) {
                canvasInfo = std::make_unique<RkCanvasInfo>();
                auto stride = cairo_format_stride_for_width(cairoFormat, imageSize.width());
                if (data == nullptr)
                        imageData = std::vector<unsigned char>(imageSize.width() * imageSize.height() * pixelLength(format), 0);
                else
                        imageData.assign(data, data + imageSize.width() * imageSize.height() * pixelLength(format));
                canvasInfo->cairo_surface = cairo_image_surface_create_for_data(imageData.data(),
                                                                                CAIRO_FORMAT_ARGB32,
                                                                                imageSize.width(),
                                                                                imageSize.height(),
                                                                                stride);
        }
#endif
}

RkCairoImageBackendCanvas::~RkCairoImageBackendCanvas()
{
        if (canvasInfo)
                cairo_surface_destroy(canvasInfo->cairo_surface);
}

cairo_format_t RkCairoImageBackendCanvas::toCairoFormat(RkImage::Format format) const
{
        switch (format)
        {
        case RkImage::Format::ARGB32: return CAIRO_FORMAT_ARGB32;
        case RkImage::Format::RGB32:  return CAIRO_FORMAT_RGB24;
        default: return CAIRO_FORMAT_INVALID;
        }
}

int RkCairoImageBackendCanvas::pixelLength(RkImage::Format format) const
{
        switch (format)
        {
        case RkImage::Format::ARGB32:
        case RkImage::Format::RGB32:  return 4;
        default: return 1;
        }
}

const RkSize& RkCairoImageBackendCanvas::size() const
{
        return imageSize;
}

bool RkCairoImageBackendCanvas::isNull() const
{
        return canvasInfo == nullptr;
}

unsigned char* RkCairoImageBackendCanvas::data()
{
        return imageData.data();
}

std::vector<unsigned char> RkCairoImageBackendCanvas::dataCopy() const
{
        return imageData;
}

RkCanvasInfo* RkCairoImageBackendCanvas::getCanvasInfo() const
{
        return canvasInfo.get();
}

void RkCairoImageBackendCanvas::fill(const RkColor &color)
{
        unsigned char *data = imageData.data();
        for (decltype(imageData.size()) i = 0; i < imageData.size() - 4; i += 4) {
                *(data + i)     = color.red();
                *(data + i + 1) = color.green();
                *(data + i + 2) = color.blue();
                *(data + i + 3) = color.alpha();
        }
}
