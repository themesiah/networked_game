#include "ResourcesMasterTable.h"

#include "Engine.h"

void CResourcesTable::Load(const std::string& aFilename)
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

void CResourcesMasterTable::Load(const std::string& aFilename)
{
	CResourcesTable::Load(aFilename);
	CEngine& lEngine = CEngine::GetInstance();
	for (auto it = m_Table.GetResourcesMap().begin(); it != m_Table.GetResourcesMap().end(); ++it)
	{
		CResourcesTable* lTable = new CResourcesTable();
		lTable->Load(it->second);
		lEngine.SetResourcesTable(it->first, lTable);
	}
}