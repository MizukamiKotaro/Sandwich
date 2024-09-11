#pragma once
#include "GameElement/Object/Object.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "RandomGenerator/RandomGenerator.h"

enum class Behavior {
	kRoot,
	kBlink,
	kEat
};

class Customer
{
public:

	void Init();
	void Update();
	void Draw(const Camera* camera);

	bool isDraw = false;
private:

	void RootInit();
	void RootUpdate();
	void BlinkInit();
	void BlinkUpdate();
	void EatInit();
	void EatUpdate();

	void SetGlobalVariables();
	void ApplyGlobalVariables();

	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//3Dモデル
	std::unique_ptr<Object> object_;

	std::vector<std::string> customerTexture;

	Vector3 scale_ = { 19.0f,2.0f,1.0f };
	Vector3 translate_ = { 0.0f,-20.0f,0.0f };

	//抽選をするフレーム
	float raffleInterval = 10.0f;
	float raffleFlame;
	//あたりの確率
	int persent = 100;
	//ランダムで抽選した結果
	int raffleResult;

	//瞬きの間隔
	float kBlinkInterval;
	float kBlinkMinInterval = 0.5f;
	float kBlinkMaxInterval = 1.0f;
	float blinkFrame;
	int currentTexture;
	int countBlink;
	int kcountBlink;
	int blinkMinTime = 1;
	int blinkMaxTime = 5;

	std::unique_ptr<GlobalVariableUser> global;
	RandomGenerator* random;
};

