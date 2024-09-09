#include "Equipment.h"
#include "EquipmentManager.h"
#include "calc.h"

std::unique_ptr<StageEditor> Equipment::stageEditor_;
std::unique_ptr<GlobalVariableUser> Equipment::global_;
InstancingModelManager* Equipment::instancingManager_ = nullptr;
std::unique_ptr<Equipment::StaticData> Equipment::staticData_;

Vector4 kDrawColor = { 1.0f,1.0f,1.0f,1.0f };
const Texture* texture[4];

RandomGenerator* rand_;
EquipmentManager* eMana;

void Equipment::StaticInitialize()
{
	if (!instancingManager_) {
		instancingManager_ = InstancingModelManager::GetInstance();

		eMana = EquipmentManager::GetInstance();

		TextureManager* tm = TextureManager::GetInstance();
		texture[0] = tm->LoadTexture("egg.png");
		texture[1] = tm->LoadTexture("ham.png");
		texture[2] = tm->LoadTexture("lettuce.png");
		texture[3] = tm->LoadTexture("tomato.png");
		
		rand_ = RandomGenerator::GetInstance();

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

Equipment::Equipment(const Vector3& pos, const Vector3& scale, const int32_t& tex, const Vector3& vect, const float& speed)
{
	data_ = std::make_unique<EquipmentData>();
	
	data_->texNum = tex;
	modelData_ = instancingManager_->GetDrawData(texture[tex]);

	data_->position = pos;
	data_->rotate = {};
	if (scale.x == 0.0f) {
		data_->scale = Vector3{ staticData_->scale ,staticData_->scale ,staticData_->scale };
	}
	else {
		data_->scale = scale;
	}
	if (speed == -1.0f) {
		data_->speed = staticData_->downSpeed;
	}
	else {
		data_->speed = speed;
	}
	data_->vect = vect;
	data_->reflecteNum = 0;
	data_->isDelete = false;

	if (rand_->RandInt(0, 2) == 0) {
		data_->isRotateRight = true;
	}
	else {
		data_->isRotateRight = false;
	}

	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask::PLAYER);
	AddTargetMask(ColliderMask::ENEMY);
}

void Equipment::Update(const float& deltaTime)
{
	MoveUpdate(deltaTime);

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

void Rotate(Vector3& v, const float& rotate, const Vector3& v2) 
{
	v.x = v2.x * std::cosf(rotate) - v2.y * std::sinf(rotate);
	v.y = v2.y * std::cosf(rotate) + v2.x * std::sinf(rotate);
}

void Equipment::MoveUpdate(const float& deltaTime)
{
	if (data_->vect.x != 0.0f) {
		data_->vect.x = Calc::Lerp(data_->vect.x, 0.0f, 0.01f);
	}
	data_->move = data_->vect * data_->speed;
	data_->move.y -= staticData_->acceleration;
	data_->speed = data_->move.Length();
	data_->vect = data_->move.Normalize();
	if (data_->speed > staticData_->maxSpeed) {
		data_->speed = staticData_->maxSpeed;
	}
	data_->moveSpeed = data_->speed * deltaTime;
	data_->move = data_->vect * data_->moveSpeed;

	data_->position += data_->move;

	if (data_->isRotateRight) {
		data_->rotate.z += 2.0f * deltaTime;
	}
	else {
		data_->rotate.z -= 2.0f * deltaTime;
	}
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

		data_->reflecteNum++;
		if (data_->reflecteNum == staticData_->reflectNum) {
			//反射の分裂の処理
			data_->reflecteNum = 0;

			float t;
			Vector3 rp = translate - data_->move;

			if (data_->vect.y >= 0.0f) {
				t = std::fabsf(y - data_->scale.y - rp.y) / data_->move.y;
				rp += data_->move * t;
			}
			else {
				t = std::fabsf(y + data_->scale.y - rp.y) / data_->move.y;
				rp += data_->move * t;
			}

			data_->vect.y *= -1.0f;
			data_->speed *= staticData_->reflectCoefficient;
			data_->scale *= 0.7f;

			Vector3 v = {};
			Rotate(v, 0.526f, data_->vect);

			eMana->AddEquipment(rp + v * data_->moveSpeed * t, data_->texNum, data_->scale, v, data_->speed);

			Rotate(v, -0.526f, data_->vect);
			data_->position = rp + v * data_->moveSpeed * t;
			data_->vect = v;
		}
		else {
			if (data_->vect.y >= 0.0f) {
				translate.y = y - data_->scale.y - std::fabsf(translate.y + data_->scale.y - y);
			}
			else {
				translate.y = y + data_->scale.y + std::fabsf(translate.y - data_->scale.y - y);
			}
			data_->position = translate;
			data_->speed *= staticData_->reflectCoefficient;
			data_->vect.y *= -1;
		}
	}
}

void Equipment::StaticSetGlobalVariables()
{
	stageEditor_->AddItem("落下速度", 0.5f, "落下関係");
	global_->AddItem("落下の加速度", 0.09f,"落下関係");
	global_->AddItem("最大速度", 5.65f, "落下関係");
	global_->AddItem("反発係数", 1.0f, "落下関係");
	global_->AddItem("分裂までの反射回数", 5, "落下関係");
	global_->AddItem("スケール", 2.0f, "生成関係");
	StaticApplyGlobalVariables();
}

void Equipment::StaticApplyGlobalVariables()
{
	staticData_->downSpeed = stageEditor_->GetFloatValue("落下速度", "落下関係");
	staticData_->maxSpeed = global_->GetFloatValue("最大速度", "落下関係");
	staticData_->reflectCoefficient = global_->GetFloatValue("反発係数", "落下関係");
	staticData_->acceleration = global_->GetFloatValue("落下の加速度", "落下関係");
	staticData_->reflectNum = global_->GetIntValue("分裂までの反射回数", "落下関係");
	staticData_->scale = global_->GetFloatValue("スケール", "生成関係");
}
