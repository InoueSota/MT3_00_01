#include "Line.h"
#include <Novice.h>


Vector3 Segment::Project(const Vector3& v1, const Vector3& v2) {
	Vector3 unitVector = v2 / Vector3::Length(v2);
	return Vector3::Multiply(Vector3::Dot(v1, unitVector), unitVector);
}

Vector3 Segment::ClosestPoint(const Vector3& point, const Segment& segment) {
	return Vector3::Add(segment.origin, Project(Vector3::Subtract(point, segment.origin), segment.diff));
}

void Segment::DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = Vector3::Transform(Vector3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Vector3::Transform(Vector3::Transform(Vector3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, color);
}