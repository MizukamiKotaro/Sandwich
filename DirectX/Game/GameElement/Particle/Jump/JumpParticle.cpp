#include "JumpParticle.h"

JumpParticle::JumpParticle()
{
	global = std::make_unique<GlobalVariableUser>("Particle", "Jump");

	tex_ = ParticleManager::GetInstance()->GetDrawData("cheese.png", BlendMode::kBlendModeNormal);

	SetGlobalVariable();
}

void JumpParticle::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
#endif
	float time = FrameInfo::GetInstance()->GetDeltaTime();

	time_ = std::fmodf(time, 0.1f);

	ParticleUpdate();
}

void JumpParticle::Draw()
{
	for (Data& ho : particleData_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix(size_, Vector3{ 0.0f,0.0f,0.0f }, ho.pos);
		ParticleManager::GetInstance()->AddParticle(ParticleData{ matrix, Matrix4x4::MakeIdentity4x4(), {ho.color.x,ho.color.y,ho.color.z,ho.alpha} }, tex_);
	}
}

void JumpParticle::Create(Vector3 pos)
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

void JumpParticle::ParticleUpdate() {

	for (std::list<Data>::iterator it = particleData_.begin(); it != particleData_.end();) {
		(*it).time += time_;

		(*it).pos += (*it).vel * time_;
		//下に落とす
		if ((*it).time > 0.5f) {
			(*it).vel.y -= downForce_ * time_;
		}


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

void JumpParticle::SetGlobalVariable()
{
	global->AddItem("座標", pos_, "Jump");
	global->AddItem("サイズ", size_, "Jump");
	global->AddItem("色", color_, "Jump");
	global->AddItem("削除までの時間", deleteTime_, "Jump");
	global->AddItem("作る個数", createNumber_, "Jump");
	global->AddItem("下に落とす力の倍率", downForce_, "Jump");

	global->AddItem("X軸の幅最小値", velsityXmin, "Jump");
	global->AddItem("X軸の幅最大値", velsityXmax, "Jump");
	global->AddItem("Y軸の幅最小値", velsityYmin, "Jump");
	global->AddItem("Y軸の幅最大値", velsityYmax, "Jump");

	ApplyGlobalVariable();
}

void JumpParticle::ApplyGlobalVariable()
{
	pos_ = global->GetVector3Value("座標", "Jump");
	size_ = global->GetVector3Value("サイズ", "Jump");
	color_ = global->GetVector4Value("色", "Jump");
	deleteTime_ = global->GetFloatValue("削除までの時間", "Jump");
	createNumber_ = global->GetIntValue("作る個数", "Jump");
	downForce_ = global->GetFloatValue("下に落とす力の倍率", "Jump");

	velsityXmin = global->GetFloatValue("X軸の幅最小値", "Jump");
	velsityXmax = global->GetFloatValue("X軸の幅最大値", "Jump");
	velsityYmin = global->GetFloatValue("Y軸の幅最小値", "Jump");
	velsityYmax = global->GetFloatValue("Y軸の幅最大値", "Jump");
}