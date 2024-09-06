#include "TwinSpotLightBoxAnimationEditor.h"
#include "Camera.h"
#include "SpotLightBoxAnimationEditor.h"

TwinSpotLightBoxAnimationEditor::TwinSpotLightBoxAnimationEditor(const std::string& mainName, const std::string& name, const uint32_t& no)
{
	lights_[0] = std::make_unique<SpotLightBoxAnimationEditor>();
	lights_[1] = std::make_unique<SpotLightBoxAnimationEditor>();
	CreateStageEditor(mainName, name, no, true);
	SetGlobalVariable();
}

void TwinSpotLightBoxAnimationEditor::Update(const float& time)
{
#ifdef _DEBUG
	UpdateGlobalVariable();
#endif // _DEBUG

	*lights_[1]->spotLights_[0]->light_ = *lights_[0]->spotLights_[0]->light_;
	lights_[1]->spotLights_[0]->light_->direction.x *= -1;
	*lights_[1]->spotLights_[1]->light_ = *lights_[0]->spotLights_[1]->light_;
	lights_[1]->spotLights_[1]->light_->direction.x *= -1;
	lights_[1]->box_->transform_ = lights_[0]->box_->transform_;
	lights_[1]->box_->transform_.translate_.x *= -1;
	lights_[1]->box_->transform_.rotate_.y *= -1;
	lights_[1]->box_->transform_.rotate_.z *= -1;

	lights_[0]->Update(time);
	lights_[1]->Update(-time);
}

void TwinSpotLightBoxAnimationEditor::Draw(const Camera& camera)
{
	lights_[0]->Draw(camera);
	lights_[1]->Draw(camera);
}

void TwinSpotLightBoxAnimationEditor::DrawLight(const Camera& camera)
{
	lights_[0]->DrawLight(camera);
	lights_[1]->DrawLight(camera);
}

void TwinSpotLightBoxAnimationEditor::SetGlobalVariable()
{
	stageEditor_->AddItem("位置", Vector3{1.0f, 0.0f, 0.0f});
	stageEditor_->AddItem("回転", Vector3{});
	stageEditor_->AddItem("位置", lights_[0]->box_->transform_.translate_);
	stageEditor_->AddItem("回転", lights_[0]->box_->transform_.rotate_);
	stageEditor_->AddItemColor("外のライトの色", lights_[0]->spotLights_[0]->light_->color, "ライトの調整");
	stageEditor_->AddItem("外のライトの輝度", lights_[0]->spotLights_[0]->light_->intensity, "ライトの調整");
	stageEditor_->AddItem("外のライトの減衰率", lights_[0]->spotLights_[0]->light_->decay, "ライトの調整");
	stageEditor_->AddItem("外のライトの開く角度", lights_[0]->spotLights_[0]->light_->cosAngle, "ライトの調整");
	stageEditor_->AddItem("外のライトの減衰し始める角度", lights_[0]->spotLights_[0]->light_->cosFalloffStart, "ライトの調整");
	stageEditor_->AddItem("外のライトの距離", lights_[0]->spotLights_[0]->light_->distance, "ライトの調整");
	stageEditor_->AddItemColor("内のライトの色", lights_[0]->spotLights_[1]->light_->color, "ライトの調整");
	stageEditor_->AddItem("内のライトの輝度", lights_[0]->spotLights_[1]->light_->intensity, "ライトの調整");
	stageEditor_->AddItem("内のライトの減衰率", lights_[0]->spotLights_[1]->light_->decay, "ライトの調整");
	stageEditor_->AddItem("内のライトの開く角度", lights_[0]->spotLights_[1]->light_->cosAngle, "ライトの調整");
	stageEditor_->AddItem("内のライトの減衰し始める角度", lights_[0]->spotLights_[1]->light_->cosFalloffStart, "ライトの調整");
	stageEditor_->AddItem("内のライトの距離", lights_[0]->spotLights_[1]->light_->distance, "ライトの調整");
	ApplyGlobalVariable();
}

void TwinSpotLightBoxAnimationEditor::ApplyGlobalVariable()
{
	lights_[0]->box_->transform_.translate_ = stageEditor_->GetVector3Value("位置");
	lights_[0]->box_->transform_.rotate_ = stageEditor_->GetVector3Value("回転");
	lights_[0]->spotLights_[0]->light_->color = stageEditor_->GetColor("外のライトの色", "ライトの調整");
	lights_[0]->spotLights_[0]->light_->intensity = stageEditor_->GetFloatValue("外のライトの輝度", "ライトの調整");
	lights_[0]->spotLights_[0]->light_->decay = stageEditor_->GetFloatValue("外のライトの減衰率", "ライトの調整");
	lights_[0]->spotLights_[0]->light_->cosAngle = stageEditor_->GetFloatValue("外のライトの開く角度", "ライトの調整");
	lights_[0]->spotLights_[0]->light_->cosFalloffStart = stageEditor_->GetFloatValue("外のライトの減衰し始める角度", "ライトの調整");
	lights_[0]->spotLights_[0]->light_->distance = stageEditor_->GetFloatValue("外のライトの距離", "ライトの調整");

	lights_[0]->spotLights_[1]->light_->color = stageEditor_->GetColor("内のライトの色", "ライトの調整");
	lights_[0]->spotLights_[1]->light_->intensity = stageEditor_->GetFloatValue("内のライトの輝度", "ライトの調整");
	lights_[0]->spotLights_[1]->light_->decay = stageEditor_->GetFloatValue("内のライトの減衰率", "ライトの調整");
	lights_[0]->spotLights_[1]->light_->cosAngle = stageEditor_->GetFloatValue("内のライトの開く角度", "ライトの調整");
	lights_[0]->spotLights_[1]->light_->cosFalloffStart = stageEditor_->GetFloatValue("内のライトの減衰し始める角度", "ライトの調整");
	lights_[0]->spotLights_[1]->light_->distance = stageEditor_->GetFloatValue("内のライトの距離", "ライトの調整");

	for (uint32_t i = 0; i < 2; i++) {
		lights_[0]->spotLights_[i]->light_->cosAngle = std::clamp<float>(lights_[0]->spotLights_[i]->light_->cosAngle, -1.0f, 0.998f);
		lights_[0]->spotLights_[i]->light_->cosFalloffStart = std::clamp<float>(lights_[0]->spotLights_[i]->light_->cosFalloffStart, lights_[0]->spotLights_[i]->light_->cosAngle + 0.001f, 0.999f);

		if (lights_[0]->spotLights_[i]->light_->distance <= 0.0f) {
			lights_[0]->spotLights_[i]->light_->distance = 0.01f;
		}
	}

	stageEditor_->SetVariable("外のライトの開く角度", lights_[0]->spotLights_[0]->light_->cosAngle, "ライトの調整");
	stageEditor_->SetVariable("外のライトの減衰し始める角度", lights_[0]->spotLights_[0]->light_->cosFalloffStart, "ライトの調整");
	stageEditor_->SetVariable("外のライトの距離", lights_[0]->spotLights_[0]->light_->distance, "ライトの調整");
	stageEditor_->SetVariable("内のライトの開く角度", lights_[0]->spotLights_[1]->light_->cosAngle, "ライトの調整");
	stageEditor_->SetVariable("内のライトの減衰し始める角度", lights_[0]->spotLights_[1]->light_->cosFalloffStart, "ライトの調整");
	stageEditor_->SetVariable("内のライトの距離", lights_[0]->spotLights_[1]->light_->distance, "ライトの調整");

	*lights_[1]->spotLights_[0]->light_ = *lights_[0]->spotLights_[0]->light_;
	lights_[1]->spotLights_[0]->light_->direction.x *= -1;
	*lights_[1]->spotLights_[1]->light_ = *lights_[0]->spotLights_[1]->light_;
	lights_[1]->spotLights_[1]->light_->direction.x *= -1;
	lights_[1]->box_->transform_ = lights_[0]->box_->transform_;
	lights_[1]->box_->transform_.translate_.x *= -1;
	lights_[1]->box_->transform_.rotate_.y *= -1;
	lights_[1]->box_->transform_.rotate_.z *= -1;

	lights_[0]->Update(0.0f);
	lights_[1]->Update(0.0f);
}
