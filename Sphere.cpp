#include "Sphere.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>



// グリッド描画
void Sphere::DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		Vector3 worldStartPosition = { -kGridEvery * kSubdivision / 2.0f, 0.0f, kGridEvery * (xIndex - kSubdivision / 2.0f) };
		Vector3 worldEndPosition = { kGridEvery * kSubdivision / 2.0f, 0.0f, kGridEvery * (xIndex - kSubdivision / 2.0f) };
		Vector3 ndcStartPosition = Vector3::Transform(worldStartPosition, viewProjectionMatrix);
		Vector3 ndcEndPosition = Vector3::Transform(worldEndPosition, viewProjectionMatrix);
		Vector3 screenStartPosition = Vector3::Transform(ndcStartPosition, viewportMatrix);
		Vector3 screenEndPosition = Vector3::Transform(ndcEndPosition, viewportMatrix);
		if (xIndex == kSubdivision / 2) {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, BLACK);
		} else {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, 0xAAAAAAFF);
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		Vector3 worldStartPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, -kGridEvery * kSubdivision / 2.0f };
		Vector3 worldEndPosition = { kGridEvery * (zIndex - kSubdivision / 2.0f), 0.0f, kGridEvery * kSubdivision / 2.0f };
		Vector3 ndcStartPosition = Vector3::Transform(worldStartPosition, viewProjectionMatrix);
		Vector3 ndcEndPosition = Vector3::Transform(worldEndPosition, viewProjectionMatrix);
		Vector3 screenStartPosition = Vector3::Transform(ndcStartPosition, viewportMatrix);
		Vector3 screenEndPosition = Vector3::Transform(ndcEndPosition, viewportMatrix);
		if (zIndex == kSubdivision / 2) {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, BLACK);
		} else {
			Novice::DrawLine((int)screenStartPosition.x, (int)screenStartPosition.y, (int)screenEndPosition.x, (int)screenEndPosition.y, 0xAAAAAAFF);
		}
	}
}

// 球描画
void Sphere::DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 16;
	const float kLonEvery = (float)M_PI / ((float)kSubdivision / 2.0f);
	const float kLatEvery = (float)M_PI / ((float)kSubdivision / 2.0f);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = (float)-M_PI / 2.0f + kLatEvery * latIndex;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;

			Vector3 a, b, c;
			a = Vector3::Add(sphere.center, Vector3::Multiply(sphere.radius, { cosf(lat) * cosf(lon), sinf(lat), cosf(lat) * sinf(lon) }));
			b = Vector3::Add(sphere.center, Vector3::Multiply(sphere.radius, { cosf(lat + kLatEvery) * cosf(lon), sinf(lat + kLatEvery), cosf(lat + kLatEvery) * sinf(lon) }));
			c = Vector3::Add(sphere.center, Vector3::Multiply(sphere.radius, { cosf(lat) * cosf(lon + kLonEvery), sinf(lat), cosf(lat) * sinf(lon + kLonEvery) }));
			Vector3 ndcA = Vector3::Transform(a, viewProjectionMatrix);
			Vector3 ndcB = Vector3::Transform(b, viewProjectionMatrix);
			Vector3 ndcC = Vector3::Transform(c, viewProjectionMatrix);
			Vector3 screenA = Vector3::Transform(ndcA, viewportMatrix);
			Vector3 screenB = Vector3::Transform(ndcB, viewportMatrix);
			Vector3 screenC = Vector3::Transform(ndcC, viewportMatrix);
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenB.x, (int)screenB.y, color);
			Novice::DrawLine((int)screenA.x, (int)screenA.y, (int)screenC.x, (int)screenC.y, color);
		}
	}
}