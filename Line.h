#pragma once
#include "Vector3.h"
#include "Renderer.h"

struct Line {

	// 始点
	Vector3 origin;
	// 終点への差分ベクトル
	Vector3 diff;
	// 色
	uint32_t color;

	/// <summary>
	/// 最近接点を求める
	/// </summary>
	static Vector3 ClosestPoint(const Vector3& point, const Line& line);
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
	/// 最近接点を求める
	/// </summary>
	static void Draw(Renderer& renderer, const Segment& segment);
};