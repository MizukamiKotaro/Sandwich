#include "SpotLightBox.h"
#include "Camera.h"
#ifdef _DEBUG
#include "ImGuiManager/ImGuiManager.h"
#endif // _DEBUG

const int kSpotNum = 2;

SpotLightBox::SpotLightBox()
{
	box_ = std::make_unique<Model>("SpotLightBox");
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
	
	/*Update();*/
}

void SpotLightBox::Initialize()
{
}

void SpotLightBox::Update()
{
#ifdef _DEBUG
	ImGui::Begin("SpotLightBoxad");
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
	ImGui::End();
#endif // _DEBUG

	spotLights_[1]->light_->direction = spotLights_[0]->light_->direction;
	spotLights_[1]->light_->distance = spotLights_[0]->light_->distance;
	box_->transform_.SetQuaRot(Quaternion::DirectionToDirection(Vector3{ 1.0f,0.0f,0.0f }, spotLights_[0]->light_->direction));
	box_->Update();
	point_->light_->position = box_->transform_.translate_;
	point_->Update();
	for (int i = 0; i < kSpotNum; i++) {
		spotLights_[i]->light_->position = box_->transform_.translate_;
		spotLights_[i]->isDraw_ = isDraw_;
		spotLights_[i]->Update();
	}
}

void SpotLightBox::Draw(Camera* camera)
{
	box_->Draw(*camera);
}

void SpotLightBox::DrawLight(const Camera& camera)
{
	//point_->Draw(*camera);
	spotLights_[0]->Draw(camera);
	spotLights_[1]->Draw(camera, BlendMode::kBlendModeAdd);
}
