#pragma once
#include "GlobalVariables/GlobalVariableUser.h"
#include "Sprite.h"
#include <memory>

class DrawNumbers {
public:
	DrawNumbers(const std::string& texName, const std::string& groupName, const std::string& tree, const Vector2& size);
	void Update();
	void Draw(const int32_t& drawNum);

private:
	void SpriteDraw(const int32_t& i, const int32_t& num, const Vector2& pos, const Vector2& size);
	void SetGlobalVariable();
	void ApplyGlobalVariable();
private:
	std::unique_ptr<GlobalVariableUser> glo_;
	std::vector<std::unique_ptr<Sprite>> sprites_;

	Vector2 baseScale_;
	float scale_;
	Vector2 pos_;
	float width_;
};