#include "SpotLightAndOutline.h"

SpotLightAndOutline::SpotLightAndOutline()
{
	light_ = std::make_unique<LightDepth>();
	outline_ = std::make_unique<MemoDepth>();
	object_ = std::make_unique<MemoDepth>();

	light_->SetOutlineTex(outline_->GetSRVGPUDescriptorHandle());
	light_->SetOutlineDepth(outline_->GetDepthHandle());
	light_->SetObjectsTex(object_->GetSRVGPUDescriptorHandle());
	light_->SetObjectsDepth(object_->GetDepthHandle());
}

SpotLightAndOutline::~SpotLightAndOutline()
{
}

void SpotLightAndOutline::Initialize()
{
}

void SpotLightAndOutline::Update()
{
}

void SpotLightAndOutline::Draw(const Camera& camera)
{
	outline_->ToReadBarrier();
	object_->ToReadBarrier();
	light_->Draw(camera);
	outline_->ToWriteBarrier();
	object_->ToWriteBarrier();
}

void SpotLightAndOutline::PreDrawLight()
{
	light_->PreDrawScene();
}

void SpotLightAndOutline::PostDrawLight()
{
	light_->PostDrawScene();
}

void SpotLightAndOutline::PreDrawOutline()
{
	outline_->PreDrawScene();
}

void SpotLightAndOutline::PostDrawOutline()
{
	outline_->PostDrawScene();
}

void SpotLightAndOutline::PreDrawObject()
{
	object_->PreDrawScene();
}

void SpotLightAndOutline::PostDrawObject()
{
	object_->PostDrawScene();
}
