#include "Triangle.h"
#include <Novice.h>


void Triangle::DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 座標変換
	Vector3 screenVertices[3];
	for (int i = 0; i < 3; i++) {
		screenVertices[i] = Vector3::Transform(Vector3::Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawTriangle((int)screenVertices[0].x, (int)screenVertices[0].y, (int)screenVertices[1].x, (int)screenVertices[1].y, (int)screenVertices[2].x, (int)screenVertices[2].y, color, kFillModeWireFrame);
}
