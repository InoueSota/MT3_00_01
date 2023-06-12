#include "Plane.h"
#include "Sphere.h"
#include <Novice.h>


void Plane::DrawPlane(Renderer& renderer, const Plane& plane, uint32_t color) {
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
		points[index] = point;
	}

	renderer.ScreenLine(points[0], points[2], color);
	renderer.ScreenLine(points[1], points[3], color);
	renderer.ScreenLine(points[2], points[1], color);
	renderer.ScreenLine(points[3], points[0], color);
}

