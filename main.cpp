#include <Novice.h>
#include <imgui.h>
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Sphere.h"
#include "AABB.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_02_05";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int preX = 0, preY = 0;
	int x = 0, y = 0;
	bool isTranslate = false, isRotate = false;

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,-0.1f,0.0f };
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	AABB aabb{
		.min{-0.5f, -0.5f, -0.5f},
		.max{ 0.0f,  0.0f,  0.0f},
		.color = WHITE
	};
	Sphere sphere{
		.center{0.5f, 0.5f, 0.0f},
		.radius = 1.0f,
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

		// カメラを動かす（ムズ）
		if (Novice::IsPressMouse(2) && (keys[DIK_LSHIFT] || keys[DIK_RSHIFT])) {
			preX = x;
			preY = y;
			isTranslate = true;
		} else if (Novice::IsPressMouse(2)) {
			preX = x;
			preY = y;
			isRotate = true;
		} else {
			preX = 0;
			preY = 0;
		}
		Novice::GetMousePosition(&x, &y);
		if (preX != 0 && preY != 0 && isTranslate && !isRotate) {
			cameraTranslate.x += std::cosf(cameraRotate.x) * (preX - x) * 0.002f;
			cameraTranslate.y += std::sinf(cameraRotate.y) * (preY - y) * 0.002f;
			cameraTranslate.z += std::sinf(cameraRotate.z) * (preY - y) * 0.002f;
		}
		if (preX != 0 && preY != 0 && isRotate && !isTranslate) {
			cameraRotate.x -= (preY - y) * 0.002f;
			cameraRotate.y -= (preX - x) * 0.002f;
		}

		// フラグ初期化
		if (!Novice::IsPressMouse(2)) {
			isTranslate = false;
			isRotate = false;
		}

		// 当たり判定
		if (AABB::IsCollision(aabb, sphere)) {
			aabb.color = RED;
		} else {
			aabb.color = WHITE;
		}

		// 各種行列の計算
		Matrix4x4 worldMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Matrix4x4::Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Sphere::DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		AABB::DrawAABB(aabb, worldViewProjectionMatrix, viewportMatrix);
		Sphere::DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix);

		ImGui::Begin("Window");
		ImGui::DragFloat3("aabb1.min", &aabb.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb.max.x, 0.01f);
		AABB::SafeParameter(aabb);
		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);
		ImGui::End();

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
