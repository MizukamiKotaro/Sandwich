#include "Timer.h"
#include "WindowsInfo/WindowsInfo.h"
#include <algorithm>
#include <cmath>

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

	drawTrop_ = std::make_unique<DrawNumbers>("TimeNumbers.png", "Timer", "テロップ", Vector2{ 192.0f,240.0f });
	ps_ = std::make_unique<PostSprite>(screenPos_);
	sps_.resize(2);
	sps_[0] = std::make_unique<Sprite>(screenPos_);
	sps_[1] = std::make_unique<Sprite>(screenPos_);
	poses_.resize(2);
	poses_[0] = sps_[0]->size_;
	poses_[1] = sps_[1]->size_;
	s_ = 1.0f;
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
	isD_ = false;
}

void Timer::Update(const float& deltaTime)
{
	time_ -= deltaTime;
	if (time_ <= 0.0f) {
		time_ = 0.0f;
	}

	PsUpdate();

	DrawSprite();
}

void Timer::Draw()
{
	sprite_->Draw();
}

void Timer::DrowTrop()
{
	/*if (isD_) {
		ps_->Draw();
	}*/
	ps_->Draw();
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
	drawTrop_->Update();
#endif // _DEBUG
}

void Timer::SetGlobalVariables()
{
	glo_->AddItem("制限時間", maxTime_);
	glo_->AddItem("残りの位置", poses_[0]);
	glo_->AddItem("秒の位置", poses_[1]);
	glo_->AddItem("テロップのスケール", 1.0f);
	ApplyGlobalVariables();
}

void Timer::ApplyGlobalVariables()
{
	maxTime_ = glo_->GetIntValue("制限時間");
	sps_[0]->pos_ = glo_->GetVector2Value("残りの位置");
	sps_[1]->pos_ = glo_->GetVector2Value("秒の位置");
	s_ = glo_->GetFloatValue("テロップのスケール");

	sps_[0]->size_ = poses_[0] * s_;
	sps_[1]->size_ = poses_[1] * s_;
	sps_[0]->Update();
	sps_[1]->Update();
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

void Timer::PsUpdate()
{
	int32_t hn = maxTime_ / 2;
	ps_->PreDrawScene();
	sps_[0]->Draw();
	sps_[1]->Draw();
	drawTrop_->Draw(hn);
	ps_->PostDrawScene();
	if (time_ <= float(hn + 1) && time_ >= float(hn - 1)) {
		float t = std::powf(time_ - float(hn), 3.0f);
		ps_->sprite_->pos_.x = screenPos_.x * 2.0f * t + screenPos_.x;
		//ps_->Update();

		ps_->PreDrawScene();
		sps_[0]->Draw();
		sps_[1]->Draw();
		drawTrop_->Draw(hn);
		ps_->PostDrawScene();
		isD_ = true;
	}
	else if (time_ <= 21.0f && time_ >= 19.0f) {
		float t = std::powf(time_ - float(hn), 3.0f);
		ps_->sprite_->pos_.x = screenPos_.x * 2.0f * t + screenPos_.x;
		//ps_->Update();
		ps_->PreDrawScene();
		sps_[0]->Draw();
		sps_[1]->Draw();
		drawTrop_->Draw(20);
		ps_->PostDrawScene();
		isD_ = true;
	}
	else {
		isD_ = false;
	}
}
