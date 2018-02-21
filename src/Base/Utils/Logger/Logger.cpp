#include "Logger.h"
#include "Utils/StringUtils.h"
#include "Utils/Defines.h"

//#include "Utils/EnumToString.h"

#include <iostream>
#include <windows.h> // WinApi header

#ifdef _FINAL
#define LOG_ERROR_APPLICATION( x, ... ){}
#define LOG_WARNING_APPLICATION( x, ...  ){}
#define LOG_INFO_APPLICATION( x, ...  ){}
#define LOG_ERROR_APPLICATION_IF( condition, x, ... ){}
#else 
#define LOG_ERROR_APPLICATION( x, ... )     base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogError,   x, __VA_ARGS__ )
#define LOG_WARNING_APPLICATION( x, ...  )  base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogWarning, x, __VA_ARGS__ )
#define LOG_INFO_APPLICATION( x, ...  )     base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogInfo,    x, __VA_ARGS__ )
#define LOG_ERROR_APPLICATION_IF( condition, x, ... ) if(!condition) base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogError,   x, __VA_ARGS__ )
#endif


Begin_Enum_String(base::utils::CLogger::ELogLevel)
{
	Enum_String_Id(base::utils::CLogger::eLogInfo, "Info");
	Enum_String_Id(base::utils::CLogger::eLogWarning, "Warning");
	Enum_String_Id(base::utils::CLogger::eLogError, "Error");
}
End_Enum_String;

namespace base{
    namespace utils{

        

        CLogger::CLogger()
        {
        }

        CLogger::~CLogger()
        {
        }

        void CLogger::AddNewLog(ELogLevel aLevel, const char* format, ...)
        {
            va_list args;
            char* buffer;
            va_start(args, format);
            int len = _vscprintf(format, args) + 1;
            buffer = (char*)malloc(len * sizeof(char));
            vsprintf_s(buffer, len, format, args);
            std::string lTag = EnumString<ELogLevel>::ToStr(aLevel);
            std::string lMsg;
            FormatString(lMsg, "[%s]%s\n", lTag.c_str(), buffer);
            OutputDebugString(lMsg.c_str());
            free(buffer);
            va_end(args);
        }
    }
}