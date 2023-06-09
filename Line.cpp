#include "Line.h"
#include <Novice.h>
#include "Plane.h"
#include "Triangle.h"
#include "AABB.h"


Vector3 Segment::ClosestPoint(const Vector3& point, const Segment& segment) {
	return Vector3::Add(segment.origin, Vector3::Project(Vector3::Subtract(point, segment.origin), segment.diff));
}

void Segment::DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 start = Vector3::Transform(Vector3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Vector3::Transform(Vector3::Transform(Vector3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, segment.color);
}