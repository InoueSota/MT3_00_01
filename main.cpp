#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Spring.h"
#include "Ball.h"
#include "Sphere.h"
#include "Line.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_04_00";

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
	Spring spring = {
		.anchor = {0.0f, 0.0f, 0.0f},
		.naturalLength = 1.0f,
		.stiffness = 100.0f,
		.dampingCoefficient = 2.0f
	};
	Ball ball = {
		.position = {1.2f, 0.0f, 0.0f},
		.mass = 2.0f,
		.radius = 0.05f,
		.color = BLUE
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

		Vector3 diff = ball.position - spring.anchor;
		float length = Vector3::Length(diff);
		if (length != 0.0f) {
			Vector3 direction = Vector3::Normalize(diff);
			Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
			Vector3 displacement = length * (ball.position - restPosition);
			Vector3 restoringForce = -spring.stiffness * displacement;
			Vector3 damingForce = -spring.dampingCoefficient * ball.velocity;
			Vector3 force = restoringForce + damingForce;
			ball.acceleration = force / ball.mass;
		}

		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		renderer.Draw();

		renderer.ScreenLine(spring.anchor, ball.position, WHITE);
		Ball::Draw(renderer, ball);

		ImGui::Begin("Window");
		if (ImGui::Button("Start")) {
			ball.position = { 1.2f, 0.0f, 0.0f };
			ball.acceleration = { 0.0f, 0.0f, 0.0f };
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
