#pragma once
#include "Vector3.h"

struct Plane;

struct Line {
	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;

	/// <summary>
	/// 平面との当たり判定
	/// </summary>
	static bool IsCollision(const Line& line, const Plane& plane);

};

struct Ray {
	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;

	/// <summary>
	/// 平面との当たり判定
	/// </summary>
	static bool IsCollision(const Ray& ray, const Plane& plane);

};

struct Segment {
	Segment(const Vector3& origin_, const Vector3& diff_) {
		origin = origin_;
		diff = diff_;
	};

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
	/// 線分描画
	/// </summary>
	static void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
};