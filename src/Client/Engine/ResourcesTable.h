#pragma once

#ifndef H_RESOURCES_TABLE
#define H_RESOURCES_TABLE

#include <inttypes.h>
#include <fstream>
#include <string>

#include "Utils\TemplatedPairMap.h"
#include "Debug\CustomAssert.h"

typedef base::utils::CTemplatedPairMap<uint16_t, std::string> ResourceTableType;
class CResourcesTable
{
public:
	enum TablesKeys : uint16_t
	{
		TEXTURES = 1
	};

	void Load(const std::string& aFilename)
	{
		std::string lResource;
		std::ifstream file(aFilename);
		if (file.is_open())
		{
			while (std::getline(file, lResource)) {
				uint16_t key;
				char value[100];
				sscanf(lResource.c_str(), "%hu:%s", &key, &value);
				std::string valueS(value);
				m_Table.Add(key, valueS);
			}
			file.close();
		}
		else {
			Assert(false, "File for Resource Table with path %s can't be opened.", aFilename);
		}
	}

	std::string GetResourcePath(uint16_t aKey)
	{
		return m_Table.Get(aKey);
	}
private:
	ResourceTableType m_Table;
};

#endif