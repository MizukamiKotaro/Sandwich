#include "LiveMusics.h"
#include "SceneSystem/IScene/IScene.h"

LiveMusics::LiveMusics()
{
	music_ = std::make_unique<Audio>();
}

void LiveMusics::Initialize() 
{
	if (music_->IsPlaying()) {
		music_->Stop();
	}
	switch (IScene::stageNo_)
	{
	case STAGE::SHINING_STAR:
		music_->Load("maou_short_14_shining_star.mp3", "シャイニングスター");
		break;
	default:
		break;
	}
	isMusicStart_ = false;
	isMusicFinish_ = false;
}

void LiveMusics::Update() 
{
	if (!isMusicStart_) {
		isMusicStart_ = true;
		music_->Play();
	}
	if (!music_->IsPlaying()) {
		isMusicFinish_ = true;
	}
}
