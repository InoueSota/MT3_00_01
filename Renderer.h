#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Input.h"

class Renderer
{
public:

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// グリッド線描画
	/// </summary>
	void DrawGrid();

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
};

