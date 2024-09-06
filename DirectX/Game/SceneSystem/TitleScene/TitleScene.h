#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"
#include "HighLumi/HighLumi.h"

#include "ScanNoise/ScanNoise.h"
#include "NegaPosiInverse/NegaPosiInverse.h"
#include "Mosaic/Mosaic.h"
#include "RGBShift/RGBShift.h"

#include "Vignette/Vignette.h"
#include "Noise/Noise.h"
#include "Bloom/Bloom.h"
#include "RadialBlur/RadialBlur.h"
#include "HSVFilter/HSVFilter.h"
#include "Drawers/Skybox/Skybox.h"
#include "SkinningModel/SkinningModel.h"
#include "Drawers/GPUParticle/GPUParticle.h"
#include "SpotLightAndOutline/SpotLightAndOutline.h"

class TitleScene : public IScene
{
public:
	TitleScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void WrightPostEffect();

private:

};

