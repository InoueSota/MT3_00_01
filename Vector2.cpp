#include "Vector2.h"
#include "Matrix3x3.h"



// 長さ
float Vector2::Length() const {
    return sqrtf(LengthSquare());
}

// 長さの2乗
float Vector2::LengthSquare() const {
    return Dot(*this);
}

// 内積
float Vector2::Dot(const Vector2& other) const {
    return x * other.x + y * other.y;
}

// 外積
float Vector2::Cross(const Vector2& other) const {
    return x * other.y - y * other.x;
}

// 距離
float Vector2::DistanceFrom(const Vector2& other) const {
    return (other - *this).Length();
}

// 正規化
Vector2 Vector2::Normalized() const {
    if (Length() != 0)
        return *this / Length();
    else
        return *this;
}

// ゼロベクトルか
bool Vector2::isZero() const {
    return x == 0.0 && y == 0.0;
}

// ゼロベクトルにする
void Vector2::setZero() {
    x = 0.0;
    y = 0.0;
}

// 座標変換
Vector2 Vector2::Transform(const Vector2& vector, Matrix3x3 matrix) {
	Vector2 result{
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1]
    };
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

// ベクトルの回転
Vector2 Vector2::Rotation(float t) const {
    return{ x * cosf(t) - x * sinf(t) ,y * sinf(t) + y * cosf(t) };
}

// 単項 +
 Vector2 Vector2::operator +() const {
    return *this;
}

 // 単項 -
 Vector2 Vector2::operator -() const {
    return{ -x, -y };
}

 // 2項 +
 Vector2 Vector2::operator +(const Vector2& other) const {
    return{ x + other.x, y + other.y };
}

 // 2項 -
Vector2 Vector2::operator -(const Vector2& other) const {
    return{ x - other.x, y - other.y };
}

// 2項 *
Vector2 Vector2::operator *(float s) const {
    return{ x * s, y * s };
}

// 2項 /
Vector2 Vector2::operator /(float s) const {
    return{ x * 1 / s, y * 1 / s };
}

// 複合代入演算 +=
Vector2& Vector2::operator +=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// 複合代入演算 -=
Vector2& Vector2::operator -=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// 複合代入演算 *=
Vector2& Vector2::operator*=(float s) {
    x *= s;
    y *= s;
    return *this;
}

// 複合代入演算 /=
Vector2& Vector2::operator/=(float s) {
    x /= s;
    y /= s;
    return *this;
}

// 行列 2項 *
Vector2 Vector2::operator*(const Matrix3x3& matrix) const {
	return {
	    x * matrix.m[0][0] + y * matrix.m[1][0],
	    x * matrix.m[0][1] + y * matrix.m[1][1]};
}

// Vector2が後にくる 2項 *
 Vector2 operator *(float s, const Vector2& v) {
    return{ s * v.x, s * v.y };
}