#pragma once
#include "GameElement/Object/Object.h"
#include "GlobalVariables/GlobalVariableUser.h"

class Customer
{
public:

	void Init();
	void Update();
	void Draw(const Camera* camera);

	bool isDraw = false;
private:
	void SetGlobalVariables();
	void ApplyGlobalVariables();

	//3Dモデル
	std::unique_ptr<Object> object_;

	Vector3 scale_ = { 19.0f,2.0f,1.0f };
	Vector3 translate_ = { 0.0f,-20.0f,0.0f };

	std::unique_ptr<GlobalVariableUser> global;
};

