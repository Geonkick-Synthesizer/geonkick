#include "RkLog.h"
#include "RkPlatform.h"

void rkLogMessage(RkLogLevel level, const std::string &func_name, const std::string &msg)
{
        std::string strLevel;

        switch (level)
        {
        case RkLogLevel::Debug:
                strLevel = "Debug";
        break;
        case RkLogLevel::Info:
                strLevel = "Info";
        break;
        case RkLogLevel::Warning:
                strLevel = "Warning";
        break;
        case RkLogLevel::Error:
                strLevel = "Error";
        break;
        default:
                strLevel = "Unknown";
        }

        OutputDebugString(("[" + strLevel + "] " + func_name + ": " + msg).c_str());
}

