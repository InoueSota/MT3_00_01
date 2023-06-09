#pragma once
#include "Vector3.h"

struct AABB {

	// 最小点
	Vector3 min;
	// 最大点
	Vector3 max;
	// 色
	uint32_t color;

	/// <summary>
	/// 変数の最小と最大が入れ替わらないようにする関数
	/// </summary>
	static void SafeParameter(AABB& aabb);

	/// <summary>
	/// AABB描画
	/// </summary>
	static void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
};
