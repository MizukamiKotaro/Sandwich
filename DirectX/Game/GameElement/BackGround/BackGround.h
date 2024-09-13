#pragma once
#include "Model.h"
#include "PostEffect/PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Sprite.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include <vector>
#include "GameElement/Timer/Timer.h"
#include "Drawers/PostSprite/PostSprite.h"
#include "Input.h"

class GameManager;

class BackGround {
public:
	BackGround();

	void Initialise();
	void Update(const float& deltaTime);
	void Draw(const Camera& camera);

	const bool GetIsTimeUp() const;
	const bool& GetIsReset() const;

private:
	void Create();
	void DrawSprites();
	void SetGlobalVariables();
	void ApplyGlobalVariables();

	void Ps0Update(const float& deltaTime);
	void Ps1Update(const float& deltaTIme);
	void Ps2Update(const float& deltaTime);
	void Ps3Update(const float& deltaTime);
	void Ps4Update(const float& deltaTime);
private:
	Input* input_;
	GameManager* gameManager_;

	enum SpriteNames {
		kTitle,
		kStartUI,
		kSpaceUI,
		kEnd,
	};
	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::vector<std::string> names_;

	struct PutData
	{
		Vector2 basePos;
		Vector2 baseScale;
		float scale;
	};
	std::vector<PutData> putDatas_;

private:
	std::unique_ptr<PostSprite> ps0_;
	std::unique_ptr<PostSprite> ps2_;
	std::unique_ptr<PostSprite> ps3_;
	float ps0Time_;
	float ps1Time_;
	bool ps1Bool_;
	bool ps1Active_;
	float ps2Time_;
	float ps4Time_;

	std::unique_ptr<Timer> timer_;

	std::unique_ptr<Model> model_;
	std::unique_ptr<PostEffect> postEffect_;
	std::unique_ptr<PostEffect> postEffect2_;
	std::unique_ptr<GlobalVariableUser> global_;

	std::unique_ptr<Sprite> back_;
	std::vector<std::unique_ptr<Sprite>> verticals_;
	std::vector<std::unique_ptr<Sprite>> horizontals_;

	Vector2 basePos_;
	float size2_;
	Vector2 move_;
	Vector2 screenSize_;
	float firstPos;
	bool preResult_;
};