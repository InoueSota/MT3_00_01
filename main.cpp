#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "CollisionManager.h"
#include "Sphere.h"
#include "Line.h"
#include "Plane.h"
#include "Ball.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_04_04";

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
	float e = 0.9f;
	Plane plane = {
		.normal = Vector3::Normalize({-0.2f, 0.9f, -0.3f}),
		.distance = 0.0f
	};
	Ball ball = {
		.position = {0.8f, 1.2f, 0.3f},
		.mass = 2.0f,
		.radius = 0.05f,
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

		ball.acceleration = { 0.0f, -9.8f, 0.0f };

		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;
		if (IsCollision(Sphere{ ball.position, ball.radius }, plane)) {
			ball.velocity = Ball::Reflect(ball.velocity, plane.normal) * e;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		renderer.Draw();

		Sphere::Draw(renderer, Sphere{ ball.position, ball.radius, WHITE });
		Plane::Draw(renderer, plane, WHITE);

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
