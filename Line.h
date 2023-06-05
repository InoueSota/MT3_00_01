#pragma once
#include "Vector3.h"

struct Plane;
struct Triangle;
struct AABB;

struct Line {

	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;
	// 色
	uint32_t color;

	/// <summary>
	/// 平面との当たり判定
	/// </summary>
	static bool IsCollision(const Line& line, const Plane& plane);

	/// <summary>
	/// 三角形との当たり判定
	/// </summary>
	static bool IsCollision(const Line& line, const Triangle& triangle);

};

struct Ray {

	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;
	// 色
	uint32_t color;

	/// <summary>
	/// 平面との当たり判定
	/// </summary>
	static bool IsCollision(const Ray& ray, const Plane& plane);

	/// <summary>
	/// 三角形との当たり判定
	/// </summary>
	static bool IsCollision(const Ray& ray, const Triangle& triangle);

};

struct Segment {

	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;
	// 色
	uint32_t color;

	/// <summary>
	/// 正射影ベクトルを求める
	/// </summary>
	static Vector3 Project(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 最近接点を求める
	/// </summary>
	static Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	/// <summary>
	/// 平面との当たり判定
	/// </summary>
	static bool IsCollision(const Segment& segment, const Plane& plane);

	/// <summary>
	/// 三角形との当たり判定
	/// </summary>
	static bool IsCollision(const Segment& segment, const Triangle& triangle);

	/// <summary>
	/// AABBとの当たり判定
	/// </summary>
	static bool IsCollision(const Segment& segment, const AABB& aabb);

	/// <summary>
	/// 線分描画
	/// </summary>
	static void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
};