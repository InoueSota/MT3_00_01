#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <Novice.h>
#include "Renderer.h"

class Sphere
{
public:

	// 中心点
	Vector3 center;
	// 半径
	float radius;
	// 色
	uint32_t color;


	/// <summary>
	/// 球描画
	/// </summary>
	static void Draw(Renderer& renderer, const Sphere& sphere);
	static void Draw(const Matrix4x4& worldViewProjectionMatrix, const Matrix4x4& viewPortMatrix, const Sphere& sphere);
};

