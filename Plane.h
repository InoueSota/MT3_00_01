#pragma once
#include "Vector3.h"
#include "Renderer.h"

struct Plane
{
	// 法線
	Vector3 normal;
	// 距離
	float distance;

	/// <summary>
	/// 球描画
	/// </summary>
	static void DrawPlane(Renderer& renderer, const Plane& plane, uint32_t color);
};

