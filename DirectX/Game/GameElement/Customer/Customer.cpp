#include "Customer.h"

void Customer::Init()
{
	object_ = std::make_unique<Object>("customer.png");
	object_->model->transform_.scale_ = scale_;
	object_->model->transform_.translate_ = translate_;

	global = std::make_unique<GlobalVariableUser>("Character", "お客さん");

	SetGlobalVariables();
}

void Customer::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif

	object_->model->transform_.scale_ = scale_;
	object_->model->transform_.translate_ = translate_;

	object_->Update();
}

void Customer::Draw(const Camera* camera)
{
	if (isDraw) {
		object_->Draw(*camera);
	}

}

void Customer::SetGlobalVariables()
{
	global->AddItem("大きさ", scale_, "お客さん");
	global->AddItem("移動", translate_, "お客さん");

	ApplyGlobalVariables();
}

void Customer::ApplyGlobalVariables()
{
	scale_ = global->GetVector3Value("大きさ", "お客さん");
	translate_ = global->GetVector3Value("移動", "お客さん");
}