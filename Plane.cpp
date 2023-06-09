#include "Plane.h"
#include "Sphere.h"
#include <Novice.h>


void Plane::DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Vector3::Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Vector3::Normalize(Vector3::Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Vector3::Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Vector3::Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Vector3::Add(center, extend);
		points[index] = Vector3::Transform(Vector3::Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, color);
	Novice::DrawLine((int)points[2].x, (int)points[2].y, (int)points[1].x, (int)points[1].y, color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, color);
	Novice::DrawLine((int)points[3].x, (int)points[3].y, (int)points[0].x, (int)points[0].y, color);
}

