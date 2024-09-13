#pragma once
#include "GameElement/Object/Object.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "RandomGenerator/RandomGenerator.h"
#include "Game/GameElement/Particle/Eat/EatParticle.h"

class Player;
enum class Behavior {
	kRoot,
	kBlink,
	kEat,
	kChange
};

class Customer
{
public:

	void Init(Player* player);
	void Update();
	void Draw(const Camera* camera);

	bool isDraw = true;
private:

	void RootInit();
	void RootUpdate();
	void BlinkInit();
	void BlinkUpdate();
	void EatInit();
	void EatUpdate();
	void ChangeInit();
	void ChangeUpdate();

	int randomTextureSelect(int PreTextture);

	void SetGlobalVariables();
	void ApplyGlobalVariables();

	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//3Dモデル
	std::unique_ptr<Object> object_;

	std::vector<std::vector<std::string>> customerTexture;

	Player* player_;

	Vector3 scale_ = { 19.0f,2.0f,1.0f };
	Vector3 translate_ = { 0.0f,-16.820f,0.0f };

	//抽選をするフレーム
	float raffleInterval = 10.0f;
	float raffleFlame;
	//あたりの確率
	int persent = 100;
	//ランダムで抽選した結果
	int raffleResult;
	//現在のテクスチャ番号
	int currentTexture;
	//ひとつ前で選択されたテクスチャ
	int preCoustomerNumber = 0;
	//現在のお客様番号
	int customerNumber = 0;

#pragma region
	//瞬きの間隔
	float kBlinkInterval;
	float kBlinkMinInterval = 0.5f;
	float kBlinkMaxInterval = 1.0f;
	float blinkFrame;

	int countBlink;
	int kcountBlink;
	int blinkMinTime = 1;
	int blinkMaxTime = 5;
#pragma endregion 瞬き

#pragma region
	//事前のフラグ
	bool isPreEatFlag;
	//本フラグ
	bool isEatFlag;
	//食べる動きの間隔
	float kEatInterval = 0.5f;
	float EatFrame;
	int countEat;
	int kcountEat = 3;
#pragma endregion 食べる動き

#pragma region
	//お客様を切り替えるフラグ
	bool isChangeFlag;
	//現在のお客様番号
	int currentCustomerNum = 0;
	//過去のお客様番号
	int preCustomerNum;
	//0.5f超えたら
	bool IsFrameOver;
	//経過フレーム
	float changeFrame;
	//踏んだ時の戻る速度
	float ratio = 2.0f;

#pragma endregion お客様切り替え

	Vector3 prePos;
	Vector3 targetPos;


	//パーティクル
	std::unique_ptr<EatParticle> eatParticle_;

	std::unique_ptr<GlobalVariableUser> global;
	RandomGenerator* random;

};

