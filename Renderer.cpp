#include "Renderer.h"
#include <Novice.h>
#include "Matrix4x4.h"


void Renderer::Initialize()
{
	input = Input::GetInstance();
}

void Renderer::Update()
{
	// 各種行列の計算
	worldMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, translate);
	cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
	viewMatrix = Matrix4x4::Inverse(cameraMatrix);
	projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
	viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

	// カメラ移動
	if (Novice::IsPressMouse(CENTER)) {
		cameraTranslate += Vector3::Transform({ -static_cast<float>(input->GetMouseMove().lX), static_cast<float>(input->GetMouseMove().lY), 0.0f }, Matrix4x4::Inverse(viewMatrix)) * 0.001f;
	}
	// ドリーイン、ドリーアウト
	else {
		cameraTranslate += Vector3::GetZaxis(Matrix4x4::MakeRotateMatrix(cameraRotate)) * static_cast<float>(input->GetWheel()) * 0.002f;
	}
	// カメラ回転
	if (Novice::IsPressMouse(RIGHT)) {
		cameraRotate.x += static_cast<float>(input->GetMouseMove().lY) * 0.001f;
		cameraRotate.y += static_cast<float>(input->GetMouseMove().lX) * 0.001f;
	}
}

void Renderer::Draw()
{
	DrawGrid();

	Novice::ScreenPrintf(0, 0, "CameraMove   : Mouse Center");
	Novice::ScreenPrintf(0, 20, "CameraRotate : Mouse Right");
	Novice::ScreenPrintf(0, 40, "Dolly In,Out : Mouse Wheel");
}

void Renderer::DrawGrid() {
	const float kGridHalWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		Vector3 worldStartPosition = { -kGridEvery * kSubdivision / 2.0f, 0.0f, kGridEvery * (xIndex - kSubdivision / 2.0f) };
		Vector3 worldEndPosition = { kGridEvery * kSubdivision / 2.0f, 0.0f, kGridEvery * (xIndex - kSubdivision / 2.0f) };
		Vector3 screenStartPosition = Vector3::Transform(Vector3::Transform(worldStartPosition, worldViewProjectionMatrix), viewportMatrix);
		Vector3 screenEndPosition = Vector3::Transform(Vector3::Transform(worldEndPosition, worldViewProjectionMatrix), viewportMatrix);
		if (xIndex == kSubdivision / 2) {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, BLACK);
		}
		else {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, 0xAAAAAAFF);
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		Vector3 worldStartPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, -kGridEvery * kSubdivision / 2.0f };
		Vector3 worldEndPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, kGridEvery * kSubdivision / 2.0f };
		Vector3 screenStartPosition = Vector3::Transform(Vector3::Transform(worldStartPosition, worldViewProjectionMatrix), viewportMatrix);
		Vector3 screenEndPosition = Vector3::Transform(Vector3::Transform(worldEndPosition, worldViewProjectionMatrix), viewportMatrix);
		if (zIndex == kSubdivision / 2) {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, BLACK);
		}
		else {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, 0xAAAAAAFF);
		}
	}
}