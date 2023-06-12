#include <Novice.h>
#include <imgui.h>
#include "Input.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "CollisionManager.h"
#include "AABB.h"
#include "Sphere.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_02_08";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// マウスで3D空間を制御する
	enum MOUSE
	{
		LEFT,
		RIGHT,
		CENTER
	};
	Input* input = nullptr;
	input = Input::GetInstance();

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,-0.1f,0.0f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	OBB obb{
		.center{ -1.0f, 0.0f, 0.0f },
		.orientations = {{ 1.0f, 0.0f, 0.0f },
						 { 0.0f, 1.0f, 0.0f },
						 { 0.0f, 0.0f, 1.0f }},
		.size{ 0.5f, 0.5f, 0.5f },
		.color = WHITE
	};
	Sphere sphere{
		.center{ 0.0f, 0.0f, 0.0f },
		.radius{ 0.5f },
		.color = WHITE
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// OBBの軸を作る
		OBB::MakeOrientations(obb, rotate);

		// 当たり判定
		if (IsCollision(obb, sphere)) {
			obb.color = RED;
		} else {
			obb.color = WHITE;
		}

		// 各種行列の計算
		Matrix4x4 worldMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, translate);
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Matrix4x4::Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// カメラ移動
		if (Novice::IsPressMouse(CENTER)) {
			cameraTranslate += Vector3::Transform({ -static_cast<float>(input->GetMouseMove().lX), static_cast<float>(input->GetMouseMove().lY), 0.0f }, Matrix4x4::Inverse(viewMatrix)) * 0.001f;
		}
		// ドリーイン、ドリーアウト
		else {
			cameraTranslate += Vector3::GetZaxis(Matrix4x4::MakeRotateMatrix(cameraRotate)) * static_cast<float>(input->GetWheel()) * 0.002f;
		}
		// カメラ回転
		if (Novice::IsPressMouse(RIGHT)) {
			cameraRotate.x += static_cast<float>(input->GetMouseMove().lY) * 0.001f;
			cameraRotate.y += static_cast<float>(input->GetMouseMove().lX) * 0.001f;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		OBB::Draw(obb, worldViewProjectionMatrix, viewportMatrix);
		Sphere::Draw(sphere, worldViewProjectionMatrix, viewportMatrix);

		ImGui::Begin("Window");
		ImGui::DragFloat3("rotate", &rotate.x, 0.01f);
		ImGui::DragFloat3("center", &obb.center.x, 0.01f);
		ImGui::DragFloat3("orientations[0]", &obb.orientations[0].x, 0.01f);
		ImGui::DragFloat3("orientations[1]", &obb.orientations[1].x, 0.01f);
		ImGui::DragFloat3("orientations[2]", &obb.orientations[2].x, 0.01f);
		ImGui::DragFloat3("size", &obb.size.x, 0.01f);
		ImGui::End();

		Novice::ScreenPrintf(0,  0, "CameraMove   : Mouse Center");
		Novice::ScreenPrintf(0, 20, "CameraRotate : Mouse Right");
		Novice::ScreenPrintf(0, 40, "Dolly In,Out : Mouse Wheel");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
