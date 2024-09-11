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

	const bool GetIsTimeUp() const;
	const bool& GetIsReset() const;

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
};