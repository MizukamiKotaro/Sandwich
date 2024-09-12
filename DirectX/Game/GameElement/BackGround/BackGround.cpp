#include "BackGround.h"
#include "WindowsInfo/WindowsInfo.h"
#include <cmath>
#include "GameElement/GameManager/GameManager.h"

const Texture* texture;

BackGround::BackGround() 
{
	gameManager_ = GameManager::GetInstance();
	screenSize_ = WindowsInfo::GetInstance()->GetWindowSize();

	postEffect_ = std::make_unique<PostEffect>();
	postEffect2_ = std::make_unique<PostEffect>();

	model_ = std::make_unique<Model>("plane");
	model_->UnUsedLight();
	model_->SetSRVGPUDescriptorHandle_(postEffect_->GetSRVGPUDescriptorHandle());
	model_->transform_.translate_ = {};
	model_->transform_.translate_.z = 2.0f;

	texture = TextureManager::GetInstance()->LoadTexture("white.png");

	back_ = std::make_unique<Sprite>(texture, screenSize_ * 0.5f);
	back_->size_ = screenSize_;
	back_->Update();

	timer_ = std::make_unique<Timer>();

	Create();

	global_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "BackGround");
	SetGlobalVariables();
	DrawSprites();
	preResult_ = false;
}

void BackGround::Initialise()
{
	timer_->Initialize();
	preResult_ = false;
}

void BackGround::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariables();
	timer_->UpdateGlobal();
#endif // _DEBUG

	basePos_ += move_ * deltaTime;
	basePos_.x = std::fmodf(basePos_.x, size2_);
	basePos_.y = std::fmodf(basePos_.y, size2_);

	Vector2 pos = { firstPos, screenSize_.y * 0.5f };
	for (std::unique_ptr<Sprite>& sp : verticals_) {
		sp->pos_ = pos + basePos_;
		sp->Update();
		pos.x += size2_;
	}
	pos = { screenSize_.x * 0.5f, firstPos };
	for (std::unique_ptr<Sprite>& sp : horizontals_) {
		sp->pos_ = pos + basePos_;
		sp->Update();
		pos.y += size2_;
	} 

	if (preResult_ && gameManager_->GetScene() == GameManager::kGame) {
		Initialise();
	}

	if (gameManager_->GetScene() == GameManager::kGame) {
		timer_->Update(deltaTime);
		if (timer_->GetIsTimeUp()) {
			gameManager_->ChangeScene(GameManager::kResult);
			preResult_ = true;
		}
	}

	DrawSprites();
}

void BackGround::Draw(const Camera& camera)
{
	model_->Draw(camera);
}

const bool BackGround::GetIsTimeUp() const
{
	return timer_->GetIsTimeUp();
}

const bool& BackGround::GetIsReset() const
{
	return timer_->GetIsReset();
}

void BackGround::Create()
{
	Vector2 pos = screenSize_ * 0.5f;

	sprites_.resize(SpriteNames::kEnd);
	sprites_[SpriteNames::kTitle] = std::make_unique<Sprite>("title.png", pos);
	sprites_[SpriteNames::kStartUI] = std::make_unique<Sprite>("startUi.png", pos);
	sprites_[SpriteNames::kSpaceUI] = std::make_unique<Sprite>("SPACEUi.png", pos);

	names_.resize(SpriteNames::kEnd);
	names_[SpriteNames::kTitle] = "さんどうぃっち";
	names_[SpriteNames::kStartUI] = "スタート";
	names_[SpriteNames::kSpaceUI] = "スペース";

	putDatas_.resize(SpriteNames::kEnd);

	for (int32_t i = 0; i < SpriteNames::kEnd; i++) {
		putDatas_[i].basePos = pos;
		putDatas_[i].baseScale = sprites_[i]->size_;
		putDatas_[i].scale = 1.0f;
	}
}

void BackGround::DrawSprites()
{
	postEffect2_->PreDrawScene();
	back_->Draw();
	for (const std::unique_ptr<Sprite>& sp : verticals_) {
		sp->Draw();
	}
	for (const std::unique_ptr<Sprite>& sp : horizontals_) {
		sp->Draw();
	}

	if (gameManager_->GetScene() == GameManager::kTitle) {
		for (const std::unique_ptr<Sprite>& sp : sprites_) {
			sp->Draw();
		}
	}
	else {
		timer_->Draw();
	}
	postEffect2_->PostDrawScene();

	postEffect_->PreDrawScene();
	postEffect2_->Draw();
	postEffect_->PostDrawScene();
}

void BackGround::SetGlobalVariables()
{
	global_->AddItem("スケール", 1.0f);
	global_->AddItemColor("背景の色", { 1.0f,1.0f,1.0f,1.0f });
	global_->AddItem("縦の数", 5, "チェック");
	global_->AddItem("横の数", 10, "チェック");
	global_->AddItem("最初の座標", -30.0f, "チェック");
	global_->AddItem("幅", 30.0f, "チェック");
	global_->AddItemColor("色", { 1.0f,1.0f,1.0f,0.4f }, "チェック");
	global_->AddItem("移動", Vector2{ 0.0f,0.0f }, "チェック");

	for (int32_t i = 0; i < SpriteNames::kEnd; i++) {
		global_->AddItem(names_[i] + "の座標", putDatas_[i].basePos, "タイトル", names_[i]);
		global_->AddItem(names_[i] + "のスケール", putDatas_[i].scale, "タイトル", names_[i]);
	}

	ApplyGlobalVariables();
}

void BackGround::ApplyGlobalVariables()
{
	float scale = global_->GetFloatValue("スケール");
	model_->transform_.scale_ = { scale,screenSize_.y / screenSize_.x * scale,1.0f };
	model_->Update();

	back_->SetColor(global_->GetColor("背景の色"));

	move_ = global_->GetVector2Value("移動", "チェック");
	Vector4 color = global_->GetColor("色", "チェック");

	float size = global_->GetFloatValue("幅", "チェック");
	firstPos = global_->GetFloatValue("最初の座標", "チェック");
	int32_t num = global_->GetIntValue("縦の数", "チェック");
	if (num <= 0) {
		num = 1;
	}
	if (verticals_.size() != size_t(num)) {
		verticals_.clear();
		verticals_.resize(num);

		Vector2 pos = { firstPos, screenSize_.y * 0.5f };
		float size2 = size * 2.0f;
		for (int32_t i = 0; i < num; i++) {
			verticals_[i] = std::make_unique<Sprite>(texture, pos);
			verticals_[i]->size_ = { size,screenSize_.y };
			verticals_[i]->SetColor(color);
			verticals_[i]->Update();
			pos.x += size2;
		}
	}
	else {
		Vector2 pos = { firstPos, screenSize_.y * 0.5f };
		float size2 = size * 2.0f;
		for (int32_t i = 0; i < num; i++) {
			verticals_[i]->pos_ = pos;
			verticals_[i]->size_ = { size,screenSize_.y };
			verticals_[i]->SetColor(color);
			verticals_[i]->Update();
			pos.x += size2;
		}
	}

	num = global_->GetIntValue("横の数", "チェック");
	if (num <= 0) {
		num = 1;
	}
	if (horizontals_.size() != size_t(num)) {
		horizontals_.clear();
		horizontals_.resize(num);

		Vector2 pos = { screenSize_.x * 0.5f, firstPos };
		float size2 = size * 2.0f;
		for (int32_t i = 0; i < num; i++) {
			horizontals_[i] = std::make_unique<Sprite>(texture, pos);
			horizontals_[i]->size_ = { screenSize_.y,size };
			horizontals_[i]->SetColor(color);
			horizontals_[i]->Update();
			pos.y += size2;
		}
	}
	else {
		Vector2 pos = { screenSize_.x * 0.5f, firstPos };
		float size2 = size * 2.0f;
		for (int32_t i = 0; i < num; i++) {
			horizontals_[i]->pos_ = pos;
			horizontals_[i]->size_ = { screenSize_.y,size };
			horizontals_[i]->SetColor(color);
			horizontals_[i]->Update();
			pos.y += size2;
		}
	}

	size2_ = size * 2.0f;


	for (int32_t i = 0; i < SpriteNames::kEnd; i++) {
		putDatas_[i].basePos = global_->GetVector2Value(names_[i] + "の座標", "タイトル", names_[i]);
		putDatas_[i].scale = global_->GetFloatValue(names_[i] + "のスケール", "タイトル", names_[i]);
		sprites_[i]->pos_ = putDatas_[i].basePos;
		sprites_[i]->size_ = putDatas_[i].baseScale * putDatas_[i].scale;
		sprites_[i]->Update();
	}
}

