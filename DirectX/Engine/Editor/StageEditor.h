#pragma once
#include "IEditor.h"

class StageEditor : public IEditor
{
public:
	StageEditor(const std::string& tree1, const std::string& tree2 = "_", const std::string& tree3 = "_", const std::string& tree4 = "_", const std::string& tree5 = "_");
	StageEditor();
};