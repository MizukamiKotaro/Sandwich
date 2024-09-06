#include "FinishLive.h"
#include "SceneSystem/IScene/IScene.h"

FinishLive::FinishLive()
{
	se_ = std::make_unique<Audio>();
	se_->Load("finish.mp3", "ライブ終わりの歓声");
}

void FinishLive::Initialize()
{
	finishCount_ = 0.0f;
	isClear_ = false;
	isFinish_ = false;
}

void FinishLive::Update(const float& deltaTime)
{
	finishCount_ += deltaTime;
	if (!isFinish_) {
		isFinish_ = true;
		se_->Play();
	}
	if (finishCount_ >= 0.75f) {
		isClear_ = true;
	}
}