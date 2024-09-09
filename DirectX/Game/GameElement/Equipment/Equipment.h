#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Editor/StageEditor.h"
#include "InstancingModelManager.h"

class Equipment : public Collider{
public:

	static void StaticInitialize();
	static void StaticUpdate();

	Equipment(const Vector3& pos, const Vector3& scale, const int32_t& tex, const Vector3& vect, const float& speed = -1.0f);
	void Update(const float& deltaTime);
	void Draw();

	const bool& GetIsDelete() const;

private:
	void MoveUpdate(const float& deltaTime);
	void ColliderUpdate();
	void OnCollision(const Collider& collider)override;
private:
	static void StaticSetGlobalVariables();
	static void StaticApplyGlobalVariables();

private:
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
	};
	std::unique_ptr<EquipmentData> data_;
};