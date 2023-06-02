#include "Line.h"
#include <Novice.h>
#include "Plane.h"
#include "Triangle.h"


bool Line::IsCollision(const Line& line, const Plane& plane) {

	// 平行だったらfalseを返す
	float dot = Vector3::Dot(line.diff, plane.normal);
	if (dot == 0.0f) {
		return false;
	}
	return true;
}

bool Line::IsCollision(const Line& line, const Triangle& triangle)
{
	// 三角形の各頂点を結ぶベクトルを求める
	Vector3 v01 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 v12 = triangle.vertices[2] - triangle.vertices[1];

	// 平面を生成する
	Vector3 normal = Vector3::Normalize(Vector3::Cross(v01, v12));
	float distance = Vector3::Dot(triangle.vertices[0], normal);
	Plane tmpPlane{ normal, distance };

	// 平面との当たり判定を取る
	if (IsCollision(line, tmpPlane)) {

		// 衝突点
		float t = (distance - Vector3::Dot(line.origin, normal)) / Vector3::Dot(line.diff, normal);
		Vector3 point = line.origin + line.diff * t;

		// 当たっている場所と三角形の頂点で3つの三角形を作る
		Vector3 v0p = point - triangle.vertices[0];
		Vector3 v1p = point - triangle.vertices[1];
		Vector3 v2p = point - triangle.vertices[2];
		Vector3 v20 = triangle.vertices[0] - triangle.vertices[2];

		// それぞれの向きを求める
		Vector3 cross01 = Vector3::Cross(v01, v1p);
		Vector3 cross12 = Vector3::Cross(v12, v2p);
		Vector3 cross20 = Vector3::Cross(v20, v0p);

		// 全てのクロス積と法線が同じ向きなら衝突
		if (Vector3::Dot(cross01, normal) >= 0.0f &&
			Vector3::Dot(cross12, normal) >= 0.0f &&
			Vector3::Dot(cross20, normal) >= 0.0f) {
			return true;
		}
		return false;
	}
	return false;
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

bool Ray::IsCollision(const Ray& ray, const Triangle& triangle)
{
	// 三角形の各頂点を結ぶベクトルを求める
	Vector3 v01 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 v12 = triangle.vertices[2] - triangle.vertices[1];

	// 平面を生成する
	Vector3 normal = Vector3::Normalize(Vector3::Cross(v01, v12));
	float distance = Vector3::Dot(triangle.vertices[0], normal);
	Plane tmpPlane{ normal, distance };

	// 平面との当たり判定を取る
	if (IsCollision(ray, tmpPlane)) {

		// 衝突点
		float t = (distance - Vector3::Dot(ray.origin, normal)) / Vector3::Dot(ray.diff, normal);
		Vector3 point = ray.origin + ray.diff * t;

		// 当たっている場所と三角形の頂点で3つの三角形を作る
		Vector3 v0p = point - triangle.vertices[0];
		Vector3 v1p = point - triangle.vertices[1];
		Vector3 v2p = point - triangle.vertices[2];
		Vector3 v20 = triangle.vertices[0] - triangle.vertices[2];

		// それぞれの向きを求める
		Vector3 cross01 = Vector3::Cross(v01, v1p);
		Vector3 cross12 = Vector3::Cross(v12, v2p);
		Vector3 cross20 = Vector3::Cross(v20, v0p);

		// 全てのクロス積と法線が同じ向きなら衝突
		if (Vector3::Dot(cross01, normal) >= 0.0f &&
			Vector3::Dot(cross12, normal) >= 0.0f &&
			Vector3::Dot(cross20, normal) >= 0.0f) {
			return true;
		}
		return false;
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

bool Segment::IsCollision(const Segment& segment, const Triangle& triangle)
{
	// 三角形の各頂点を結ぶベクトルを求める
	Vector3 v01 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 v12 = triangle.vertices[2] - triangle.vertices[1];

	// 平面を生成する
	Vector3 normal = Vector3::Normalize(Vector3::Cross(v01, v12));
	float distance = Vector3::Dot(triangle.vertices[0], normal);
	Plane tmpPlane{ normal, distance };

	// 平面との当たり判定を取る
	if (IsCollision(segment, tmpPlane)) {

		// 衝突点
		float t = (distance - Vector3::Dot(segment.origin, normal)) / Vector3::Dot(segment.diff, normal);
		Vector3 point = segment.origin + segment.diff * t;

		// 当たっている場所と三角形の頂点で3つの三角形を作る
		Vector3 v0p = point - triangle.vertices[0];
		Vector3 v1p = point - triangle.vertices[1];
		Vector3 v2p = point - triangle.vertices[2];
		Vector3 v20 = triangle.vertices[0] - triangle.vertices[2];

		// それぞれの向きを求める
		Vector3 cross01 = Vector3::Cross(v01, v1p);
		Vector3 cross12 = Vector3::Cross(v12, v2p);
		Vector3 cross20 = Vector3::Cross(v20, v0p);

		// 全てのクロス積と法線が同じ向きなら衝突
		if (Vector3::Dot(cross01, normal) >= 0.0f &&
			Vector3::Dot(cross12, normal) >= 0.0f &&
			Vector3::Dot(cross20, normal) >= 0.0f) {
			return true;
		}
		return false;
	}
	return false;
}

void Segment::DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 start = Vector3::Transform(Vector3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Vector3::Transform(Vector3::Transform(Vector3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, segment.color);
}