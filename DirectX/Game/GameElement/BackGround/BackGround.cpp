#include "BackGround.h"
#include "WindowsInfo/WindowsInfo.h"
#include <cmath>

const Texture* texture;

BackGround::BackGround() 
{
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

	global_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "BackGround");
	SetGlobalVariables();
	DrawSprites();
}

void BackGround::Update(const float& deltaTime)
{
#ifdef _DEBUG
	ApplyGlobalVariables();
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

	DrawSprites();
}

void BackGround::Draw(const Camera& camera)
{
	model_->Draw(camera);
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
}

