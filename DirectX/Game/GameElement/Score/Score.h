#pragma once
#include "GameElement/DrawNumbers/DrawNumbers.h"
#include "PostEffect/PostEffect.h"
#include "Audio.h"
class Player;
class GameManager;
class Score
{
public:
	static Score* GetInstance();

	void FirstInitialize();
	void SetPlayer(const Player* player);
	void Initialize();
	void Update(const float& deltaTime);
	void Draw();

	void AddAddNum();
	void AddNum();

	const int32_t& GetCustomer() const;

private:
	Score() = default;
	~Score() = default;
	Score(const Score&) = delete;
	Score& operator=(const Score&) = delete;

	void CreateSprites();
	void SetGlobalVariables();
	void ApplyGlobalVariables();
	void DrawSprite();

private:
	std::unique_ptr<Audio> seSatisfaction_;
	GameManager* gameManager_;
	const Player* player_;
	enum SpriteNames {
		kFrame,
		kBonus,
		kOutSideFrameAdd,
		kOutSideFrame,
		kOutSideFrameCustomer,
		kPlus,
		kCustomerNum,
		kMaxFrame,
		kMax,
		kLine,
		kArrow,
		kMaxSpriteNames,
	};
	std::vector<std::string> names_;

	std::unique_ptr<DrawNumbers> drawNum_;
	std::unique_ptr<DrawNumbers> drawMaxNum_;
	std::unique_ptr<DrawNumbers> drawCustomerNum_;
	std::unique_ptr<DrawNumbers> drawAddNum_;

	std::unique_ptr<Sprite> arrow_;

	std::unique_ptr<PostEffect> post_;
	std::unique_ptr<Sprite> sprite_;
	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::unique_ptr<GlobalVariableUser> glo_;

	std::unique_ptr<PostEffect> post1_;
	std::unique_ptr<Sprite> sprite1_;
	std::unique_ptr<PostEffect> post2_;
	std::unique_ptr<Sprite> sprite2_;
	float easeTime_;

	struct BaseData
	{
		Vector2 basePos;
		Vector2 baseScale;
		float scale;
	};
	std::vector<BaseData> baseDatas_;

	int32_t customerNum_;
	int32_t num_;
	int32_t maxNum_;
	int32_t addNum_;
	bool preDrop_;

	Vector2 screenPos_;

	int32_t cusEditMaxNum_;
	std::vector<int32_t> maxNums_;
};