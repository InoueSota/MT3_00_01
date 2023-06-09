#pragma once
#include "Vector3.h"

struct Line {

	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;
	// 色
	uint32_t color;
};

struct Ray {

	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;
	// 色
	uint32_t color;
};

struct Segment {

	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;
	// 色
	uint32_t color;

	/// <summary>
	/// 最近接点を求める
	/// </summary>
	static Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	/// <summary>
	/// 線分描画
	/// </summary>
	static void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
};