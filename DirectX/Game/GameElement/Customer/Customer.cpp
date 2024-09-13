#include "Customer.h"
#include "FrameInfo/FrameInfo.h"
#include "Game/GameElement/Player/Player.h"
#include "Game/GameElement/Score/Score.h"

void Customer::Init(Player* player)
{
	customerNumber = 0;
	customerTexture.resize(3);

	customerTexture[0].push_back("customer.png");
	customerTexture[0].push_back("customerClose.png");
	customerTexture[0].push_back("customerEat.png");
	
	customerTexture[1].push_back("customer2.png");
	customerTexture[1].push_back("customerClose2.png");
	customerTexture[1].push_back("customerEat2.png");
	
	customerTexture[2].push_back("customer3.png");
	customerTexture[2].push_back("customerClose3.png");
	customerTexture[2].push_back("customerEat3.png");

	for (std::string tex : customerTexture[0]) {
		TextureManager::GetInstance()->LoadTexture(tex);
	}
	for (std::string tex : customerTexture[1]) {
		TextureManager::GetInstance()->LoadTexture(tex);
	}
	for (std::string tex : customerTexture[2]) {
		TextureManager::GetInstance()->LoadTexture(tex);
	}

	object_ = std::make_unique<Object>(customerTexture[0][0]);

	player_ = player;
	
	//ランダム生成器のインスタンスを生成
	random = RandomGenerator::GetInstance();

	global = std::make_unique<GlobalVariableUser>("Character", "Customer");

	//パーティクル
	eatParticle_ = std::make_unique<EatParticle>();

	SetGlobalVariables();

	object_->model->transform_.scale_ = scale_;
	object_->model->transform_.translate_ = translate_;
	//SE
	EatSE = std::make_unique<Audio>();
	EatSE->Load("eat.mp3", "お客様が食べる音");
}

void Customer::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif
	//番号が違ったらお客さんが変わってる
	//フラグをtrueにする
	currentCustomerNum = Score::GetInstance()->GetCustomer();

	if (currentCustomerNum != preCustomerNum) {
		behaviorRequest_ = Behavior::kChange;
	}

	preCustomerNum = currentCustomerNum;

#pragma region
	//初期化
	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case Behavior::kRoot:
		default:
			RootInit();
			break;
		case Behavior::kBlink:
			BlinkInit();
			break;
		case Behavior::kEat:
			EatInit();
			break;
		case Behavior::kChange:
			ChangeInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
	//更新
	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		RootUpdate();
		break;
	case Behavior::kBlink:
		BlinkUpdate();
		break;
	case Behavior::kEat:
		EatUpdate();
		break;
	case Behavior::kChange:
		ChangeUpdate();
		break;
	}
#pragma endregion BehaviorTree

	if (canEatFlag) {
		if (player_->GetIsDrop()) {
			isPreEatFlag = true;
		}
	}
	else {
		controllEatFrame += FrameInfo::GetInstance()->GetDeltaTime();
		if (controllEatFrame > kcontrollEatFrame) {
			canEatFlag = true;
		}
	}

	//パーティクル
	eatParticle_->Update();

	object_->Update();
}

void Customer::Draw(const Camera* camera)
{	//パーティクル
	eatParticle_->Draw();
	if (isDraw) {
		object_->Draw(*camera);
	}

}

void Customer::RootInit()
{
	raffleFlame = 0.0f;

	currentTexture = 0;
	object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(customerTexture[customerNumber][currentTexture]));
}

void Customer::RootUpdate()
{	
	if (isPreEatFlag) {
		behaviorRequest_ = Behavior::kEat;
		return;
	}

	raffleFlame += FrameInfo::GetInstance()->GetDeltaTime();
	//特定のフレームに到達したら抽選を行う
	if (raffleFlame > raffleInterval) {
		raffleFlame = 0;
		raffleResult = random->RandInt(1, 100);

		if (persent >= raffleResult) {
			behaviorRequest_ = Behavior::kBlink;
		}
	}
}

void Customer::BlinkInit()
{
	blinkFrame = 0;
	currentTexture = 0;
	countBlink = 0;
	kcountBlink = random->RandInt(blinkMinTime, blinkMaxTime);
	kBlinkInterval = random->RandFloat(kBlinkMinInterval, kBlinkMaxInterval);
}

void Customer::BlinkUpdate()
{
	blinkFrame += FrameInfo::GetInstance()->GetDeltaTime();
	if (blinkFrame > kBlinkInterval) {
		
		if (countBlink >= kcountBlink) {
			behaviorRequest_ = Behavior::kRoot;
			return;
		}

		if (isPreEatFlag) {
			behaviorRequest_ = Behavior::kEat;
			return;
		}

		blinkFrame = 0.0f;
		kBlinkInterval = random->RandFloat(kBlinkMinInterval, kBlinkMaxInterval);
		if (currentTexture == 0) {
			currentTexture = 1;
			countBlink++;
		}
		else {
			currentTexture = 0;
		}
	}
	//テクスチャを変える
	object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(customerTexture[customerNumber][currentTexture]));
	

}

void Customer::EatInit()
{


	EatFrame = kEatInterval;
	currentTexture = 0;
	countEat = 0;
	//テクスチャを変える
	object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(customerTexture[customerNumber][currentTexture]));
}

void Customer::EatUpdate()
{
	if (player_->GetIsPlayerBack()) {
		isPreEatFlag = false;
		isEatFlag = true;

	}
	if (isEatFlag) {
		EatFrame += FrameInfo::GetInstance()->GetDeltaTime();
		if (EatFrame > kEatInterval) {

			if (countEat >= kcountEat) {
				isEatFlag = false;
				canEatFlag = false;
				controllEatFrame = 0.0f;
				behaviorRequest_ = Behavior::kRoot;
				return;
			}

			EatFrame = 0.0f;
			if (currentTexture == 0) {
				//パーティクル生成
				eatParticle_->Create();
				currentTexture = 2;
				EatSE->Play();
				countEat++;
			}
			else {
				currentTexture = 0;
			}
		}
		//テクスチャを変える
		object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(customerTexture[customerNumber][currentTexture]));
	}
}

void Customer::ChangeInit()
{
	changeFrame = 0;
	prePos = object_->model->transform_.translate_;
	targetPos = { prePos.x,prePos.y - 4.0f,prePos.z };
	IsFrameOver = false;

}

void Customer::ChangeUpdate()
{
	changeFrame += FrameInfo::GetInstance()->GetDeltaTime() * ratio;

	changeFrame = std::min(changeFrame, 1.0f);

	if (IsFrameOver == false) {
		object_->model->transform_.translate_ = Ease::UseEase(prePos, targetPos, changeFrame, Ease::EaseIn);
	}
	else {
		object_->model->transform_.translate_ = Ease::UseEase(targetPos, prePos, changeFrame, Ease::EaseIn);
	}
	//終了条件
	if (changeFrame >= 1.0f && IsFrameOver) {
		changeFrame = 0.0f;
		EatInit();
		isEatFlag = false;
		//Rootにする
		behaviorRequest_ = Behavior::kRoot;
	}
	//折り返し
	if (IsFrameOver == false && changeFrame >= 1.0f) {
		IsFrameOver = true;
		changeFrame = 0.0f;
		customerNumber = randomTextureSelect(preCoustomerNumber);
		preCoustomerNumber = customerNumber;
		currentTexture = 0;
		object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(customerTexture[customerNumber][currentTexture]));
	}
}

int Customer::randomTextureSelect(int PreTextture)
{
	currentTexture = random->RandInt(0, 3);
	if (currentTexture == PreTextture) {
		currentTexture = randomTextureSelect(PreTextture);
	}
	return currentTexture;
}

void Customer::SetGlobalVariables()
{
	global->AddItem("大きさ", scale_, "トランスフォーム");
	global->AddItem("移動", translate_, "トランスフォーム");

	global->AddItem("抽選するフレーム", raffleInterval, "瞬き","抽選");
	global->AddItem("当たる確率", persent, "瞬き","抽選");

	global->AddItem("間隔の最小値", kBlinkMinInterval, "瞬き","間隔");
	global->AddItem("間隔の最大値", kBlinkMaxInterval, "瞬き","間隔");
	global->AddItem("回数の最小値", blinkMinTime, "瞬き","間隔");
	global->AddItem("回数の最大値", blinkMaxTime, "瞬き","間隔");

	global->AddItem("抽選するフレーム", kEatInterval, "食べる動き","間隔");
	global->AddItem("回数", kcountEat, "食べる動き","間隔");

	ApplyGlobalVariables();
}

void Customer::ApplyGlobalVariables()
{
	scale_ = global->GetVector3Value("大きさ", "トランスフォーム");
	translate_ = global->GetVector3Value("移動", "トランスフォーム");

	raffleInterval = global->GetFloatValue("抽選するフレーム", "瞬き", "抽選");
	persent = global->GetIntValue("当たる確率", "瞬き", "抽選");

	kBlinkMinInterval = global->GetFloatValue("間隔の最小値", "瞬き", "間隔");
	kBlinkMaxInterval = global->GetFloatValue("間隔の最大値", "瞬き", "間隔");
	blinkMinTime = global->GetIntValue("回数の最小値", "瞬き","間隔");
	blinkMaxTime = global->GetIntValue("回数の最大値", "瞬き","間隔");

	kEatInterval = global->GetFloatValue("抽選するフレーム", "食べる動き", "間隔");
	kcountEat = global->GetIntValue("回数", "食べる動き", "間隔");


}