#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "CollisionManager.h"
#include "OBB.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_02_09";

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
	Vector3 rotate1{ 0.0f, 0.0f, 0.0f };
	Vector3 rotate2{ 0.05f, -2.49f, 0.15f };
	OBB obb1{
		.center{0.0f, 0.0f, 0.0f},
		.orientations = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		.size{0.83f, 0.26f, 0.24f},
		.color = WHITE
	};
	OBB obb2{
		.center{0.0f, 0.66f, 0.78f},
		.orientations = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		.size{0.5f, 0.37f, 0.5f},
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

		obb1.MakeOrientations(obb1, rotate1);
		obb2.MakeOrientations(obb2, rotate2);
		if (IsCollision(obb1, obb2)) {
			obb1.color = RED;
		}
		else
		{
			obb1.color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		obb1.Draw(renderer, obb1);
		obb2.Draw(renderer, obb2);
		renderer.Draw();

		ImGui::Begin("Window");
		ImGui::DragFloat3("1.center", &obb1.center.x, 0.01f);
		ImGui::DragFloat3("1.orientations[0]", &obb1.orientations[0].x, 0.01f);
		ImGui::DragFloat3("1.orientations[1]", &obb1.orientations[1].x, 0.01f);
		ImGui::DragFloat3("1.orientations[2]", &obb1.orientations[2].x, 0.01f);
		ImGui::DragFloat3("1.size", &obb1.size.x, 0.01f);
		ImGui::DragFloat3("2.center", &obb2.center.x, 0.01f);
		ImGui::DragFloat3("2.orientations[0]", &obb2.orientations[0].x, 0.01f);
		ImGui::DragFloat3("2.orientations[1]", &obb2.orientations[1].x, 0.01f);
		ImGui::DragFloat3("2.orientations[2]", &obb2.orientations[2].x, 0.01f);
		ImGui::DragFloat3("2.size", &obb2.size.x, 0.01f);
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
