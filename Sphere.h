#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <Novice.h>



class Sphere
{
public:

	/// <summary>
	/// グリッド描画
	/// </summary>
	static void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

	/// <summary>
	/// 球描画
	/// </summary>
	static void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

	/// <summary>
	/// 球同士当たり判定
	/// </summary>
	static bool IsCollision(const Sphere& s1, const Sphere& s2);
	

	// 中心点
	Vector3 center;
	// 半径
	float radius;
	// 色
	uint32_t color;
};

