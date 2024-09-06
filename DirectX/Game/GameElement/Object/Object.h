#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "Camera.h"

class Object {
public:
	Object(const std::string& textureName);

	void Update();
	void Draw(const Camera& camera);

	const EulerTransform& GetWorldTransform() const { return model->transform_; };

	void SetTranslate(Vector3 pos) { model->transform_.translate_ = pos; };
	//Planeモデル
	std::unique_ptr<Model> model;
private://プライベート関数


};