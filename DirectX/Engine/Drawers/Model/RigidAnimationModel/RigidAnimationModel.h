#pragma once
#include "BaseModel/BaseModel.h"

class RigidAnimationModel : public BaseModel
{
public:
	RigidAnimationModel(const std::string& fileName);
	RigidAnimationModel(const ModelData* modelData, const std::string& fileName);

	void Update(const float& time = 0.0f) override;
	void Draw(const Camera& camera, const BlendMode& blendMode = BlendMode::kBlendModeNormal) const override;

public:
	void LoadGLTF(const std::string& fileName);
	void LoadAnimation(const std::string& fileName);

	const Matrix4x4 GetRotateMatrix() override;
	const QuaternionTransform& GetAnimTransform() const { return animTransform_; }
private:
	void AnimationUpdate(const float& time);
	Vector3 CalculateValue(const AnimationCurve<Vector3>& keyframes, const float& time);
	Quaternion CalculateValue(const AnimationCurve<Quaternion>& keyframes, const float& time);

	int32_t Createjoint(const NodeData& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

private:
	std::unique_ptr<Animation> animation_;
	float animationTime_;
	QuaternionTransform animTransform_;
};