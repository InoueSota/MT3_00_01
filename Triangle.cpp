#include "Triangle.h"
#include <Novice.h>


void Triangle::DrawTriangle(Renderer& renderer, const Triangle& triangle, uint32_t color)
{
	// 座標変換
	Vector3 screenVertices[3];
	for (int i = 0; i < 3; i++) {
		screenVertices[i] = triangle.vertices[i];
	}
	renderer.ScreenTriangle(triangle.vertices, color);
}
