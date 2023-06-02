#pragma once
#include "Vector3.h"


struct Triangle {

	// 3頂点
	Vector3 vertices[3];
	
	/// <summary>
	/// 線分描画
	/// </summary>
	static void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
};
