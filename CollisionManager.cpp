#include "CollisionManager.h"
#include <algorithm>

// 球
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

bool IsCollision(const Capsule& capsule, const Plane& plane)
{
	// カプセルを使ったパターンは分からなかったです

	Vector3 d = plane.normal - capsule.segment.origin;
	Vector3 ba = capsule.segment.diff;
	Vector3 e = Vector3::Normalize(ba);

	float t = Vector3::Dot(d, e) / Vector3::Length(ba);
	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 f = (1.0f - t) * capsule.segment.origin * t * (capsule.segment.origin + capsule.segment.diff);

	float distance = Vector3::Length(plane.normal - f);

	if (distance < capsule.radius) {
		return true;
	}
	return false;
}

// 直線
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
bool IsCollision(const Line& line, const OBB& obb)
{
	// OBBのWorldMatrixの逆行列を作成
	Matrix4x4 worldMatrixInverse = OBB::MakeInverse(OBB::MakeRotationFromOrientations(obb.orientations), obb.center);

	// ローカル座標を計算
	Vector3 localOrigin = Vector3::Transform(line.origin, worldMatrixInverse);
	Vector3 localEnd = Vector3::Transform(line.origin + line.diff, worldMatrixInverse);

	// ローカル空間で衝突判定
	AABB localAABB{ {-obb.size.x, -obb.size.y, -obb.size.z}, {obb.size.x, obb.size.y, obb.size.z} };
	Segment localLine{ localOrigin, localEnd - localOrigin };

	return IsCollision(localLine, localAABB);
}

// 半直線
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
bool IsCollision(const Ray& ray, const OBB& obb)
{
	// OBBのWorldMatrixの逆行列を作成
	Matrix4x4 worldMatrixInverse = OBB::MakeInverse(OBB::MakeRotationFromOrientations(obb.orientations), obb.center);

	// ローカル座標を計算
	Vector3 localOrigin = Vector3::Transform(ray.origin, worldMatrixInverse);
	Vector3 localEnd = Vector3::Transform(ray.origin + ray.diff, worldMatrixInverse);

	// ローカル空間で衝突判定
	AABB localAABB{ {-obb.size.x, -obb.size.y, -obb.size.z}, {obb.size.x, obb.size.y, obb.size.z} };
	Segment localRay{ localOrigin, localEnd - localOrigin };

	return IsCollision(localRay, localAABB);
}

// 線分
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
bool IsCollision(const Segment& segment, const OBB& obb)
{
	// OBBのWorldMatrixの逆行列を作成
	Matrix4x4 worldMatrixInverse = OBB::MakeInverse(OBB::MakeRotationFromOrientations(obb.orientations), obb.center);

	// ローカル座標を計算
	Vector3 localOrigin = Vector3::Transform(segment.origin, worldMatrixInverse);
	Vector3 localEnd = Vector3::Transform(segment.origin + segment.diff, worldMatrixInverse);

	// ローカル空間で衝突判定
	AABB localAABB{{-obb.size.x, -obb.size.y, -obb.size.z}, {obb.size.x, obb.size.y, obb.size.z}};
	Segment localSegment{localOrigin, localEnd - localOrigin};

	return IsCollision(localSegment, localAABB);
}

// AABB
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

// OBB
bool IsCollision(const OBB& obb, const Sphere& sphere)
{
	// OBBのWorldMatrixの逆行列を作成
	Matrix4x4 worldMatrixInverse = OBB::MakeInverse(OBB::MakeRotationFromOrientations(obb.orientations), obb.center);

	// ローカル座標を計算
	Vector3 centerInOBBLocalSpace = Vector3::Transform(sphere.center, worldMatrixInverse);

	// ローカル空間で衝突判定
	AABB aabbOBBLocal{ .min = -obb.size, .max = obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };

	return IsCollision(aabbOBBLocal, sphereOBBLocal);
}
bool IsCollision(const OBB& obb1, const OBB& obb2)
{
	// 面法線
	Vector3 planeNormal[6] = {
		obb1.orientations[0] - obb1.center,
		obb1.orientations[1] - obb1.center,
		obb1.orientations[2] - obb1.center,
		obb2.orientations[0] - obb2.center,
		obb2.orientations[1] - obb2.center,
		obb2.orientations[2] - obb2.center
	};

	Vector3 obb1RotateVertex[8];
	Vector3 obb2RotateVertex[8];
	obb1.MakeVertex(obb1RotateVertex, obb1);
	obb2.MakeVertex(obb2RotateVertex, obb2);

	for (uint32_t i = 0; i < 6; i++)
	{
		float min1 = 0.0f, min2 = 0.0f;
		float max1 = 0.0f, max2 = 0.0f;
		float dot1[8];
		float dot2[8];

		for (uint32_t j = 0; j < 8; j++)
		{
			dot1[j] = Vector3::Dot(planeNormal[i], obb1RotateVertex[j]);
			dot2[j] = Vector3::Dot(planeNormal[i], obb2RotateVertex[j]);
			if (j == 0) {
				min1 = dot1[j];
				min2 = dot2[j];
				max1 = dot1[j];
				max2 = dot2[j];
			}
			else
			{
				min1 = (std::min)(dot1[j], min1);
				min2 = (std::min)(dot2[j], min2);
				max1 = (std::max)(dot1[j], max1);
				max2 = (std::max)(dot2[j], max2);
			}
		}

		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = std::fabs(L1) + std::fabs(L2);
		float longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);

		if (sumSpan < longSpan) {
			return false;
		}
	}

	// 各辺の組み合わせのクロス積
	Vector3 cross[9] = {
		Vector3::Cross(obb1.orientations[0], obb2.orientations[0]),
		Vector3::Cross(obb1.orientations[0], obb2.orientations[1]),
		Vector3::Cross(obb1.orientations[0], obb2.orientations[2]),
		Vector3::Cross(obb1.orientations[1], obb2.orientations[0]),
		Vector3::Cross(obb1.orientations[1], obb2.orientations[1]),
		Vector3::Cross(obb1.orientations[1], obb2.orientations[2]),
		Vector3::Cross(obb1.orientations[2], obb2.orientations[0]),
		Vector3::Cross(obb1.orientations[2], obb2.orientations[1]),
		Vector3::Cross(obb1.orientations[2], obb2.orientations[2])
	};

	for (uint32_t i = 0; i < 9; i++)
	{
		float min1 = 0.0f, min2 = 0.0f;
		float max1 = 0.0f, max2 = 0.0f;
		float dot1[8];
		float dot2[8];

		for (uint32_t j = 0; j < 8; j++)
		{
			dot1[j] = Vector3::Dot(cross[i], obb1RotateVertex[j]);
			dot2[j] = Vector3::Dot(cross[i], obb2RotateVertex[j]);
			if (j == 0) {
				min1 = dot1[j];
				min2 = dot2[j];
				max1 = dot1[j];
				max2 = dot2[j];
			}
			else
			{
				min1 = (std::min)(dot1[j], min1);
				min2 = (std::min)(dot2[j], min2);
				max1 = (std::max)(dot1[j], max1);
				max2 = (std::max)(dot2[j], max2);
			}
		}

		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);

		if (sumSpan < longSpan) {
			return false;
		}
	}

	return true;
}

