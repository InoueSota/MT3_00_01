#include "Sphere.h"
#define _USE_MATH_DEFINES
#include <math.h>


// 球描画
void Sphere::Draw(Renderer& renderer, const Sphere& sphere) {
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
			renderer.ScreenLine(a, b, sphere.color);
			renderer.ScreenLine(a, c, sphere.color);
		}
	}
}