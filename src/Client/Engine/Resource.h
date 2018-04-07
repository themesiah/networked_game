#pragma once

#ifndef H_RESOURCEASD
#define H_RESOURCEASD

#include <string>

#define GET_RESOURCE_NAME(resourceId, resourceTableId) CEngine::GetInstance().GetResourceTable(aResourceTable)->GetResourcePath(aResourceId);

class IResource {
public:
	virtual bool LoadResource(const std::string& aResourceName) = 0;
};

#endif