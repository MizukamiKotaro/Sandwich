#include "Equipment.h"

std::unique_ptr<StageEditor> Equipment::stageEditor_;
std::unique_ptr<GlobalVariableUser> Equipment::global_;
InstancingModelManager* Equipment::instancingManager_ = nullptr;
const InstancingMeshTexData* Equipment::modelData_;
std::unique_ptr<Equipment::StaticData> Equipment::staticData_;

Vector4 kDrawColor = { 1.0f,1.0f,1.0f,1.0f };

void Equipment::StaticInitialize()
{
	if (!instancingManager_) {
		instancingManager_ = InstancingModelManager::GetInstance();
		modelData_ = instancingManager_->GetDrawData("uvChecker.png");

		stageEditor_ = std::make_unique<StageEditor>("具の設定");
		stageEditor_->Initialize();
		global_ = std::make_unique<GlobalVariableUser>("AdjustmentItems", "Equipment");
		staticData_ = std::make_unique<StaticData>();
		StaticSetGlobalVariables();
	}
	else {
		stageEditor_->Initialize();
		StaticApplyGlobalVariables();
	}
}

void Equipment::StaticUpdate()
{
#ifdef _DEBUG
	StaticApplyGlobalVariables();
#endif // _DEBUG
}

Equipment::Equipment(const Vector3& pos)
{
	data_ = std::make_unique<EquipmentData>();
	
	data_->position = pos;
	data_->rotate = {};
	data_->scale = { 0.5f,0.5f,0.5f };
	data_->speed = staticData_->downSpeed;
	data_->yVector = -1;
	data_->reflecteNum = 0;
	data_->isDelete = false;

	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::ENEMY);
}

void Equipment::Update(const float& deltaTime)
{

	data_->position.y += data_->speed * deltaTime * data_->yVector;

	ColliderUpdate();
}

void Equipment::Draw()
{
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data_->scale, data_->rotate, data_->position);
	instancingManager_->AddBox(modelData_, InstancingModelData{ matrix, Matrix4x4::MakeIdentity4x4(), kDrawColor });
}

const bool& Equipment::GetIsDelete() const
{
	return data_->isDelete;
}

void Equipment::ColliderUpdate()
{	
	SetBox2D(data_->position, data_->scale);
	SetCollider();
}

void Equipment::OnCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::ENEMY) {
		float y = collider.GetLine()->y_;
		Vector3 translate = data_->position;
		translate.y = y + data_->scale.y;
		data_->position = translate;
	}
}

void Equipment::StaticSetGlobalVariables()
{
	stageEditor_->AddItem("落下速度", 0.5f, "落下関係");
	StaticApplyGlobalVariables();
}

void Equipment::StaticApplyGlobalVariables()
{
	staticData_->downSpeed = stageEditor_->GetFloatValue("落下速度", "落下関係");
}
