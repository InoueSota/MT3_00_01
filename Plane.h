#pragma once
#include "Vector3.h"


class Sphere;

struct Plane
{
	// 法線
	Vector3 normal;
	// 距離
	float distance;

	/// <summary>
	/// 球描画
	/// </summary>
	static void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	static bool IsCollision(const Sphere& sphere, const Plane& plane);
};

