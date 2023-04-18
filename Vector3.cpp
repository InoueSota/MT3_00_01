#include "Vector3.h"
#include <Novice.h>



//表示
void Vector3::VectorScreenPrintf(int a, int b, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(a, b, "%.02f", vector.x);
	Novice::ScreenPrintf(a + kColumnWidth, b, "%.02f", vector.y);
	Novice::ScreenPrintf(a + kColumnWidth * 2, b, "%.02f", vector.z);
	Novice::ScreenPrintf(a + kColumnWidth * 3, b, "%s", label);
}