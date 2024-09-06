#pragma once
#include "Matrix4x4.h"
#include "Drawers/IDrawManager/IDrawManager.h"
#include <wrl.h>
#include <vector>
#include <memory>

class Camera;
class ModelData;
enum class BlendMode;
class PointLight;
class SpotLight;

class DrawLightManager : public IDrawManager
{
public:
	DrawLightManager();

	void Draw(const PointLight& light,const Camera& camera, const BlendMode& blendMode);
	void Draw(const SpotLight& light, const Camera& camera, const BlendMode& blendMode);

private:
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

	const ModelData* modelData_;
	Matrix4x4 scaleMat_;
	Matrix4x4 scaleInverseMat_;
};