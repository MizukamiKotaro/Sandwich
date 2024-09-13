#include "EatParticle.h"

EatParticle::EatParticle()
{
	global = std::make_unique<GlobalVariableUser>("Particle", "Eat");

	tex_ = ParticleManager::GetInstance()->GetDrawData("cheese.png", BlendMode::kBlendModeNormal);

	SetGlobalVariable();
}

void EatParticle::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	ImGui::Begin("Eat");
	if (ImGui::Button("Create")) {
		Create(pos_);
	}
	ImGui::End();
#endif

	float time = FrameInfo::GetInstance()->GetDeltaTime();

	time_ = std::fmodf(time, 0.1f);

	ParticleUpdate();
}

void EatParticle::Draw()
{
	for (Data& ho : particleData_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(size_, Vector3{ 0.0f,0.0f,0.0f }, ho.pos);
		ParticleManager::GetInstance()->AddParticle(ParticleData{ matrix, Matrix4x4::MakeIdentity4x4(), {ho.color.x,ho.color.y,ho.color.z,ho.alpha} }, tex_);
	}
}

void EatParticle::Create(Vector3 pos)
{
	for (int It = 0; It <= createNumber_; It++) {

		RandomGenerator* rand = RandomGenerator::GetInstance();
		Data data{};
		data.pos = pos;
		data.vel = { rand->RandFloat(velsityXmin,velsityXmax), rand->RandFloat(velsityYmin,velsityYmax),0.0f };
		data.color = color_;
		data.time = 0.0f;
		data.alpha = 1.0f;

		particleData_.push_back(data);

	}

}

void EatParticle::ParticleUpdate(){

	for (std::list<Data>::iterator it = particleData_.begin(); it != particleData_.end();) {
		(*it).time += time_;

		(*it).pos += (*it).vel * time_;
		//下に落とす
		//if ((*it).time > 0.5f) {
		//	
		//}
		(*it).pos.y -= downForce_ * time_ ;
		
		(*it).alpha -= 0.01f;
		//特定の時間になったら
		if ((*it).time >= deleteTime_) {
			it = particleData_.erase(it);
		}
		else {
			it++;
		}
	}
}

void EatParticle::SetGlobalVariable()
{
	global->AddItem("座標", pos_, "Eat");
	global->AddItem("サイズ", size_, "Eat");
	global->AddItem("色", color_, "Eat");
	global->AddItem("削除までの時間", deleteTime_, "Eat");
	global->AddItem("作る個数", createNumber_, "Eat");
	global->AddItem("下に落とす力の倍率", downForce_, "Eat");

	global->AddItem("X軸の幅最小値", velsityXmin, "Eat");
	global->AddItem("X軸の幅最大値", velsityXmax, "Eat");
	global->AddItem("Y軸の幅最小値", velsityYmin, "Eat");
	global->AddItem("Y軸の幅最大値", velsityYmax, "Eat");

	ApplyGlobalVariable();
}

void EatParticle::ApplyGlobalVariable()
{
	pos_ = global->GetVector3Value("座標", "Eat");
	size_ = global->GetVector3Value("サイズ", "Eat");
	color_ = global->GetVector4Value("色", "Eat");
	deleteTime_ = global->GetFloatValue("削除までの時間", "Eat");
	createNumber_ = global->GetIntValue("作る個数", "Eat");
	downForce_ = global->GetFloatValue("下に落とす力の倍率", "Eat");

	velsityXmin = global->GetFloatValue("X軸の幅最小値", "Eat");
	velsityXmax = global->GetFloatValue("X軸の幅最大値", "Eat");
	velsityYmin = global->GetFloatValue("Y軸の幅最小値", "Eat");
	velsityYmax = global->GetFloatValue("Y軸の幅最大値", "Eat");
}