#pragma once
#include <memory>
#include "GameElement/LiveEditor/LiveEditor.h"
#include "GameElement/Notes/Notes.h"
#include "GameElement/Score/Score.h"
#include "GameElement/StageUI/StageUI.h"
#include "GameElement/Live/LiveMusics/LiveMusics.h"
#include "GameElement/Live/FinishLive/FinishLive.h"
#include "Camera.h"
#include "Audio.h"
#include "Input.h"

class Game {
public: 
	Game(Camera* camera);

	void Initialize();
	void Update();
	void Draw();
	void FirstUpdate();
	const bool& GetIsClear() const { return finishLive_->IsClear(); }

private:

private:
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;

private:
	
	std::unique_ptr<LiveEditor> live_;
	std::unique_ptr<Score> score_;
	std::unique_ptr<StageUI> stageUI_;
	std::unique_ptr<LiveMusics> music_;
	std::unique_ptr<FinishLive> finishLive_;
	NotesList* notesList_ = nullptr;

	float time_;
#ifdef _DEBUG
	bool isEdit_;
#endif // _DEBUG

};
