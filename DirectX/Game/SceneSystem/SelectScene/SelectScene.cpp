#include "SelectScene.h"
#include "Kyoko.h"

SelectScene::SelectScene()
{
	FirstInit();

}

void SelectScene::Initialize()
{

}

void SelectScene::Update()
{

	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		// シーン切り替え
		ChangeScene(STAGE);
		Audio::AllStop();
	}
#ifdef _DEBUG
	else if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(STAGE);
		Audio::AllStop();
	}
#endif // _DEBUG

}

void SelectScene::Draw()
{

	Kyoko::Engine::PreDraw();

	BlackDraw();

	Kyoko::Engine::PostDraw();
}

