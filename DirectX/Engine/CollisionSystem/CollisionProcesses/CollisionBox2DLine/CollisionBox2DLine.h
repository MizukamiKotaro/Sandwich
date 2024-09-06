#pragma once
class Collider;

class CollisionBox2DLine {
public:
	static bool IsCollision(Collider* a, Collider* b);
};