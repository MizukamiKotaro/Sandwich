#include "Timer.h"
#include "WindowsInfo/WindowsInfo.h"

Timer::Timer()
{
	maxTime_ = 60;
	time_ = 60.0f;
	isReset_ = false;

	post_ = std::make_unique<PostEffect>();
	sprite_ = std::make_unique<Sprite>(WindowsInfo::GetInstance()->GetWindowSize() * 0.5f);
	sprite_->SetSRVGPUDescriptorHandle_(post_->GetSRVGPUDescriptorHandle());
	drawNum_ = std::make_unique<DrawNumbers>("TimeNumbers.png", "Timer", "背景", Vector2{ 192.0f,240.0f });
	glo_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "Timer");
	SetGlobalVariables();
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
#ifdef _DEBUG
	int32_t maxTime = maxTime_;
	ApplyGlobalVariables();
	if (maxTime_ != maxTime) {
		isReset_ = true;
		time_ = float(maxTime_);
	}
	drawNum_->Update();
#endif // _DEBUG
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

const bool Timer::GetIsTimeUp() const
{
	if (time_ <= 0.0f) {
		return 0.0f;
	}
	return false;
}

const bool& Timer::GetIsReset() const
{
	return isReset_;
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
	post_->PreDrawScene();
	drawNum_->Draw(int32_t(time_));
	post_->PostDrawScene();
}
