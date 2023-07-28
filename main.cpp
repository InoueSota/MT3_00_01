#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Pendulum.h"
#include "Sphere.h"
#include "Line.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_04_02";

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
	Pendulum pendulum = {
		.anchor = {0.0f, 1.0f, 0.0f},
		.length = 0.8f,
		.angle = 0.7f,
		.angularVelocity = 0.0f,
		.angularAcceleration = 0.0f
	};
	Sphere sphere = {
		.center = {0.0f, 0.0f, 0.0f},
		.radius = 0.05f,
		.color = WHITE
	};
	Segment segment = {
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

		pendulum.angularAcceleration = -(9.8f / pendulum.length) * std::sin(pendulum.angle);
		pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
		pendulum.angle += pendulum.angularVelocity * deltaTime;

		sphere.center.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
		sphere.center.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
		sphere.center.z = pendulum.anchor.z;

		segment.origin = pendulum.anchor;
		segment.diff = sphere.center - segment.origin;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		renderer.Draw();

		Segment::Draw(renderer, segment);
		Sphere::Draw(renderer, sphere);

		ImGui::Begin("Window");
		if (ImGui::Button("Start")) {
			pendulum.angularVelocity = 0.0f;
			pendulum.angularAcceleration = 0.0f;
			pendulum.angle = 0.7f;
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
