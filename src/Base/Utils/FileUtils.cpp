#include "StringUtils.h"
#include <sstream>
#include <stdio.h>
#include <vector>
#include <Windows.h>
#include "Utils\Types.h"
#include <sstream>
#include <string>
#include <sys\stat.h>
#include <streambuf>
#include <fstream>

namespace base {
    namespace utils
    {
		long GetFileSize(std::string filename)
		{
			struct stat stat_buf;
			int rc = stat(filename.c_str(), &stat_buf);
			return rc == 0 ? stat_buf.st_size : -1;
		}

		bool DoesFileExists(const std::string& file)
		{
			struct stat buf;
			return (stat(file.c_str(), &buf) == 0);
		}

		void GetFilesFromPath( const std::string& Path, const std::string& Extension, std::vector<std::string>& _OutFiles)
		{
			std::string FilesToLookUp = Path + "*." + Extension;
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = FindFirstFile(FilesToLookUp.c_str(), &FindFileData);

			while (hFind != INVALID_HANDLE_VALUE)
			{
				_OutFiles.push_back(FindFileData.cFileName);

				if (!FindNextFile(hFind, &FindFileData))
				{
					FindClose(hFind);
					hFind = INVALID_HANDLE_VALUE;
				}
			}
		}

		std::string GetTextFromFile(const std::string& path)
		{
			std::ifstream f(path);
			std::string str = "";
			if (f.is_open() == true)
			{
				str = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			}
			return str;
		}
    }
}