#pragma once
#include "Vector3.h"
#include "Renderer.h"

struct Triangle {

	// 3頂点
	Vector3 vertices[3];
	
	/// <summary>
	/// 線分描画
	/// </summary>
	static void DrawTriangle(Renderer& renderer, const Triangle& triangle, uint32_t color);
};
