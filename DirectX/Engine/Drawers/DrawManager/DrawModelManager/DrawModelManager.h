#pragma once
#include "Matrix4x4.h"
#include <vector>
#include <memory>
#include <wrl.h>
#include "Drawers/IDrawManager/IDrawManager.h"

class Camera;
class ModelData;
enum class BlendMode;
class Model;
class RigidAnimationModel;
class SkinningModel;
class BaseModel;

class DrawModelManager : public IDrawManager
{
public:
	DrawModelManager();
	void Draw(const Model& model,const Camera& camera, const BlendMode& blendMode);
	void Draw(const RigidAnimationModel& model, const Camera& camera, const BlendMode& blendMode);
	void Draw(const SkinningModel& model, const Camera& camera, const BlendMode& blendMode);

private:
	void Draw(const BaseModel& model, const Camera& camera, const BlendMode& blendMode, const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView);
	class Transformation
	{
	public:
		Transformation();
		~Transformation();
		struct TransformationMatrix {
			Matrix4x4 WVP;
			Matrix4x4 World;
			Matrix4x4 WorldInverse;
		};
		Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource;
		TransformationMatrix* transformationData;
	};
	std::vector<std::unique_ptr<Transformation>> transformation_;
};