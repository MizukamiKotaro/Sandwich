#include "DrawNumbers.h"
#include "WindowsInfo/WindowsInfo.h"

DrawNumbers::DrawNumbers(const std::string& texName, const std::string& groupName, const std::string& tree, const Vector2& size)
{
	glo_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", groupName, tree);

	Vector2 pos = WindowsInfo::GetInstance()->GetWindowSize() * 0.5f;
	Vector2 leftTop = { 0.0f,0.0f };
	Vector2 texsize = { 0.1f,1.0f };
	sprites_.resize(3);
	for (int32_t i = 0; i < 3; i++) {
		sprites_[i] = std::make_unique<Sprite>(texName, pos, leftTop, texsize);
		sprites_[i]->size_ = size;
		sprites_[i]->Update();
	}
	baseScale_ = size;
	scale_ = 1.0f;
	pos_ = pos;
	width_ = size.x;
	SetGlobalVariable();
}

void DrawNumbers::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif // _DEBUG
}

void DrawNumbers::Draw(const int32_t& drawNum)
{
	Vector2 size = baseScale_ * scale_;
	if (drawNum >= 100) {
		int32_t num = drawNum / 100;
		Vector2 pos = { pos_.x - width_, pos_.y };
		SpriteDraw(0, num, pos, size);
		int32_t num2 = drawNum % 100;
		num = num2 / 10;
		pos.x += width_;
		SpriteDraw(1, num, pos, size);
		num = num2 % 10;
		pos.x += width_;
		SpriteDraw(2, num, pos, size);
	}
	else if (drawNum >= 10) {
		int32_t num = drawNum / 10;
		Vector2 pos = { pos_.x - width_ * 0.5f, pos_.y };
		SpriteDraw(0, num, pos, size);
		num = drawNum % 10;
		pos.x += width_;
		SpriteDraw(1, num, pos, size);
	}
	else {
		SpriteDraw(0, drawNum, pos_, size);
	}
}

void DrawNumbers::SpriteDraw(const int32_t& i, const int32_t& num, const Vector2& pos, const Vector2& size)
{
	sprites_[i]->SetTextureTopLeft({ 0.1f * num, 0.0f });
	sprites_[i]->pos_ = pos;
	sprites_[i]->size_ = size;
	sprites_[i]->Update();
	sprites_[i]->Draw();
}

void DrawNumbers::SetGlobalVariable()
{
	glo_->AddItem("座標", pos_);
	glo_->AddItem("スケール", scale_);
	glo_->AddItem("間隔", width_);
	ApplyGlobalVariable();
}

void DrawNumbers::ApplyGlobalVariable()
{
	pos_ = glo_->GetVector2Value("座標");
	scale_ = glo_->GetFloatValue("スケール");
	width_ = glo_->GetFloatValue("間隔");
}
