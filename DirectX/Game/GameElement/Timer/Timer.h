#pragma once
#include "GameElement/DrawNumbers/DrawNumbers.h"
#include "PostEffect/PostEffect.h"

class Timer
{
public:
	Timer();

	void Initialize();
	void Update(const float& deltaTime);
	void Draw();

	void UpdateToGame(const float& deltaTime);

	const bool GetIsTimeUp() const;
	const bool& GetIsReset() const;
	void UpdateGlobal();

private:
	void SetGlobalVariables();
	void ApplyGlobalVariables();
	void DrawSprite();

private:
	std::unique_ptr<DrawNumbers> drawNum_;
	std::unique_ptr<PostEffect> post_;
	std::unique_ptr<Sprite> sprite_;
	std::unique_ptr<GlobalVariableUser> glo_;

	int32_t maxTime_;
	float time_;
	bool isReset_;

	float easeTime_;
	Vector2 screenPos_;
};