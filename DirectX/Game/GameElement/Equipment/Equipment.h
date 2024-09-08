#pragma once
#include "GameElement/Object/Object.h"
#include "CollisionSystem/Collider/Collider.h"
#include "Editor/StageEditor.h"
#include "InstancingModelManager.h"

class Equipment : public Collider{
public:

	static void StaticInitialize();
	static void StaticUpdate();

	Equipment(const Vector3& pos);
	void Update(const float& deltaTime);
	void Draw();

	const bool& GetIsDelete() const;

private:
	void ColliderUpdate();
	void OnCollision(const Collider& collider)override;
private:
	static void StaticSetGlobalVariables();
	static void StaticApplyGlobalVariables();

private:
	static std::unique_ptr<StageEditor> stageEditor_;
	static std::unique_ptr<GlobalVariableUser> global_;
	static InstancingModelManager* instancingManager_;
	static const InstancingMeshTexData* modelData_;
	struct StaticData
	{
		float downSpeed;
	};
	static std::unique_ptr<StaticData> staticData_;

	struct EquipmentData
	{
		Vector3 position;
		Vector3 scale;
		Vector3 rotate;
		float speed;
		int32_t reflecteNum;
		int32_t yVector;
		bool isDelete;
	};
	std::unique_ptr<EquipmentData> data_;
};