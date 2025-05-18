#pragma once

enum CollisionSide {
    NONE = 0,
    TOP = 1 << 0, // 1
    BOTTOM = 1 << 1, // 2
    LEFT = 1 << 2, // 4
    RIGHT = 1 << 3  // 8
};