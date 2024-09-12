#include "Timer.h"
#include "WindowsInfo/WindowsInfo.h"
#include <algorithm>

Timer::Timer()
{
	maxTime_ = 60;
	time_ = 60.0f;
	isReset_ = false;

	post_ = std::make_unique<PostEffect>();
	screenPos_ = WindowsInfo::GetInstance()->GetWindowSize() * 0.5f;
	sprite_ = std::make_unique<Sprite>(screenPos_);
	sprite_->SetSRVGPUDescriptorHandle_(post_->GetSRVGPUDescriptorHandle());
	drawNum_ = std::make_unique<DrawNumbers>("TimeNumbers.png", "Timer", "背景", Vector2{ 192.0f,240.0f });
	glo_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "Timer");
	SetGlobalVariables();

	easeTime_ = 0.0f;
}

void Timer::Initialize()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG
	time_ = float(maxTime_);
	isReset_ = false;
}

void Timer::Update(const float& deltaTime)
{
	time_ -= deltaTime;
	if (time_ <= 0.0f) {
		time_ = 0.0f;
	}

	DrawSprite();
}

void Timer::Draw()
{
	sprite_->Draw();
}

void Timer::UpdateToGame(const float& deltaTime)
{
	float a = 1.0f;
	easeTime_ = std::clamp<float>(easeTime_ + deltaTime, 0.0f, a);
	float t = easeTime_ / a;
	sprite_->pos_.y = screenPos_.y * t - screenPos_.y * (1.0f - t);
	sprite_->Update();
}

const bool Timer::GetIsTimeUp() const
{
	if (time_ <= 0.0f) {
		return true;
	}
	return false;
}

const bool& Timer::GetIsReset() const
{
	return isReset_;
}

void Timer::UpdateGlobal()
{
#ifdef _DEBUG
	int32_t maxTime = maxTime_;
	ApplyGlobalVariables();
	if (maxTime_ != maxTime) {
		isReset_ = true;
		time_ = float(maxTime_);
	}
	drawNum_->Update();
#endif // _DEBUG
}

void Timer::SetGlobalVariables()
{
	glo_->AddItem("制限時間", maxTime_);
	ApplyGlobalVariables();
}

void Timer::ApplyGlobalVariables()
{
	maxTime_ = glo_->GetIntValue("制限時間");
}

void Timer::DrawSprite()
{
	float time = time_;
	if (time_ != maxTime_ && time_ > 0.0f) {
		time += 1.0f;
	}
	post_->PreDrawScene();
	drawNum_->Draw(int32_t(time));
	post_->PostDrawScene();
}
