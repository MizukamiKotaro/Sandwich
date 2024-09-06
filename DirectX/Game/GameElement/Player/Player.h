#pragma once
#include "GameElement/Object/Object.h"

class Player {
public://パブリック関数
#pragma region
	void Init();
	void Update();
	void Draw(const Camera* camera);
#pragma endregion
public://パブリック変数
#pragma region 

#pragma endregion
private://プライベート関数
#pragma region 

#pragma endregion
private://プライベート変数
#pragma region 
	//3Dモデル
	std::unique_ptr<Object> object_;
#pragma endregion
};