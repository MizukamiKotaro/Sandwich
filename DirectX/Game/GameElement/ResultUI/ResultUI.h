#pragma once
#include "Model.h"
#include "PostEffect/PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <vector>
#include "Input.h"
#include "GameElement/DrawNumbers/DrawNumbers.h"
#include "Audio.h"

class GameManager;
class Score;

class ResultUI {
public:
	ResultUI();
	void Initialize(const bool& isClear = true);
	void Update(const float& deltaTime);
	void Draw();
	void DrawPlane(const Camera& camera);

	const bool& GetIsTranHalf() const;
	const bool& GetIsTitle() const;
	const bool& GetIsGame() const;

private:
	void DrawSp();
	void ClearInitialize();
	void GameOverInitialize();
	void Create();
	void SetGlobalVariables();
	void ApplyGlobalVariables();

private:
	Input* input_;
	GameManager* gameManager_;
	Score* score_;
	Vector2 screenSize_;

	std::unique_ptr<PostEffect> postEf_;
	std::unique_ptr<Sprite> postSp_;
	float postTime_;

	std::unique_ptr<Audio> seClear_;
	std::unique_ptr<Audio> seDecision_;
	std::unique_ptr<Audio> seCursor_;

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
		Vector2 basePos;
	};

	std::vector<std::unique_ptr<Sprite>> sps_;
	std::vector<PutData> putSps_;
	std::vector<std::string> spsNames_;

	std::unique_ptr<DrawNumbers> drawNum_;

	enum SpsNames
	{
		kBack,
		kSatisfaction,
		kcustomer,
		kEnd,
	};

	std::vector<PutData> putData_;
	std::vector<Vector2> clearBasePoses_;
	std::vector<Vector2> gameOverBasePoses_;

	int32_t yesNoSpriteMaxNum_;
	int32_t yesNoSpriteNum_;
	bool isClear_;

	std::unique_ptr<Model> plane_;
	std::vector<const Texture*> equipmentTeses_;
	int32_t texNum_;
	float rotate_;
	float planeSize_;
	Vector3 pos_;
	float maxTime_;
	float time_;
	bool isTitle_;
	bool isGame_;
	bool isTran_;
	bool isTranHalf_;
};