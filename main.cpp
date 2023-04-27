#include <Novice.h>
#include "Matrix4x4.h"
#include "Vector3.h"

const char kWindowTitle[] = "LD2A_02_イノウエソウタ_MT3_00_03";
const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,0.0f,10.0f };
	Vector3 cameraPosition{ 0.0f,0.0f,0.0f };
	Vector3 kLocalVertices[3];
	kLocalVertices[0] = { -0.5f,-0.5f,0.0f };
	kLocalVertices[1] = {  0.5f,-0.5f,0.0f };
	kLocalVertices[2] = {  0.0f, 0.5f,0.0f };

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

		Vector3 v1{ 1.2f, -3.9f, 2.5f };
		Vector3 v2{ 2.8f, 0.4f, -1.3f };
		Vector3 cross = Vector3::Cross(v1, v2);

		if (keys[DIK_W]) {
			translate.z += 0.15f;
		}
		if (keys[DIK_S]) {
			translate.z -= 0.15f;
		}
		if (keys[DIK_A]) {
			translate.x -= 0.05f;
		}
		if (keys[DIK_D]) {
			translate.x += 0.05f;
		}
		rotate.y += 0.05f;

		// 各種行列の計算
		Matrix4x4 worldMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Matrix4x4::Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndeVertex = Vector3::Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Vector3::Transform(ndeVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Vector3::VectorScreenPrintf(0, 0, cross, "Cross");
		Novice::ScreenPrintf(0, 20, "%.1f rotate.y", rotate.y);
		if (Vector3::Dot({0.0f,0.0f,1.0f}, Vector3::Cross(Vector3::Subtract(screenVertices[1], screenVertices[0]), Vector3::Subtract(screenVertices[2], screenVertices[1]))) <= 0.0f) {
			Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);
		}

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
