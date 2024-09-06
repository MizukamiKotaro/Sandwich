#pragma once
#include <memory>
#include "Audio.h"

class FinishLive {
public: 
	FinishLive();

	void Initialize();
	void Update(const float& deltaTime);
	const bool& IsClear() const { return isClear_; }

private:
	float finishCount_;
	bool isClear_;
	bool isFinish_;

	std::unique_ptr<Audio> se_;
};