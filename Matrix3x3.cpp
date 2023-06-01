#include "Matrix3x3.h"
#include "Vector2.h"
#define _USE_MATH_DEFINES
#include <math.h>



// 加算
Matrix3x3 Matrix3x3::Add(Matrix3x3 matrix1, Matrix3x3 matrix2) {

	return {
		matrix1.m[0][0] + matrix2.m[0][0],
		matrix1.m[0][1] + matrix2.m[0][1],
		matrix1.m[0][2] + matrix2.m[0][2],
		matrix1.m[1][0] + matrix2.m[1][0],
		matrix1.m[1][1] + matrix2.m[1][1],
		matrix1.m[1][2] + matrix2.m[1][2],
		matrix1.m[2][0] + matrix2.m[2][0],
		matrix1.m[2][1] + matrix2.m[2][1],
		matrix1.m[2][2] + matrix2.m[2][2]
	};
}

// 積
Matrix3x3 Matrix3x3::Subtract(Matrix3x3 matrix1, Matrix3x3 matrix2) {

	return {
		matrix1.m[0][0] - matrix2.m[0][0],
		matrix1.m[0][1] - matrix2.m[0][1],
		matrix1.m[0][2] - matrix2.m[0][2],
		matrix1.m[1][0] - matrix2.m[1][0],
		matrix1.m[1][1] - matrix2.m[1][1],
		matrix1.m[1][2] - matrix2.m[1][2],
		matrix1.m[2][0] - matrix2.m[2][0],
		matrix1.m[2][1] - matrix2.m[2][1],
		matrix1.m[2][2] - matrix2.m[2][2]
	};
}

// 2項 *
Matrix3x3 Matrix3x3::operator*(Matrix3x3 matrix) const {

	return {
		m[0][0] * matrix.m[0][0] + m[0][1] * matrix.m[1][0] + m[0][2] * matrix.m[2][0],
		m[0][0] * matrix.m[0][1] + m[0][1] * matrix.m[1][1] + m[0][2] * matrix.m[2][1],
		m[0][0] * matrix.m[0][2] + m[0][1] * matrix.m[1][2] + m[0][2] * matrix.m[2][2],
		m[1][0] * matrix.m[0][0] + m[1][1] * matrix.m[1][0] + m[1][2] * matrix.m[2][0],
		m[1][0] * matrix.m[0][1] + m[1][1] * matrix.m[1][1] + m[1][2] * matrix.m[2][1],
		m[1][0] * matrix.m[0][2] + m[1][1] * matrix.m[1][2] + m[1][2] * matrix.m[2][2],
		m[2][0] * matrix.m[0][0] + m[2][1] * matrix.m[1][0] + m[2][2] * matrix.m[2][0],
		m[2][0] * matrix.m[0][1] + m[2][1] * matrix.m[1][1] + m[2][2] * matrix.m[2][1],
		m[2][0] * matrix.m[0][2] + m[2][1] * matrix.m[1][2] + m[2][2] * matrix.m[2][2]
	};
}

// 拡大縮小行列
Matrix3x3 Matrix3x3::MakeScaleMatrix(const Vector2& scale) {
	return {
			scale.x, 0, 0,
			0, scale.y, 0,
			0, 0, 1
	};
}

// 回転行列
Matrix3x3 Matrix3x3::MakeRotateMatrix(float theta) {
	float s = sinf(theta);
	float c = cosf(theta);
	return {
			c,		s,		0,
			-s,		c,		0,
			0,		0,		1
	};
}

// 平行移動行列
Matrix3x3 Matrix3x3::MakeTranslateMatrix(const Vector2& translate) {
	return {
			1, 0, 0,
			0, 1, 0,
			translate.x, translate.y, 1
	};
}

// 2次元アフィン変換行列
Matrix3x3 Matrix3x3::MakeAffineMatrix(const Vector2& scale, float rotate, const Vector2& translate) {
	float s = sinf(rotate);
	float c = cosf(rotate);
	return{
		scale.x * c,  scale.x * s, 0,
		scale.y * -s, scale.y * c, 0,
		translate.x, translate.y, 1
	};
}
