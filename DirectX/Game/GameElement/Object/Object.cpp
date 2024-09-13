#include "Object.h"

Object::Object(const std::string& textureName) {
	model = std::make_unique<Model>("plane");
	//テクスチャをセット
	model->SetTexture(TextureManager::GetInstance()->LoadTexture(textureName));
	model->UnUsedLight();
}

void Object::Update()
{
	model->Update();
}

void Object::Draw(const Camera& camera)
{
	model->Draw(camera);
}

