#include "CollisionManager.h"
#include <algorithm>


bool IsCollision(const Sphere& s1, const Sphere& s2)
{
    // 半径の合計よりも短ければ衝突
    if (Vector3::LengthSquare(s2.center - s1.center) <= (s1.radius + s2.radius) * (s1.radius + s2.radius)) {
        return true;
    }
    return false;
}

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
    Vector3 normal = Vector3(plane.normal);
    float distance = std::abs(Vector3::Dot(normal, sphere.center) - plane.distance);

    if (distance <= sphere.radius) {
        return true;
    }
    return false;
}

bool IsCollision(const Line& line, const Plane& plane)
{
    // 平行だったらfalseを返す
    float dot = Vector3::Dot(line.diff, plane.normal);
    if (dot == 0.0f) {
        return false;
    }
    return true;
}

bool IsCollision(const Line& line, const Triangle& triangle)
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

bool IsCollision(const Line& line, const AABB& aabb)
{
	if (line.diff.x == 0.0f || line.diff.y == 0.0f || line.diff.z == 0.0f) {

		// 終点を求める
		Vector3 end = line.origin + line.diff;

		// X軸に平行のとき
		if (line.diff.y == 0.0f && line.diff.z == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.x <= line.origin.x && aabb.max.x >= line.origin.x) ||
				(aabb.min.x <= end.x && aabb.max.x >= end.x)) &&
				(aabb.min.y <= line.origin.y && aabb.max.y >= line.origin.y) &&
				(aabb.min.z <= line.origin.z && aabb.max.z >= line.origin.z)) {
				return true;
			}
			return false;
		}

		// Y軸に平行のとき
		if (line.diff.x == 0.0f && line.diff.z == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.y <= line.origin.y && aabb.max.y >= line.origin.y) ||
				(aabb.min.y <= end.y && aabb.max.y >= end.y)) &&
				(aabb.min.x <= line.origin.x && aabb.max.x >= line.origin.x) &&
				(aabb.min.z <= line.origin.z && aabb.max.z >= line.origin.z)) {
				return true;
			}
			return false;
		}

		// Z軸に平行のとき
		if (line.diff.x == 0.0f && line.diff.y == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.z <= line.origin.z && aabb.max.z >= line.origin.z) ||
				(aabb.min.z <= end.z && aabb.max.z >= end.z)) &&
				(aabb.min.x <= line.origin.x && aabb.max.x >= line.origin.x) &&
				(aabb.min.y <= line.origin.y && aabb.max.y >= line.origin.y)) {
				return true;
			}
			return false;
		}
	}

	// AABBの各軸と線の判定
	float tXmin = (aabb.min.x - line.origin.x) / line.diff.x;
	float tXmax = (aabb.max.x - line.origin.x) / line.diff.x;
	float tYmin = (aabb.min.y - line.origin.y) / line.diff.y;
	float tYmax = (aabb.max.y - line.origin.y) / line.diff.y;
	float tZmin = (aabb.min.z - line.origin.z) / line.diff.z;
	float tZmax = (aabb.max.z - line.origin.z) / line.diff.z;

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

bool IsCollision(const Ray& ray, const Plane& plane)
{
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

bool IsCollision(const Ray& ray, const Triangle& triangle)
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

bool IsCollision(const Ray& ray, const AABB& aabb)
{
	if (ray.diff.x == 0.0f || ray.diff.y == 0.0f || ray.diff.z == 0.0f) {

		// 終点を求める
		Vector3 end = ray.origin + ray.diff;

		// X軸に平行のとき
		if (ray.diff.y == 0.0f && ray.diff.z == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.x <= ray.origin.x && aabb.max.x >= ray.origin.x) ||
				(aabb.min.x <= end.x && aabb.max.x >= end.x)) &&
				(aabb.min.y <= ray.origin.y && aabb.max.y >= ray.origin.y) &&
				(aabb.min.z <= ray.origin.z && aabb.max.z >= ray.origin.z)) {
				return true;
			}
			return false;
		}

		// Y軸に平行のとき
		if (ray.diff.x == 0.0f && ray.diff.z == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.y <= ray.origin.y && aabb.max.y >= ray.origin.y) ||
				(aabb.min.y <= end.y && aabb.max.y >= end.y)) &&
				(aabb.min.x <= ray.origin.x && aabb.max.x >= ray.origin.x) &&
				(aabb.min.z <= ray.origin.z && aabb.max.z >= ray.origin.z)) {
				return true;
			}
			return false;
		}

		// Z軸に平行のとき
		if (ray.diff.x == 0.0f && ray.diff.y == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.z <= ray.origin.z && aabb.max.z >= ray.origin.z) ||
				(aabb.min.z <= end.z && aabb.max.z >= end.z)) &&
				(aabb.min.x <= ray.origin.x && aabb.max.x >= ray.origin.x) &&
				(aabb.min.y <= ray.origin.y && aabb.max.y >= ray.origin.y)) {
				return true;
			}
			return false;
		}
	}

	// AABBの各軸と線の判定
	float tXmin = (aabb.min.x - ray.origin.x) / ray.diff.x;
	float tXmax = (aabb.max.x - ray.origin.x) / ray.diff.x;
	float tYmin = (aabb.min.y - ray.origin.y) / ray.diff.y;
	float tYmax = (aabb.max.y - ray.origin.y) / ray.diff.y;
	float tZmin = (aabb.min.z - ray.origin.z) / ray.diff.z;
	float tZmax = (aabb.max.z - ray.origin.z) / ray.diff.z;

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

bool IsCollision(const Segment& segment, const Plane& plane)
{
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

bool IsCollision(const Segment& segment, const Triangle& triangle)
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

bool IsCollision(const Segment& segment, const AABB& aabb)
{
	if (segment.diff.x == 0.0f || segment.diff.y == 0.0f || segment.diff.z == 0.0f) {

		// 終点を求める
		Vector3 end = segment.origin + segment.diff;

		// X軸に平行のとき
		if (segment.diff.y == 0.0f && segment.diff.z == 0.0f) {

			// 範囲内か判定
			if (((aabb.min.x <= segment.origin.x && aabb.max.x >= segment.origin.x) ||
				(aabb.min.x <= end.x && aabb.max.x >= end.x)) &&
				(aabb.min.y <= segment.origin.y && aabb.max.y >= segment.origin.y) &&
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

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}
	return false;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y) , std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

	// 最近接点と球の中心との距離を求める
	float distance = Vector3::Length(closestPoint - sphere.center);

	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const OBB& obb, const Sphere& sphere)
{
	Matrix4x4 worldMatrix = {
		obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0.0f,
		obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0.0f,
		obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0.0f,
		obb.center.x, obb.center.y, obb.center.z, 1.0f
	};

	Matrix4x4 worldMatrixInverse = OBB::MakeInverse(OBB::MakeRotationFromOrientations(obb.orientations), obb.center);

	Vector3 centerInOBBLocalSpace = Vector3::Transform(sphere.center, worldMatrixInverse);

	AABB aabbOBBLocal{ .min = -obb.size, .max = obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };

	// ローカル平面で衝突判定
	bool isCollision = IsCollision(aabbOBBLocal, sphereOBBLocal);

	return isCollision;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		Vector3 worldStartPosition = { -kGridEvery * kSubdivision / 2.0f, 0.0f, kGridEvery * (xIndex - kSubdivision / 2.0f) };
		Vector3 worldEndPosition = { kGridEvery * kSubdivision / 2.0f, 0.0f, kGridEvery * (xIndex - kSubdivision / 2.0f) };
		Vector3 screenStartPosition = Vector3::Transform(Vector3::Transform(worldStartPosition, viewProjectionMatrix), viewportMatrix);
		Vector3 screenEndPosition = Vector3::Transform(Vector3::Transform(worldEndPosition, viewProjectionMatrix), viewportMatrix);
		if (xIndex == kSubdivision / 2) {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, BLACK);
		}
		else {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, 0xAAAAAAFF);
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		Vector3 worldStartPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, -kGridEvery * kSubdivision / 2.0f };
		Vector3 worldEndPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, kGridEvery * kSubdivision / 2.0f };
		Vector3 screenStartPosition = Vector3::Transform(Vector3::Transform(worldStartPosition, viewProjectionMatrix), viewportMatrix);
		Vector3 screenEndPosition = Vector3::Transform(Vector3::Transform(worldEndPosition, viewProjectionMatrix), viewportMatrix);
		if (zIndex == kSubdivision / 2) {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, BLACK);
		}
		else {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, 0xAAAAAAFF);
		}
	}
}
