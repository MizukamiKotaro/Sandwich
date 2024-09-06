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

private:

private:
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;

private:
};
