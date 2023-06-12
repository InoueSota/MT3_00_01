#pragma once
#include "Vector3.h"
#include "Renderer.h"

struct OBB
{
	// 中心点
	Vector3 center;
	// 座標軸 正規化・直交必須
	Vector3 orientations[3];
	// 座標軸方向の長さの半分 中心から面までの距離
	Vector3 size;
	// 色
	uint32_t color;

	/// <summary>
	/// OBBの軸の作成
	/// </summary>
	static void MakeOrientations(OBB& obb, const Vector3& rotate);

	/// <summary>
	/// OBBの軸から転置した回転行列を作る
	/// </summary>
	static Matrix4x4 MakeRotationFromOrientations(const Vector3 orientations[3]);

	/// <summary>
	/// 平行移動行列の逆行列を作る
	/// </summary>
	static Matrix4x4 MakeInverse(const Matrix4x4& rotation, const Vector3& translate);

	/// <summary>
	/// OBB描画
	/// </summary>
	static void Draw(Renderer& renderer, const OBB& obb);
};