#pragma once
#include "GameElement/Object/Object.h"

class Floor {
public:
	void Init();
	void Update();
	void Draw(const Camera* camera);

private:
	//3Dモデル
	std::unique_ptr<Object> object_;
};