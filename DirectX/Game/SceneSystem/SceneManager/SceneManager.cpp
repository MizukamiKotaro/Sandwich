#include "SceneManager.h"

#include "Kyoko.h"
#include "Input.h"
#include "FrameInfo/FrameInfo.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SceneSystem/SceneFactory/SceneFactory.h"

FrameInfo* frameInfo = nullptr;

SceneManager::SceneManager()
{

	IScene::sceneNo_ = STAGE;

	sceneFactory_ = std::make_unique<SceneFactory>();

	scene_.reset(sceneFactory_->CreateScene(IScene::sceneNo_));

	currentSceneNo_ = IScene::sceneNo_;
	preSceneNo_ = currentSceneNo_;
	IScene::stageNo_ = 0;

	inputManager_ = Input::GetInstance();
	frameInfo = FrameInfo::GetInstance();
}

int SceneManager::Run()
{
	scene_->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// フレームの開始
		if (Kyoko::Engine::ProcessMessage() || (inputManager_->PressedKey(DIK_ESCAPE) && IScene::sceneNo_ == TITLE)) {
			break;
		}

		Kyoko::Engine::FirstUpdateInLoop();

		// 更新処理
		preSceneNo_ = currentSceneNo_;
		currentSceneNo_ = IScene::sceneNo_;

		if (preSceneNo_ != currentSceneNo_) {
			scene_.reset(sceneFactory_->CreateScene(IScene::sceneNo_));
			scene_->Initialize();
			scene_->FirstUpdate();
		}

		scene_->Play();

#ifdef _DEBUG
		ImGui::Begin("SCENE");
		switch (currentSceneNo_)
		{
		case SCENE::SELECT:
			ImGui::Text("SELECT");
			break;
		case SCENE::TITLE:
			ImGui::Text("TITLE");
			break;
		case SCENE::STAGE:
			ImGui::Text("STAGE");
			break;
		case SCENE::CLEAR:
			ImGui::Text("CLEAR");
			break;
		default:
			break;
		}
		ImGui::End();
		ImGui::Begin("フレーム");
		ImGui::Text("フレーム : %4.1f", frameInfo->GetFramerate());
		ImGui::End();
#endif // _DEBUG
		
		scene_->Draw();

		frameInfo->End();
	}

	scene_.reset();
	sceneFactory_.reset();

	return 0;
}