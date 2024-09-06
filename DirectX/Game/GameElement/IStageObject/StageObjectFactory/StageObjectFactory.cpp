#include "StageObjectFactory.h"
#include "GameElement/IStageObject/StageObjectConfig.h"
#include "GameElement/LiveEditor/DancingIdolEditor/DancingIdolEditor.h"
#include "GameElement/LiveEditor/MultipleScreenEditor/MultipleScreenEditor.h"
#include "GameElement/LiveEditor/ScreenEditor/ScreenEditor.h"
#include "GameElement/LiveEditor/SpotLightBoxAnimationEditor/SpotLightBoxAnimationEditor.h"
#include "GameElement/LiveEditor/SpotLightBoxAnimationEditor/TwinSpotLightBoxAnimationEditor.h"
#include "GameElement/LiveEditor/StageFloorEditor/StageFloorEditor.h"
#include "GameElement/LiveEditor/DomeEditor/DomeEditor.h"
#include "GameElement/LiveEditor/FireParticleEditor/FireParticleEditor.h"

IStageObject* StageObjectFactory::CreateStageObject(const StageObjectType& type, const std::string& mainName, const std::string& name, const uint32_t& no)
{
    IStageObject* object = nullptr;
	switch (type)
	{
	case StageObjectType::SPOTLIGHT:
		object = new SpotLightBoxAnimationEditor(mainName, name, no);
		break;
	case StageObjectType::TWIN_SPOTLIGHT:
		object = new TwinSpotLightBoxAnimationEditor(mainName, name, no);
		break;
	case StageObjectType::SCREEN:
		object = new ScreenEditor(mainName, name, no);
		break;
	case StageObjectType::MULTIPLE_SCREEN:
		object = new MultipleScreenEditor(mainName, name, no);
		break;
	case StageObjectType::IDOL:
		object = new DancingIdolEditor(mainName, name, no);
		break;
	case StageObjectType::FLOOR:
		object = new StageFloorEditor(mainName, name, no);
		break;
	case StageObjectType::DOME:
		object = new DomeEditor(mainName, name, no);
		break;
	case StageObjectType::FIRE_PARTICLE:
		object = new FireParticleEditor(mainName, name, no);
		break;
	default:
		break;
	}
    return object;
}
