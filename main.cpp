#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Sphere.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_03_00";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, Renderer::kWindowWidth, Renderer::kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Renderer renderer;
	renderer.Initialize();

	// 初期化
	Vector3 controlPoints[4] = {
		{-0.8f, 0.58f, 1.0f },
		{1.76f, 1.0f, -0.3f },
		{0.94f, -0.7f, 2.3f },
		{-0.53f, -0.26f, -0.15f }
	};
	Sphere sphere1 = {
		.radius = 0.01f,
		.color = BLACK
	};
	Sphere sphere2 = {
		.radius = 0.01f,
		.color = BLACK
	};
	Sphere sphere3 = {
		.radius = 0.01f,
		.color = BLACK
	};
	Sphere sphere4 = {
		.radius = 0.01f,
		.color = BLACK
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

		renderer.Update();
		sphere1.center = controlPoints[0];
		sphere2.center = controlPoints[1];
		sphere3.center = controlPoints[2];
		sphere4.center = controlPoints[3];

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		renderer.Draw();
		Vector3::DrawCatmullRom(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], renderer, BLUE);
		Sphere::Draw(renderer, sphere1);
		Sphere::Draw(renderer, sphere2);
		Sphere::Draw(renderer, sphere3);
		Sphere::Draw(renderer, sphere4);

		ImGui::Begin("Window");
		ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x, 0.01f);
		ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x, 0.01f);
		ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x, 0.01f);
		ImGui::DragFloat3("controlPoints[3]", &controlPoints[3].x, 0.01f);
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
