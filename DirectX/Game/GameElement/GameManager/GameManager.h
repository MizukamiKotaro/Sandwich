#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "Camera.h"

class GameManager {
public:
	static GameManager* GetInstance();
	void Initialize();
	
	void ChangeScene(const int32_t& nextScene);
	void CompletedTransition();
	const int32_t& GetScene() const;
	const bool& GetIsTransition() const;

	enum State
	{
		kTitle,
		kGame,
		kResult,
	};
private:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	int32_t state_;
	int32_t stageNum_;

	bool isTransition_;
};