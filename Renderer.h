#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Input.h"

struct Renderer
{
	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 階層構造専用更新関数
	/// </summary>
	void Update(const Vector3 scales[3], const Vector3 rotates[3], const Vector3 translates[3]);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// グリッド線描画
	/// </summary>
	void DrawGrid();

	/// <summary>
	/// スクリーン座標に変換した後に描画する
	/// </summary>
	void ScreenLine(const Vector3& start, const Vector3& end, uint32_t color);

	/// <summary>
	/// スクリーン座標に変換した後に描画する
	/// </summary>
	void ScreenTriangle(const Vector3 vertex[3], uint32_t color);

	// ウィンドウサイズ
	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;

	// レンダリングパイプライン
	Matrix4x4 worldMatrix;
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewportMatrix;

	// カメラ
	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,-0.1f,0.0f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	// マウスで3D空間を制御する
	enum MOUSE
	{
		LEFT,
		RIGHT,
		CENTER
	};
	Input* input = nullptr;

	// 階層構造専用
	Matrix4x4 shoulderWorldMatrix;
	Matrix4x4 shoulderWorldViewProjectionMatrix;
	Matrix4x4 elbowWorldMatrix;
	Matrix4x4 elbowWorldViewProjectionMatrix;
	Matrix4x4 handWorldMatrix;
	Matrix4x4 handWorldViewProjectionMatrix;
};

