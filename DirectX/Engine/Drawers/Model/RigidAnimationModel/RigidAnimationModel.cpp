#include "RigidAnimationModel.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "ModelDataManager.h"
#include "Camera.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "calc.h"
#include "DescriptorHeapManager/DescriptorHeapManager.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "Drawers/DrawManager/DrawManager.h"

RigidAnimationModel::RigidAnimationModel(const std::string& fileName)
{
	LoadGLTF(fileName);
	CreateResources();
	InitVariables();
	AnimationUpdate(0.0f);
}

RigidAnimationModel::RigidAnimationModel(const ModelData* modelData, const std::string& fileName)
{
	modelData_ = modelData;
	texture_ = modelData_->texture;
	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;
	LoadAnimation(fileName);
	CreateResources();
	InitVariables();
	AnimationUpdate(0.0f);
}

void RigidAnimationModel::Update(const float& time)
{
	TransformUpdate();
	AnimationUpdate(time);
}

void RigidAnimationModel::Draw(const Camera& camera, const BlendMode& blendMode) const
{
	drawManager_->Draw(*this, camera, blendMode);
}

void RigidAnimationModel::AnimationUpdate(const float& time)
{
	if (animation_) {
		if (time < 0) {
			animationTime_ += time;
			if (animationTime_ < 0) {
				animationTime_ = animation_->duration + animationTime_;
			}
		}
		else {
			animationTime_ += time;
			animationTime_ = std::fmod(animationTime_, animation_->duration);
		}

		NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[modelData_->rootNode.name];
		animTransform_.translate_ = CalculateValue(rootNodeAnimation.translate, animationTime_);
		animTransform_.rotate_ = CalculateValue(rootNodeAnimation.rotate, animationTime_);
		animTransform_.scale_ = CalculateValue(rootNodeAnimation.scale, animationTime_);
		animTransform_.Update();
	}
}

void RigidAnimationModel::LoadGLTF(const std::string& fileName)
{
	modelData_ = modelDataManager_->LoadGLTF(fileName);

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;

	LoadAnimation(fileName);
}

void RigidAnimationModel::LoadAnimation(const std::string& fileName)
{
	animationTime_ = 0.0f;
	animation_ = std::make_unique<Animation>(modelDataManager_->LoadAnimation(fileName));
}

const Matrix4x4 RigidAnimationModel::GetRotateMatrix()
{
	NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[modelData_->rootNode.name];
	Quaternion rotate = CalculateValue(rootNodeAnimation.rotate, animationTime_);
	return  Matrix4x4::MakeRotateMatrix(rotate) * Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_);
}

Vector3 RigidAnimationModel::CalculateValue(const AnimationCurve<Vector3>& keyframes, const float& time)
{
	assert(!keyframes.keyframes.empty());
	if (keyframes.keyframes.size() == 1 || time <= keyframes.keyframes[0].time) {
		return keyframes.keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.keyframes.size() - 1; index++) {
		size_t nextIndex = index + 1;
		if (keyframes.keyframes[index].time <= time && time <= keyframes.keyframes[nextIndex].time) {
			float t = (time - keyframes.keyframes[index].time) / (keyframes.keyframes[nextIndex].time - keyframes.keyframes[index].time);
			return Calc::Lerp(keyframes.keyframes[index].value, keyframes.keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.keyframes.rbegin()).value;
}

Quaternion RigidAnimationModel::CalculateValue(const AnimationCurve<Quaternion>& keyframes, const float& time) {
	assert(!keyframes.keyframes.empty());
	if (keyframes.keyframes.size() == 1 || time <= keyframes.keyframes[0].time) {
		return keyframes.keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.keyframes.size() - 1; index++) {
		size_t nextIndex = index + 1;
		if (keyframes.keyframes[index].time <= time && time <= keyframes.keyframes[nextIndex].time) {
			float t = (time - keyframes.keyframes[index].time) / (keyframes.keyframes[nextIndex].time - keyframes.keyframes[index].time);
			return Quaternion::Slerp(keyframes.keyframes[index].value, keyframes.keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.keyframes.rbegin()).value;
}

int32_t RigidAnimationModel::Createjoint(const NodeData& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const NodeData& child : node.children) {
		int32_t childIndex = Createjoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	return 0;
}