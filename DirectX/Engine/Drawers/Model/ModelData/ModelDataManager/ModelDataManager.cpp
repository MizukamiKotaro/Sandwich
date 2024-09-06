#include "ModelDataManager.h"
#include "Engine/Base/DirectXBase/DirectXBase.h"
#include <cassert>
#include "Engine/Base/DebugLog/DebugLog.h"
#include <format>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "TextureManager/TextureManager.h"
#include "ModelData/ModelData.h"

ModelDataManager* ModelDataManager::GetInstance()
{
	static ModelDataManager instance;
	return &instance;
}

void ModelDataManager::Finalize()
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {
		modelDatas_[modelNum]->mesh.vertexResource_->Release();
		modelDatas_[modelNum]->mesh.indexResource_->Release();
	}
}

const ModelData* ModelDataManager::LoadObj(const std::string& fileName)
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {

		if (modelDatas_[modelNum]->fileName == fileName) {
			return modelDatas_[modelNum].get();
		}
	}

	LoadObjFile(directoryPath_, fileName);

	return modelDatas_.back().get();
}

const ModelData* ModelDataManager::LoadGLTF(const std::string& fileName)
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {

		if (modelDatas_[modelNum]->fileName == fileName) {
			return modelDatas_[modelNum].get();
		}
	}

	LoadGLTFFile(directoryPath_, fileName);

	return modelDatas_.back().get();
}

Animation ModelDataManager::LoadAnimation(const std::string& fileName)
{
	return LoadAnimation(directoryPath_, fileName);
}

const ModelData* ModelDataManager::LoadSkinAnimationModel(const std::string& fileName)
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {

		if (modelDatas_[modelNum]->fileName == fileName) {
			return modelDatas_[modelNum].get();
		}
	}

	LoadSkinAnimationFile(directoryPath_, fileName);

	return modelDatas_.back().get();
}

void ModelDataManager::LoadObjFile(const std::string& directoryPath, const std::string& fileName)
{
	// 1. 中で必要となる変数の宣言
	modelDatas_.push_back(std::make_unique<ModelData>());; // 構築するModelData

	modelDatas_.back()->fileName = fileName;

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName + "/" + fileName + ".obj";
	const aiScene* scene_ = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene_->HasMeshes());
	uint32_t vertexNum = 0;
	// meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene_->mNumMeshes; meshIndex++) {
		aiMesh* mesh = scene_->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないmeshは非対応
		assert(mesh->HasTextureCoords(0)); // texCoordがないmeshは非対応
		
		// vertexを解析
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			modelDatas_.back()->mesh.verteces.push_back(VertexData{ { -position.x,position.y,position.z,1.0f },{ texcoord.x,texcoord.y },{ -normal.x,normal.y,normal.z } });
		}


		// indexを解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			for (uint32_t element = 0; element < face.mNumIndices; element++) {
				modelDatas_.back()->mesh.indices.push_back(face.mIndices[element] + vertexNum);
			}
		}
		vertexNum += mesh->mNumVertices;
	}
	// rootNodeの解析
	modelDatas_.back()->rootNode = ReadNode(scene_->mRootNode);

	bool isLoadTexture = false;
	// materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene_->mNumMaterials; materialIndex++) {
		aiMaterial* material = scene_->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			std::string texFilePath;
			if (textureFilePath.length > 0) {
				texFilePath = textureFilePath.C_Str();
				std::filesystem::path filePathName(texFilePath);
				texFilePath = filePathName.filename().string();
			}
			else {
				texFilePath = "white.png";
			}
			isLoadTexture = true;
			modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
		}
	}

	if (!isLoadTexture) {
		std::string texFilePath = "white.png";
		modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
	}

	modelDatas_.back()->mesh.indexResource_ = DirectXBase::CreateBufferResource(sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());

	modelDatas_.back()->mesh.indexBufferView_.BufferLocation = modelDatas_.back()->mesh.indexResource_->GetGPUVirtualAddress();
	modelDatas_.back()->mesh.indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());
	modelDatas_.back()->mesh.indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	modelDatas_.back()->mesh.indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.mappedIndex));
	std::memcpy(modelDatas_.back()->mesh.mappedIndex, modelDatas_.back()->mesh.indices.data(), sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());

	modelDatas_.back()->mesh.vertexResource_ = DirectXBase::CreateBufferResource(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());

	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	modelDatas_.back()->mesh.vertexBufferView_.BufferLocation = modelDatas_.back()->mesh.vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	modelDatas_.back()->mesh.vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
	//頂点当たりのサイズ
	modelDatas_.back()->mesh.vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	modelDatas_.back()->mesh.vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.vertexData_));
	std::memcpy(modelDatas_.back()->mesh.vertexData_, modelDatas_.back()->mesh.verteces.data(), sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
}

NodeData ModelDataManager::ReadNode(aiNode* node)
{
	NodeData result;
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);
	result.transform.scale_ = { scale.x,scale.y,scale.z };
	result.transform.rotate_ = { rotate.x,-rotate.y,-rotate.z,rotate.w };
	result.transform.translate_ = { -translate.x,translate.y,translate.z };
	result.localMatrix = Matrix4x4::MakeAffinMatrix(result.transform);

	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

Animation ModelDataManager::LoadAnimation(const std::string& directoryPath, const std::string& fileName)
{
	Animation animation;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName + "/" + fileName + ".gltf";
	const aiScene* scene_ = importer.ReadFile(filePath.c_str(), 0);
	assert(scene_->mNumAnimations != 0);
	aiAnimation* animationAssimp = scene_->mAnimations[0];
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; channelIndex++) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];

		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

		for (uint32_t positionIndex = 0; positionIndex < nodeAnimationAssimp->mNumPositionKeys; positionIndex++) {
			aiVectorKey& key = nodeAnimationAssimp->mPositionKeys[positionIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(key.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { -key.mValue.x,key.mValue.y ,key.mValue.z };

			nodeAnimation.translate.keyframes.push_back(keyframe);
		}
		for (uint32_t rotateIndex = 0; rotateIndex < nodeAnimationAssimp->mNumRotationKeys; rotateIndex++) {
			aiQuatKey key = nodeAnimationAssimp->mRotationKeys[rotateIndex];
			KeyframeQuaternion keyframe;
			keyframe.time = float(key.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { key.mValue.x,-key.mValue.y ,-key.mValue.z, key.mValue.w };

			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		for (uint32_t scaleIndex = 0; scaleIndex < nodeAnimationAssimp->mNumScalingKeys; scaleIndex++) {
			aiVectorKey key = nodeAnimationAssimp->mScalingKeys[scaleIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(key.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { key.mValue.x,key.mValue.y ,key.mValue.z };

			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}

	return animation;
}

void ModelDataManager::LoadGLTFFile(const std::string& directoryPath, const std::string& fileName)
{
	// 1. 中で必要となる変数の宣言
	modelDatas_.push_back(std::make_unique<ModelData>());; // 構築するModelData

	modelDatas_.back()->fileName = fileName;

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName + "/" + fileName + ".gltf";
	const aiScene* scene_ = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene_->HasMeshes());

	uint32_t vertexNum = 0;
	// meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene_->mNumMeshes; meshIndex++) {
		aiMesh* mesh = scene_->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないmeshは非対応
		assert(mesh->HasTextureCoords(0)); // texCoordがないmeshは非対応

		// vertexを解析
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			modelDatas_.back()->mesh.verteces.push_back(VertexData{ { -position.x,position.y,position.z,1.0f },{ texcoord.x,texcoord.y },{ -normal.x,normal.y,normal.z } });
		}

		// indexを解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			for (uint32_t element = 0; element < face.mNumIndices; element++) {
				modelDatas_.back()->mesh.indices.push_back(face.mIndices[element] + vertexNum);
			}
		}
		vertexNum += mesh->mNumVertices;
	}
	// rootNodeの解析
	modelDatas_.back()->rootNode = ReadNode(scene_->mRootNode);

	bool isLoadTexture = false;
	// materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene_->mNumMaterials; materialIndex++) {
		aiMaterial* material = scene_->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			std::string texFilePath;
			if (textureFilePath.length > 0) {
				texFilePath = textureFilePath.C_Str();
				std::filesystem::path filePathName(texFilePath);
				texFilePath = filePathName.filename().string();
			}
			else {
				texFilePath = "white.png";
			}
			isLoadTexture = true;
			modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
		}
	}

	if (!isLoadTexture) {
		std::string texFilePath = "white.png";
		modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
	}

	modelDatas_.back()->mesh.indexResource_ = DirectXBase::CreateBufferResource(sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());

	modelDatas_.back()->mesh.indexBufferView_.BufferLocation = modelDatas_.back()->mesh.indexResource_->GetGPUVirtualAddress();
	modelDatas_.back()->mesh.indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());
	modelDatas_.back()->mesh.indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	modelDatas_.back()->mesh.indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.mappedIndex));
	std::memcpy(modelDatas_.back()->mesh.mappedIndex, modelDatas_.back()->mesh.indices.data(), sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());

	modelDatas_.back()->mesh.vertexResource_ = DirectXBase::CreateBufferResource(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());

	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	modelDatas_.back()->mesh.vertexBufferView_.BufferLocation = modelDatas_.back()->mesh.vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	modelDatas_.back()->mesh.vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
	//頂点当たりのサイズ
	modelDatas_.back()->mesh.vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	modelDatas_.back()->mesh.vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.vertexData_));
	std::memcpy(modelDatas_.back()->mesh.vertexData_, modelDatas_.back()->mesh.verteces.data(), sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
}

void ModelDataManager::LoadSkinAnimationFile(const std::string& directoryPath, const std::string& fileName)
{
	// 1. 中で必要となる変数の宣言
	modelDatas_.push_back(std::make_unique<ModelData>());; // 構築するModelData

	modelDatas_.back()->fileName = fileName;

	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName + "/" + fileName + ".gltf";
	const aiScene* scene_ = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene_->HasMeshes());

	// meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene_->mNumMeshes; meshIndex++) {
		aiMesh* mesh = scene_->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないmeshは非対応
		assert(mesh->HasTextureCoords(0)); // texCoordがないmeshは非対応

		// vertexを解析
		//modelDatas_.back()->mesh.verteces.resize(mesh->mNumVertices);
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			/*modelDatas_.back()->mesh.verteces[vertexIndex].vertexPos = { -position.x,position.y,position.z,1.0f };
			modelDatas_.back()->mesh.verteces[vertexIndex].normal = { -normal.x,normal.y,normal.z };
			modelDatas_.back()->mesh.verteces[vertexIndex].texcoord = { texcoord.x,texcoord.y };*/
			modelDatas_.back()->mesh.verteces.push_back(VertexData{ { -position.x,position.y,position.z,1.0f },{ texcoord.x,texcoord.y }, { -normal.x,normal.y,normal.z } });
		}

		// indexを解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			for (uint32_t element = 0; element < face.mNumIndices; element++) {
				modelDatas_.back()->mesh.indices.push_back(face.mIndices[element]);
			}
		}

		// boneの解析
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelDatas_.back()->skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = Matrix4x4::MakeAffinMatrix(Vector3{ scale.x,scale.y,scale.z },
				Quaternion{ rotate.x,-rotate.y,-rotate.z,rotate.w }, Vector3{ -translate.x,translate.y,translate.z });
			jointWeightData.inverseBindPoseMatrix = Matrix4x4::Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
			}
		}
	}
	// rootNodeの解析
	modelDatas_.back()->rootNode = ReadNode(scene_->mRootNode);

	bool isLoadTexture = false;
	// materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene_->mNumMaterials; materialIndex++) {
		aiMaterial* material = scene_->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			std::string texFilePath;
			if (textureFilePath.length > 0) {
				texFilePath = textureFilePath.C_Str();
				std::filesystem::path filePathName(texFilePath);
				texFilePath = filePathName.filename().string();
			}
			else {
				texFilePath = "white.png";
			}
			isLoadTexture = true;
			modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
		}
	}

	if (!isLoadTexture) {
		std::string texFilePath = "white.png";
		modelDatas_.back()->texture = TextureManager::GetInstance()->LoadTexture(texFilePath);
	}

	modelDatas_.back()->mesh.indexResource_ = DirectXBase::CreateBufferResource(sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());

	modelDatas_.back()->mesh.indexBufferView_.BufferLocation = modelDatas_.back()->mesh.indexResource_->GetGPUVirtualAddress();
	modelDatas_.back()->mesh.indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());
	modelDatas_.back()->mesh.indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	modelDatas_.back()->mesh.indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.mappedIndex));
	std::memcpy(modelDatas_.back()->mesh.mappedIndex, modelDatas_.back()->mesh.indices.data(), sizeof(uint32_t) * modelDatas_.back()->mesh.indices.size());

	modelDatas_.back()->mesh.vertexResource_ = DirectXBase::CreateBufferResource(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());

	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	modelDatas_.back()->mesh.vertexBufferView_.BufferLocation = modelDatas_.back()->mesh.vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	modelDatas_.back()->mesh.vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
	//頂点当たりのサイズ
	modelDatas_.back()->mesh.vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	modelDatas_.back()->mesh.vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back()->mesh.vertexData_));
	std::memcpy(modelDatas_.back()->mesh.vertexData_, modelDatas_.back()->mesh.verteces.data(), sizeof(VertexData) * modelDatas_.back()->mesh.verteces.size());
}
