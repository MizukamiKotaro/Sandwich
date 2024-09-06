#include "TitleScene.h"
#include "Kyoko.h"
#include "ImGuiManager/ImGuiManager.h"
#include "ImGuiManager/DopeSheet.h"

TitleScene::TitleScene()
{
	FirstInit();

}

void TitleScene::Initialize()
{

}

void TitleScene::Update()
{
	if ((input_->PressedKey(DIK_RETURN) || input_->PressedGamePadButton(Input::GamePadButton::A))) {
		// シーン切り替え
		ChangeScene(STAGE);
	}
#ifdef _DEBUG
	else if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(SELECT);
	}
#endif
}

void TitleScene::Draw()
{
	WrightPostEffect();

	Kyoko::Engine::PreDraw();

	BlackDraw();

	Kyoko::Engine::PostDraw();
}

void TitleScene::WrightPostEffect()
{
}

