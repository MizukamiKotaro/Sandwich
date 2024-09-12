#include "Score.h"
#include "WindowsInfo/WindowsInfo.h"
#include "GameElement/Player/Player.h"

Score* Score::GetInstance()
{
	static Score instance;
	return &instance;
}

void Score::FirstInitialize()
{
	customerNum_ = 1;
	num_ = 0;
	maxNum_ = 100;
	addNum_ = 0;
	screenPos_ = WindowsInfo::GetInstance()->GetWindowSize() * 0.5f;

	post_ = std::make_unique<PostEffect>();
	sprite_ = std::make_unique<Sprite>(screenPos_);
	sprite_->SetSRVGPUDescriptorHandle_(post_->GetSRVGPUDescriptorHandle());

	CreateSprites();

	Vector2 size = { 36.0f,36.0f };
	drawNum_ = std::make_unique<DrawNumbers>("equipmentsNumbers.png", "Frame", "食べた数字", size);
	drawMaxNum_ = std::make_unique<DrawNumbers>("equipmentsNumbers.png", "Frame", "最大の数字", size);
	drawCustomerNum_ = std::make_unique<DrawNumbers>("equipmentsNumbers.png", "Frame", "客の数字", size);
	drawAddNum_ = std::make_unique<DrawNumbers>("equipmentsNumbers.png", "Frame", "プラスの数字", size);

	glo_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "Frame");
	SetGlobalVariables();
}

void Score::SetPlayer(const Player* player)
{
	player_ = player;
}

void Score::Initialize()
{
	customerNum_ = 1;
	num_ = 0;
	addNum_ = 0;
	preDrop_ = false;
}

void Score::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariables();
	drawNum_->Update();
	drawAddNum_->Update();
	drawCustomerNum_->Update();
	drawMaxNum_->Update();
#endif // _DEBUG
	if (player_) {
		if (player_->GetIsDrop()) {
			preDrop_ = true;
		}
		else if (preDrop_) {
			preDrop_ = false;
			AddNum();
		}
	}
	deltaTime;
	DrawSprite();
}

void Score::Draw()
{
	sprite_->Draw();
}

void Score::AddAddNum()
{
	addNum_++;
	if (addNum_ == 1000) {
		addNum_ = 999;
	}
}

void Score::AddNum()
{
	num_ += addNum_;
	addNum_ = 0;
	if (num_ >= maxNum_) {
		num_ = 0;
		customerNum_++;
	}
}

void Score::CreateSprites()
{
	Vector2 pos = WindowsInfo::GetInstance()->GetWindowSize() * 0.5f;
	sprites_.resize(SpriteNames::kMaxSpriteNames);
	baseDatas_.resize(SpriteNames::kMaxSpriteNames);

	arrow_ = std::make_unique<Sprite>("bounusArrow.png", pos);
	arrow_->SetIsFlipX(true);

	sprites_[SpriteNames::kFrame] = std::make_unique<Sprite>("gameFram.png", pos);
	sprites_[SpriteNames::kBonus] = std::make_unique<Sprite>("bounusUi.png", pos);
	sprites_[SpriteNames::kOutSideFrameAdd] = std::make_unique<Sprite>("outsideFram.png", pos);
	sprites_[SpriteNames::kOutSideFrame] = std::make_unique<Sprite>("outsideFram.png", pos);
	sprites_[SpriteNames::kOutSideFrameCustomer] = std::make_unique<Sprite>("outsideFram.png", pos);
	sprites_[SpriteNames::kPlus] = std::make_unique<Sprite>("plus.png", pos);
	sprites_[SpriteNames::kMaxFrame] = std::make_unique<Sprite>("maxFram.png", pos);
	sprites_[SpriteNames::kMax] = std::make_unique<Sprite>("maxUi.png", pos);
	sprites_[SpriteNames::kLine] = std::make_unique<Sprite>("equipmentsNumberLine.png", pos);
	sprites_[SpriteNames::kArrow] = std::make_unique<Sprite>("bounusArrow.png", pos);
	sprites_[SpriteNames::kCustomerNum] = std::make_unique<Sprite>("nowCustomerNumber.png", pos);

	for (int32_t i = 0; i < SpriteNames::kMaxSpriteNames; i++) {
		baseDatas_[i].basePos = pos;
		baseDatas_[i].baseScale = sprites_[i]->size_;
		baseDatas_[i].scale = 1.0f;
	}

	names_.resize(SpriteNames::kMaxSpriteNames);
	names_[SpriteNames::kFrame] = "全体の枠";
	names_[SpriteNames::kBonus] = "ボーナス";
	names_[SpriteNames::kOutSideFrameAdd] = "加算分の枠";
	names_[SpriteNames::kOutSideFrame] = "今の数の枠";
	names_[SpriteNames::kOutSideFrameCustomer] = "客の枠";
	names_[SpriteNames::kPlus] = "プラス";
	names_[SpriteNames::kMaxFrame] = "最大の枠";
	names_[SpriteNames::kMax] = "最大";
	names_[SpriteNames::kLine] = "分数の棒";
	names_[SpriteNames::kCustomerNum] = "人目";
	names_[SpriteNames::kArrow] = "ボーナスライン";
}

void Score::SetGlobalVariables()
{
	glo_->AddItem("最大数", maxNum_);

	for (int32_t i = 0; i < SpriteNames::kMaxSpriteNames; i++) {
		glo_->AddItem(names_[i] + "のスケール", 1.0f, "配置", names_[i]);
		glo_->AddItem(names_[i] + "の座標", baseDatas_[i].basePos, "配置", names_[i]);
	}
	ApplyGlobalVariables();
}

void Score::ApplyGlobalVariables()
{
	maxNum_ = glo_->GetIntValue("最大数");
	for (int32_t i = 0; i < SpriteNames::kMaxSpriteNames; i++) {
		baseDatas_[i].scale = glo_->GetFloatValue(names_[i] + "のスケール", "配置", names_[i]);
		baseDatas_[i].basePos = glo_->GetVector2Value(names_[i] + "の座標", "配置", names_[i]);
		sprites_[i]->pos_ = baseDatas_[i].basePos;
		sprites_[i]->size_ = baseDatas_[i].baseScale * baseDatas_[i].scale;
		sprites_[i]->Update();
	}

	arrow_->pos_ = sprites_[SpriteNames::kArrow]->pos_;
	arrow_->pos_.x += (screenPos_.x - arrow_->pos_.x) * 2.0f;
	arrow_->size_ = sprites_[SpriteNames::kArrow]->size_;
	arrow_->Update();
}

void Score::DrawSprite()
{
	post_->PreDrawScene();
	for (int32_t i = 0; i < SpriteNames::kMaxSpriteNames; i++) {
		sprites_[i]->Draw();
	}
	arrow_->Draw();
	drawNum_->Draw(num_);
	drawMaxNum_->Draw(maxNum_);
	drawAddNum_->Draw(addNum_);
	drawCustomerNum_->Draw(customerNum_);
	post_->PostDrawScene();
}
