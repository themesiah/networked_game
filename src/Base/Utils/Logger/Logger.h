#pragma once
#ifndef __H_LOGGER__
#define __H_LOGGER__

#include "Utils/Singleton.h"
#include "Utils/Types.h"

#include <vector>
#include <string>

#include "Utils/EnumToString.h"

#ifdef _FINAL
#define LOG_ERROR_APPLICATION( x, ... ){}
#define LOG_WARNING_APPLICATION( x, ...  ){}
#define LOG_INFO_APPLICATION( x, ...  ){}
#define LOG_ERROR_APPLICATION_IF( condition, x, ... ){}
#define LOG_WARNING_APPLICATION_IF( condition, x, ... ){}
#define LOG_INFO_APPLICATION_IF( condition, x, ... ){}
#else 
#define LOG_ERROR_APPLICATION( x, ... )     base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogError,   x, __VA_ARGS__ )
#define LOG_WARNING_APPLICATION( x, ...  )  base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogWarning, x, __VA_ARGS__ )
#define LOG_INFO_APPLICATION( x, ...  )     base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogInfo,    x, __VA_ARGS__ )
#define LOG_ERROR_APPLICATION_IF( condition, x, ... ) if(!condition) base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogError,   x, __VA_ARGS__ )
#define LOG_WARNING_APPLICATION_IF( condition, x, ... ) if(!condition) base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogWarning, x, __VA_ARGS__ )
#define LOG_INFO_APPLICATION_IF( condition, x, ... ) if(!condition) base::utils::CLogger::GetInstance().AddNewLog( base::utils::CLogger::eLogInfo,    x, __VA_ARGS__ )
#endif
/*
Begin_Enum_String(base::utils::CLogger::ELogLevel)
{
	Enum_String_Id(base::utils::CLogger::eLogInfo, "Info");
	Enum_String_Id(base::utils::CLogger::eLogWarning, "Warning");
	Enum_String_Id(base::utils::CLogger::eLogError, "Error");
}
End_Enum_String;*/


namespace base
{
    namespace utils
    {
        class CLogger : public CSingleton<CLogger>
        {
        public:
            enum ELogLevel
            {
                eLogInfo = 0,
                eLogWarning,
                eLogError
            };
        public:
            CLogger();
            virtual ~CLogger();
            void AddNewLog(ELogLevel ll, const char* format, ...);
        };
    }
}

#endif // __H_LOGGER__