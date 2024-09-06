#pragma once
#include "SpotLight/SpotLight.h"
#include <memory>
#include <array>
#include "GameElement/Live/SpotLightBox/TwinSpotLightBox.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Camera;

class StageLights {
public: 

	StageLights();

	void Initialize();

	void Update(const float time);

	void Draw(Camera* camera);

private:
	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void SetLoadLight();
	void LoadLightUpadate(const float time);

private:
	
	std::array<std::array<std::unique_ptr<SpotLight>, 2>, 5> spotLights_;

	std::array<std::unique_ptr<TwinSpotLightBox>, 4> loadLights_;

	enum FloatLoadLight {
		kRadius,
		kPositionInterval,
		kRotateTime,
		kEndFloatLoadLight,
	};
	float floatLoadLightInfo_[kEndFloatLoadLight];
	std::string floatLoadLightName[kEndFloatLoadLight] = {
		"回転の半径",
		"位置の間隔",
		"1回転の時間",
	};
	enum Vector3LoadLight {
		kPosition,
		kDirection,
		kEndVector3LoadLight,
	};
	Vector3 vector3LoadLightInfo_[kEndVector3LoadLight];
	std::string vector3LoadLightName[kEndVector3LoadLight] = {
		"ポジション",
		"向き",
	};

	std::unique_ptr<GlobalVariableUser> loadLightGlobal_;
};
