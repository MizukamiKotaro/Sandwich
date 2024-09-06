#include "LiveEditor.h"
#include "Camera.h"
#include "GameElement/IStageObject/StageObjectConfig.h"
#include "GameElement/IStageObject/StageObjectFactory/StageObjectFactory.h"
#include "ScreenEditor/ScreenEditor.h"
#include "ParticleManager.h"

LiveEditor::LiveEditor(Camera* camera)
{
	camera_ = camera;
	camera_->Initialize();
	
	screenCamera_ = std::make_unique<Camera>();
	lightAndOutline_ = std::make_unique<SpotLightAndOutline>();

	screenManager_ = std::make_unique<MultipleScreenEditor>("スクリーン", "マルチスクリーン", 0);
	idolManager_ = std::make_unique<IStageObjectManager>();
	idolManager_->AddType(StageObjectType::IDOL, "アイドル", "メインアイドル");


	lightManager_ = std::make_unique<IStageObjectManager>();
	lightManager_->AddType(StageObjectType::SPOTLIGHT, "ライト", "スポットライト");
	lightManager_->AddType(StageObjectType::TWIN_SPOTLIGHT, "ライト", "ツインスポットライト");
	floor_.reset(StageObjectFactory::CreateStageObject(StageObjectType::FLOOR, "ステージ床", "ステージ床", 0));
	dome_.reset(StageObjectFactory::CreateStageObject(StageObjectType::DOME, "ドーム", "ドーム", 0));

	fireManager_ = std::make_unique<IStageObjectManager>();
	fireManager_->AddType(StageObjectType::FIRE_PARTICLE, "パーティクル", "炎");

	screenMap_ = screenManager_->GetScreenMap();
	lightAndOutlineMap_ = screenManager_->GetOutlineMap();
}

void LiveEditor::Initialize()
{
	camera_->Initialize();
	camera_->transform_.translate_ = { 0.0f,25.0f,-100.0f };
	camera_->transform_.rotate_.x = 0.1f;
	camera_->Update();

	screenCamera_->transform_.translate_ = { 0.0f,15.0f,-80.0f };
	screenCamera_->Update();

	lightManager_->Initialize();
	idolManager_->Initialize();
	floor_->Initialize();
	dome_->Initialize();
	fireManager_->Initialize();
}

void LiveEditor::Update(const float& time)
{
#ifdef _DEBUG
	/*ImGui::Begin("Camera");
	ImGui::DragFloat3("ポジション", &camera_->transform_.translate_.x, 0.01f);
	ImGui::SliderFloat3("角度", &camera_->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::End();*/
	
#endif // _DEBUG
	camera_->Update();

	lightManager_->Update(time);
	screenManager_->Update(time);
	idolManager_->Update(time);
	floor_->Update(time);
	dome_->Update(time);
	fireManager_->Update(time);

	WriteScreen();
	WriteOutline();
}

void LiveEditor::Draw()
{
	ParticleManager::GetInstance()->Clear();
	lightAndOutline_->Draw(*camera_);
	fireManager_->Draw(*camera_);
	ParticleManager::GetInstance()->Draw(*camera_);
}

void LiveEditor::WriteScreen()
{
	for (uint32_t i = 0; i < screenManager_->GetScreenNum(); i++) {
		const Camera& camera = (*screenMap_)[i]->GetCamera();
		(*lightAndOutlineMap_)[i]->PreDrawOutline();
		idolManager_->Draw(camera);
		floor_->DrawSub(camera);
		(*lightAndOutlineMap_)[i]->PostDrawOutline();

		(*lightAndOutlineMap_)[i]->PreDrawObject();
		dome_->Draw(camera);
		floor_->Draw(camera);
		lightManager_->Draw(camera);
		(*lightAndOutlineMap_)[i]->PostDrawObject();

		(*lightAndOutlineMap_)[i]->PreDrawLight();
		lightManager_->DrawLight(camera);
		(*lightAndOutlineMap_)[i]->PostDrawLight();

		(*screenMap_)[i]->PreDrawScene();
		(*lightAndOutlineMap_)[i]->Draw(camera);
		(*screenMap_)[i]->PostDrawScene();
	}
}

void LiveEditor::WriteOutline()
{
	lightAndOutline_->PreDrawOutline();
	idolManager_->Draw(*camera_);
	floor_->DrawSub(*camera_);
	lightAndOutline_->PostDrawOutline();

	lightAndOutline_->PreDrawObject();
	dome_->Draw(*camera_);
	floor_->Draw(*camera_);
	screenManager_->Draw(*camera_);
	lightManager_->Draw(*camera_);
	lightAndOutline_->PostDrawObject();

	lightAndOutline_->PreDrawLight();
	lightManager_->DrawLight(*camera_);
	lightAndOutline_->PostDrawLight();
}
