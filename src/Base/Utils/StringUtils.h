#pragma once
#ifndef _H_VISIBLE__
#define _H_VISIBLE__

#include "Defines.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <sstream>
#include <stdio.h>

namespace base {
    namespace utils
    {
         static void FormatString(std::string& output, const char* format, ...)
        {
            va_list args;
            char* buffer;
            va_start(args, format);
            int len = _vscprintf(format, args) + 1;
            buffer = (char*)malloc(len * sizeof(char));
            vsprintf_s(buffer, len, format, args);
            output = buffer;
            free(buffer);
            va_end(args);
        }

		 static std::vector<std::string>& Split(const std::string& s, char delim,
            std::vector<std::string>& elems)
        {
            std::stringstream ss(s);
            std::string item;

            while (std::getline(ss, item, delim))
                elems.push_back(item);

            return elems;
        }

		 static std::vector<std::string> Split(const std::string& s, char delim)
        {
            std::vector<std::string> elems;
            Split(s, delim, elems);
            return elems;
        }


		 static std::string GetFilenameExtension(std::string aName)
		 {
			 std::string lExtension;

			 if (aName.find_last_of(".") != std::string::npos)
			 {
				 lExtension = aName.substr(aName.find_last_of(".") + 1);
			 }
			 else{
				 lExtension = "";
			 }

			return lExtension;
		 }

		 static std::string GetFilename(std::string aName, bool aNoExtension = false)
		 {
			 std::string lFilename;

			 if (aName.find_last_of("\\") != std::string::npos)
			 {
				 lFilename = aName.substr(aName.find_last_of("\\") + 1);
			 }
			 else {
				 if(aName.find_last_of("/") != std::string::npos)
				 {
					lFilename = aName.substr(aName.find_last_of("/") + 1);
				 }
				 else {
					lFilename = "";
				 }
			 }
			 if (aNoExtension)
			 {
				 if (lFilename.find_last_of(".") != std::string::npos)
				 {
					 lFilename = lFilename.substr(0, lFilename.find_last_of("."));
				 }
			 }
			 return lFilename;
		 }
    }
}

#endif // _H_VISIBLE__