#pragma once
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Line.h"
#include "AABB.h"
#include "OBB.h"

/// <summary>
/// 球同士当たり判定
/// </summary>
bool IsCollision(const Sphere& s1, const Sphere& s2);

/// <summary>
/// 球と平面の当たり判定
/// </summary>
bool IsCollision(const Sphere& sphere, const Plane& plane);

/// <summary>
/// 直線と平面の当たり判定
/// </summary>
bool IsCollision(const Line& line, const Plane& plane);

/// <summary>
/// 直線と三角形の当たり判定
/// </summary>
bool IsCollision(const Line& line, const Triangle& triangle);

/// <summary>
/// 直線とAABBの当たり判定
/// </summary>
bool IsCollision(const Line& line, const AABB& aabb);

/// <summary>
/// 半直線と平面の当たり判定
/// </summary>
bool IsCollision(const Ray& ray, const Plane& plane);

/// <summary>
/// 半直線と三角形の当たり判定
/// </summary>
bool IsCollision(const Ray& ray, const Triangle& triangle);

/// <summary>
/// 半直線とAABBの当たり判定
/// </summary>
bool IsCollision(const Ray& ray, const AABB& aabb);

/// <summary>
/// 線分と平面の当たり判定
/// </summary>
bool IsCollision(const Segment& segment, const Plane& plane);

/// <summary>
/// 線分と三角形の当たり判定
/// </summary>
bool IsCollision(const Segment& segment, const Triangle& triangle);

/// <summary>
/// 線分とAABBの当たり判定
/// </summary>
bool IsCollision(const Segment& segment, const AABB& aabb);

/// <summary>
/// 線分とOBBの当たり判定
/// </summary>
bool IsCollision(const Segment& segment, const OBB& obb);

/// <summary>
/// AABB同士の当たり判定
/// </summary>
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

/// <summary>
/// AABBと球の当たり判定
/// </summary>
bool IsCollision(const AABB& aabb, const Sphere& sphere);

/// <summary>
/// OBBと球の当たり判定
/// </summary>
bool IsCollision(const OBB& obb, const Sphere& sphere);

/// <summary>
/// グリッド線描画
/// </summary>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);