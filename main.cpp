#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Spring.h"
#include "Ball.h"
#include "Sphere.h"
#include "Line.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_04_01";

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
	float deltaTime = 1.0f / 60.0f;
	float angularVelocity = 3.14f;
	float angle = 0.0f;
	float radius = 0.8f;
	Sphere sphere = {
		.radius = 0.03f,
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

		renderer.Update();

		angle += angularVelocity * deltaTime;
		sphere.center = { -radius * std::sin(angle), radius * std::cos(angle), 0.0f };

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		renderer.Draw();

		Sphere::Draw(renderer, sphere);

		ImGui::Begin("Window");
		if (ImGui::Button("Start")) {

		}
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
