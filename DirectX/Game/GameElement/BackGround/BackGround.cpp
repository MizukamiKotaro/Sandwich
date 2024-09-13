#include "BackGround.h"
#include "WindowsInfo/WindowsInfo.h"
#include <cmath>
#include "GameElement/GameManager/GameManager.h"
#include "Ease/Ease.h"

const Texture* texture;

BackGround::BackGround() 
{
	gameManager_ = GameManager::GetInstance();
	screenSize_ = WindowsInfo::GetInstance()->GetWindowSize();
	input_ = Input::GetInstance();

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
	ps0_ = std::make_unique<PostSprite>(screenSize_ * 0.5f);
	ps2_ = std::make_unique<PostSprite>(screenSize_ * 0.5f);
	ps3_ = std::make_unique<PostSprite>(screenSize_ * 0.5f);
	ps0Time_ = 0.0f;
	ps4Time_ = 0.0f;

	global_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "BackGround");
	SetGlobalVariables();
	DrawSprites();
	preResult_ = false;

}

void BackGround::Initialise()
{
	timer_->Initialize();
	preResult_ = false;
	int32_t scene = gameManager_->GetScene();

	ps2Time_ = 0.0f;
	ps1Time_ = 0.0f;
	ps1Bool_ = false;
	ps1Active_ = false;
	if (scene == GameManager::kTitle) {
		ps2_->sprite_->pos_ = screenSize_ * 0.5f;
		ps2_->Update();
	}
	else if (scene == GameManager::kGame) {
		ps2_->sprite_->pos_.x = -screenSize_.x ;
		ps2_->Update();
	}
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
		Ps2Update(deltaTime);
	}
	else if (gameManager_->GetScene() == GameManager::kTitle) {
		Ps0Update(deltaTime);
		Ps4Update(deltaTime);
		if (input_->PressedKey(DIK_SPACE)) {
			ps1Time_ = 0.0f;
			ps1Active_ = true;
		}
		if (ps1Active_) {
			Ps1Update(deltaTime);
		}
	}

	DrawSprites();
}

void BackGround::Draw(const Camera& camera)
{
	model_->Draw(camera);
}

void BackGround::DrawTrop()
{
	timer_->DrowTrop();
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
	ps0_->PreDrawScene();
	sprites_[SpriteNames::kStartUI]->Draw();
	ps0_->PostDrawScene();

	ps2_->PreDrawScene();
	ps0_->Draw();
	sprites_[SpriteNames::kSpaceUI]->Draw();
	sprites_[SpriteNames::kTitle]->Draw();
	ps2_->PostDrawScene();

	ps3_->PreDrawScene();
	if (gameManager_->GetScene() != GameManager::kTitle) {
		timer_->Draw();
	}
	ps3_->PostDrawScene();

	postEffect2_->PreDrawScene();
	back_->Draw();
	for (const std::unique_ptr<Sprite>& sp : verticals_) {
		sp->Draw();
	}
	for (const std::unique_ptr<Sprite>& sp : horizontals_) {
		sp->Draw();
	}

	ps2_->Draw();
	ps3_->Draw();
	
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

void BackGround::Ps0Update(const float& deltaTime)
{
	float a = 1.5f;
	float r = 5.0f;
	ps0Time_ = std::fmodf(ps0Time_ + deltaTime, a);
	float t = ps0Time_ / a * 6.28f;
	ps0_->sprite_->pos_.y = r * std::sinf(t) + screenSize_.y * 0.5f;
	ps0_->Update();
}

void BackGround::Ps1Update(const float& deltaTIme)
{
	float a = 0.15f;
	float b = 0.2f;
	float c = 1.3f;
	ps1Time_ = std::clamp(ps1Time_ + deltaTIme, 0.0f, a + b);
	float t = ps1Time_;
	if (t < a) {
		t /= a;
		sprites_[SpriteNames::kSpaceUI]->size_ = Ease::UseEase(1.0f, c, t, Ease::EaseIn) * 
			putDatas_[SpriteNames::kSpaceUI].baseScale * putDatas_[SpriteNames::kSpaceUI].scale;
	}
	else {
		t = (t - a) / b;
		sprites_[SpriteNames::kSpaceUI]->size_ = Ease::UseEase(c, 1.0f, t, Ease::EaseIn) *
			putDatas_[SpriteNames::kSpaceUI].baseScale * putDatas_[SpriteNames::kSpaceUI].scale;
	}
	sprites_[SpriteNames::kSpaceUI]->Update();
}

void BackGround::Ps2Update(const float& deltaTime)
{
	float a = 1.0f;
	if (ps2Time_ < a) {
		ps2Time_ = std::clamp(ps2Time_ + deltaTime, 0.0f, a);
		float t = ps2Time_ / a;
		float h = screenSize_.y * 0.5f;
		ps2_->sprite_->pos_.y = t * (h + screenSize_.y) + (1.0f - t) * h;
		ps2_->Update();
		ps3_->sprite_->pos_.y = t * (h) + (1.0f - t) * (-h * 0.5f);
		ps3_->Update();
	}
}

void BackGround::Ps3Update(const float& deltaTime)
{
	float a = 1.0f;
	if (ps3_->time_ < a) {
		ps3_->time_ = std::clamp(ps3_->time_ + deltaTime, 0.0f, a);
		float t = ps3_->time_ / a;
		float h = screenSize_.y * 0.5f;
		ps3_->sprite_->pos_.y = t * (h) + (1.0f - t) * (-h);
		ps3_->Update();
	}
}

void BackGround::Ps4Update(const float& deltaTime)
{
	float a = 4.0f;
	float r = 0.1f;
	ps4Time_ = std::fmodf(ps4Time_ + deltaTime, a);
	float t = ps4Time_ / a * 6.28f;
	sprites_[SpriteNames::kTitle]->size_ = putDatas_[SpriteNames::kTitle].baseScale * putDatas_[SpriteNames::kTitle].scale *
		(1.0f + r * std::sinf(t));
	sprites_[SpriteNames::kTitle]->Update();
}

