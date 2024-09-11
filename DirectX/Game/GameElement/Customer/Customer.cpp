#include "Customer.h"
#include "FrameInfo/FrameInfo.h"

void Customer::Init()
{
	customerTexture.push_back("customer.png");
	customerTexture.push_back("customerClose.png");
	customerTexture.push_back("customerEat.png");

	for (std::string tex : customerTexture) {
		TextureManager::GetInstance()->LoadTexture(tex);
	}

	object_ = std::make_unique<Object>(customerTexture[0]);
	
	//ランダム生成器のインスタンスを生成
	random = RandomGenerator::GetInstance();

	object_->model->transform_.scale_ = scale_;
	object_->model->transform_.translate_ = translate_;

	global = std::make_unique<GlobalVariableUser>("Character", "Customer");

	SetGlobalVariables();
}

void Customer::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif


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
	}
#pragma endregion BehaviorTree


	object_->model->transform_.scale_ = scale_;
	object_->model->transform_.translate_ = translate_;

	object_->Update();
}

void Customer::Draw(const Camera* camera)
{
	if (isDraw) {
		object_->Draw(*camera);
	}

}

void Customer::RootInit()
{
	raffleFlame = 0.0f;

	currentTexture = 0;
	object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(customerTexture[currentTexture]));
}

void Customer::RootUpdate()
{	
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
		}

		countBlink++;
		blinkFrame = 0.0f;
		kBlinkInterval = random->RandFloat(kBlinkMinInterval, kBlinkMaxInterval);
		if (currentTexture == 0) {
			currentTexture = 1;
		}
		else {
			currentTexture = 0;
		}
	}
	//テクスチャを変える
	object_->model->SetTexture(TextureManager::GetInstance()->LoadTexture(customerTexture[currentTexture]));
	

}

void Customer::EatInit()
{
}

void Customer::EatUpdate()
{
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


}