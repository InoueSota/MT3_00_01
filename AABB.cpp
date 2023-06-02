#include "AABB.h"
#include <Novice.h>
#include "Sphere.h"
#include <algorithm>


void AABB::SafeParameter(AABB& aabb)
{
    aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
    aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
    aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
    aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
    aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
    aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
}

bool AABB::IsCollision(const AABB& aabb1, const AABB& aabb2)
{
    if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
        (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
        (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
        return true;
    }
    return false;
}

bool AABB::IsCollision(const AABB& aabb, const Sphere& sphere)
{
    // 最近接点を求める
    Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y) , std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

    // 最近接点と球の中心との距離を求める
    float distance = Vector3::Length(closestPoint - sphere.center);

    // 距離が半径よりも小さければ衝突
    if (distance <= sphere.radius) {
        return true;
    }
    return false;
}

void AABB::DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
    // zがmin（手前）
    Vector3 frontLeftTop = Vector3::Transform(Vector3::Transform({ aabb.min.x, aabb.max.y, aabb.min.z }, viewProjectionMatrix), viewportMatrix);
    Vector3 frontRightTop = Vector3::Transform(Vector3::Transform({ aabb.max.x, aabb.max.y, aabb.min.z }, viewProjectionMatrix), viewportMatrix);
    Vector3 frontLeftBottom = Vector3::Transform(Vector3::Transform({ aabb.min.x, aabb.min.y, aabb.min.z }, viewProjectionMatrix), viewportMatrix);
    Vector3 frontRightBottom = Vector3::Transform(Vector3::Transform({ aabb.max.x, aabb.min.y, aabb.min.z }, viewProjectionMatrix), viewportMatrix);
    Novice::DrawLine((int)frontLeftTop.x, (int)frontLeftTop.y, (int)frontRightTop.x, (int)frontRightTop.y, aabb.color);
    Novice::DrawLine((int)frontRightTop.x, (int)frontRightTop.y, (int)frontRightBottom.x, (int)frontRightBottom.y, aabb.color);
    Novice::DrawLine((int)frontRightBottom.x, (int)frontRightBottom.y, (int)frontLeftBottom.x, (int)frontLeftBottom.y, aabb.color);
    Novice::DrawLine((int)frontLeftBottom.x, (int)frontLeftBottom.y, (int)frontLeftTop.x, (int)frontLeftTop.y, aabb.color);

    // zがmax（奥）
    Vector3 backLeftTop = Vector3::Transform(Vector3::Transform({ aabb.min.x, aabb.max.y, aabb.max.z }, viewProjectionMatrix), viewportMatrix);
    Vector3 backRightTop = Vector3::Transform(Vector3::Transform({ aabb.max.x, aabb.max.y, aabb.max.z }, viewProjectionMatrix), viewportMatrix);
    Vector3 backLeftBottom = Vector3::Transform(Vector3::Transform({ aabb.min.x, aabb.min.y, aabb.max.z }, viewProjectionMatrix), viewportMatrix);
    Vector3 backRightBottom = Vector3::Transform(Vector3::Transform({ aabb.max.x, aabb.min.y, aabb.max.z }, viewProjectionMatrix), viewportMatrix);
    Novice::DrawLine((int)backLeftTop.x, (int)backLeftTop.y, (int)backRightTop.x, (int)backRightTop.y, aabb.color);
    Novice::DrawLine((int)backRightTop.x, (int)backRightTop.y, (int)backRightBottom.x, (int)backRightBottom.y, aabb.color);
    Novice::DrawLine((int)backRightBottom.x, (int)backRightBottom.y, (int)backLeftBottom.x, (int)backLeftBottom.y, aabb.color);
    Novice::DrawLine((int)backLeftBottom.x, (int)backLeftBottom.y, (int)backLeftTop.x, (int)backLeftTop.y, aabb.color);

    // 手前と奥を繋ぐ線
    Novice::DrawLine((int)frontLeftTop.x, (int)frontLeftTop.y, (int)backLeftTop.x, (int)backLeftTop.y, aabb.color);
    Novice::DrawLine((int)frontRightTop.x, (int)frontRightTop.y, (int)backRightTop.x, (int)backRightTop.y, aabb.color);
    Novice::DrawLine((int)frontLeftBottom.x, (int)frontLeftBottom.y, (int)backLeftBottom.x, (int)backLeftBottom.y, aabb.color);
    Novice::DrawLine((int)frontRightBottom.x, (int)frontRightBottom.y, (int)backRightBottom.x, (int)backRightBottom.y, aabb.color);
}
