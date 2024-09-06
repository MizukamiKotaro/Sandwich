#pragma once
#include "Model.h"

class Object {
public:
	Object() {

		model->SetTexture();
	}


private://プライベート関数
	//Planeモデル
	std::unique_ptr<Model> model;

};