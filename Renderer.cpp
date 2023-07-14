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

void Renderer::Update(const Vector3 scales[3], const Vector3 rotates[3], const Vector3 translates[3])
{
	worldMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, translate);
	cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
	viewMatrix = Matrix4x4::Inverse(cameraMatrix);
	projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
	viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

	// 肩
	shoulderWorldMatrix = Matrix4x4::MakeAffineMatrix(scales[0], rotates[0], translates[0]);
	shoulderWorldViewProjectionMatrix = Matrix4x4::Multiply(shoulderWorldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));

	// 肘
	elbowWorldMatrix = Matrix4x4::MakeAffineMatrix(scales[1], rotates[1], translates[1]) * shoulderWorldMatrix;
	elbowWorldViewProjectionMatrix = Matrix4x4::Multiply(elbowWorldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));

	// 手
	handWorldMatrix = Matrix4x4::MakeAffineMatrix(scales[2], rotates[2], translates[2]) * elbowWorldMatrix;
	handWorldViewProjectionMatrix = Matrix4x4::Multiply(handWorldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));


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
		if (xIndex == kSubdivision / 2) {
			ScreenLine(worldStartPosition, worldEndPosition, BLACK);
		}
		else {
			ScreenLine(worldStartPosition, worldEndPosition, 0xAAAAAAFF);
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		Vector3 worldStartPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, -kGridEvery * kSubdivision / 2.0f };
		Vector3 worldEndPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, kGridEvery * kSubdivision / 2.0f };
		if (zIndex == kSubdivision / 2) {
			ScreenLine(worldStartPosition, worldEndPosition, BLACK);
		}
		else {
			ScreenLine(worldStartPosition, worldEndPosition, 0xAAAAAAFF);
		}
	}
}

void Renderer::ScreenLine(const Vector3& start, const Vector3& end, uint32_t color)
{
	// スクリーン空間に変換する
	Matrix4x4 transformMatrix = worldViewProjectionMatrix * viewportMatrix;
	Vector3 screenStart = Vector3::Transform(start, transformMatrix);
	Vector3 screenEnd = Vector3::Transform(end, transformMatrix);

	// 描画する
	Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y), static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), color);
}

void Renderer::ScreenTriangle(const Vector3 vertex[3], uint32_t color)
{
	// スクリーン空間に変換する
	Matrix4x4 transformMatrix = worldViewProjectionMatrix * viewportMatrix;
	Vector3 screenVertex[] = {
		Vector3::Transform(vertex[0], transformMatrix),
		Vector3::Transform(vertex[1], transformMatrix),
		Vector3::Transform(vertex[2], transformMatrix)
	};

	// 描画する
	Novice::DrawTriangle(static_cast<int>(vertex[0].x), static_cast<int>(vertex[0].y), static_cast<int>(vertex[1].x), static_cast<int>(vertex[1].y), static_cast<int>(vertex[2].x), static_cast<int>(vertex[2].y), color, kFillModeWireFrame);
}
