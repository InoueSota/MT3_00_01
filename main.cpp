#include <Novice.h>
#include <imgui.h>
#include "Renderer.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "CollisionManager.h"
#include "Line.h"
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

	OBB obb{
		.center{ -1.0f, 0.0f, 0.0f },
		.orientations = {{ 1.0f, 0.0f, 0.0f },
						 { 0.0f, 1.0f, 0.0f },
						 { 0.0f, 0.0f, 1.0f }},
		.size{ 0.5f, 0.5f, 0.5f },
		.color = WHITE
	};
	Segment segment{
		.origin{-0.8f, -0.3f, 0.0f},
		.diff{0.5f, 0.5f, 0.5f},
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

		// OBBの軸を作る
		OBB::MakeOrientations(obb, renderer.rotate);

		// 当たり判定
		if (IsCollision(segment, obb)) {
			obb.color = RED;
		} else {
			obb.color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		renderer.Draw();

		OBB::Draw(obb, renderer.worldViewProjectionMatrix, renderer.viewportMatrix);
		Segment::Draw(segment, renderer.worldViewProjectionMatrix, renderer.viewportMatrix);

		ImGui::Begin("Window");
		ImGui::DragFloat3("rotate", &renderer.rotate.x, 0.01f);
		ImGui::DragFloat3("center", &obb.center.x, 0.01f);
		ImGui::DragFloat3("orientations[0]", &obb.orientations[0].x, 0.01f);
		ImGui::DragFloat3("orientations[1]", &obb.orientations[1].x, 0.01f);
		ImGui::DragFloat3("orientations[2]", &obb.orientations[2].x, 0.01f);
		ImGui::DragFloat3("size", &obb.size.x, 0.01f);
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
