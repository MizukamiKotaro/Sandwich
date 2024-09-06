#include "SpotLightBoxAnimation.h"
#include "Camera.h"
#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

const int kSpotNum = 2;

SpotLightBoxAnimation::SpotLightBoxAnimation(const std::string& name)
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

	spotLights_[1]->light_->color = { 44.0f / 255.0f,44.0f/ 255.0f,44.0f / 255.0f,1.0f };
	spotLights_[1]->light_->intensity = 22.0f;
	spotLights_[1]->light_->decay = 3.2f;
	spotLights_[1]->light_->cosAngle = 0.97f;
	spotLights_[1]->light_->cosFalloffStart = 0.99f;
	spotLights_[1]->light_->distance = 23.0f;

	box_->SetLight(point_.get());
	box_->SetLight(spotLights_[0].get());
	
	global_ = std::make_unique<GlobalVariableUser>("SpotLight", name);
	SetGlobalVariable();
	LightUpdate();
}

void SpotLightBoxAnimation::Initialize()
{
}

void SpotLightBoxAnimation::Update(const float& time)
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	/*ImGui::Begin("SpotLightBox");
	ImGui::DragFloat3("位置", &box_->transform_.translate_.x, 0.01f);
	ImGui::SliderFloat3("角度", &spotLights_[0]->light_->direction.x, -1.0f, 1.0f);
	ImGui::ColorEdit3("外のスポットライトの色", &spotLights_[0]->light_->color.x);
	ImGui::SliderFloat("外のスポットライトの輝度", &spotLights_[0]->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("外のスポットライトの開く減衰率", &spotLights_[0]->light_->decay, 0.0f, 100.0f);
	ImGui::SliderFloat("外のスポットライトの開く角度", &spotLights_[0]->light_->cosAngle, -1.0f, 1.0f);
	ImGui::SliderFloat("外のスポットライトの減衰し始める角度", &spotLights_[0]->light_->cosFalloffStart, -1.0f, 1.0f);
	ImGui::SliderFloat("外のスポットライトの距離", &spotLights_[0]->light_->distance, 0.0f, 100.0f);
	ImGui::ColorEdit3("内のスポットライトの色", &spotLights_[1]->light_->color.x);
	ImGui::SliderFloat("内のスポットライトの輝度", &spotLights_[1]->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("内のスポットライトの開く減衰率", &spotLights_[1]->light_->decay, 0.0f, 100.0f);
	ImGui::SliderFloat("内のスポットライトの開く角度", &spotLights_[1]->light_->cosAngle, -1.0f, 1.0f);
	ImGui::SliderFloat("内のスポットライトの減衰し始める角度", &spotLights_[1]->light_->cosFalloffStart, -1.0f, 1.0f);
	ImGui::SliderFloat("内のスポットライトの距離", &spotLights_[1]->light_->distance, 0.0f, 100.0f);
	ImGui::End();*/
#endif // _DEBUG

	box_->Update(time);

	LightUpdate();
}

void SpotLightBoxAnimation::Draw(Camera* camera)
{
	box_->Draw(*camera);
}

void SpotLightBoxAnimation::DrawLight(const Camera& camera)
{	
	//point_->Draw(*camera);
	spotLights_[0]->Draw(camera);
	spotLights_[1]->Draw(camera, BlendMode::kBlendModeAdd);
}

void SpotLightBoxAnimation::SetGlobalVariable()
{
	global_->AddItem("位置", box_->transform_.translate_);
	global_->AddItem("角度", box_->transform_.rotate_);
	ApplyGlobalVariable();
}

void SpotLightBoxAnimation::ApplyGlobalVariable()
{
	box_->transform_.translate_ = global_->GetVector3Value("位置");
	box_->transform_.rotate_ = global_->GetVector3Value("角度");
	box_->Update(0.0f);
}

void SpotLightBoxAnimation::LightUpdate()
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
