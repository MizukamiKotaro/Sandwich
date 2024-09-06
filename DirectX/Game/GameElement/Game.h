#pragma once
#include <memory>
#include "Camera.h"
#include "Audio.h"
#include "Input.h"
#include "Object/Object.h"

class Game {
public: 
	Game(Camera* camera);

	void Initialize();
	void Update();
	void Draw();
	void FirstUpdate();
	const bool& GetIsClear() const { return IsClear; }

private:
	Object* object_;
private:
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;


private:
	bool IsClear = false;
};
