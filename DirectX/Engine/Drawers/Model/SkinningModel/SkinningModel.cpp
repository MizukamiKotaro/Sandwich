#include "SkinningModel.h"

#include <cassert>
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include "ModelDataManager.h"
#include "Camera.h"
#include "DescriptorHeapManager/DescriptorHandles/DescriptorHandles.h"
#include "calc.h"
#include "DescriptorHeapManager/DescriptorHeapManager.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"
#include "Drawers/DrawManager/DrawManager.h"
#include "ComputePipelineSystem/ComputePipelineManager/ComputePipelineManager.h"
#include "ComputePipelineSystem/ComputePipelineTypeConfig.h"

DescriptorHeap* SkinningModel::srvHeap_ = nullptr;

SkinningModel::SkinningModel(const std::string& fileName)
{
	LoadGLTF(fileName);

	CreateResources();

	InitVariables();
}

SkinningModel::~SkinningModel()
{
	skinCluter_->influenceResouce->Release();
	skinCluter_->paletteResouce->Release();
	skinCluter_->informationResouce->Release();
	skinCluter_->outputVertexResouce->Release();
	srvHeap_->DeleteDescriptor(skinCluter_->influenceSrvHandle);
	srvHeap_->DeleteDescriptor(skinCluter_->inputVertexSrvHandle);
	srvHeap_->DeleteDescriptor(skinCluter_->outputVertexSrvHandle);
	srvHeap_->DeleteDescriptor(skinCluter_->paletteSrvHandle);
}

void SkinningModel::Update(const float& time)
{
	TransformUpdate();
	AnimationUpdate(time);
	UpdateCompute();
}

void SkinningModel::Draw(const Camera& camera, const BlendMode& blendMode) const
{
	drawManager_->Draw(*this, camera, blendMode);
}

void SkinningModel::AnimationUpdate(float time)
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
		ApplyAnimation();
		UpdateSkeleton();
		UpdateSkinAnimation();
	}
}

void SkinningModel::StaticInitialize()
{
	srvHeap_ = DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap();
}

void SkinningModel::LoadGLTF(const std::string& fileName)
{
	modelData_ = modelDataManager_->LoadSkinAnimationModel(fileName);

	texture_ = modelData_->texture;

	srvGPUDescriptorHandle_ = texture_->handles_->gpuHandle;

	LoadAnimation(fileName);
	CreateSkeleton();
	CreateSkinCluster();
}

void SkinningModel::LoadAnimation(const std::string& fileName)
{
	animationTime_ = 0.0f;
	animation_ = std::make_unique<Animation>(modelDataManager_->LoadAnimation(fileName));
}

const Matrix4x4 SkinningModel::GetRotateMatrix()
{
	NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[modelData_->rootNode.name];
	Quaternion rotate = CalculateValue(rootNodeAnimation.rotate, animationTime_);
	return Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_) * Matrix4x4::MakeRotateMatrix(rotate);
}

Vector3 SkinningModel::CalculateValue(const AnimationCurve<Vector3>& keyframes, const float& time)
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

Quaternion SkinningModel::CalculateValue(const AnimationCurve<Quaternion>& keyframes, const float& time) {
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

void SkinningModel::CreateSkeleton()
{
	Skeleton skeleton;
	skeleton.root = Createjoint(modelData_->rootNode, {}, skeleton.joints);

	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}
	skeleton_.reset();
	skeleton_ = std::make_unique<Skeleton>(skeleton);
	UpdateSkeleton();
}

void SkinningModel::CreateSkinCluster()
{
	SkinCluster skinCluster;
	// palette用のResourceを確保
	skinCluster.paletteResouce = DirectXBase::CreateBufferResource(sizeof(WellForGPU) * skeleton_->joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResouce->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette,skeleton_->joints.size() };
	skinCluster.paletteSrvHandle = srvHeap_->GetNewDescriptorHandles();

	// palette用のsrvの作成。StructuredBufferでアクセスできるようにする
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSRVDesc{};
	paletteSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSRVDesc.Buffer.FirstElement = 0;
	paletteSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSRVDesc.Buffer.NumElements = UINT(skeleton_->joints.size());
	paletteSRVDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	DirectXBase::GetInstance()->GetDevice()->CreateShaderResourceView(skinCluster.paletteResouce.Get(), &paletteSRVDesc, skinCluster.paletteSrvHandle->cpuHandle);

	// influence用のResourceを確保。頂点ごとにinfluence情報を追加できるようにする
	skinCluster.influenceResouce = DirectXBase::CreateBufferResource(sizeof(VertexInfluence) * modelData_->mesh.verteces.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResouce->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData_->mesh.verteces.size());
	skinCluster.mappedInfluence = { mappedInfluence,modelData_->mesh.verteces.size() };
	skinCluster.influenceSrvHandle = srvHeap_->GetNewDescriptorHandles();

	// influence用のsrvの作成。StructuredBufferでアクセスできるようにする
	D3D12_SHADER_RESOURCE_VIEW_DESC influenceSRVDesc{};
	influenceSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	influenceSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	influenceSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	influenceSRVDesc.Buffer.FirstElement = 0;
	influenceSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	influenceSRVDesc.Buffer.NumElements = UINT(modelData_->mesh.verteces.size());
	influenceSRVDesc.Buffer.StructureByteStride = sizeof(VertexInfluence);
	DirectXBase::GetInstance()->GetDevice()->CreateShaderResourceView(skinCluster.influenceResouce.Get(), &influenceSRVDesc, skinCluster.influenceSrvHandle->cpuHandle);

	// InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton_->joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), Matrix4x4::MakeIdentity4x4);

	// ModelDataを解析してInfluenceを埋める
	for (const std::pair<const std::string, JointWeightData>& jointWeight : modelData_->skinClusterData) {
		std::map<std::string, int32_t>::iterator it = skeleton_->jointMap.find(jointWeight.first);
		if (it == skeleton_->jointMap.end()) {
			continue;
		}
		
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const VertexWeightData& vertexWeight : jointWeight.second.vertexWeights) {
			VertexInfluence& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; index++) {
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	// inputVertex用のsrvの作成。StructuredBufferでアクセスできるようにする
	skinCluster.inputVertexSrvHandle = srvHeap_->GetNewDescriptorHandles();
	D3D12_SHADER_RESOURCE_VIEW_DESC inputVertexSRVDesc{};
	inputVertexSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	inputVertexSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	inputVertexSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	inputVertexSRVDesc.Buffer.FirstElement = 0;
	inputVertexSRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	inputVertexSRVDesc.Buffer.NumElements = UINT(modelData_->mesh.verteces.size());
	inputVertexSRVDesc.Buffer.StructureByteStride = sizeof(VertexData);
	DirectXBase::GetInstance()->GetDevice()->CreateShaderResourceView(modelData_->mesh.vertexResource_.Get(), &inputVertexSRVDesc, skinCluster.inputVertexSrvHandle->cpuHandle);

	// outputVertex用のuavの作成
	skinCluster.outputVertexResouce = DirectXBase::CreateBufferResource(sizeof(VertexData) * modelData_->mesh.verteces.size(), D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	
	skinCluster.outputVertexSrvHandle = srvHeap_->GetNewDescriptorHandles();
	D3D12_UNORDERED_ACCESS_VIEW_DESC outputVertexUAVDesc{};
	outputVertexUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	outputVertexUAVDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	outputVertexUAVDesc.Buffer.FirstElement = 0;
	outputVertexUAVDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	outputVertexUAVDesc.Buffer.NumElements = UINT(modelData_->mesh.verteces.size());
	outputVertexUAVDesc.Buffer.StructureByteStride = sizeof(VertexData);
	DirectXBase::GetInstance()->GetDevice()->CreateUnorderedAccessView(skinCluster.outputVertexResouce.Get(), nullptr, &outputVertexUAVDesc, skinCluster.outputVertexSrvHandle->cpuHandle);


	// informationのConstantBuffer
	skinCluster.informationResouce = DirectXBase::CreateBufferResource(sizeof(uint32_t));
	skinCluster.information = nullptr;
	skinCluster.informationResouce->Map(0, nullptr, reinterpret_cast<void**>(&skinCluster.information));
	*skinCluster.information = uint32_t(modelData_->mesh.verteces.size());

	skinCluster.vertexBufferView.BufferLocation = skinCluster.outputVertexResouce->GetGPUVirtualAddress();
	skinCluster.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData_->mesh.verteces.size());
	skinCluster.vertexBufferView.StrideInBytes = sizeof(VertexData);

	skinCluter_.reset();
	skinCluter_ = std::make_unique<SkinCluster>(skinCluster);
}

int32_t SkinningModel::Createjoint(const NodeData& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
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

void SkinningModel::UpdateSkeleton()
{
	if (skeleton_) {
		for (Joint& joint : skeleton_->joints) {
			joint.localMatrix = Matrix4x4::MakeAffinMatrix(joint.transform);
			if (joint.parent) {
				joint.skeletonSpaceMatrix = joint.localMatrix * skeleton_->joints[*joint.parent].skeletonSpaceMatrix;
			}
			else {
				joint.skeletonSpaceMatrix = joint.localMatrix;
			}
		}
	}
}

void SkinningModel::UpdateSkinAnimation()
{
	for (size_t jointIndex = 0; jointIndex < skeleton_->joints.size(); jointIndex++) {
		assert(jointIndex < skinCluter_->inverseBindPoseMatrices.size());
		skinCluter_->mappedPalette[jointIndex].skeletonSpaceMatrix = skinCluter_->inverseBindPoseMatrices[jointIndex] * skeleton_->joints[jointIndex].skeletonSpaceMatrix;
		skinCluter_->mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix = Matrix4x4::Transpose(Matrix4x4::Inverse(skinCluter_->mappedPalette[jointIndex].skeletonSpaceMatrix));
	}
}

void SkinningModel::UpdateCompute()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { DescriptorHeapManager::GetInstance()->GetSRVDescriptorHeap()->GetHeap() };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps);
	ComputePipelineManager::GetInstance()->PreCompute(ComputePipelineType::SKINNING);
	commandList_->SetComputeRootDescriptorTable(0, skinCluter_->paletteSrvHandle->gpuHandle);
	commandList_->SetComputeRootDescriptorTable(1, skinCluter_->inputVertexSrvHandle->gpuHandle);
	commandList_->SetComputeRootDescriptorTable(2, skinCluter_->influenceSrvHandle->gpuHandle);
	commandList_->SetComputeRootDescriptorTable(3, skinCluter_->outputVertexSrvHandle->gpuHandle);
	commandList_->SetComputeRootConstantBufferView(4, skinCluter_->informationResouce->GetGPUVirtualAddress());
	commandList_->Dispatch(UINT(modelData_->mesh.verteces.size() + 1023) / 1024, 1, 1);

	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrierDesc.Transition.pResource = skinCluter_->outputVertexResouce.Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;  // Compute Shader の書き込み後の状態
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;  // Vertex Shader での読み取りに適した状態
	barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	commandList_->ResourceBarrier(1, &barrierDesc);

}

void SkinningModel::ApplyAnimation()
{
	for (Joint& joint : skeleton_->joints) {
		if (auto it = animation_->nodeAnimations.find(joint.name); it != animation_->nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate_ = CalculateValue(rootNodeAnimation.translate, animationTime_);
			joint.transform.rotate_ = CalculateValue(rootNodeAnimation.rotate, animationTime_);
			joint.transform.scale_ = CalculateValue(rootNodeAnimation.scale, animationTime_);
		}
	}
}