#pragma once
enum class ColliderType {
    UNKNOWN = 0, // 何もなし
    RIGID_BODY, // 体あり。これ同士はめり込み処理あり
    COLLIDER, // 体なし。あたり判定のみ
};

enum class ColliderShape
{
    UNKNOWN = 0,
    CIRCLE,
    BOX2D,
    MAPCHIP2D,
    SIMPLE_LINE_Y,
};

enum class ColliderMask {
    UNKNOWN = 0,
    PLAYER,
    ENEMY,
    FLOOR,
};