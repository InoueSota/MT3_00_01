#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "ConicalPendulum.h"
#include "Sphere.h"
#include "Line.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_04_03";

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
	ConicalPendulum conicalPendulum = {
		.anchor = {0.0f, 1.0f, 0.0f},
		.length = 0.8f,
		.halfApexAngle = 0.7f,
		.angle = 0.7f,
		.angularVelocity = 0.0f,
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

		conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
		conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;

		float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;

		sphere.center.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
		sphere.center.y = conicalPendulum.anchor.y - height;
		sphere.center.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;

		segment.origin = conicalPendulum.anchor;
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
			conicalPendulum.angularVelocity = 0.0f;
			conicalPendulum.angle = 0.7f;
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
