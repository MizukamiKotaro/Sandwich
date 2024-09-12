#include "ResultUI.h"
#include "WindowsInfo/WindowsInfo.h"
#include "GameElement/GameManager/GameManager.h"

ResultUI::ResultUI()
{
	screenSize_ = WindowsInfo::GetInstance()->GetWindowSize();
	gameManager_ = GameManager::GetInstance();
	Create();

	global_ = std::make_unique<GlobalVariableUser>("UI_AdjustmentItems", "Result");
	SetGlobalVariables();
	input_ = Input::GetInstance();
}

void ResultUI::Initialize(const bool& isClear)
{
	isClear_ = isClear;
	yesNoSpriteNum_ = 0;
	if (isClear_) {
		ClearInitialize();
	}
	else {
		GameOverInitialize();
	}
}

void ResultUI::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG
	if (gameManager_->GetScene() == GameManager::kResult) {
		if (input_->PressedKey(DIK_DOWN) || input_->PressedKey(DIK_S)) {
			yesNoSpriteNum_--;
		}
		else if (input_->PressedKey(DIK_UP) || input_->PressedKey(DIK_W)) {
			yesNoSpriteNum_++;
		}
		if (yesNoSpriteNum_ < 0) {
			yesNoSpriteNum_ = yesNoSpriteMaxNum_;
		}
		else if (yesNoSpriteNum_ > yesNoSpriteMaxNum_) {
			yesNoSpriteNum_ = 0;
		}
		for (int32_t i = 0; i < yesNoSpriteMaxNum_ + 1; i++) {
			if (i == yesNoSpriteNum_) {
				sprites_[i]->SetTexture(textures_[i * 2], false);
			}
			else {
				sprites_[i]->SetTexture(textures_[i * 2 + 1], false);
			}
		}

		if (input_->PressedKey(DIK_SPACE)) {
			if (yesNoSpriteNum_ == SpriteNameEnum::kRestart) {
				gameManager_->ChangeScene(GameManager::kGame);
				gameManager_->CompletedTransition();
			}
			else {
				gameManager_->ChangeScene(GameManager::kTitle);
				gameManager_->CompletedTransition();
			}
		}
	}
	deltaTime;
}

void ResultUI::Draw()
{
	if (gameManager_->GetScene() == GameManager::kResult) {
		if (isClear_) {
			for (size_t i = 0; i < sprites_.size() - 1; i++) {
				sprites_[i]->Draw();
			}
		}
		else {
			for (size_t i = 0; i < sprites_.size(); i++) {
				sprites_[i]->Draw();
			}
		}
	}
}

void ResultUI::ClearInitialize()
{
	yesNoSpriteMaxNum_ = 1;
}

void ResultUI::GameOverInitialize()
{
	yesNoSpriteMaxNum_ = 2;
}

void ResultUI::Create()
{
	TextureManager* texMa = TextureManager::GetInstance();
	textures_.resize(8);
	textures_[0] = texMa->LoadTexture("restartUi.png");
	textures_[1] = texMa->LoadTexture("notRestartUi.png");
	textures_[2] = texMa->LoadTexture("titleUi.png");
	textures_[3] = texMa->LoadTexture("notTitleUi.png");
	textures_[4] = texMa->LoadTexture("gameClear.png");
	textures_[5] = texMa->LoadTexture("gameOver.png");
	textures_[6] = texMa->LoadTexture("continueUi.png");
	textures_[7] = texMa->LoadTexture("notContinueUi.png");

	sprites_.resize(SpriteNameEnum::kMaxNameEnum);
	gameOverBasePoses_.resize(SpriteNameEnum::kMaxNameEnum);
	clearBasePoses_.resize(SpriteNameEnum::kMaxNameEnum - 1);

	Vector2 pos = { screenSize_.x * 0.5f, screenSize_.y * 0.2f };

	for (int32_t i = 0; i < SpriteNameEnum::kMaxNameEnum; i++) {
		sprites_[i] = std::make_unique<Sprite>(textures_[i * 2], pos);
		gameOverBasePoses_[i] = pos;

		if (i <= SpriteNameEnum::kMaxNameEnum - 2) {
			clearBasePoses_[i] = pos;
			pos.y += 120.0f;
		}
	}

	putData_.resize(SpriteScaleEnum::kMaxScaleNum);
	putData_[SpriteScaleEnum::kClear].baseScale = sprites_[SpriteNameEnum::kResult]->size_;
	putData_[SpriteScaleEnum::kClear].scale = 1.0f;
	putData_[SpriteScaleEnum::kGameOver].baseScale = sprites_[SpriteNameEnum::kResult + 1]->size_;
	putData_[SpriteScaleEnum::kGameOver].scale = 1.0f;
	putData_[SpriteScaleEnum::kOthers].baseScale = sprites_[SpriteNameEnum::kRestart]->size_;
	putData_[SpriteScaleEnum::kOthers].scale = 1.0f;

	scaleNames_.resize(SpriteScaleEnum::kMaxScaleNum);
	scaleNames_[SpriteScaleEnum::kClear] = "クリア";
	scaleNames_[SpriteScaleEnum::kGameOver] = "ゲームオーバー";
	scaleNames_[SpriteScaleEnum::kOthers] = "その他";

	names_.resize(SpriteNameEnum::kMaxNameEnum);
	names_[SpriteNameEnum::kRestart] = "はじめから";
	names_[SpriteNameEnum::kTitle] = "タイトル";
	names_[SpriteNameEnum::kResult] = "リザルト";
	names_[SpriteNameEnum::kContinue] = "つづきから";
}

void ResultUI::SetGlobalVariables()
{
	for (int32_t i = 0; i < SpriteNameEnum::kMaxNameEnum; i++) {
		global_->AddItem(names_[i] + "の位置", gameOverBasePoses_[i], "ゲームオーバー");
		if (i <= SpriteNameEnum::kMaxNameEnum - 2) {
			global_->AddItem(names_[i] + "の位置", clearBasePoses_[i], "クリア");
		}
		if (i <= SpriteScaleEnum::kMaxScaleNum - 1) {
			global_->AddItem(scaleNames_[i] + "のスケール", putData_[i].scale, "スケール");
		}
	}
	ApplyGlobalVariables();
}

void ResultUI::ApplyGlobalVariables()
{
	for (int32_t i = 0; i < SpriteNameEnum::kMaxNameEnum; i++) {
		gameOverBasePoses_[i] = global_->GetVector2Value(names_[i] + "の位置", "ゲームオーバー");
		if (i <= SpriteNameEnum::kMaxNameEnum - 2) {
			clearBasePoses_[i] = global_->GetVector2Value(names_[i] + "の位置", "クリア");
		}
		if (i <= SpriteScaleEnum::kMaxScaleNum - 1) {
			putData_[i].scale = global_->GetFloatValue(scaleNames_[i] + "のスケール", "スケール");
		}

		if (!isClear_) {
			sprites_[i]->pos_ = gameOverBasePoses_[i];
		}
		else {
			if (i <= SpriteNameEnum::kMaxNameEnum - 2) {
				sprites_[i]->pos_ = clearBasePoses_[i];
			}
		}
	}

	for (int32_t i = 0; i < SpriteNameEnum::kMaxNameEnum; i++) {
		if (i != SpriteNameEnum::kResult) {
			sprites_[i]->size_ = putData_[SpriteScaleEnum::kOthers].baseScale * putData_[SpriteScaleEnum::kOthers].scale;
		}
		else {
			if (isClear_) {
				sprites_[i]->size_ = putData_[SpriteScaleEnum::kClear].baseScale * putData_[SpriteScaleEnum::kClear].scale;
			}
			else {
				sprites_[i]->size_ = putData_[SpriteScaleEnum::kGameOver].baseScale * putData_[SpriteScaleEnum::kGameOver].scale;
			}
		}
		sprites_[i]->Update();
	}
}
