#pragma once
#include "Model.h"
#include "PostEffect/PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <vector>
#include "Input.h"

class GameManager;

class ResultUI {
public:
	ResultUI();
	void Initialize(const bool& isClear = true);
	void Update(const float& deltaTime);
	void Draw();

private:

	void ClearInitialize();
	void GameOverInitialize();
	void Create();
	void SetGlobalVariables();
	void ApplyGlobalVariables();

private:
	Input* input_;
	GameManager* gameManager_;
	Vector2 screenSize_;

	enum SpriteNameEnum
	{
		kRestart,
		kTitle,
		kResult,
		kContinue,
		kMaxNameEnum,
	};
	std::vector<std::string> names_;

	enum SpriteScaleEnum
	{
		kClear,
		kGameOver,
		kOthers,
		kMaxScaleNum,
	};
	std::vector<std::string> scaleNames_;

	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::vector<const Texture*> textures_;
	std::unique_ptr<GlobalVariableUser> global_;

	struct PutData
	{
		Vector2 baseScale;
		float scale;
	};
	std::vector<PutData> putData_;
	std::vector<Vector2> clearBasePoses_;
	std::vector<Vector2> gameOverBasePoses_;

	int32_t yesNoSpriteMaxNum_;
	int32_t yesNoSpriteNum_;
	bool isClear_;
};