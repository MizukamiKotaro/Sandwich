#pragma once
#include "Equipment.h"
#include <list>
#include "RandomGenerator/RandomGenerator.h"

class EquipmentManager {
public:
	static EquipmentManager* GetInstance();
	void FirstInitialize();
	void SetPlayer(const Player* player);
	void Initialize();
	void Update(const float& deltaTime);
	void Draw();

	void Clear();
	void AddEquipment(const Vector3& pos, const int32_t& tex, const int32_t& division, const Vector3& scale = {0.0f,0.0f,0.0f}, const Vector3& vect = {0.0f,-1.0f,0.0f}, const float& speed = -1.0f);

private:
	EquipmentManager() = default;
	~EquipmentManager() = default;
	EquipmentManager(const EquipmentManager&) = delete;
	EquipmentManager& operator=(const EquipmentManager&) = delete;

	void DropEquipment(const float& deltaTime);

	void SetGlobalVariables();
	void ApplyGlobalVariables();

private:
	std::unique_ptr<GlobalVariableUser> global_;
	std::unique_ptr<StageEditor> stageEditor_;
	std::list<std::unique_ptr<Equipment>> equipments_;

	const Player* player_;

	float time_;
	float coolTime_;
	float width_;
	float dropY_;
	int32_t dropNum_;
	int32_t divisionNum_;
	RandomGenerator* rand;
};