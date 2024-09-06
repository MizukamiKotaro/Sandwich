#include "StageLights.h"
#include "Camera.h"
#include "TextureManager.h"
#include "RandomGenerator/RandomGenerator.h"
#include <algorithm>

StageLights::StageLights()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			spotLights_[i][j] = std::make_unique<SpotLight>();
			spotLights_[i][j]->light_->position = { 2.0f,-1.25f,-10.0f + 5.0f * i };
			spotLights_[i][j]->light_->direction = { 1.0f,1.0f,0.0f };
			spotLights_[i][j]->light_->intensity = 4.0f;
			spotLights_[i][j]->light_->decay = 2.0f;

			if (j != 0) {
				spotLights_[i][j]->light_->position.x *= -1;
				spotLights_[i][j]->light_->direction.x *= -1;
			}

			spotLights_[i][j]->Update();
		}
	}

	loadLightGlobal_ = std::make_unique<GlobalVariableUser>("Live", "LoadLight");
	loadLightGlobal_->CreateGroup();
	for (int i = 0; i < kEndFloatLoadLight; i++) {
		floatLoadLightInfo_[i] = 0.1f;
	}
	for (int i = 0; i < kEndVector3LoadLight; i++) {
		vector3LoadLightInfo_[i] = { 0.0f,-1.0,0.0f };
	}

	for (std::unique_ptr<TwinSpotLightBox>& light : loadLights_) {
		light = std::make_unique<TwinSpotLightBox>();
	}

	SetGlobalVariable();
}

void StageLights::Initialize() {

	RandomGenerator* rand = RandomGenerator::GetInstance();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			if (j == 0) {
				spotLights_[i][j]->light_->position = { rand->RandFloat(0.1f,5.0f),rand->RandFloat(-5.0f,5.0f),-10.0f + 5.0f * i };
				if (spotLights_[i][j]->light_->position.y < 0) {
					spotLights_[i][j]->light_->direction = { rand->RandFloat(-0.6f,0.6f),rand->RandFloat(0.2f,1.0f),0.0f };
				}
				else {
					spotLights_[i][j]->light_->direction = { rand->RandFloat(-0.6f,0.6f),rand->RandFloat(-1.0f,-0.2f),0.0f };
				}
				spotLights_[i][j]->light_->intensity = 7.0f - 6.0f;
				spotLights_[i][j]->light_->decay = 2.0f;
				spotLights_[i][j]->light_->distance = 35.0f + rand->RandFloat(-7.0f, 4.0f);
				spotLights_[i][j]->light_->cosAngle = 0.7f + rand->RandFloat(-0.03f,0.3f);
				spotLights_[i][j]->light_->cosFalloffStart = 0.8f + rand->RandFloat(-0.01f, 0.05f);
				spotLights_[i][j]->light_->color = { rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),1.0f };
			}
			else {
				*spotLights_[i][j]->light_ = *spotLights_[i][0]->light_;

				spotLights_[i][j]->light_->position.x *= -1;
				spotLights_[i][j]->light_->direction.x *= -1;
			}

			spotLights_[i][j]->Update();
		}
	}

	SetLoadLight();
	Update(0.0f);
}

void StageLights::Update(const float time) {

#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG


	if (time > 4.0f && time < 11.0f) {
		spotLights_[0][0]->light_->intensity += 0.01f;
		spotLights_[0][1]->light_->intensity += 0.01f;
	}
	if (time > 4.5f && time < 11.5f) {
		spotLights_[1][0]->light_->intensity += 0.01f;
		spotLights_[1][1]->light_->intensity += 0.01f;
	}
	if (time > 5.0f && time < 12.0f) {
		spotLights_[2][0]->light_->intensity += 0.01f;
		spotLights_[2][1]->light_->intensity += 0.01f;
	}
	if (time > 5.5f && time < 12.5f) {
		spotLights_[3][0]->light_->intensity += 0.01f;
		spotLights_[3][1]->light_->intensity += 0.01f;
	}
	if (time > 6.0f && time < 13.0f) {
		spotLights_[4][0]->light_->intensity += 0.01f;
		spotLights_[4][1]->light_->intensity += 0.01f;
	}

	if (time > 30.0f && time < 30.5f) {
		RandomGenerator* rand = RandomGenerator::GetInstance();
		int hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = true;
		spotLights_[hoge][1]->isDraw_ = true;
		Vector4 color = { rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),1.0f };
		spotLights_[hoge][0]->light_->color = color;
		spotLights_[hoge][1]->light_->color = color;
		hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = false;
		spotLights_[hoge][1]->isDraw_ = false;

	}
	if (time > 31.0f && time < 31.5f) {
		RandomGenerator* rand = RandomGenerator::GetInstance();
		int hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = true;
		spotLights_[hoge][1]->isDraw_ = true;
		Vector4 color = { rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),1.0f };
		spotLights_[hoge][0]->light_->color = color;
		spotLights_[hoge][1]->light_->color = color;
		hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = false;
		spotLights_[hoge][1]->isDraw_ = false;
		hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = false;
		spotLights_[hoge][1]->isDraw_ = false;
	}
	if (time > 32.0f && time < 32.5f) {
		RandomGenerator* rand = RandomGenerator::GetInstance();
		int hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = true;
		spotLights_[hoge][1]->isDraw_ = true;
		Vector4 color = { rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),1.0f };
		spotLights_[hoge][0]->light_->color = color;
		spotLights_[hoge][1]->light_->color = color;
		hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = false;
		spotLights_[hoge][1]->isDraw_ = false;
		hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = false;
		spotLights_[hoge][1]->isDraw_ = false;
		hoge = rand->RandInt(0, 5);
		spotLights_[hoge][0]->isDraw_ = false;
		spotLights_[hoge][1]->isDraw_ = false;
	}
	if (time > 35.0f && time < 35.3f) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 2; j++) {

				spotLights_[i][j]->isDraw_ = false;
			}
		}
	}

	if (time > 35.3f) {
		RandomGenerator* rand = RandomGenerator::GetInstance();
		if (!spotLights_[0][0]->isDraw_) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 2; j++) {
					spotLights_[i][j]->light_->color = { rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),rand->RandFloat(0.0f, 1.0f),1.0f };
					spotLights_[i][j]->isDraw_ = true;
				}
			}
		}
	}


	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {

			if (j != 0) {
				*spotLights_[i][j]->light_ = (*spotLights_[i][0]->light_);

				spotLights_[i][j]->light_->position.x *= -1;
				spotLights_[i][j]->light_->direction.x *= -1;
			}

			spotLights_[i][j]->Update();
		}
	}

	LoadLightUpadate(time);
}

void StageLights::Draw(Camera* camera) {

	/*for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			spotLights_[i][j]->Draw(*camera);
		}
	}*/

	for (std::unique_ptr<TwinSpotLightBox>& light : loadLights_) {
		light->Draw(camera);
	}
}

void StageLights::SetGlobalVariable()
{
	for (int i = 0; i < kEndFloatLoadLight; i++) {
		loadLightGlobal_->AddItem(floatLoadLightName[i], floatLoadLightInfo_[i]);
	}
	for (int i = 0; i < kEndVector3LoadLight; i++) {
		loadLightGlobal_->AddItem(vector3LoadLightName[i], vector3LoadLightInfo_[i]);
	}

	ApplyGlobalVariable();
}

void StageLights::ApplyGlobalVariable()
{
	for (int i = 0; i < kEndFloatLoadLight; i++) {
		floatLoadLightInfo_[i] = loadLightGlobal_->GetFloatValue(floatLoadLightName[i]);
	}
	floatLoadLightInfo_[FloatLoadLight::kRadius] = std::clamp(floatLoadLightInfo_[FloatLoadLight::kRadius], 0.0f, 0.99f);
	for (int i = 0; i < kEndVector3LoadLight; i++) {
		vector3LoadLightInfo_[i] = loadLightGlobal_->GetVector3Value(vector3LoadLightName[i]);
	}

	SetLoadLight();
}

void StageLights::SetLoadLight()
{
	for (int i = 0; i < int(loadLights_.size()); i++) {
		loadLights_[i]->lights_[0]->box_->transform_.translate_ = vector3LoadLightInfo_[Vector3LoadLight::kPosition];
		loadLights_[i]->lights_[0]->box_->transform_.translate_.z -= floatLoadLightInfo_[FloatLoadLight::kPositionInterval] * i;
		loadLights_[i]->Update();
	}
}

void StageLights::LoadLightUpadate(const float time)
{
	float t = std::fmodf(time, floatLoadLightInfo_[FloatLoadLight::kRotateTime]) / floatLoadLightInfo_[FloatLoadLight::kRotateTime];
	float rotate = 6.28f * t;

	Vector3 direction = { floatLoadLightInfo_[FloatLoadLight::kRadius] * std::cosf(rotate),-1.0f,floatLoadLightInfo_[FloatLoadLight::kRadius] * std::sinf(rotate) };
	direction = direction.Normalize() * Matrix4x4::DirectionToDirection({0.0f,-1.0f,0.0f}, vector3LoadLightInfo_[Vector3LoadLight::kDirection].Normalize());

	for (int i = 0; i < int(loadLights_.size()); i++) {
		loadLights_[i]->lights_[0]->spotLights_[0]->light_->direction = direction.Normalize();
		loadLights_[i]->Update();
	}
}
