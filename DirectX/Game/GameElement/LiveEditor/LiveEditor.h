#pragma once
#include <memory>
#include "GameElement/IStageObject/IStageObjectManager.h"
#include "MultipleScreenEditor/MultipleScreenEditor.h"
#include "SpotLightAndOutline/SpotLightAndOutline.h"

class LiveEditor {
public: 

	LiveEditor(Camera* camera);

	void Initialize();

	void Update(const float& time);

	void Draw();

	// スクリーンに書き出す
	void WriteScreen();

private:
	LiveEditor() = default;

	void WriteOutline();

private:
	std::unique_ptr<IStageObjectManager> lightManager_;
	std::unique_ptr<IStageObjectManager> idolManager_;
	std::unique_ptr<IStageObjectManager> fireManager_;
	std::unique_ptr<MultipleScreenEditor> screenManager_;
	std::unique_ptr<IStageObject> floor_;
	std::unique_ptr<IStageObject> dome_;

	std::map<uint32_t, std::unique_ptr<ScreenEditor>>* screenMap_ = nullptr;
	std::map<uint32_t, std::unique_ptr<SpotLightAndOutline>>* lightAndOutlineMap_ = nullptr;

	std::unique_ptr<SpotLightAndOutline> lightAndOutline_;
	Camera* camera_ = nullptr;
	std::unique_ptr<Camera> screenCamera_;
};
