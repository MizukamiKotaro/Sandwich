#pragma once
#include "ParticleManager.h"
#include <memory>
#include "TextureManager.h"
#include "RandomGenerator/RandomGenerator.h"
#include "FrameInfo/FrameInfo.h"

class EatParticle {
public:
	EatParticle();

	void Update();
	void Draw();

private:
	struct Data {
		Vector3 pos;
		Vector3 vel;
		Vector4 color;
		float time;
	};
	uint32_t num_;
	Vector3 pos_ = {0.0f,0.0f,1.0f};
	float time_;

	std::list<Data> particleData_;
	//パーティクルオブジェクト
	const ParticleMeshTexData* tex_;
};