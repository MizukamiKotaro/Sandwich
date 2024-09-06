#pragma once
#include "Model.h"
#include "TextureManager.h"
#include "Camera.h"

class Object {
public:
	Object(const std::string& textureName);

	void Draw(const Camera& camera);

private://プライベート関数
	//Planeモデル
	std::unique_ptr<Model> model;

};