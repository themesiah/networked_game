#pragma once

#ifndef H_LOADABLE_RESOURCE
#define H_LOADABLE_RESOURCE

#include <string>

class LoadableResource
{
public:
	virtual ~LoadableResource()
	{
		Destroy();
	}
	bool Load(const std::string& path)
	{
		m_ResourcePath = path;
		return DoLoad(path);
	}
	virtual bool Reload() = 0;
protected:
	virtual bool DoLoad(const std::string& path) = 0;
	virtual void Destroy() {};
	std::string m_ResourcePath;
};

#endif
