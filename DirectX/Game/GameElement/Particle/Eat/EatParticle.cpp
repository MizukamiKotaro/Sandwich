#include "EatParticle.h"

EatParticle::EatParticle()
{
	tex_ = ParticleManager::GetInstance()->GetDrawData("circle.png", BlendMode::kBlendModeNone);
}

void EatParticle::Update()
{
	float time = FrameInfo::GetInstance()->GetDeltaTime();

	RandomGenerator* rand = RandomGenerator::GetInstance();
	Data data{};
	data.pos = pos_;
	data.vel = { rand->RandFloat(-1.5f,1.5f), rand->RandFloat(3.0f,5.0f),0.0f };
	data.color = { 1.0f/*rand->RandFloat(0.5f,1.0f)*/,1.0f/*rand->RandFloat(0.1f,0.9f)*/,1.0f, 1.0f };
	data.time = 0.0f;

	particleData_.push_back(data);
	time_ = std::fmodf(time_, 0.1f);

	for (std::list<Data>::iterator it = particleData_.begin(); it != particleData_.end();) {
		(*it).time += time;

		(*it).pos += (*it).vel * time;
		(*it).vel.y -= 1.0f * time;

		if ((*it).time >= 4.0f) {
			it = particleData_.erase(it);
		}
		else {
			it++;
		}
	}
}

void EatParticle::Draw()
{
	for (Data& ho : particleData_) {
		Matrix4x4 matrix = Matrix4x4::MakeAffinMatrix({ 1.5f,1.5f,1.0f }, Vector3{ 0.0f,0.0f,0.0f }, ho.pos);
		ParticleManager::GetInstance()->AddParticle(ParticleData{ matrix, Matrix4x4::MakeIdentity4x4(), ho.color }, tex_);
	}
}
