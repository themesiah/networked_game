#ifndef H_SINGLETON
#define H_SINGLETON

#include "Defines.h"

namespace base{
    namespace utils{

        template <typename T>
        class CSingleton
        {
        public:
            static T& GetInstance()
            {
                static T    instance;
                return instance;
            }
        protected:
            CSingleton() {}
            DISALLOW_COPY_AND_ASSIGN(CSingleton)
        };
    }
}

#endif //_BASE_SINGLETON_LUIS_RUIZ_1409161556_H