#include "LevelDataLoader.h"
#include <fstream>
#include <Windows.h>
#include "LevelData/LevelDataManager.h"

const std::string kDirectoryPath = "Resources/LevelDatas/";

LevelDataLoader* LevelDataLoader::GetInstance()
{
	static LevelDataLoader instance;
	return &instance;
}

void LevelDataLoader::LoadFiles()
{
	std::filesystem::path dir(kDirectoryPath);

	if (!std::filesystem::exists(dir)) {
		return;
	}

	std::filesystem::directory_iterator dir_it(dir);
	for (const std::filesystem::directory_entry& entry : dir_it) {

		const std::filesystem::path& filePath = entry.path();

		std::string extension = filePath.extension().string();

		if (extension.compare(".json") != 0) {
			continue;
		}

		std::filesystem::path relativePath = std::filesystem::relative(filePath, dir);

		LoadFile(relativePath.string());
	}
}

void LevelDataLoader::LoadFile(const std::string& filePath)
{
	std::string fullpath = kDirectoryPath + filePath;

	std::ifstream ifs;

	ifs.open(fullpath);

	if (ifs.fail()) {
#ifdef _DEBUG
		std::string message = "Failed open data file file for read";
		MessageBoxA(nullptr, message.c_str(), "LevelData", 0);
#endif // _DEBUG
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	ifs >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルファイルかチェック
	assert(name.compare("scene") == 0);

	// レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	ParseRecursive(deserialized, levelData->objects);

	LevelDataManager::GetInstance()->CreateData(filePath, *levelData);

	ifs.close();
	delete levelData;
}

void LevelDataLoader::ParseRecursive(nlohmann::json& deserialized, std::vector<LevelData::ObjectData>& objects, const std::string& name)
{
	for (nlohmann::json& object : deserialized[name]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {
			// 要素の追加
			objects.emplace_back(LevelData::ObjectData{});
			LevelData::ObjectData& objectData = objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				objectData.fileName = object["file_name"];
			}
			else {
				objectData.fileName = "Cube";
			}

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			// 回転角
			objectData.rotation.x = -(float)transform["rotation"][0];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];

			// TODO: コライダーのパラメータ読み込み
		}

		// TODO: 再帰関数
		if (object.contains("children")) {
			ParseRecursive(object, objects.back().children, "children");
		}
	}
}
