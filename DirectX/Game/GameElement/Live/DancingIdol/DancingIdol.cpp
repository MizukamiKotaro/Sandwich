#include "DancingIdol.h"
#include "Camera.h"
#include "TextureManager.h"

DancingIdol::DancingIdol()
{
	idol_ = std::make_unique<SkinningModel>("CharactorSkinning");
	idol_->transform_.rotate_.y = 3.14f;
	idol_->transform_.scale_ *= 5;
	idol_->SetColor(Vector4{ 0.8f,0.8f,0.2f,1.0f });
	idol_->Update(0.0f);
}

void DancingIdol::Initialize() {
	
}

void DancingIdol::Update(const float& time) {
	idol_->Update(time);
}

void DancingIdol::Draw(Camera* camera) {
	idol_->Draw(*camera);
}
