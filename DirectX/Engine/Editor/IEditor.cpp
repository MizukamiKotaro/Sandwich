#include "IEditor.h"
#include "SceneSystem/IScene/IScene.h"

void IEditor::Initialize()
{
	global_->ResetGroupName("Stage" + std::to_string(IScene::stageNo_));
}

void IEditor::AddItem(const std::string& key, const int32_t& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItem(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::AddItem(const std::string& key, const float& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItem(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::AddItem(const std::string& key, const Vector2& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItem(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::AddItem(const std::string& key, const Vector3& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItem(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::AddItem(const std::string& key, const Vector4& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItem(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::AddItem(const std::string& key, const bool& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItem(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::AddItem(const std::string& key, const std::string& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItem(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::AddItemColor(const std::string& key, const Vector4& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->AddItemColor(key, value, tree2, tree3, tree4, tree5, tree6);
}

const int32_t& IEditor::GetIntValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetIntValue(key, tree2, tree3, tree4, tree5, tree6);
}

const float& IEditor::GetFloatValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetFloatValue(key, tree2, tree3, tree4, tree5, tree6);
}

const Vector2& IEditor::GetVector2Value(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetVector2Value(key, tree2, tree3, tree4, tree5, tree6);
}

const Vector3& IEditor::GetVector3Value(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetVector3Value(key, tree2, tree3, tree4, tree5, tree6);
}

const Vector4& IEditor::GetVector4Value(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetVector4Value(key, tree2, tree3, tree4, tree5, tree6);
}

const bool& IEditor::GetBoolValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetBoolValue(key, tree2, tree3, tree4, tree5, tree6);
}

const std::string& IEditor::GetStringValue(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetStringValue(key, tree2, tree3, tree4, tree5, tree6);
}

const Vector4& IEditor::GetColor(const std::string& key, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6) const
{
	return global_->GetColor(key, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::SetVariable(const std::string& key, const int32_t& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->SetVariable(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::SetVariable(const std::string& key, const float& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->SetVariable(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::SetVariable(const std::string& key, const Vector2& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->SetVariable(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::SetVariable(const std::string& key, const Vector3& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->SetVariable(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::SetVariable(const std::string& key, const Vector4& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->SetVariable(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::SetVariable(const std::string& key, const bool& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->SetVariable(key, value, tree2, tree3, tree4, tree5, tree6);
}

void IEditor::SetVariable(const std::string& key, const std::string& value, const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	global_->SetVariable(key, value, tree2, tree3, tree4, tree5, tree6);
}

bool IEditor::IsTreeOpen(const std::string& tree2, const std::string& tree3, const std::string& tree4, const std::string& tree5, const std::string& tree6)
{
	return global_->IsTreeOpen(tree2, tree3, tree4, tree5, tree6);
}

bool IEditor::IsTreeOpen(const int& treeNo)
{
	return global_->IsTreeOpen(treeNo);
}

const bool IEditor::IsChangedStage()
{
	if (preStageNo_ != IScene::stageNo_) {
		preStageNo_ = IScene::stageNo_;
		return true;
	}
	return false;
}

