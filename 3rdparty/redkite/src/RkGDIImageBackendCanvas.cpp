#include "RkGDIImageBackendCanvas.h"
#include "RkCanvasInfo.h"
#include "RkLog.h"
#include "RkPlatform.h"

RkGDIImageBackendCanvas::RkGDIImageBackendCanvas(const RkSize &size,
                                                     RkImage::Format format,
                                                     const unsigned char *data)
        : canvasInfo{nullptr}
        , imageSize{size}
{
        auto nBytes = imageSize.height() * imageSize.width() * pixelLength(RkImage::Format::ARGB32);
        if (nBytes > 0) {
            if (data == nullptr)
				imageData = std::vector<unsigned char>(nBytes, 0);
			else
				imageData.assign(data, data + nBytes);
        }
}

RkGDIImageBackendCanvas::~RkGDIImageBackendCanvas()
{
}

int RkGDIImageBackendCanvas::pixelLength(RkImage::Format format) const
{
        switch (format) {
        case RkImage::Format::ARGB32:
        case RkImage::Format::RGB32:  return 4;
        default: return 1;
        }
}

const RkSize& RkGDIImageBackendCanvas::size() const
{
        return imageSize;
}

bool RkGDIImageBackendCanvas::isNull() const
{
        return imageData.empty();
}

unsigned char* RkGDIImageBackendCanvas::data()
{
        return imageData.data();
}

std::vector<unsigned char> RkGDIImageBackendCanvas::dataCopy() const
{
        return imageData;
}

std::shared_ptr<RkCanvasInfo> RkGDIImageBackendCanvas::getCanvasInfo() const
{
        return canvasInfo;
}

void RkGDIImageBackendCanvas::fill(const RkColor &color)
{
}
