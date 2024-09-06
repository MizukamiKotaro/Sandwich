#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Matrix4x4.h"
#include "Quaternion.h"

class EulerTransform {

public:
	EulerTransform();

	void Initialize();

	void UpdateMatrix();

	const Vector3& GetWorldPosition() const;

	bool IsCollisionXZ(const EulerTransform& transform);

	/// <summary>
	/// 平行移動のみの親のセット
	/// </summary>
	/// <param name="transform">親にするTransformのポインタ</param>
	void SetWorldTranslateParent(const EulerTransform* transform);

	/// <summary>
	/// 平行移動の親子関係の解除
	/// </summary>
	void ClearWorldTranslateParent();

	void SetQuaRot(const Quaternion& qua);

public:

	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;

	Matrix4x4 worldMat_;

	Vector3 worldPos_;

	Quaternion rot_;
	bool isQua_;

	const EulerTransform* parent_ = nullptr;

	const EulerTransform* worldTranslateParent_ = nullptr;
};