#pragma once
#include "Matrix4x4.h"
#include "Vector4.h"
#include "Vector3.h"

struct GPUParticleData {
	Vector3 translate;
	Vector3 scale;
	Vector3 rotate;
	float lifeTime;
	Vector3 velocity;
	float currentTime;
	Vector4 color;
};

struct EmitterSphere
{
	Vector3 translate; // 位置
	float radius; // 射出半径
	uint32_t count; // 射出数
	float frequency; // 射出間隔
	float frequencyTime; // 射出間隔調整用時間
	uint32_t emit; // 射出許可
};