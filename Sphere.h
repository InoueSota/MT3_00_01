#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <Novice.h>



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
	static void Draw(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
};

