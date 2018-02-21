#pragma once
#ifndef __H_File_UTILS__
#define __H_File_UTILS__

#include "Defines.h"
#include <vector>
#include <Windows.h>
#include <sstream>
#include <stdio.h>

namespace base {
    namespace utils
    {
		long GetFileSize(std::string filename);
		bool DoesFileExists(const std::string& file);
		void GetFilesFromPath(const std::string& Path, const std::string& Extension, std::vector<std::string>& _OutFiles);
    }
}

#endif