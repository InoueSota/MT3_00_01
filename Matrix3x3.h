#pragma once

struct Vector2;

class Matrix3x3 {
public:
	/*　変数　*/
	float m[3][3];

	Matrix3x3(
	    float _11 = 1.0f, float _12 = 0.0f, float _13 = 0.0f, 
		float _21 = 0.0f, float _22 = 1.0f, float _23 = 0.0f, 
		float _31 = 0.0f, float _32 = 0.0f, float _33 = 1.0f) {
		m[0][0] = _11, m[0][1] = _12, m[0][2] = _13;
		m[1][0] = _21, m[1][1] = _22, m[1][2] = _23;
		m[2][0] = _31, m[2][1] = _32, m[2][2] = _33;
	}

	/// <summary>
	/// 加算
	/// </summary>
	static Matrix3x3 Add(Matrix3x3 matrix1, Matrix3x3 matrix2);

	/// <summary>
	/// 積
	/// </summary>
	static Matrix3x3 Subtract(Matrix3x3 matrix1, Matrix3x3 matrix2);

	/// <summary>
	/// 2項 *
	/// </summary>
	Matrix3x3 operator*(Matrix3x3 matrix) const;

	/// <summary>
	/// 拡大縮小行列
	/// </summary>
	static Matrix3x3 MakeScaleMatrix(const Vector2& scale);

	/// <summary>
	/// 回転行列
	/// </summary>
	static Matrix3x3 MakeRotateMatrix(float theta);

	/// <summary>
	/// 平行移動行列
	/// </summary>
	static Matrix3x3 MakeTranslateMatrix(const Vector2& translate);

	/// <summary>
	/// 2次元アフィン変換行列
	/// </summary>
	static Matrix3x3 MakeAffineMatrix(const Vector2& scale, float rotate, const Vector2& translate);
};
