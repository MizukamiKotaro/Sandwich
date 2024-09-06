#pragma once
class Collider;

class CollisionBox2DBox2D {
public:
	static bool IsCollision(Collider* a, Collider* b);
};