#include "LevelDataManager.h"
#include "LevelData.h"

LevelDataManager* LevelDataManager::GetInstance()
{
	static LevelDataManager instance;
	return &instance;
}

void LevelDataManager::CreateData(const std::string& sceneName, const LevelData& data)
{
	datas_[sceneName].reset();
	datas_[sceneName] = std::make_unique<LevelData>(data);
}

const LevelData& LevelDataManager::GetLevelData(const std::string& sceneName)
{
	return *datas_[sceneName].get();
}

void LevelDataManager::Clear()
{
	datas_.clear();
}
