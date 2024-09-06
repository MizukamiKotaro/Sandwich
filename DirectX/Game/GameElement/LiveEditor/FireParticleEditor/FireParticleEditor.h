#pragma once
#include "GameElement/IStageObject/IStageObject.h"
#include "ParticleManager.h"
#include <memory>

class FireParticleEditor : public IStageObject
{
public: 

	FireParticleEditor(const std::string& mainName, const std::string& name, const uint32_t& no);
	void Update(const float& time) override;
	void Draw(const Camera& camera) override;

private:
	void SetGlobalVariable() override;
	void ApplyGlobalVariable() override;
	
	struct hoge {
		Vector3 pos;
		Vector3 vel;
		Vector4 color;
		float time;
	};

	std::list<hoge> aho_;
	uint32_t num_;
	Vector3 pos_;
	float time_;

	const ParticleMeshTexData* tex_;
};
