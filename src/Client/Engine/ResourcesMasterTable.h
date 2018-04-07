#pragma once

#ifndef H_RESOURCES_MASTER_TABLE
#define H_RESOURCES_MASTER_TABLE

#include "ResourcesTable.h"

class CResourcesMasterTable : public CResourcesTable
{
public:
	virtual void Load(const std::string& aFilename) override;
};

#endif