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
		TEXTURES = 1,
		ANIMATIONS = 2
	};

	virtual void Load(const std::string& aFilename);

	std::string GetResourcePath(uint16_t aKey)
	{
		return m_Table.Get(aKey);
	}
protected:
	ResourceTableType m_Table;
};

#endif