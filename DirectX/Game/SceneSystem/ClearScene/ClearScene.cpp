#include "ClearScene.h"
#include "Kyoko.h"

ClearScene::ClearScene()
{
	FirstInit();

}

void ClearScene::Initialize()
{

}

void ClearScene::Update()
{

	if ((input_->PressedKey(DIK_RETURN) || input_->PressedGamePadButton(Input::GamePadButton::A)) && isEnd_) {
		// シーン切り替え
		ChangeScene(SELECT);
		se_.Play();
	}
#ifdef _DEBUG
	else if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(SELECT);
	}
#endif // _DEBUG
}

void ClearScene::Draw()
{

	Kyoko::Engine::PreDraw();

	BlackDraw();

	Kyoko::Engine::PostDraw();
}


