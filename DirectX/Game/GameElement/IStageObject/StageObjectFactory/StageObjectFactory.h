#pragma once
#include <string>
enum class StageObjectType;
class IStageObject;

class StageObjectFactory
{
public:
	static IStageObject* CreateStageObject(const StageObjectType& type, const std::string& mainName, const std::string& name, const uint32_t& no);
};