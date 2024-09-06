#include "DomeEditor.h"
#include "Camera.h"
#include "TextureManager.h"

DomeEditor::DomeEditor(const std::string& mainName, const std::string& name, const uint32_t& no)
{
	CreateStageEditor(mainName, name, no);
	model_ = std::make_unique<Model>("skydome");
	//model_->SetTexture(TextureManager::GetInstance()->LoadTexture("skydome1.png"));
	model_->Update();
	model_->UnUsedLight();
	model_->SetColor(Vector4{ 0.02f,0.02f,0.02f,1.0f });
	SetGlobalVariable();
}

void DomeEditor::Initialize()
{
}

void DomeEditor::Update(const float& time)
{
#ifdef _DEBUG
	UpdateGlobalVariable();
#endif // _DEBUG
	if(time){}
}

void DomeEditor::Draw(const Camera& camera)
{
	model_->Draw(camera);
}

void DomeEditor::SetGlobalVariable()
{
	stageEditor_->AddItem("スケール", model_->transform_.scale_.x);
	stageEditor_->AddItem("回転", model_->transform_.rotate_.y);
	ApplyGlobalVariable();
}

void DomeEditor::ApplyGlobalVariable()
{
	model_->transform_.scale_.x = stageEditor_->GetFloatValue("スケール");
	model_->transform_.scale_ = { model_->transform_.scale_.x ,model_->transform_.scale_.x ,model_->transform_.scale_.x };
	model_->transform_.rotate_.y = stageEditor_->GetFloatValue("回転");
	model_->Update();
}
