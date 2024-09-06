#include "SpotLightBoxAnimationEditor.h"
#include "Camera.h"

const int kSpotNum = 2;

SpotLightBoxAnimationEditor::SpotLightBoxAnimationEditor(const std::string& mainName, const std::string& name, const uint32_t& no)
{
	Create();
	CreateStageEditor(mainName, name, no, true);
	SetGlobalVariable();
}

SpotLightBoxAnimationEditor::SpotLightBoxAnimationEditor(const std::string& mainName, const std::string& name, const std::string& treeName, const uint32_t& no)
{
	Create();
	CreateStageEditor(mainName, name, treeName, no);
	SetGlobalVariable();
}

SpotLightBoxAnimationEditor::SpotLightBoxAnimationEditor()
{
	Create();
}

void SpotLightBoxAnimationEditor::Update(const float& time)
{
#ifdef _DEBUG
	UpdateGlobalVariable();
#endif // _DEBUG

	box_->Update(time);
	LightUpdate();
}

void SpotLightBoxAnimationEditor::Draw(const Camera& camera)
{
	box_->Draw(camera);
}

void SpotLightBoxAnimationEditor::DrawLight(const Camera& camera)
{
	spotLights_[0]->Draw(camera);
	spotLights_[1]->Draw(camera, BlendMode::kBlendModeAdd);
}

void SpotLightBoxAnimationEditor::SetGlobalVariable()
{
	stageEditor_->AddItem("位置", box_->transform_.translate_);
	stageEditor_->AddItem("回転", box_->transform_.rotate_);
	stageEditor_->AddItemColor("外のライトの色", spotLights_[0]->light_->color, "ライトの調整");
	stageEditor_->AddItem("外のライトの輝度", spotLights_[0]->light_->intensity, "ライトの調整");
	stageEditor_->AddItem("外のライトの減衰率", spotLights_[0]->light_->decay, "ライトの調整");
	stageEditor_->AddItem("外のライトの開く角度", spotLights_[0]->light_->cosAngle, "ライトの調整");
	stageEditor_->AddItem("外のライトの減衰し始める角度", spotLights_[0]->light_->cosFalloffStart, "ライトの調整");
	stageEditor_->AddItem("外のライトの距離", spotLights_[0]->light_->distance, "ライトの調整");
	stageEditor_->AddItemColor("内のライトの色", spotLights_[1]->light_->color, "ライトの調整");
	stageEditor_->AddItem("内のライトの輝度", spotLights_[1]->light_->intensity, "ライトの調整");
	stageEditor_->AddItem("内のライトの減衰率", spotLights_[1]->light_->decay, "ライトの調整");
	stageEditor_->AddItem("内のライトの開く角度", spotLights_[1]->light_->cosAngle, "ライトの調整");
	stageEditor_->AddItem("内のライトの減衰し始める角度", spotLights_[1]->light_->cosFalloffStart, "ライトの調整");
	stageEditor_->AddItem("内のライトの距離", spotLights_[1]->light_->distance, "ライトの調整");
	ApplyGlobalVariable();
}

void SpotLightBoxAnimationEditor::ApplyGlobalVariable()
{

	box_->transform_.translate_ = stageEditor_->GetVector3Value("位置");
	box_->transform_.rotate_ = stageEditor_->GetVector3Value("回転");

	spotLights_[0]->light_->color = stageEditor_->GetColor("外のライトの色", "ライトの調整");
	spotLights_[0]->light_->intensity = stageEditor_->GetFloatValue("外のライトの輝度", "ライトの調整");
	spotLights_[0]->light_->decay = stageEditor_->GetFloatValue("外のライトの減衰率", "ライトの調整");
	spotLights_[0]->light_->cosAngle = stageEditor_->GetFloatValue("外のライトの開く角度", "ライトの調整");
	spotLights_[0]->light_->cosFalloffStart = stageEditor_->GetFloatValue("外のライトの減衰し始める角度", "ライトの調整");
	spotLights_[0]->light_->distance = stageEditor_->GetFloatValue("外のライトの距離", "ライトの調整");

	spotLights_[1]->light_->color = stageEditor_->GetColor("内のライトの色", "ライトの調整");
	spotLights_[1]->light_->intensity = stageEditor_->GetFloatValue("内のライトの輝度", "ライトの調整");
	spotLights_[1]->light_->decay = stageEditor_->GetFloatValue("内のライトの減衰率", "ライトの調整");
	spotLights_[1]->light_->cosAngle = stageEditor_->GetFloatValue("内のライトの開く角度", "ライトの調整");
	spotLights_[1]->light_->cosFalloffStart = stageEditor_->GetFloatValue("内のライトの減衰し始める角度", "ライトの調整");
	spotLights_[1]->light_->distance = stageEditor_->GetFloatValue("内のライトの距離", "ライトの調整");

	for (uint32_t i = 0; i < 2; i++) {
		spotLights_[i]->light_->cosAngle = std::clamp<float>(spotLights_[i]->light_->cosAngle, -1.0f, 0.998f);
		spotLights_[i]->light_->cosFalloffStart = std::clamp<float>(spotLights_[i]->light_->cosFalloffStart, spotLights_[i]->light_->cosAngle + 0.001f, 0.999f);

		if (spotLights_[i]->light_->distance <= 0.0f) {
			spotLights_[i]->light_->distance = 0.01f;
		}
	}

	stageEditor_->SetVariable("外のライトの開く角度", spotLights_[0]->light_->cosAngle, "ライトの調整");
	stageEditor_->SetVariable("外のライトの減衰し始める角度", spotLights_[0]->light_->cosFalloffStart, "ライトの調整");
	stageEditor_->SetVariable("外のライトの距離", spotLights_[0]->light_->distance, "ライトの調整");
	stageEditor_->SetVariable("内のライトの開く角度", spotLights_[1]->light_->cosAngle, "ライトの調整");
	stageEditor_->SetVariable("内のライトの減衰し始める角度", spotLights_[1]->light_->cosAngle, "ライトの調整");
	stageEditor_->SetVariable("内のライトの距離", spotLights_[1]->light_->cosAngle, "ライトの調整");
	
	box_->Update(0.0f);
	LightUpdate();
}

void SpotLightBoxAnimationEditor::LightUpdate()
{
	spotLights_[0]->light_->direction = Vector3{ 1.0f,0.0f,0.0f } * box_->GetRotateMatrix();
	spotLights_[1]->light_->direction = spotLights_[0]->light_->direction;
	spotLights_[1]->light_->distance = spotLights_[0]->light_->distance;
	point_->light_->position = box_->transform_.translate_;
	point_->Update();
	for (int i = 0; i < kSpotNum; i++) {
		spotLights_[i]->light_->position = box_->transform_.translate_;
		spotLights_[i]->isDraw_ = isDraw_;
		spotLights_[i]->Update();
	}
}

void SpotLightBoxAnimationEditor::Create()
{
	box_ = std::make_unique<RigidAnimationModel>("SpotLightBox");
	box_->transform_.scale_ *= 1.01f;
	box_->color_ = { 0.0f,0.0f,0.0f,1.0f };
	point_ = std::make_unique<PointLight>();
	for (int i = 0; i < kSpotNum; i++) {
		spotLights_[i] = std::make_unique<SpotLight>();
		spotLights_[i]->light_->intensity = 1.0f + i;
	}

	spotLights_[0]->light_->color = { 24.0f / 255.0f,1.0f,211.0f / 255.0f,1.0f };
	spotLights_[0]->light_->intensity = 12.196f;
	spotLights_[0]->light_->decay = 3.485f;
	spotLights_[0]->light_->cosAngle = 0.95f;
	spotLights_[0]->light_->cosFalloffStart = 0.98f;
	spotLights_[0]->light_->distance = 23.0f;

	spotLights_[1]->light_->color = { 44.0f / 255.0f,44.0f / 255.0f,44.0f / 255.0f,1.0f };
	spotLights_[1]->light_->intensity = 22.0f;
	spotLights_[1]->light_->decay = 3.2f;
	spotLights_[1]->light_->cosAngle = 0.97f;
	spotLights_[1]->light_->cosFalloffStart = 0.99f;
	spotLights_[1]->light_->distance = 23.0f;

	box_->SetLight(point_.get());
	box_->SetLight(spotLights_[0].get());
}
