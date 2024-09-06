#pragma once
class Collider;

class CollisionCircleLine {
public:
	static bool IsCollision(Collider* a, Collider* b);
};