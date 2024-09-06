#include "CameraEditor.h"
#include "Camera.h"
#include "TextureManager.h"

CameraEditor::CameraEditor(const std::string& mainName, const std::string& name, const uint32_t& no)
{
	CreateStageEditor(mainName, name, no);
	SetGlobalVariable();
}

void CameraEditor::Initialize()
{
}

void CameraEditor::Update(const float& time)
{
#ifdef _DEBUG
	UpdateGlobalVariable();
#endif // _DEBUG
	if(time){}
}

void CameraEditor::Draw(const Camera& camera)
{
	if(&camera){}
}

void CameraEditor::SetGlobalVariable()
{
	ApplyGlobalVariable();
}

void CameraEditor::ApplyGlobalVariable()
{
	
}
