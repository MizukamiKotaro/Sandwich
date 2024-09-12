#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Editor/StageEditor.h"
#include "InstancingModelManager.h"

class Player;
class Score;

class Equipment : public Collider{
public:
	static void SetPlayer(const Player* player);
	static void StaticInitialize();
	static void StaticUpdate(const float& deltaTime);

	Equipment(const Vector3& pos, const Vector3& scale, const int32_t& tex, const int32_t& division, const Vector3& vect, const float& speed = -1.0f);
	~Equipment();
	void Update(const float& deltaTime);
	void Draw();

	const bool& GetIsDelete() const;
	static void DrawBonusLine();

private:
	void MoveUpdate(const float& deltaTime);
	void DropUpdate();
	void ColliderUpdate();
	void OnCollision(const Collider& collider)override;
	void NotDropCollision(const Collider& collider);
	void DropCollision(const Collider& collider);
private:
	static void StaticSetGlobalVariables();
	static void StaticApplyGlobalVariables();

private:
	static const InstancingMeshTexData* bonusLineModelData_;

private:
	static Score* score_;
	static const Player* player_;
	static std::unique_ptr<StageEditor> stageEditor_;
	static std::unique_ptr<GlobalVariableUser> global_;
	static InstancingModelManager* instancingManager_;
	const InstancingMeshTexData* modelData_;
	struct StaticData
	{
		float downSpeed;
		float maxSpeed;
		int32_t reflectNum;
		float reflectCoefficient;
		float acceleration;
		float scale;
		int32_t divisionNum;
		float divisionScale;
		float stageWidthX;

		float bonusHeight;
		Vector4 bonusLineColor;
		float bonusWeight;
		Vector2 bonusLineScale;
		float bonusFirstX;
		int32_t bonusLineNum;

		float seTime_;
		float seMaxTime_;
	};
	static std::unique_ptr<StaticData> staticData_;

	struct EquipmentData
	{
		Vector3 position;
		Vector3 scale;
		Vector3 rotate;
		float speed;
		int32_t reflecteNum;
		Vector3 vect;
		Vector3 move;
		float moveSpeed;
		bool isDelete;
		int32_t texNum;
		bool isRotateRight;
		bool isSand;
		int32_t division;
	};
	std::unique_ptr<EquipmentData> data_;
};