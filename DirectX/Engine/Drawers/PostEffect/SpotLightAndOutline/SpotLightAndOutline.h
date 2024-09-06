#pragma once
#include "MemoDepth/MemoDepth.h"
#include "LightDepth/LightDepth.h"
#include <memory>

class SpotLightAndOutline
{
public:

	SpotLightAndOutline();

	~SpotLightAndOutline();

	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	void PreDrawLight();
	void PostDrawLight();

	void PreDrawOutline();
	void PostDrawOutline();

	void PreDrawObject();
	void PostDrawObject();

private:
	std::unique_ptr<LightDepth> light_;
	std::unique_ptr<MemoDepth> outline_;
	std::unique_ptr<MemoDepth> object_;

};

