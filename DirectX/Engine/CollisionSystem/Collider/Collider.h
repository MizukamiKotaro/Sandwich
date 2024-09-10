#pragma once
#include "CollisionSystem/CollisionConfig.h"
#include "CollisionSystem/ColliderShapes/ShapeCircle/ShapeCircle.h"
#include "CollisionSystem/ColliderShapes/ShapeBox2D/ShapeBox2D.h"
#include "CollisionSystem/ColliderShapes/ShapeLine/ShapeLine.h"
#include <list>
#include <memory>

class CollisionManager;

class Collider {
public:
	virtual ~Collider() = default;

	virtual void OnCollision(const Collider& collider) = 0;

	void SetCollider();

	void CreateCollider(ColliderShape shape, ColliderType type, ColliderMask mask, bool isBeDrived = false);

	void AddTargetMask(ColliderMask mask);
	void ClearTarget();

	const ColliderType GetType() const { return type_; }
	const ColliderShape GetShape() const { return shape_; }
	const ColliderMask GetMask() const { return mask_; }
	const std::list<ColliderMask>& GetTargetMasks() const { return targetMasks_; }
	const bool GetIsBeDrived() const { return isBeDrived_; }
	const bool GetIsHit() const { return isHit_; }
	void SetIsHit(bool isHit) { isHit_ = isHit; }

	ShapeCircle* GetCircle() const { return shapeCircle_.get(); }
	ShapeBox2D* GetBox2D() const { return shapeBox2D_.get(); }
	ShapeLine* GetLine() const { return shapeSimpleLineY_.get(); }

	void SetCircle(const Vector3& position, const float& radius);
	void SetBox2D(const Vector3& position, const Vector3& scale);
	void SetSimpleLineY(const float& y);

private:
	CollisionManager* manager_ = nullptr;

	std::unique_ptr<ShapeCircle> shapeCircle_;
	std::unique_ptr<ShapeBox2D> shapeBox2D_;
	std::unique_ptr<ShapeLine> shapeSimpleLineY_;

	ColliderType type_;
	bool isBeDrived_; // めり込み処理をするとき動くか
	ColliderShape shape_;
	ColliderMask mask_;
	std::list<ColliderMask> targetMasks_;

	bool isHit_;
};