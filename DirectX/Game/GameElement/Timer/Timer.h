#pragma once
#include "GameElement/DrawNumbers/DrawNumbers.h"
#include "PostEffect/PostEffect.h"
#include "Drawers/PostSprite/PostSprite.h"

class Timer
{
public:
	Timer();

	void Initialize();
	void Update(const float& deltaTime);
	void Draw();

	void DrowTrop();

	void UpdateToGame(const float& deltaTime);

	const bool GetIsTimeUp() const;
	const bool& GetIsReset() const;
	void UpdateGlobal();

private:
	void SetGlobalVariables();
	void ApplyGlobalVariables();
	void DrawSprite();

	void PsUpdate();

private:
	std::unique_ptr<PostSprite> ps_;
	std::unique_ptr<DrawNumbers> drawTrop_;
	std::vector<std::unique_ptr<Sprite>> sps_;
	std::vector<Vector2> poses_;
	float s_;
	bool isD_;

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