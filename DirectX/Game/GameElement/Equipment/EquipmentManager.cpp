#include "EquipmentManager.h"
#include "GameElement/Player/Player.h"
#include "GameElement/GameManager/GameManager.h"

Vector3 scale3_ = { 0.5f,0.5f,0.5f };

EquipmentManager* EquipmentManager::GetInstance()
{
	static EquipmentManager instance;
	return &instance;
}

void EquipmentManager::FirstInitialize()
{
	gameManager_ = GameManager::GetInstance();
	stageEditor_ = std::make_unique<StageEditor>("具の設定");
	stageEditor_->Initialize();
	global_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "Equipment");
	rand = RandomGenerator::GetInstance();
	dropType_ = 0;
	dropNum_ = 0;
}

void EquipmentManager::SetPlayer(const Player* player)
{
	player_ = player;
	Equipment::SetPlayer(player);
}

void EquipmentManager::Initialize()
{
	stageEditor_->Initialize();
	time_ = 0.0f;
	Clear();
	Equipment::StaticInitialize();
	SetGlobalVariables();
	time_ = coolTime_ * 0.5f;
}

void EquipmentManager::Update(const float& deltaTime)
{
	if (stageEditor_->IsChangedStage()) {
		Initialize();
	}
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG
	Equipment::StaticUpdate(deltaTime);
	if (gameManager_->GetScene() != GameManager::kGame || gameManager_->GetIsTransition()) {
		return;
	}
	
	DropEquipment(deltaTime);

	for (std::list<std::unique_ptr<Equipment>>::iterator it = equipments_.begin(); it != equipments_.end();) {
		if ((*it)->GetIsDelete()) {
			(*it).reset(nullptr);
			it = equipments_.erase(it);
		}
		else {
			(*it)->Update(deltaTime);
			if ((*it)->GetIsDelete()) {
				(*it).reset(nullptr);
				it = equipments_.erase(it);
			}
			else {
				it++;
			}
		}
	}
}

void EquipmentManager::Draw()
{
	for (std::unique_ptr<Equipment>& equipment : equipments_) {
		equipment->Draw();
	}
	Equipment::DrawBonusLine();
}

void EquipmentManager::Clear()
{
	equipments_.clear();
}

void EquipmentManager::AddEquipment(const Vector3& pos, const int32_t& tex, const int32_t& division, const Vector3& scale, const Vector3& vect, const float& speed)
{
	equipments_.push_back(std::make_unique<Equipment>(pos, scale, tex, division, vect, speed));
}

void EquipmentManager::DropEquipment(const float& deltaTime)
{
	time_ += deltaTime;
	if (time_ >= coolTime_) {
		time_ -= coolTime_;

		int32_t num = rand->RandInt(0, divisionNum_);
		while (std::abs(dropNum_ - num) <= 1)
		{
			num = rand->RandInt(0, divisionNum_);
		}
		dropNum_ = num;
		Vector3 pos = { -0.5f * width_ + (width_ / (divisionNum_ * 2.0f)) + (width_ / divisionNum_) * num ,dropY_,0.001f };

		num = rand->RandInt(0, 4);
		while (num == dropType_)
		{
			num = rand->RandInt(0, 4);
		}
		dropType_ = num;
		AddEquipment(pos, dropType_, 0);
	}
}

void EquipmentManager::SetGlobalVariables()
{
	stageEditor_->AddItem("生成間隔", 1.5f, "生成関係");
	global_->AddItem("ステージの幅", 10.0f, "生成関係");
	global_->AddItem("ステージの分割数", 10, "生成関係");
	global_->AddItem("初期のy座標", 6.0f, "生成関係");
	ApplyGlobalVariables();
}

void EquipmentManager::ApplyGlobalVariables()
{
	coolTime_ = stageEditor_->GetFloatValue("生成間隔", "生成関係");
	width_ = global_->GetFloatValue("ステージの幅", "生成関係");
	divisionNum_ = global_->GetIntValue("ステージの分割数", "生成関係");
	dropY_ = global_->GetFloatValue("初期のy座標", "生成関係");
}
