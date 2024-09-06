#pragma once
#include "BaseModel/BaseModel.h"

class Model : public BaseModel
{
public:

	// Model(const std::string& fileName);
	// 例)
	// Cubeフォルダ
	//   Cube.obj
	//   Cube.mtl
	// の状態

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="fileName">.objが入っているフォルダの名前( "Cube" 等)</param>
	Model(const std::string& fileName);

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="meshHundle">モデルデータのハンドル( ModelDataManager::GetInstance()->LoadObj("Cube") 等)</param>
	Model(const ModelData* modelData);
	void Update(const float& time = 0) override;
	void Draw(const Camera& camera, const BlendMode& blendMode = BlendMode::kBlendModeNormal) const override;
};