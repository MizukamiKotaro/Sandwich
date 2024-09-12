#include "ResultUI.h"
#include "WindowsInfo/WindowsInfo.h"
#include "GameElement/GameManager/GameManager.h"
#include "RandomGenerator/RandomGenerator.h"

ResultUI::ResultUI()
{
	screenSize_ = WindowsInfo::GetInstance()->GetWindowSize();
	gameManager_ = GameManager::GetInstance();
	Create();

	global_ = std::make_unique<GlobalVariableUser>("UIAdjustmentItems", "Result");
	input_ = Input::GetInstance();

	TextureManager* tm = TextureManager::GetInstance();
	equipmentTeses_.resize(4);
	equipmentTeses_[0] = tm->LoadTexture("egg.png");
	equipmentTeses_[1] = tm->LoadTexture("ham.png");
	equipmentTeses_[2] = tm->LoadTexture("lettuce.png");
	equipmentTeses_[3] = tm->LoadTexture("tomato.png");
	equipmentTeses_;
	plane_ = std::make_unique<Model>("plane");
	plane_->SetTexture(equipmentTeses_[0]);
	texNum_ = 0;
	time_ = 0.0f;
	isTitle_ = false;
	isGame_ = false;
	isTran_ = false;

	SetGlobalVariables();
}

void ResultUI::Initialize(const bool& isClear)
{
	isTran_ = false;
	isTranHalf_ = false;
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
		if (!isTran_) {
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
					isTran_ = true;
					isGame_ = true;
				}
				else {
					isTran_ = true;
					isTitle_ = true;
				}
			}
#ifdef _DEBUG
			plane_->transform_.translate_ = pos_;
			plane_->Update();
#endif // _DEBUG
		}
	}
#ifdef _DEBUG
	else {
		plane_->transform_.translate_ = pos_;
		plane_->Update();
	}
#endif // _DEBUG

	if(isTran_) {
		time_ = std::clamp(time_ + deltaTime, 0.0f, maxTime_);
		float t = time_ / maxTime_;
		plane_->transform_.translate_.x = (1.0f - t) * pos_.x - t * pos_.x;
		plane_->transform_.rotate_.z += rotate_ * deltaTime;
		plane_->Update();

		if (t >= 1.0f) {
			Initialize();
		}
		else if (t >= 0.5f) {
			isTranHalf_ = true;
			if (isTitle_) {
				gameManager_->ChangeScene(GameManager::kTitle);
				gameManager_->CompletedTransition();
			}
			else if (isGame_) {
				gameManager_->ChangeScene(GameManager::kGame);
				gameManager_->CompletedTransition();
			}
		}
	}
}

void ResultUI::Draw()
{
	if (gameManager_->GetScene() == GameManager::kResult && !isTranHalf_) {
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

void ResultUI::DrawPlane(const Camera& camera)
{
	plane_->Draw(camera);
}

const bool& ResultUI::GetIsTranHalf() const
{
	return isTranHalf_;
}

const bool& ResultUI::GetIsTitle() const
{
	return isTitle_;
}

const bool& ResultUI::GetIsGame() const
{
	return isGame_;
}

void ResultUI::ClearInitialize()
{
	yesNoSpriteMaxNum_ = 1;
	time_ = 0.0f;
	texNum_ = RandomGenerator::GetInstance()->RandInt(0, 4);
	plane_->SetTexture(equipmentTeses_[texNum_]);
	plane_->transform_.translate_ = pos_;
	plane_->Update();
	for (int32_t i = 0; i < yesNoSpriteMaxNum_ + 1; i++) {
		if (i == yesNoSpriteNum_) {
			sprites_[i]->SetTexture(textures_[i * 2], false);
		}
		else {
			sprites_[i]->SetTexture(textures_[i * 2 + 1], false);
		}
	}
}

void ResultUI::GameOverInitialize()
{
	yesNoSpriteMaxNum_ = 2;
}

void ResultUI::Create()
{
	TextureManager* texMa = TextureManager::GetInstance();
	textures_.resize(8);
	textures_[0] = texMa->LoadTexture("retryUi.png");
	textures_[1] = texMa->LoadTexture("notRestartUi.png");
	textures_[2] = texMa->LoadTexture("titleUi.png");
	textures_[3] = texMa->LoadTexture("notTitleUi.png");
	textures_[4] = texMa->LoadTexture("result.png");
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

	global_->AddItem("サイズ", 1.0f, "遷移");
	global_->AddItem("回転速度", 1.0f, "遷移");
	global_->AddItem("時間", 1.0f, "遷移");
	global_->AddItem("位置", Vector3{ 0.0f,0.0f,-1.0f }, "遷移");

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

	planeSize_ = global_->GetFloatValue("サイズ", "遷移");
	rotate_ = global_->GetFloatValue("回転速度", "遷移");
	maxTime_ = global_->GetFloatValue("時間", "遷移");
	pos_ = global_->GetVector3Value("位置", "遷移");
	plane_->transform_.scale_ = { planeSize_,planeSize_ ,planeSize_ };
	plane_->Update();
}
