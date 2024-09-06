#pragma once
#include <string>
#include <map>
#include <memory>
class LevelData;
class LevelDataManager
{
public:
	static LevelDataManager* GetInstance();
	void CreateData(const std::string& sceneName, const LevelData& data);
	const LevelData& GetLevelData(const std::string& sceneName);
	void Clear();

private:
	LevelDataManager() = default;
	~LevelDataManager() = default;
	LevelDataManager(const LevelDataManager& obj) = delete;
	LevelDataManager& operator=(const LevelDataManager& obj) = delete;
	
private:
	std::map<std::string, std::unique_ptr<LevelData>> datas_;

};