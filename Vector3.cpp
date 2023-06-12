#include "Vector3.h"
#include <Novice.h>
#include "Matrix4x4.h"



//加算
Vector3 Vector3::Add(const Vector3& v1, const Vector3& v2) {
	return {
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
	};
}
Vector3 Vector3::operator+=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}
Vector3 Vector3::operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }

//減算
Vector3 Vector3::Subtract(const Vector3& v1, const Vector3& v2) {
	return {
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
	};
}
Vector3 Vector3::operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
Vector3 Vector3::operator-() const { return { -x, -y, -z }; }

//スカラー倍
Vector3 Vector3::Multiply(float scalar, const Vector3& v) {
	return {
		scalar * v.x,
		scalar * v.y,
		scalar * v.z
	};
}
Vector3 Vector3::operator*(const Vector3& vector) const { return { x * vector.x, y * vector.y, z * vector.z }; }
Vector3 Vector3::operator*(float f) const { return { x * f, y * f, z * f }; }
Vector3 Vector3::operator*(float f) { return { x * f, y * f, z * f }; }

// 除算
Vector3 Vector3::operator/(float f) const { return { x / f, y / f, z / f }; }

//内積
float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// クロス積
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

//長さ（ノルム）
float Vector3::Length(const Vector3& v) {
	return std::sqrt(Dot(v, v));
}
float Vector3::LengthSquare(const Vector3& v) {
	return Dot(v, v);
}

//正規化
Vector3 Vector3::Normalize(const Vector3& v) {

	Vector3 tmpVector3 = { 0, 0, 0 };

	if (Length(v) != 0) {
		tmpVector3.x = v.x / Length(v);
		tmpVector3.y = v.y / Length(v);
		tmpVector3.z = v.z / Length(v);
	}

	return tmpVector3;
}

// 垂直なベクトルを１つ求める
Vector3 Vector3::Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

// 正射影ベクトルを求める
Vector3 Vector3::Project(const Vector3& v1, const Vector3& v2) {
	Vector3 unitVector = v2 / Vector3::Length(v2);
	return Vector3::Multiply(Vector3::Dot(v1, unitVector), unitVector);
}

// 座標変換
Vector3 Vector3::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

// ベクトル変換
Vector3 Vector3::TransformNormal(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1],
		vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2]
	};
	return result;
}

Vector3 Vector3::GetXaxis(const Matrix4x4& m)
{
	return { m.m[0][0], m.m[0][1], m.m[0][2] };
}

Vector3 Vector3::GetYaxis(const Matrix4x4& m)
{
	return { m.m[1][0], m.m[1][1], m.m[1][2] };
}

Vector3 Vector3::GetZaxis(const Matrix4x4& m)
{
	return { m.m[2][0], m.m[2][1], m.m[2][2] };
}

Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return v1 + (v2 - v1) * t;
}
float Vector3::Lerp(const float f1, const float f2, float t) 
{
	return f1 + (f2 - f1) * t;
}

Vector3 Vector3::Slerp(const Vector3& v1, const Vector3& v2, float t)
{
	float s = Lerp(Vector3::Length(v1), Vector3::Length(v2), t);
	Vector3 nv1 = Vector3::Normalize(v1);
	Vector3 nv2 = Vector3::Normalize(v2);
	float theta = 1 / std::cos(Dot(nv1, nv2));

	return ((nv1 * std::sin((1 - t) * theta) + nv2 * std::sin(t * theta)) / std::sin(theta)) * s;
}

Vector3 Vector3::CatmullRom(const std::vector<Vector3>& point, float t)
{
	//for (int i = 1; i < point.size() - 2; i++) {

	//	Vector3 a4 = point[i];
	//	Vector3 a3 = (point[i + 1] - point[i - 1]) / 2.0f;
	//	Vector3 a2 = (point[i + 2] - point[i]) / 2.0f - a3 * 2.0f - a4 * 3.0f;
	//	Vector3 a1 = point[i] * 3.0f - (point[i + 2] - point[i]) / 2.0f - a3 * 2.0f - a4 * 3.0f;

	//	return a1 * t * t * t + a2 * t * t + a3 * t + a4;
	//}

	point;
	t;

	return Vector3();
}

//表示
void Vector3::VectorScreenPrintf(int a, int b, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(a, b, "%.02f", vector.x);
	Novice::ScreenPrintf(a + kColumnWidth, b, "%.02f", vector.y);
	Novice::ScreenPrintf(a + kColumnWidth * 2, b, "%.02f", vector.z);
	Novice::ScreenPrintf(a + kColumnWidth * 3, b, "%s", label);
}