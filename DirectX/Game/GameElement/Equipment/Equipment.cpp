#include "Equipment.h"
#include "EquipmentManager.h"
#include "calc.h"
#include "GameElement/Player/Player.h"
#include "Audio/Audio.h"
#include "GameElement/Score/Score.h"

std::unique_ptr<StageEditor> Equipment::stageEditor_;
std::unique_ptr<GlobalVariableUser> Equipment::global_;
InstancingModelManager* Equipment::instancingManager_ = nullptr;
std::unique_ptr<Equipment::StaticData> Equipment::staticData_;
const Player* Equipment::player_;

Vector4 kDrawColor = { 1.0f,1.0f,1.0f,1.0f };
const Texture* texture[4];
std::unique_ptr<Audio> seReflect;

RandomGenerator* rand_;
EquipmentManager* eMana;
const InstancingMeshTexData* Equipment::bonusLineModelData_;

Score* Equipment::score_;

void Equipment::SetPlayer(const Player* player) 
{
	player_ = player;
}

void Equipment::StaticInitialize()
{
	if (!instancingManager_) {
		instancingManager_ = InstancingModelManager::GetInstance();

		bonusLineModelData_ = instancingManager_->GetDrawData("white.png");
		score_ = Score::GetInstance();

		seReflect = std::make_unique<Audio>();
		seReflect->Load("reflect.mp3", "具材の反射");

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
		staticData_->seTime_ = 0.0f;
		StaticSetGlobalVariables();
	}
	else {
		stageEditor_->Initialize();
		StaticApplyGlobalVariables();
	}
}

void Equipment::StaticUpdate(const float& deltaTime)
{
#ifdef _DEBUG
	StaticApplyGlobalVariables();
#endif // _DEBUG
	staticData_->seTime_ += deltaTime;
}

Equipment::Equipment(const Vector3& pos, const Vector3& scale, const int32_t& tex, const int32_t& division, const Vector3& vect, const float& speed)
{
	score_->AddAddNum();

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
	data_->isSand = false;
	data_->division = division;

	CreateCollider(ColliderShape::BOX2D, ColliderType::COLLIDER, ColliderMask::EQUIPMENT);
	AddTargetMask(ColliderMask::FLOOR);
	AddTargetMask(ColliderMask::PAN);
}

Equipment::~Equipment()
{
}

void Equipment::Update(const float& deltaTime)
{
	if (player_->GetIsDrop()) {
		DropUpdate();
	}
	else {
		MoveUpdate(deltaTime);
	}

	if (data_->isSand && !player_->GetIsDrop()) {
		data_->isDelete = true;
	}
	else {
		ColliderUpdate();
	}
}

void Equipment::Draw()
{
	data_->position.z = -0.001f;
	Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(data_->scale, data_->rotate, data_->position);
	instancingManager_->AddBox(modelData_, InstancingModelData{ matrix, Matrix4x4::MakeIdentity4x4(), kDrawColor });
}

const bool& Equipment::GetIsDelete() const
{
	return data_->isDelete;
}

void Equipment::DrawBonusLine()
{
	if (staticData_->bonusLineNum >= 1) {
		Vector3 scale = { staticData_->bonusLineScale.x,staticData_->bonusLineScale.y,1.0f };
		Vector3 rotate = { 0.0f,0.0f,0.0f };
		Vector3 translate = { staticData_->bonusFirstX,staticData_->bonusHeight,0.001f };
		Matrix4x4 matrix = {};
		for (int32_t i = 0; i < staticData_->bonusLineNum; i++) {
			matrix = Matrix4x4::MakeAffinMatrix(scale, rotate, translate);
			instancingManager_->AddBox(bonusLineModelData_, InstancingModelData{ matrix, Matrix4x4::MakeIdentity4x4(), staticData_->bonusLineColor });
			translate.x += staticData_->bonusWeight;
		}
	}
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

	float x = std::fabs(data_->position.x);
	if (x > staticData_->stageWidthX) {
		x -= staticData_->stageWidthX;
		Vector3 prepos = data_->position - data_->move;
		if (data_->position.x > 0.0f) {
			data_->position.x -= x;
		}
		else {
			data_->position.x += x;
		}
		data_->vect.x *= -1;
	}

	if (data_->isRotateRight) {
		data_->rotate.z += 2.0f * deltaTime;
	}
	else {
		data_->rotate.z -= 2.0f * deltaTime;
	}
}

void Equipment::DropUpdate()
{
	if (data_->isSand) {
		data_->position.y -= player_->GetIsDropSpeed();
	}
}

void Equipment::ColliderUpdate()
{	
	SetBox2D(data_->position, data_->scale);
	SetCollider();
}

void Equipment::OnCollision(const Collider& collider)
{
	if (player_->GetIsDrop()) {
		DropCollision(collider);
	}
	else {
		NotDropCollision(collider);
	}
}

bool IsCollision(const Vector3& pos0, const Vector3& scale0, const Vector3& pos1, const Vector3& scale1)
{

	Vector2 min0 = { pos0.x - scale0.x,pos0.y - scale0.y };
	Vector2 max0 = { pos0.x + scale0.x,pos0.y + scale0.y };
	Vector2 min1 = { pos1.x - scale1.x,pos1.y - scale1.y };
	Vector2 max1 = { pos1.x + scale1.x,pos1.y + scale1.y };

	if (min0.x <= max1.x && max0.x >= min1.x &&
		min0.y <= max1.y && max0.y >= min1.y) {
		return true;
	}

	return false;
}

void Equipment::NotDropCollision(const Collider& collider)
{
	if (collider.GetMask() == ColliderMask::FLOOR) {
		if (staticData_->seTime_ >= staticData_->seMaxTime_) {
			seReflect->Play();
			staticData_->seTime_ = 0.0f;
		}

		ColliderShape::BOX2D;
		ShapeBox2D* box = collider.GetBox2D();
		float y = 0.0f;
		if (data_->vect.y >= 0.0f) {
			y = box->position_.y - box->scale_.y;
		}
		else {
			y = box->position_.y + box->scale_.y;
		}

		Vector3 translate = data_->position;

		if (IsCollision(box->position_, box->scale_, data_->position - data_->move, data_->scale)) {
			data_->reflecteNum = 0;

			if (data_->vect.y < 0.0f) {
				data_->vect.y *= -1;
			}

			data_->position.y = box->position_.y + box->scale_.y + data_->scale.y + data_->vect.y;

			return;
		}

		if (data_->division < staticData_->divisionNum) {
			data_->reflecteNum++;
		}
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
			data_->speed += staticData_->reflectCoefficient;
			data_->scale *= staticData_->divisionScale;
			data_->division++;

			Vector3 v = {};
			float speed = data_->moveSpeed * t;

			if (y >= staticData_->bonusHeight) {
				// 4 つに分裂
				Rotate(v, 0.263f, data_->vect);
				eMana->AddEquipment(rp + v * speed, data_->texNum, data_->division, data_->scale, v, data_->speed);

				Rotate(v, 0.785f, data_->vect);
				eMana->AddEquipment(rp + v * speed, data_->texNum, data_->division, data_->scale, v, data_->speed);

				Rotate(v, -0.263f, data_->vect);
				eMana->AddEquipment(rp + v * speed, data_->texNum, data_->division, data_->scale, v, data_->speed);

				Rotate(v, -0.785f, data_->vect);
				data_->position = rp + v * speed;
				data_->vect = v;

			}
			else {
				// 2つに分裂
				Rotate(v, 0.526f, data_->vect);
				eMana->AddEquipment(rp + v * speed, data_->texNum, data_->division, data_->scale, v, data_->speed);

				Rotate(v, -0.526f, data_->vect);
				data_->position = rp + v * speed;
				data_->vect = v;
			}
		}
		else {
			if (data_->vect.y >= 0.0f) {
				translate.y = y - data_->scale.y - std::fabsf(translate.y + data_->scale.y - y);
			}
			else {
				translate.y = y + data_->scale.y + std::fabsf(translate.y - data_->scale.y - y);
			}
			data_->position = translate;
			data_->speed += staticData_->reflectCoefficient;
			data_->vect.y *= -1;
		}
	}
}

void Equipment::DropCollision(const Collider& collider)
{
	if (!data_->isSand) {
		collider;
		data_->isSand = true;
	}
}

void Equipment::StaticSetGlobalVariables()
{
	stageEditor_->AddItem("落下速度", 0.5f, "落下関係");
	global_->AddItem("落下の加速度", 0.09f,"落下関係");
	global_->AddItem("最大速度", 5.65f, "落下関係");
	global_->AddItem("反発係数", 1.0f, "落下関係");
	global_->AddItem("分裂までの反射回数", 5, "落下関係");
	global_->AddItem("分裂の回数", 3, "生成関係");
	global_->AddItem("分裂のスケール係数", 0.8f, "生成関係");
	global_->AddItem("スケール", 2.0f, "生成関係");
	global_->AddItem("ステージの横幅", 18.0f, "ステージ関係");

	global_->AddItem("高さ", 6.0f, "ボーナス関係");
	global_->AddItem("ラインの幅", 1.0f, "ボーナス関係");
	global_->AddItem("ラインのスケール", Vector2{0.4f,0.01f}, "ボーナス関係");
	global_->AddItem("ラインの最初のx座標", -2.0f, "ボーナス関係");
	global_->AddItem("ラインの数", 6, "ボーナス関係");
	global_->AddItemColor("ラインの色", Vector4{ 1.0f,1.0f,1.0f,1.0f }, "ボーナス関係");

	global_->AddItem("反射SEの間隔", 0.01f, "SE関係");
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
	staticData_->divisionNum = global_->GetIntValue("分裂の回数", "生成関係");
	staticData_->divisionScale = global_->GetFloatValue("分裂のスケール係数", "生成関係");
	staticData_->stageWidthX = global_->GetFloatValue("ステージの横幅", "ステージ関係");

	staticData_->bonusHeight = global_->GetFloatValue("高さ", "ボーナス関係");
	staticData_->bonusWeight = global_->GetFloatValue("ラインの幅", "ボーナス関係");
	staticData_->bonusLineScale = global_->GetVector2Value("ラインのスケール", "ボーナス関係");
	staticData_->bonusFirstX = global_->GetFloatValue("ラインの最初のx座標", "ボーナス関係");
	staticData_->bonusLineNum = global_->GetIntValue("ラインの数", "ボーナス関係");
	staticData_->bonusLineColor = global_->GetColor("ラインの色", "ボーナス関係");

	staticData_->seMaxTime_ = global_->GetFloatValue("反射SEの間隔", "SE関係");
}
