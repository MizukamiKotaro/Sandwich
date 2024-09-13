#pragma once
#include "ParticleManager.h"
#include <memory>
#include "TextureManager.h"
#include "RandomGenerator/RandomGenerator.h"
#include "FrameInfo/FrameInfo.h"
#include "GlobalVariables/GlobalVariableUser.h"
#include "ImGuiManager/ImGuiManager.h"

class JumpParticle {
public:
	JumpParticle();

	void Update();
	void Draw();

	void Create(Vector3 pos);
private:
	void ParticleUpdate();

	struct Data {
		Vector3 pos;
		Vector3 vel;
		Vector4 color;
		float time;
		float alpha;
	};
	//初期位置
	Vector3 pos_ = { 0.0f,0.0f,1.0f };
	//サイズ
	Vector3 size_ = { 1.0f,1.0f,1.0f };
	float time_;
	//色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//消えるまでの時間
	float deleteTime_ = 1.0f;
	//作る個数
	int createNumber_ = 6;
	//下に落とす力の倍率
	float downForce_ = 1.0f;

	float velsityXmin = -1.5f;
	float velsityXmax = 1.5f;

	float velsityYmin = -3.0f;
	float velsityYmax = 1.5f;

	std::list<Data> particleData_;
	//パーティクルオブジェクト
	const ParticleMeshTexData* tex_;

	std::unique_ptr<GlobalVariableUser> global;

	void SetGlobalVariable();
	void ApplyGlobalVariable();
};