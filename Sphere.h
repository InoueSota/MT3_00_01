#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"



class Sphere
{
public:
	Sphere(const Vector3& center_, float radius_) {
		center = center_;
		radius = radius_;
	}

	/// <summary>
	/// グリッド描画
	/// </summary>
	static void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

	/// <summary>
	/// 球描画
	/// </summary>
	static void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
	
	// 中心点
	Vector3 center;

	// 半径
	float radius;
};

