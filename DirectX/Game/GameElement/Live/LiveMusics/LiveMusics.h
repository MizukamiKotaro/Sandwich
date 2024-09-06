#pragma once
#include <memory>
#include "Audio.h"

class LiveMusics {
public: 
	LiveMusics();

	void Initialize();
	void Update();
	const bool& IsFinish() const { return isMusicFinish_; }

private:
	std::unique_ptr<Audio> music_;
	bool isMusicStart_;
	bool isMusicFinish_;
};