#include "IParticle.h"
#include "TextureManager.h"

const Texture* IParticle::GetTexture() const
{
	return texture_;
}

void IParticle::CreateParticle(const std::string& particleName, const std::string& textureName, bool isStageEditor)
{
	texture_ = TextureManager::GetInstance()->LoadTexture(textureName);
	if (isStageEditor) {
		stageEditor_ = std::make_unique<StageEditor>("パーティクルの設定", particleName);
	}
	else {
		globalVariable_ = std::make_unique<GlobalVariableUser>("Particle", particleName);
	}
	SetGlobalVariable();
}