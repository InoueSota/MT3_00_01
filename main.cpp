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
	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f}
	};
	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f,  0.0f}
	};
	Vector3 scales[3] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f}
	};

	Sphere shoulderSphere = {
		.radius = 0.05f,
		.color = RED
	};
	Sphere elbowSphere = {
		.radius = 0.05f,
		.color = GREEN
	};
	Sphere handSphere = {
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

		renderer.Update(scales, rotates, translates);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		renderer.Draw();

		Vector3 screenShoulder = Vector3::Transform(shoulderSphere.center, renderer.shoulderWorldViewProjectionMatrix * renderer.viewportMatrix);
		Vector3 screenElbow = Vector3::Transform(elbowSphere.center, renderer.elbowWorldViewProjectionMatrix * renderer.viewportMatrix);
		Vector3 screenHand = Vector3::Transform(handSphere.center, renderer.handWorldViewProjectionMatrix * renderer.viewportMatrix);

		// 描画する
		Novice::DrawLine(static_cast<int>(screenShoulder.x), static_cast<int>(screenShoulder.y), static_cast<int>(screenElbow.x), static_cast<int>(screenElbow.y), WHITE);
		Novice::DrawLine(static_cast<int>(screenElbow.x), static_cast<int>(screenElbow.y), static_cast<int>(screenHand.x), static_cast<int>(screenHand.y), WHITE);

		Sphere::Draw(renderer.shoulderWorldViewProjectionMatrix, renderer.viewportMatrix, shoulderSphere);
		Sphere::Draw(renderer.elbowWorldViewProjectionMatrix, renderer.viewportMatrix, elbowSphere);
		Sphere::Draw(renderer.handWorldViewProjectionMatrix, renderer.viewportMatrix, handSphere);

		ImGui::Begin("Window");
		ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotates[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotates[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);
		ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);
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
