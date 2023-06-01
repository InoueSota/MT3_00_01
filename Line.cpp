#include "Line.h"
#include <Novice.h>
#include "Plane.h"


bool Line::IsCollision(const Line& line, const Plane& plane) {

	// 平行だったらfalseを返す
	float dot = Vector3::Dot(line.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}
	return true;
}

bool Ray::IsCollision(const Ray& ray, const Plane& plane) {

	// 平行だったらfalseを返す
	float dot = Vector3::Dot(ray.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Vector3::Dot(ray.origin, plane.normal)) / dot;
	if (t >= 0.0f) {
		return true;
	}
	return false;
}

Vector3 Segment::Project(const Vector3& v1, const Vector3& v2) {
	Vector3 unitVector = v2 / Vector3::Length(v2);
	return Vector3::Multiply(Vector3::Dot(v1, unitVector), unitVector);
}

Vector3 Segment::ClosestPoint(const Vector3& point, const Segment& segment) {
	return Vector3::Add(segment.origin, Project(Vector3::Subtract(point, segment.origin), segment.diff));
}

bool Segment::IsCollision(const Segment& segment, const Plane& plane) {

	// 平行だったらfalseを返す
	float dot = Vector3::Dot(segment.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Vector3::Dot(segment.origin, plane.normal)) / dot;
	if (0.0f <= t && t <= 1.0f) {
		return true;
	}
	return false;
}

void Segment::DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 start = Vector3::Transform(Vector3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Vector3::Transform(Vector3::Transform(Vector3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, segment.color);
}