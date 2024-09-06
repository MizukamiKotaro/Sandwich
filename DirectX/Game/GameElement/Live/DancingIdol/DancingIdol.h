#pragma once
#include "SkinningModel/SkinningModel.h"
#include <memory>

class Camera;

class DancingIdol {
public: 

	DancingIdol();

	void Initialize();

	void Update(const float& time);

	void Draw(Camera* camera);

private:
	
	std::unique_ptr<SkinningModel> idol_;
};
