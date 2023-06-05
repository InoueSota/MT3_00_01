#include "Line.h"
#include <Novice.h>
#include "Plane.h"
#include "Triangle.h"
#include "AABB.h"


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

bool Segment::IsCollision(const Segment& segment, const AABB& aabb)
{

	if (segment.diff.x == 0.0f || segment.diff.y == 0.0f || segment.diff.z == 0.0f) {
		
		// 終点を求める
		Vector3 end = segment.origin + segment.diff;

		// X軸に平行のとき
		if (segment.diff.y == 0.0f && segment.diff.z == 0.0f) {
			
			// 範囲内か判定
			if (((aabb.min.x <= segment.origin.x && aabb.max.x >= segment.origin.x)  ||
				 (aabb.min.x <= end.x && aabb.max.x >= end.x)) &&
				 (aabb.min.y <= segment.origin.y && aabb.max.y >= segment.origin.y)  &&
				 (aabb.min.z <= segment.origin.z && aabb.max.z >= segment.origin.z)) {
				return true;
			}
			return false;
		}

		// Y軸に平行のとき
		if (segment.diff.x == 0.0f && segment.diff.z == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.y <= segment.origin.y && aabb.max.y >= segment.origin.y) ||
				(aabb.min.y <= end.y && aabb.max.y >= end.y)) &&
				(aabb.min.x <= segment.origin.x && aabb.max.x >= segment.origin.x) &&
				(aabb.min.z <= segment.origin.z && aabb.max.z >= segment.origin.z)) {
				return true;
			}
			return false;
		}

		// Z軸に平行のとき
		if (segment.diff.x == 0.0f && segment.diff.y == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.z <= segment.origin.z && aabb.max.z >= segment.origin.z) ||
				(aabb.min.z <= end.z && aabb.max.z >= end.z)) &&
				(aabb.min.x <= segment.origin.x && aabb.max.x >= segment.origin.x) &&
				(aabb.min.y <= segment.origin.y && aabb.max.y >= segment.origin.y)) {
				return true;
			}
			return false;
		}
	}

	// AABBの各軸と線の判定
	float tXmin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tXmax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tYmin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tYmax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tZmin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tZmax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	// 近い方、遠い方を求める
	float tNearX = (std::min)(tXmin, tXmax);
	float tFarX = (std::max)(tXmin, tXmax);
	float tNearY = (std::min)(tYmin, tYmax);
	float tFarY = (std::max)(tYmin, tYmax);
	float tNearZ = (std::min)(tZmin, tZmax);
	float tFarZ = (std::max)(tZmin, tZmax);

	// AABBとの衝突点のtが小さいのを求める
	float tmin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	// AABBとの衝突点のtが大きいのを求める
	float tmax = (std::min)((std::min)(tFarX, tFarY), tFarZ);

	// 近い方の大きい方 > 遠い方の小さい方 なら衝突していない
	if (tmin > tmax) { return false; }

	// 線分の条件
	if ((tmin < 0.0f && tmax < 0.0f) || (tmin > 1.0f && tmax > 1.0f)) { return false; }

	// 条件を抜けたら衝突
	return true;
}

void Segment::DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	Vector3 start = Vector3::Transform(Vector3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Vector3::Transform(Vector3::Transform(Vector3::Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, segment.color);
}